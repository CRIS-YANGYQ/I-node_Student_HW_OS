#include "authenticator.h"
// authenticator.cpp
//std::string password_filename = passwordsPath;
// 构造函数，可以选择从文件加载密码本
PasswordAuthenticator::PasswordAuthenticator(std::string folderPath, std::string filename) {
	loadPasswordMapFromFile(folderPath, filename);
}

// 增加用户和密码到密码本
void PasswordAuthenticator::addEntry(const std::string& userId, const std::string& password) {
	passwordMap[userId] = password;
}

// 输出所有键值对
void PasswordAuthenticator::listEntries() const {
	for (const auto& entry : passwordMap) {
		std::cout << "User ID: " << entry.first << ", Password: " << entry.second << std::endl;
	}
}

// 检查密码是否正确
bool PasswordAuthenticator::authenticate(const std::string& userId, const std::string& password) const {
	auto it = passwordMap.find(userId);
	if (it != passwordMap.end() && it->second == password) {
		return true;
	}
	return false;
}

// 保存密码本到CSV文件
void PasswordAuthenticator::savePasswordMapToFile(std::string folderPath, const std::string& filename) {
	std::ofstream file(folderPath + filename);
	if (file.is_open()) {
		for (const auto& entry : passwordMap) {
			file << entry.first << ',' << entry.second << std::endl;
		}
		file.close();
	}
	else {
		std::cerr << "Unable to open the file for saving." << std::endl;
	}
}

// 从CSV文件加载密码本
void PasswordAuthenticator::loadPasswordMapFromFile(std::string folderPath, const std::string& filename) {
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

    std::ifstream file(folderPath + filename);
    if (file.is_open()) {
        passwordMap.clear(); // 清空已有的密码映射

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string userId, password;

            // 使用 ',' 作为分隔符
            if (std::getline(iss, userId, ',') && std::getline(iss, password, ',')) {
                // 移除 password 末尾的回车符
                if (!password.empty() && password.back() == '\r') {
                    password.pop_back();
                }

                passwordMap[userId] = password;
            }
        }

        file.close();
    }
    else {
        std::cerr << "Unable to open the file for loading." << std::endl;
    }
}

// void PasswordAuthenticator::loadPasswordMapFromFile(const std::string& filename) {
// 	std::ifstream file(filename);
// 	if (file.is_open()) {
// 		passwordMap.clear(); // ������е����뱾
// 		std::string line;
// 		while (std::getline(file, line)) {
// 			std::istringstream iss(line);
// 			std::string userId, password;
// 			if (std::getline(iss, userId, ',') && std::getline(iss, password)) {
// 				passwordMap[userId] = password;
// 			}
// 		}
// 		file.close();
// 	}
// 	else {
// 		std::cerr << "Unable to open the file for loading." << std::endl;
// 	}
// }

void PasswordAuthenticator::modifyEntry(const std::string& userId, const std::string& newPassword) {
	auto it = passwordMap.find(userId);
	if (it != passwordMap.end()) {
		// Modify the password for the given user
		it->second = newPassword;
		std::cout << "Password for user ID " << userId << " modified successfully." << std::endl;
	}
	else {
		std::cerr << "User ID " << userId << " not found. Unable to modify password." << std::endl;
	}
}

bool userAuthentic(const std::string& enterdUserID, const std::string& enteredUserPwd, std::string folderPath, std::string filename) {
	// 载入最新密码本
	PasswordAuthenticator authenticator;
	authenticator.loadPasswordMapFromFile(folderPath,filename);
	if (authenticator.authenticate(enterdUserID, enteredUserPwd)) {
		std::cout << "authentication successful." << std::endl;
		return true;
	}
	else {
		std::cout << "authentication failed." << std::endl;
		return false;
	}

}