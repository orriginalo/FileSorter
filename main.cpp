#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;
using namespace std;

fs::path downloadsPath;
fs::path desktopPath;
fs::path picturesPath;
fs::path soundsPath;
fs::path bookPath;
fs::path videoPath;

std::_Put_time<char> curTime;
string logFileName;

vector<string> pictureExtentions = {".png",  ".jpg", ".jpeg", ".webp", ".svg", ".tiff", ".heic",
                                    ".jfif", ".bmp", ".apng", ".avif", ".tif", ".tga",  ".psd",
                                    ".eps",  ".ai",  ".indd", ".raw",  ".ico"};
vector<string> soundExtentions   = {".mp3", ".wav", ".flac", ".ogg", ".aac", ".m4a", ".wma", ".aiff", ".au", ".opus"};
vector<string> bookExtentions    = {".pdf",  ".epub", ".mobi", ".cbz", ".cbr", ".chm",
                                    ".djvu", ".fb2",  ".lit",  ".prc", ".xps"};
vector<string> videoExtentions   = {".mp4", ".mkv", ".avi", ".flv", ".webm", ".wmv",
                                    ".mov", ".m4v", ".3gp", ".3g2", ".swf"};

vector<fs::path> toPaths;
map<fs::path, string> fromPaths;

int totalFilesSorted = 0;
fs::path path        = "";
bool exitFromLoop    = false;

void sortFiles(const fs::path &, bool);
void createDirectoryIfNotExists(const fs::path &);
void loadConfig();
fs::path getPathByNumber(int &);
std::_Put_time<char> getCurTime();
void writeToLog(fs::path, fs::path);

int main() {
  setlocale(LC_ALL, "ru_RU.UTF-8");
  vector<int> variantsArr;
  string variants;

  loadConfig();
  curTime = getCurTime();
  stringstream oss;
  oss << "./logs/" << curTime << ".log";
  logFileName = oss.str();
  // for (const auto& pair : fromPaths) {
  //   cout << pair.first << " : " << pair.second << endl;
  // }

  cout << "Select from: (You can select more than one)" << endl;
  int counter = 1;
  for (const auto &pair : fromPaths) {
    cout << counter << ". " << pair.second << endl;
    counter++;
  }

  do {
    variantsArr = {};

    cout << "Enter: ";
    getline(cin, variants);
    stringstream ss(variants);

    int variant;
    while (ss >> variant) {
      fs::path path = getPathByNumber(variant);
      auto it       = fromPaths.find(path);
      if (it != fromPaths.end()) {
        variantsArr.push_back(variant);
        exitFromLoop = true;
      } else {
        cout << "Variant " << variant << " doesnt exists" << endl;
        exitFromLoop = false;
      }
    }
  } while (!exitFromLoop);

  for (fs::path &path : toPaths) {
    createDirectoryIfNotExists(path);
  }
  createDirectoryIfNotExists(picturesPath);
  createDirectoryIfNotExists(soundsPath);

  string answer;
  do {
    cout << "Do you want to sort files recursively? [y/n]: ";
    cin >> answer;
  } while ((answer != "y") && (answer != "n"));

  bool isRecursive = answer == "y";

  auto start = chrono::high_resolution_clock::now();
  for (int &var : variantsArr) {
    fs::path currentPath = getPathByNumber(var);
    if (currentPath != "") {
      sortFiles(currentPath, isRecursive);
    }
  }
  auto end = chrono::high_resolution_clock::now();

  auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end-start);

  if (totalFilesSorted >= 1) {
    cout << "File sorting is complete." << endl;
    cout << "Total sorted " << totalFilesSorted << " files in " << static_cast<float>(elapsed_time.count())/1000 << " seconds." << endl;
  } else {
    cout << "There are no files to sort." << endl;
  }
  cout << "Press Enter to exit . . .";
  cin.get(); // Ждет нажатия клавиши Enter
}

fs::path getPathByNumber(int &num) {
  int counter = 1;
  if (num <= fromPaths.size() && num > 0) {
    for (const auto &pair : fromPaths) {
      if (counter == num) {
        return pair.first;
      }
      counter++;
    }
  }
  return "";
}

void createDirectoryIfNotExists(const fs::path &path) {
  if (!fs::exists(path) && path != "") {
    try {
      fs::create_directory(path);
      cout << "Folder created: " << path << endl;
    } catch (const fs::filesystem_error &e) {
      cerr << "Failed to create folder" << path << ": " << e.what() << endl;
    }
  }
}

