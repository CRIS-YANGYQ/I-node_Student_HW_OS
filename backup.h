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
    std::map<__time, std::string>getBackupPathMap();
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
