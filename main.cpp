#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;
using namespace std;

fs::path downloadsPath = "C:\\Users\\orriginalo\\Downloads";
fs::path desktopPath = "C:\\Users\\orriginalo\\Desktop";
fs::path picturesPath = "C:\\Users\\orriginalo\\Desktop\\Pictures";
fs::path soundsPath = "C:\\Users\\orriginalo\\Desktop\\Sounds";

vector<string> pictureExtentions = {".png", ".jpg", ".jpeg", ".webp", ".svg", ".tiff", ".heic", ".jfif", ".bmp", ".apng", ".avif", ".tif", ".tga", ".psd", ".eps", ".ai", ".indd", ".raw", ".ico"};
vector<string> soundExtentions = {".mp3", ".wav", ".flac", ".ogg", ".aac", ".m4a", ".wma", ".aiff", ".au", ".opus"};

map<fs::path, string> fromPaths;

int totalFilesSorted = 0;
fs::path path = "";
bool exitFromLoop = false;

void sortFiles(const fs::path&);
void createDirectoryIfNotExists(const fs::path&);
void loadConfig();
fs::path getPathByNumber(int&);


int main() {
  setlocale(LC_ALL, "ru_RU.UTF-8");
  vector<int> variantsArr;
  string variants;

  loadConfig();

  // for (const auto& pair : fromPaths) {
  //   cout << pair.first << " : " << pair.second << endl;
  // }

  cout << "Select from:" << endl;
  int counter = 1;
  for (const auto& pair : fromPaths) {
    cout << counter << ". " << pair.second << endl;
    counter++;
  }


  do {
    variantsArr = {};

    cout << "Enter: ";
    getline(cin, variants);
    stringstream ss(variants);

    int variant;
    while(ss >> variant) {
      fs::path path = getPathByNumber(variant);
      auto it = fromPaths.find(path);
      if(it != fromPaths.end()) {
        variantsArr.push_back(variant);
        exitFromLoop = true;
      }
      else {
        cout << "Variant " << variant << " doesnt exists" << endl;
        exitFromLoop = false;
      }
    }
    
    // for (auto& el : variantsArr) {
    //   cout << el;
    // }

    cout << endl;
  } while(!exitFromLoop);
  // getline(cin, variants); 

  createDirectoryIfNotExists(picturesPath);
  createDirectoryIfNotExists(soundsPath);

  for (int& var : variantsArr) {
    fs::path currentPath = getPathByNumber(var);
    sortFiles(currentPath);
  }





  if (totalFilesSorted >= 1) {
    cout << "File sorting is complete." << endl;
    cout << "Total sorted " << totalFilesSorted << " files." << endl;
  }
  else {
    cout << "There are no files to sort." << endl;
  }
  cout << "Press Enter to exit . . .";
  cin.get(); // Ждет нажатия клавиши Enter
}


fs::path getPathByNumber(int& num) {
  int counter = 1;
  if(num <= fromPaths.size() && num > 0) {
    for (const auto& pair : fromPaths) {
      if (counter == num) {
        return pair.first;
      }
      counter++;
    }
  }
  else {
    return "";
  }
}

void createDirectoryIfNotExists(const fs::path& path) {
  if (!fs::exists(path)) {
    try {
      fs::create_directory(path);
      cout << "Folder created: " << path << endl;
    } catch (const fs::filesystem_error& e) {
      cerr << "Failed to create folder" << path << ": " << e.what() << endl;
    }
  }
}

void sortFiles(const fs::path& src) {
  try {
      for(const auto& entry : fs::directory_iterator(src)) {
        try {
          auto extension = entry.path().extension();
          auto picturesIt = find(pictureExtentions.begin(), pictureExtentions.end(), extension);
          if(picturesIt != pictureExtentions.end()) {
            fs::rename(entry, picturesPath / entry.path().filename());
            totalFilesSorted++;
          }

          auto soundsIt = find(soundExtentions.begin(), soundExtentions.end(), extension);
          if(soundsIt != soundExtentions.end()) {
            fs::rename(entry, soundsPath / entry.path().filename());
            totalFilesSorted++;
          }
        }
        catch (fs::filesystem_error& e) {
          cout << "Error while transferring file " << entry.path().filename() << ": " << e.what() << endl;
        }
      }
    }
    catch (fs::filesystem_error& e) {
      cout << "Filesystem error: " << e.what() << endl;
    }
}

void loadConfig() {
  ifstream configFile("cppsorter.cfg");
  string line;
  vector<string> lines;

  bool readFromPaths = false;

  while(getline(configFile, line)) {
    lines.push_back(line);
  }

  for (int i = 0; i < lines.size(); i++) {
    if(lines[i].rfind("from_paths {", 0) == 0) {
      readFromPaths = true;
      continue;
    }

    if(lines[i].rfind("}", 0) == 0) {
      readFromPaths = false;
    }

    else if(readFromPaths == true) {
      size_t separator = lines[i].find(" - ");
      string strPath = lines[i].substr(0, separator);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      string alias = lines[i].substr(separator + 3);
      fromPaths[path] = alias;
    }

    if(readFromPaths == false && lines[i].rfind("pictures_dir", 0) == 0) {
      size_t separator = lines[i].find(" = ");
      string strPath = lines[i].substr(separator + 3);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      picturesPath = path;
    }

    if(readFromPaths == false && lines[i].rfind("sounds_dir", 0) == 0) {
      size_t separator = lines[i].find(" = ");
      string strPath = lines[i].substr(separator + 3);
      strPath.erase(remove(strPath.begin(), strPath.end(), '"'), strPath.end());
      fs::path path = static_cast<fs::path>(strPath);
      soundsPath = path;
    }
  }
  configFile.close();
}