void sortFiles(const fs::path &src, bool isRecursive = false) {
  try {
    if(isRecursive) {
      for (const auto &entry : fs::recursive_directory_iterator(src)) {
        try {
          auto extension  = entry.path().extension();
          auto picturesIt = find(pictureExtentions.begin(), pictureExtentions.end(), extension);
          if (picturesIt != pictureExtentions.end()) {
            fs::rename(entry, picturesPath / entry.path().filename());
            totalFilesSorted++;
          }

          auto soundsIt = find(soundExtentions.begin(), soundExtentions.end(), extension);
          if (soundsIt != soundExtentions.end()) {
            fs::rename(entry, soundsPath / entry.path().filename());
            totalFilesSorted++;
          }

          auto booksIt = find(bookExtentions.begin(), bookExtentions.end(), extension);
          if (booksIt != bookExtentions.end()) {
            fs::rename(entry, bookPath / entry.path().filename());
            totalFilesSorted++;
          }

          auto videosIt = find(videoExtentions.begin(), videoExtentions.end(), extension);
          if (videosIt != videoExtentions.end()) {
            fs::rename(entry, videoPath / entry.path().filename());
            totalFilesSorted++;
          }
        } catch (fs::filesystem_error &e) {
          cout << "Error while transferring file " << entry.path().filename() << ": " << e.what() << endl;
        }
      }
    } else {
      for (const auto &entry : fs::directory_iterator(src)) {
        try {
          auto extension  = entry.path().extension();
          auto picturesIt = find(pictureExtentions.begin(), pictureExtentions.end(), extension);
          if (picturesIt != pictureExtentions.end()) {
            fs::path dst = picturesPath / entry.path().filename();
            fs::rename(entry, dst);
            writeToLog(entry, dst);
            totalFilesSorted++;
          }

          auto soundsIt = find(soundExtentions.begin(), soundExtentions.end(), extension);
          if (soundsIt != soundExtentions.end()) {
            fs::path dst = soundsPath / entry.path().filename();
            fs::rename(entry, dst);
            writeToLog(entry, dst);
            totalFilesSorted++;
          }

          auto booksIt = find(bookExtentions.begin(), bookExtentions.end(), extension);
          if (booksIt != bookExtentions.end()) {
            fs::path dst = bookPath / entry.path().filename();
            fs::rename(entry, dst);
            writeToLog(entry, dst);
            totalFilesSorted++;
          }

          auto videosIt = find(videoExtentions.begin(), videoExtentions.end(), extension);
          if (videosIt != videoExtentions.end()) {
            fs::path dst = videoPath / entry.path().filename();
            fs::rename(entry, dst);
            writeToLog(entry, dst);
            totalFilesSorted++;
          }
        } catch (fs::filesystem_error &e) {
          cout << "Error while transferring file " << entry.path().filename() << ": " << e.what() << endl;
        }
      }
    }
  } catch (fs::filesystem_error &e) {
    cout << "Filesystem error: " << e.what() << endl;
  }
}

void writeToLog(fs::path src, fs::path dst) {
  fstream logFile(logFileName, ios::app);
  logFile << src << " moved to " << dst << endl;
  logFile.close();
}

std::_Put_time<char> getCurTime() {
  auto time = chrono::system_clock::now();
  auto time_c = chrono::system_clock::to_time_t(time);

  return put_time(localtime(&time_c), "%d-%m-%Y %H-%M-%S"); 
}

void loadConfig() {
  ifstream configFile("cppsorter.cfg");
  string line;
  vector<string> lines;

  bool readFromPaths = false;

  while (getline(configFile, line)) {
    lines.push_back(line);
  }

  for (int i = 0; i < lines.size(); i++) {
    if (lines[i].rfind("from_paths {", 0) == 0) {
      readFromPaths = true;
      continue;
    }

    if (lines[i].rfind("}", 0) == 0) {
      readFromPaths = false;
    }

    else if (readFromPaths == true) {
      size_t separator = lines[i].find(" - ");
      string strPath   = lines[i].substr(0, separator);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path   = static_cast<fs::path>(strPath);
      string alias    = lines[i].substr(separator + 3);
      fromPaths[path] = alias;
      if (path != "")
        toPaths.push_back(path);
    }

    if (readFromPaths == false && lines[i].rfind("pictures_dir", 0) == 0) {
      size_t separator = lines[i].find(" = ");
      string strPath   = lines[i].substr(separator + 3);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      picturesPath  = path;
      if (path != "")
        toPaths.push_back(path);
    }

    if (readFromPaths == false && lines[i].rfind("sounds_dir", 0) == 0) {
      size_t separator = lines[i].find(" = ");
      string strPath   = lines[i].substr(separator + 3);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      soundsPath    = path;
      if (path != "")
        toPaths.push_back(path);
    }

    if (readFromPaths == false && lines[i].rfind("books_dir", 0) == 0) {
      size_t separator = lines[i].find(" = ");
      string strPath   = lines[i].substr(separator + 3);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      bookPath      = path;
      if (path != "")
        toPaths.push_back(path);
    }

    if (readFromPaths == false && lines[i].rfind("video_dir", 0) == 0) {
      size_t separator = lines[i].find(" = ");
      string strPath   = lines[i].substr(separator + 3);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      videoPath     = path;
      if (path != "")
        toPaths.push_back(path);
    }
  }
  configFile.close();
}