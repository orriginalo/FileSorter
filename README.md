<img src="https://img.shields.io/github/release/orriginalo/FileSorter"/>

[English readme](https://github.com/orriginalo/FileSorter/blob/main/README.md) • [Русский readme](https://github.com/orriginalo/FileSorter/blob/main/README.ru.md)

# File Sorter

## Description

File Sorter is a program designed to organize files into categories (images, audio, books, videos, etc.) by specifying source and destination folders. The program supports a configuration file for setting paths and can operate in recursive mode.

---

## Features

- Sort files by extensions.
- Configuration file support.
- Operation logging.
- Recursive mode support.
- Automatic creation of directories if they do not exist.

---

## Installation

### 1. Clone the repository:

```bash
git clone https://github.com/orriginalo/FileSorter.git
cd FileSorter
```

### 2. Compile the program:

Make sure you have a C++ compiler with C++17 or later support installed.

```bash
g++ -std=c++17 -Iinclude -o filesorter src/main.cpp src/argparser.cpp
```

---

## Usage

### 1. Run the program

```bash
./file_sorter
```

### 2. Configuration

On the first run, the program will automatically create the `filesorter.cfg` file if it does not exist.

#### Example `filesorter.cfg`:

```cfg
# Specify source paths:
from_paths {
  "C:\\Users\\User\\Downloads" - Downloads
  "C:\\Users\\User\\Desktop" - Desktop
}

# Specify destination paths:
pictures_dir = "C:\\Users\\User\\Pictures"
sounds_dir = "C:\\Users\\User\\Music"
video_dir = "C:\\Users\\User\\Videos"
books_dir = "C:\\Users\\User\\Books"
```

#### Configuration Details

- In the `from_paths` section, specify the paths to the folders to sort files from and assign their aliases.
- Specify the destination paths for different file categories (images, music, videos, books).

### 3. Program Interface

After starting, the program will prompt you to:

1. Select source folders.
2. Enable or disable recursive sorting.

### 4. Logging

All file movements are recorded in the `logs` folder. Logs are created with unique names based on the current date and time.

---

## Supported Formats

- **Images**: `.png`, `.jpg`, `.jpeg`, `.webp`, `.svg`, etc.
- **Music**: `.mp3`, `.wav`, `.flac`, `.ogg`, etc.
- **Books**: `.pdf`, `.epub`, `.mobi`, `.cbz`, etc.
- **Videos**: `.mp4`, `.mkv`, `.avi`, `.webm`, etc.

---

## Notes

- The program works only in Windows and Linux environments.
- Ensure that the folder paths are correct and writable.

---

## License

This project is licensed under the MIT License. Details can be found in the `LICENSE` file.
