#include "user.h"


// 重载输入运算符 >>，用于从流中读取用户信息到 User 结构体
std::istream& operator >> (std::istream& in, User& user) {
    in >> user.name >> user.kind >> user.id >> user.gender
        >> user.priority;
    return in;
}

// 重载输出运算符 <<，用于将 User 结构体的用户信息输出到流
std::ostream& operator << (std::ostream& out, const User& user) {
    out << "name: " << user.name << "\tkind: " << user.kind
        << "\tid: " << user.id << "\tgender: " << user.gender
        << "\tpriority: " << user.priority << std::endl;
    return out;
}
// 保存必要信息
User userTable::initInfo(User origin){
    User temp;
    temp.id = origin.id;
    return temp;
}
// 利用req_vector初始化infovector
void userTable::initInfoVector(){
    for (int i = 0; i < this->size; ++i ){ 
        User user = this->userVector[i];
        User user_info = initInfo(user);
        this->infoVector.emplace_back(user_info);
    }
}

// 构造函数，初始化用户表的容量
userTable::userTable(int capacity) {
    this->capacity = capacity;
    this->userVector = std::vector<User>(capacity);
}

// 带参数的构造函数，初始化用户表的容量和用户信息向量
userTable::userTable(int capacity, std::vector<User> userVector) {
    this->capacity = capacity;
    this->userVector = userVector;
}

// 析构函数，释放资源
userTable::~userTable() {}

// 初始化用户信息结构体
User userTable::initUser(std::vector<std::string> elements) {
    User temp;
    temp.name = elements[0];
    temp.kind = elements[1];
    temp.id = elements[2];
    temp.gender = elements[3];
    temp.priority = atoi(elements[4].c_str());
    return temp;
}

// 初始化数据库，将用户信息写入文件
void userTable::initDatabase(std::string destinationPath, std::string databaseName) {
    std::ofstream outFile;
    outFile.open(destinationPath + databaseName, std::ios::out | std::ios::trunc);

    // 写入标题行
    outFile << "user name" << ','
        << "user kind" << ','
        << "user id" << ','
        << "user gender" << ','
        << "user priority"
        << std::endl;

    // 写入两行数据
    outFile << "zhangsan" << ','
        << "student" << ','
        << "20210700" << ','
        << "Male" << ','
        << 1 << std::endl;

    outFile << "lisi" << ','
        << "student" << ','
        << "20210800" << ','
        << "Male" << ','
        << 1 << std::endl;

    outFile << "Doc Lee" << ','
        << "teacher" << ','
        << "20010060" << ','
        << "Female" << ','
        << 2 << ','
        << std::endl;

    outFile.close();
}

// 从数据库文件中读取用户信息
void userTable::readDatabase(std::string destinationPath, std::string databaseName) {
    std::ifstream csv_data(destinationPath + databaseName, std::ios::in);
    std::string line;
    std::cout << "Read User Database"<< destinationPath + databaseName << std::endl;
    if (!csv_data.is_open()) {
        std::cout << "Error: opening file fail" << std::endl;
        std::exit(1);
    }

    // ��ȡ������
    std::getline(csv_data, line);

    // ��ȡ����
    int count_hw = 0;
    while (std::getline(csv_data, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        std::istringstream lineStream;         // 将整行字符串 line 读入到字符串 istringstream 中
        std::vector<std::string> words;        // 声明一个字符串向量
        std::string word;
        lineStream.clear();
        lineStream.str(line);
        words.clear();
        while (std::getline(lineStream, word, ',')) {
            words.push_back(word);             // 将每一格中的数据逐个 push
        }
        userVector[count_hw] = initUser(words);
        count_hw++;
        if (count_hw >= capacity) {
            std::cerr << "Variable 'count_hw' exceeds capacity of userVector!" << std::endl;
            exit(0);
        }
    }
    this->size = count_hw;
    csv_data.close();
}

void userTable::updateDatabase(std::string destinationPath, std::string databaseName) {
    // Check if the file exists, if not, create it
    std::ifstream fileCheck(destinationPath + databaseName);
    if (!fileCheck) {
        std::ofstream createFile(destinationPath + databaseName);
        if (!createFile) {
            std::cerr << "Error: creating file failed" << std::endl;
            std::exit(1);
        }
        createFile.close();
    }
    fileCheck.close();

    std::ofstream outFile;
    outFile.open(destinationPath + databaseName, std::ios::out | std::ios::trunc);

    // Write CSV column names
    outFile << "user name" << ',' << "user kind" << ',' << "user id" << ',' << "user gender" << ',' << "user priority" << std::endl;

    // Write user information
    for (int i = 0; i < this->size; ++i) {
        outFile << userVector[i].name << ','
                << userVector[i].kind << ','
                << userVector[i].id << ','
                << userVector[i].gender << ','
                << userVector[i].priority
                << std::endl;
    }

    outFile.close();
}
/**
 * @brief 从用户向量中删除具有指定用户 ID 的用户
 * @param user_id 要删除的用户的 ID
 * @return 如果成功删除用户，则返回 true；否则返回 false
 */
bool userTable::deleteUserFromVector(std::string user_id){
    for(int i = 0; i < this->size; ++i){
        if(this->userVector[i].id == user_id){
            this->userVector.erase(this->userVector.begin() + i);
            this->size--;
            return true;
        }
    }
    return false;
}
// 显示用户信息向量
void userTable::displayUserVector() {
    std::cout << "User Vector: " << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cout << userVector[i];
    }
}
bool userTable::addDatabase(const User& newUser, std::string destinationPath, std::string databaseName) {
    std::ofstream outFile;
    outFile.open(destinationPath + databaseName, std::ios::out | std::ios::app); // Open in append mode

    if (!outFile.is_open()) {
        std::cerr << "Error: opening file failed" << std::endl;
        return false;
    }

    // Write the new data to the file
    outFile << newUser.name << ','
            << newUser.kind << ','
            << newUser.id << ','
            << newUser.gender << ',' 
            << newUser.priority << std::endl;

    outFile.close();
    return true;

}
/**
 * @brief Checks if a given user ID exists in the database.
 *
 * @param user_id The ID of the user.
 * @return bool Returns true if the user ID exists, false otherwise.
 */
bool userTable::isUserIDexists(std::string user_id){
    for(int i = 0; i < this->size; ++i){
        if(this->userVector[i].id == user_id){
            return true;
        }
    }
    return false;
}

// 根据用户ID搜索用户信息
std::vector<User> userTable::searchUserByID(std::string id) {
    std::vector<User> result;
    for (int i = 0; i < size; ++i) {
        User temp = userVector[i];
        if (temp.id == id) {
            result.emplace_back(temp);
        }
    }
    return result;
}

// 根据用户姓名搜索用户信息
std::vector<User> userTable::searchUserByName(std::string name) {
    std::vector<User> result;
    for (int i = 0; i < size; ++i) {
        User temp = userVector[i];
        if (temp.name == name) {
            result.emplace_back(temp);
        }
    }
    return result;
}
/**
 * @brief 根据用户ID搜索用户名字
 * @param id 用户ID
 * @return std::string 用户名字
 */
std::string userTable::searchUserNameByID(std::string id){
    std::vector<User> result = searchUserByID(id);
    if (result.size() == 0) {
        return "";
    }
    return result[0].name;
}