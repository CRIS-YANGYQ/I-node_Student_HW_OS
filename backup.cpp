#include "backup.h"



backupCopy::backupCopy() {
    // Constructor implementation
}

backupCopy::~backupCopy() {
    // Destructor implementation
}

bool backupCopy::loadBackupCopyFromFile(const std::string filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        this->backupPathMap.clear(); // 清空已有的密码映射
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string backupTime, backupPath;

            // 使用 ',' 作为分隔符
            if (std::getline(iss, backupTime, ',') && std::getline(iss, backupPath, ',')) {
                // 移除 password 末尾的回车符
                if (!backupPath.empty() && backupPath.back() == '\r') {
                    backupPath.pop_back();
                }

                this->backupPathMap[localtime2__time(backupTime)] = backupPath;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open the file for loading." << std::endl;
    }

    
};
// TODO: 使用已经封装好的号
bool backupCopy::saveBackupCopyToFile(const std::string filename) {	
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& entry : this->backupPathMap) {
            file << entry.first << ',' << entry.second << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open the file for saving." << std::endl;
    }
};


bool backupCopy::copyFile(const std::string& source, const std::string& destination) {
    std::ifstream sourceFile(source, std::ios::binary);
    std::ofstream destFile(destination, std::ios::binary);

    destFile << sourceFile.rdbuf();

    return sourceFile && destFile;
}



bool backupCopy::createBackupCopy(__time backupTime) {
    const std::string original = databaseFolder;// ./database/
    std::cout<<backupTime.getTimeString()<<std::endl;
    const std::string backupPath = backupDatabasePath + backupTime.getTimeString() + "/";//./backup/2023 12 11 21 38 40
    std::cout<<backupPath<<std::endl;
    std::cout<<original<<std::endl;
    // 创建备份文件夹
    if (MKDIR(backupPath.c_str()) != 0) {
        std::cerr << "Failed to create backup directory: " << backupPath << std::endl;
        return false;
    }

    // 遍历原始文件夹中的所有文件
    #ifdef _WIN32
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile((original + "*").c_str(), &findFileData);
        if (hFind == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to open original directory: " << original << std::endl;
            return false;
        }

        do {
            const std::string fileName = findFileData.cFileName;
            const std::string sourcePath = original + fileName;
            const std::string destinationPath = backupPath + fileName;

            // 尝试复制文件
            if (!copyFile(sourcePath, destinationPath)) {
                std::cerr << "Failed to copy file: " << sourcePath << std::endl;
                FindClose(hFind);
                return false;
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    #else
        DIR* dir;
        struct dirent* entry;

        if ((dir = opendir(original.c_str())) != nullptr) {
            while ((entry = readdir(dir)) != nullptr) {
                const std::string fileName = entry->d_name;
                const std::string sourcePath = original + fileName;
                const std::string destinationPath = backupPath + fileName;

                // 尝试复制文件
                if (!copyFile(sourcePath, destinationPath)) {
                    std::cerr << "Failed to copy file: " << sourcePath << std::endl;
                    closedir(dir);
                    return false;
                }
            }

            closedir(dir);
        } else {
            std::cerr << "Failed to open original directory: " << original << std::endl;
            return false;
        }
    #endif

    std::cout << "Backup completed successfully." << std::endl;
    addEntry(backupTime, backupPath);
    return true;
}
    

bool backupCopy::deleteBackupCopy(__time backupTime) {
    // Implementation for deleteBackupCopy
    // ...
}

bool backupCopy::addEntry(__time backupTime, std::string backupPath) {
    this->backupPathMap[backupTime] = backupPath;
};
// 输出所有键值对

void backupCopy::listEntries() const {
    for (const auto& entry : this->backupPathMap) {
        std::cout << "Back up Time: " << entry.first << ", Saving Path: " << entry.second << std::endl;
    }
};
