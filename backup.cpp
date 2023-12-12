#include "backup.h"


bool createDirectory(const std::string& directoryPath) {
    int status = mkdir(directoryPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (status == 0) {
        std::cout << "Directory created successfully: " << directoryPath << std::endl;
        return true;
    } else {
        std::cerr << "Error creating directory: " << directoryPath << std::endl;
        return false;
    }
}

backupCopy::backupCopy() {
    // Constructor implementation
}

backupCopy::~backupCopy() {
    // Destructor implementation
}

bool backupCopy::loadBackupCopyFromFile(const std::string folderPath, const std::string filename) {

    
    std::ifstream file(folderPath + filename);
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
bool backupCopy::saveBackupCopyToFile(const std::string folderPath, const std::string filename) {
    // Check if the file exists, if not, create it
    std::ifstream fileCheck(folderPath + filename);
    if (!fileCheck) {
        std::ofstream createFile(folderPath + filename);
        if (!createFile) {
            std::cerr << "Error: creating file failed" << std::endl;
            std::exit(1);
        }
        createFile.close();
    }
    fileCheck.close();	
    std::ofstream file(folderPath + filename);
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





bool backupCopy::createBackupCopy(__time backupTime) {
    const std::string original = databaseFolder;// ./database/
    const std::string backupFolder = backupDatabasePath + backupTime.getTimeString() + "/";

    std::cout<<"Backup Operation Time: "<<backupTime.getTimeString()<<std::endl;
    if (createDirectory(backupFolder)){
        std::cout<<"Success create a dir!"<<std::endl;
    }
    else{
        std::cerr<<"Failed create a dir!"<<std::endl;
    }

    userTable user_table(MAX_USER_NUMBER);
	user_table.readDatabase(original, userTablePath);
	user_table.updateDatabase(backupFolder, userTablePath);

	Course2UserTable selection_table(MAX_SELECTION_NUMBER);
	selection_table.readDatabase(original, selectionTablePath);
    selection_table.updateDatabase(backupFolder, selectionTablePath);

	homeworkSet homework_table(MAX_HOMEWORK_NUMBER);
	homework_table.readDatabase(original, homeworkSetPath);
    homework_table.updateDatabase(backupFolder, homeworkSetPath);

	requestSet request_table(MAX_REQUEST_NUMBER);
	request_table.readDatabase(original, requestTablePath);
    request_table.updateDatabase(backupFolder, requestTablePath);


    PasswordAuthenticator authenticator(databaseFolder, passwordsPath);
    authenticator.loadPasswordMapFromFile(databaseFolder, passwordsPath);
    authenticator.savePasswordMapToFile(backupFolder, passwordsPath);

    addEntry(backupTime, backupFolder);
}
    

bool backupCopy::deleteEntery(__time backupTime) {
    auto it = backupPathMap.find(backupTime);
    if (it != backupPathMap.end()) {
        backupPathMap.erase(it);
        std::cout << "Backup Map Entry with key " << backupTime << " removed successfully." << std::endl;
    } else {
        std::cerr << "Backup Map Entry with key " << backupTime << " not found." << std::endl;
    }
}

bool backupCopy::addEntry(__time backupTime, std::string backupPath) {
    this->backupPathMap[backupTime] = backupPath;
};

std::string backupCopy::getPathByTime(const __time& backupTime) const {
    auto it = this->backupPathMap.find(backupTime);
    if (it != this->backupPathMap.end()) {
        return it->second; // 返回对应的值
    } else {
        std::cerr<<"Backup Map Entry with key " << backupTime << " not found." << std::endl;
        return ""; // 或者抛出异常，或者返回一个默认值，取决于你的需求
    }
}
__time backupCopy::showEntriesAndWait()  {

    for (const auto& entry : this->backupPathMap) {
        __time temp_key = entry.first;
        std::cout << "Key: " << temp_key.getTimeString() << ", Value: " << entry.second << std::endl;
        char userInput;
        std::cout << "Do you want to get this backup? (y/n): ";
        std::cin >> userInput;

        if (userInput == 'y') {
            return entry.first;  // 如果用户输入 'y'，返回当前值
        }
        // 继续展示下一个键值对
    }
    // 如果循环结束但用户没有选择任何值，可以根据你的需求返回一个默认值或者抛出异常
    __time null_time;
    null_time.year = 0;
    null_time.month = 0;
    null_time.date = 0;
    null_time.hour = 0;
    null_time.minute = 0;
    null_time.second = 0;
    return null_time;
}
// 输出所有键值对
void backupCopy::listEntries() const {
    for (const auto& entry : this->backupPathMap) {
        std::cout << "Back up Time: " << entry.first << "\t, Saving Path: \t" << entry.second << std::endl;
    }
};

bool backupCopy::tracebackDatabse(__time backupTime, const std::string folderPath, const std::string filename){
    const std::string original = databaseFolder;// ./database/
    __time nowtime = localtime2__time(getLocaltime());
    std::cout<<"Backup for current database: "<<backupTime.getTimeString()<<std::endl;
    createBackupCopy(nowtime);
    saveBackupCopyToFile(folderPath, filename);
    std::cout<<"Traceback Operation Time: "<<backupTime.getTimeString()<<std::endl;

    std::cout<<"getPathbyTime\n";
    std::string backupTargetPath = getPathByTime(backupTime);
    std::cout<<"userTable\n";
    userTable user_table(MAX_USER_NUMBER);
	user_table.readDatabase(backupTargetPath, userTablePath);
	user_table.updateDatabase(original, userTablePath);
    std::cout<<"selectionTable\n";
	Course2UserTable selection_table(MAX_SELECTION_NUMBER);
	selection_table.readDatabase(backupTargetPath, selectionTablePath);
    selection_table.updateDatabase(original, selectionTablePath);
    std::cout<<"homeworkTable\n";
	homeworkSet homework_table(MAX_HOMEWORK_NUMBER);
	homework_table.readDatabase(backupTargetPath, homeworkSetPath);
    homework_table.updateDatabase(original, homeworkSetPath);
    std::cout<<"requestTable\n";
	requestSet request_table(MAX_REQUEST_NUMBER);
	request_table.readDatabase(backupTargetPath, requestTablePath);
    request_table.updateDatabase(original, requestTablePath);


    PasswordAuthenticator authenticator(backupTargetPath, passwordsPath);
    authenticator.savePasswordMapToFile(original, passwordsPath);

}