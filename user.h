/*
* @file user.h
* @brief 定义用户结构体 User 和用户表类 userTable，提供对用户信息的输入输出运算符重载、用户表的初始化、数据库读写等功能。
*
* 该头文件声明了以下结构体和类：
*   - 结构体 User，包括用户的姓名、类别、ID、性别和优先级等信息，以及重载输入输出运算符
*   - 类 userTable，用于管理用户表，包括用户表的初始化、数据库读写、显示用户信息等功能
*
* 结构体 User 包括以下成员：
*   - std::string name：用户姓名
*   - std::string kind：用户类别
*   - std::string id：用户ID
*   - std::string gender：用户性别
*   - int priority：用户优先级，默认为-1
*
* 类 userTable 包括以下成员：
*   - int capacity：用户表容量
*   - int size：用户表当前大小
*   - std::vector<User> requestVector：存储用户信息的向量
*
* @杨裕钦 2023/11/14 17:25
*/


#ifndef _USER_H
#define _USER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>     // 文件的输入输出流操作
#include <iomanip>     // 对齐输出
#include <vector>
#include "parameters.h"
/**
 * @struct User
 * @brief 用户信息结构体，包括姓名、类别、ID、性别和优先级等信息
 */
struct User {
    std::string name;
    std::string kind;
    std::string id;
    std::string gender;
    int priority = -1;

    /**
     * @brief 重载输入运算符 >>，用于从流中读取用户信息到 User 结构体
     * @param in 输入流
     * @param user 要读取的用户信息结构体
     * @return 输入流
     */
    friend std::istream& operator >> (std::istream& in, User& user);

    /**
     * @brief 重载输出运算符 <<，用于将 User 结构体的用户信息输出到流
     * @param out 输出流
     * @param user 要输出的用户信息结构体
     * @return 输出流
     */
    friend std::ostream& operator << (std::ostream& out, const User& user);
};

/**
 * @class userTable
 * @brief 用户表类，用于管理用户信息，包括用户表的初始化、数据库读写、显示用户信息等功能
 */
class userTable {
private:
    int capacity;
    int size = 0;
    std::vector<User> userVector;

public:
    std::vector<User> infoVector;
    // 存储必要信息
    User initInfo(User origin);
    // 利用user_vector初始化infovector
    void initInfoVector();
    /**
     * @brief 构造函数，初始化用户表的容量
     * @param capacity 用户表的容量
     */
    userTable(int capacity = 0);

    /**
     * @brief 带参数的构造函数，初始化用户表的容量和用户信息向量
     * @param capacity 用户表的容量
     * @param requestVector 存储用户信息的向量
     */
    userTable(int capacity, std::vector<User> requestVector);

    /**
     * @brief 析构函数，释放资源
     */
    ~userTable();

    /**
     * @brief 初始化用户信息结构体
     * @param elements 包含用户信息的字符串向量
     * @return 初始化后的 User 结构体
     */
    User initUser(std::vector<std::string> elements);

    /**
     * @brief 初始化数据库，将用户信息写入文件
     * @param destinationPath 目标路径
     * @param databaseName 数据库名称
     */
    void initDatabase(std::string destinationPath, std::string databaseName);

    /**
     * @brief 从数据库文件中读取用户信息
     * @param destinationPath 目标路径
     * @param databaseName 数据库名称
     */
    void readDatabase(std::string destinationPath, std::string databaseName);

    /**
     * @brief 添加数据到数据库文件
     * @param newUser 新的用户
     * @param destinationPath 目标路径
     * @param databaseName 数据库名称
     */
    bool addDatabase(const User& newUser, std::string destinationPath, std::string databaseName) ;

    /**
     * @brief 显示用户信息向量
     */
    void displayUserVector();

    /**
     * @brief 根据用户ID搜索用户信息
     * @param id 用户ID
     * @return 包含搜索结果的用户信息向量
     */
    std::vector<User> searchUserByID(std::string id);

    /**
     * @brief 根据用户姓名搜索用户信息
     * @param name 用户姓名
     * @return 包含搜索结果的用户信息向量
     */
    std::vector<User> searchUserByName(std::string name);
    /**
     * @brief 根据用户ID搜索用户名字
     * @param id 用户ID
     * @return std::string 用户名字
     */
    std::string searchUserNameByID(std::string id);
};

#endif // !_USER_H
