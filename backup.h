#include <iostream>
#include <map>
#include <sstream>
#include <sys/stat.h>
// #ifdef _WIN32
//     #include <direct.h>
//     #include <windows.h>
//     #define MKDIR(path) _mkdir(path)
// #else
//     #include <sys/stat.h>
//     #include <sys/types.h>
//     #include <dirent.h>
//     #define MKDIR(path) mkdir(path, 0755)
//     #define RMDIR(path) rmdir(path)
// #endif
#include <fstream>						//文件的输出输出流操作
#include "authenticator.h"
#include "definedTime.h"
#include "homework.h"
#include "user.h"
#include "course.h"
bool createDirectory(const std::string& directoryPath) ;
class backupCopy {
private:
    std::map<__time, std::string> backupPathMap;

public:
    backupCopy();
    ~backupCopy();
    bool loadBackupCopyFromFile(const std::string folderPath, const std::string filename);
    bool saveBackupCopyToFile(const std::string folderPath, const std::string filename);
    bool createBackupCopy(__time backupTime);
    bool deleteEntery(__time backupTime);
    bool addEntry(const __time backupTime, const std::string backupPath);
    // 使用 find 函数来检查键是否存在，并返回对应的值
    std::string getPathByTime(const __time& backupTime) const ;
        // 展示每个键值对，等待用户输入 y/n
    __time showEntriesAndWait()  ;
    void listEntries() const;
    bool tracebackDatabse(const __time backupTime, const std::string folderPath, const std::string filename);
};
// class backupCopy{
// private:
//     std::map<__time, std::string> backupPathMap; ///< 用于存储时间与备份路径的映射表
// public:
//     backupCopy(){};
//     ~backupCopy(){};
//     bool loadBackupCopyFromFile(const std::string filename){
//         std::ifstream file(filename);
//         if (file.is_open()) {
//             this->backupPathMap.clear(); // 清空已有的密码映射
//             std::string line;
//             while (std::getline(file, line)) {
//                 std::istringstream iss(line);
//                 std::string backupTime, backupPath;

//                 // 使用 ',' 作为分隔符
//                 if (std::getline(iss, backupTime, ',') && std::getline(iss, backupPath, ',')) {
//                     // 移除 password 末尾的回车符
//                     if (!backupPath.empty() && backupPath.back() == '\r') {
//                         backupPath.pop_back();
//                     }

//                     this->backupPathMap[localtime2__time(backupTime)] = backupPath;
//                 }
//             }
//             file.close();
//         }
//         else {
//             std::cerr << "Unable to open the file for loading." << std::endl;
//         }

        
//     };
//     bool saveBackupCopyToFile(const std::string filename){	
//         std::ofstream file(filename);
//         if (file.is_open()) {
//             for (const auto& entry : this->backupPathMap) {
//                 file << entry.first << ',' << entry.second << std::endl;
//             }
//             file.close();
//         }
//         else {
//             std::cerr << "Unable to open the file for saving." << std::endl;
//         }
//     };
//     bool copyFile(const std::string& source, const std::string& destination) {
//         std::ifstream sourceFile(source, std::ios::binary);
//         std::ofstream destFile(destination, std::ios::binary);

//         destFile << sourceFile.rdbuf();

//         return sourceFile && destFile;
//     }


//     bool createBackupCopy(__time backupTime){
//         const std::string original = databaseFolder;// ./database/
//         std::cout<<backupTime.getTimeString()<<std::endl;
//         const std::string backupPath = backupDatabasePath + backupTime.getTimeString() + "/";//./backup/2023 12 11 21 38 40
//         // 创建备份文件夹
//         if (MKDIR(backupPath.c_str()) != 0) {
//             std::cerr << "Failed to create backup directory: " << backupPath << std::endl;
//             return false;
//         }

//         // 遍历原始文件夹中的所有文件
//         #ifdef _WIN32
//             WIN32_FIND_DATA findFileData;
//             HANDLE hFind = FindFirstFile((original + "*").c_str(), &findFileData);
//             if (hFind == INVALID_HANDLE_VALUE) {
//                 std::cerr << "Failed to open original directory: " << original << std::endl;
//                 return false;
//             }

//             do {
//                 const std::string fileName = findFileData.cFileName;
//                 const std::string sourcePath = original + fileName;
//                 const std::string destinationPath = backupPath + fileName;

//                 // 尝试复制文件
//                 if (!copyFile(sourcePath, destinationPath)) {
//                     std::cerr << "Failed to copy file: " << sourcePath << std::endl;
//                     FindClose(hFind);
//                     return false;
//                 }
//             } while (FindNextFile(hFind, &findFileData) != 0);

//             FindClose(hFind);
//         #else
//             DIR* dir;
//             struct dirent* entry;

//             if ((dir = opendir(original.c_str())) != nullptr) {
//                 while ((entry = readdir(dir)) != nullptr) {
//                     const std::string fileName = entry->d_name;
//                     const std::string sourcePath = original + fileName;
//                     const std::string destinationPath = backupPath + fileName;

//                     // 尝试复制文件
//                     if (!copyFile(sourcePath, destinationPath)) {
//                         std::cerr << "Failed to copy file: " << sourcePath << std::endl;
//                         closedir(dir);
//                         return false;
//                     }
//                 }

//                 closedir(dir);
//             } else {
//                 std::cerr << "Failed to open original directory: " << original << std::endl;
//                 return false;
//             }
//         #endif

//         std::cout << "Backup completed successfully." << std::endl;
//         addEntry(backupTime, backupPath);
//         return true;
//     }
    
//     bool deleteBackupCopy(__time backupTime);
//     bool addEntry(const __time backupTime, const std::string backupPath) {
//         this->backupPathMap[backupTime] = backupPath;
//     };
//     // 输出所有键值对
//     void listEntries() const {
//         for (const auto& entry : this->backupPathMap) {
//             std::cout << "Back up Time: " << entry.first << ", Saving Path: " << entry.second << std::endl;
//         }
//     };
// };
