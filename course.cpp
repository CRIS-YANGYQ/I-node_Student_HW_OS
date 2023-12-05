#include "course.h"

/**
 * @brief 重载输入运算符 >>，用于从流中读取选课关系信息到 Course2User 结构体
 * @param in 输入流
 * @param course2user 要读取的选课关系信息结构体
 * @return 输入流
 */
std::istream& operator >> (std::istream& in, Course2User& course2user) {
	in >> course2user.kind >> course2user.course_name >> course2user.course_id
		>> course2user.user_name >> course2user.user_id;
	return in;
}

/**
 * @brief 重载输出运算符 <<，用于将 Course2User 结构体的选课关系信息输出到流
 * @param out 输出流
 * @param course2user 要输出的选课关系信息结构体
 * @return 输出流
 */
std::ostream& operator << (std::ostream& out, const Course2User& course2user) {
	out << "kind: " << course2user.kind << "\tcourse_name: " << course2user.course_name
		<< "\tcourse_id: " << course2user.course_id << "\tuser_name: " << course2user.user_name
		<< "\tuser_id: " << course2user.user_id << std::endl;
	return out;
}
// 存储必要信息
Course2User Course2UserTable::initInfo(Course2User origin){
	Course2User temp;
	temp.course_id = origin.course_id;
	temp.user_id = origin.user_id;
	return temp;
}
// 利用rsel_vector初始化infovector
void Course2UserTable::initInfoVector(){
	for (int i = 0; i < this->size; ++i ){ 
		Course2User sel = this->selectionVector[i];
		Course2User sel_info = initInfo(sel);
		this->infoVector.emplace_back(sel_info);
	}
}
Course2UserTable::Course2UserTable(int capacity) {
	this->capacity = capacity;
	this->selectionVector = std::vector<Course2User>(capacity);
}
Course2UserTable::Course2UserTable(int capacity, std::vector<Course2User>homeworkVector) {
	this->capacity = capacity;
	this->selectionVector = homeworkVector;
}
Course2UserTable::~Course2UserTable()
{

}
Course2User Course2UserTable::initCourse2User(std::vector<std::string>elements) {
	Course2User temp;
	temp.kind = atoi(elements[0].c_str());
	temp.course_name = elements[1];
	temp.course_id = elements[2];
	temp.user_name = elements[3];
	temp.user_id = elements[4];

	return temp;

}
void Course2UserTable::initDatabase(std::string destinationPath, std::string databaseName) {
	std::ofstream outFile;
	outFile.open(destinationPath + databaseName, std::ios::out | std::ios::trunc);
		// 写入标题行
	outFile << "kind" << ','
		<< "course_name" << ','
		<< "course_id" << ','
		<< "user_name" << ','
		<< "user_id"
		<< std::endl;
	// ********写入两行数据*********
	// 写入字符串(数字)
	outFile << 1 << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "Doc Lee" << ','
		<< "20010060"
		<< std::endl;
	outFile << 0 << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "zhangsan" << ','
		<< "20210700"
		<< std::endl;
	outFile << 0 << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "lisi" << ','
		<< "20210800"
		<< std::endl;
	// 写入浮点数(转为字符串)
	//outFile << "lisi" << ','
	//	<< std::to_string(2032.1) << ','
	//	<< std::to_string(789.2) << ','
	//	<< "中国 陕西省" << std::endl;
	outFile.close();
}

// Win

// void Course2UserTable::readDatabase(std::string destinationPath, std::string databaseName) {
// 	std::ifstream csv_data(destinationPath + databaseName, std::ios::in);
// 	std::string line;
// 	if (!csv_data.is_open())
// 	{
// 		std::cout << "Error: opening file fail" << std::endl;
// 		std::exit(1);
// 	}
// 	// ��ȡ������
// 	std::getline(csv_data, line);
// 	// ��ȡ����
// 	int count_hw = 0;
// 	while (std::getline(csv_data, line))
// 	{
// 		std::istringstream lineStream;         //�������ַ���line���뵽�ַ���istringstream��
// 		std::vector<std::string> words; //����һ���ַ�������
// 		std::string word;
// 		lineStream.clear();
// 		lineStream.str(line);
// 		words.clear();
// 		while (std::getline(lineStream, word, ',')) //���ַ�����sin�е��ַ�����field�ַ����У��Զ���Ϊ�ָ���
// 		{
// 			words.push_back(word); //��ÿһ���е��������push
// 			// std::cout << word << " ";
// 			// std::cout << atol(word.c_str());
// 		}
// 		selectionVector[count_hw] = initCourse2User(words);
// 		// std::cout << std::endl;
// 		count_hw++;
// 		if (count_hw >= capacity) {
// 			std::cerr << "Variable 'count_hw' exceeds capacity of homeworkVector!" << std::endl;
// 			exit(0);
// 		}
// 	}
// 	this->size = count_hw;
// 	csv_data.close();
// }

// Linux
#include <algorithm> // 添加这行头文件

void Course2UserTable::readDatabase(std::string destinationPath, std::string databaseName) {
    std::ifstream csv_data(destinationPath + databaseName, std::ios::in);
    std::string line;

    if (!csv_data.is_open()) {
        std::cout << "Error: opening file fail" << std::endl;
        std::exit(1);
    }

    // 读取表头
    std::getline(csv_data, line);

    // 读取数据
    int count_hw = 0;
    while (std::getline(csv_data, line)) {
        // 移除行末尾的回车符
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        std::istringstream lineStream(line);
        std::vector<std::string> words;
        std::string word;

        words.clear();
        while (std::getline(lineStream, word, ',')) {
            words.push_back(word);
        }

        selectionVector[count_hw] = initCourse2User(words);

        count_hw++;
        if (count_hw >= capacity) {
            std::cerr << "Variable 'count_hw' exceeds capacity of homeworkVector!" << std::endl;
            exit(0);
        }
    }

    this->size = count_hw;
    csv_data.close();
}


void Course2UserTable::displaySelectionVector() {
	std::cout << "Selection Vector: " << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << selectionVector[i];
	}
}

std::vector<Course2User> Course2UserTable::searchSelectionByCourse(std::string course_id) {
	std::vector<Course2User> result;
	for (int i = 0; i < size; ++i) {
		Course2User temp = selectionVector[i];
		if (temp.course_id == course_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}

std::vector<Course2User> Course2UserTable::searchSelectionByUserId(std::string user_id) {
	std::vector<Course2User> result;
	for (int i = 0; i < size; ++i) {
		Course2User temp = selectionVector[i];
		if (temp.user_id == user_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}


std::string Course2UserTable::searchTeacherIDByCourseID(std::string course_id) {
	for (int i = 0; i < size; ++i) {
		Course2User temp = selectionVector[i];
		if (temp.course_id == course_id && temp.kind == 1) {
			// 查询教学老师
			return temp.user_id;
		}
	}
}
std::vector<std::string> Course2UserTable::searchStudentIDByCourseID(std::string course_id) {
	std::vector<std::string> results;
	for (int i = 0; i < size; ++i) {
		Course2User temp = selectionVector[i];
		if (temp.course_id == course_id && temp.kind == 0) {
			// 查询选课学生
			results.emplace_back(temp.user_id);
		}
	}
	return results;
}