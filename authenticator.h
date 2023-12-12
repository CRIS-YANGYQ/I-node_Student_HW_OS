/*
* @file Authenticator.h
* @brief 定义密码认证器类，用于检查用户密码输入的正确性
* 使用CSV文件存储密码信息。支持从文件加载密码本，将密码本保存到文件，添加用户和密码，以及验证密码。
*
* 该头文件声明了 PasswordAuthenticator 类，该类包括以下功能：
*   - 从CSV文件加载密码本
*   - 将密码本保存到CSV文件
*   - 添加用户ID和密码到密码本
*   - 验证用户密码的正确性
*   - 列出密码本中的所有用户ID和密码
*
*  注意：为了避免头文件被多次包含，采用了 #ifndef、#define 和 #endif 的条件编译宏。
* @杨裕钦 2023/11/13 18:00
*/
#ifndef _AUTHENTICATOR_H
#define _AUTHENTICATOR_H

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>						//文件的输出输出流操作
#include "parameters.h"
/**
 * @class PasswordAuthenticator
 * @brief 密码认证器类，实现用户密码验证和管理密码本
 */
class PasswordAuthenticator {
private:
    std::map<std::string, std::string> passwordMap; ///< 用于存储用户ID和对应密码的映射

public:
    /**
     * @brief 构造函数，可以选择从文件加载密码本
     * @param filename 密码本的文件名，默认为"database/passwords.csv"
     */
    PasswordAuthenticator(std::string folderPath = "database/", std::string filename = "passwords.csv");

    /**
     * @brief 增加用户和密码到密码本
     * @param userId 用户ID
     * @param password 用户密码
     */
    void addEntry(const std::string& userId, const std::string& password);

    /**
     * @brief 输出所有用户ID和密码的键值对
     */
    void listEntries() const;
    /**
     * @brief 检查密码是否正确
     * @param userId 用户ID
     * @param password 待检查的密码
     * @return 密码正确返回true，否则返回false
     */
    bool authenticate(const std::string& userId, const std::string& password) const;

    /**
     * @brief 保存密码本到CSV文件
     * @param filename 要保存的文件名
     */
    void savePasswordMapToFile(std::string folderPath, const std::string& filename);

    /**
     * @brief 从CSV文件加载密码本
     * @param filename 要加载的文件名
     */
    void loadPasswordMapFromFile(std::string folderPath , const std::string& filename);

    /**
     * @brief 从CSV文件加载密码本
     * @param userId 要更改的用户名
     * @param newPassword 更改后的密码
     */
    void modifyEntry(const std::string& userId, const std::string& newPassword);
};

/**
 * @function userAuthentic
 * @brief 密码认证函数，返回密码是否正确的判断
 */
bool userAuthentic(const std::string& enterdUserID, const std::string& enteredUserPwd, std::string folderPath, std::string filename);

#endif // !_AUTHENTICATOR_H
