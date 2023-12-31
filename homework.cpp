#include"homework.h"



/**
	* @brief 构造函数，初始化作业集合容量
	* @param capacity 作业集合的容量
	*/
homeworkSet::homeworkSet(int capacity) {
	this->capacity = capacity;
	this->homeworkVector = std::vector<homework>(capacity);
}

/**
	* @brief 构造函数，初始化作业集合容量和作业信息向量
	* @param capacity 作业集合的容量
	* @param homeworkVector 作业信息向量
	*/
homeworkSet::homeworkSet(int capacity, std::vector<homework> homeworkVector) {
	this->capacity = capacity;
	this->homeworkVector = homeworkVector;
}

/**
	* @brief 析构函数，释放资源
	*/
homeworkSet::~homeworkSet() {}
int homeworkSet::getCapacity() { return this->capacity; }
int homeworkSet::getSize() { return this->size; }
std::vector<homework> homeworkSet::getHomeworkVector(){
	std::vector<homework> return_request_vector;
	for(int i = 0; i < this->size; ++i){
		return_request_vector.emplace_back(this->homeworkVector[i]);
	}
	return return_request_vector;
}
// 利用hwvector初始化infovector
homework homeworkSet::initInfo(homework origin) {
	homework info;
	info.student_id = origin.student_id;
	info.teacher_id = origin.teacher_id;
	info.course_id = origin.course_id;
	info.hw_id = origin.hw_id;
	return info;
}

// 拷贝必要信息，去除其他信息，使用原始vec初始化info_vec
void homeworkSet::initInfoVector() {
	for ( int i = 0; i < this->size; ++i ) {
		auto hw = this->homeworkVector[i];
		homework hw_info = initInfo(hw);
		this->infoVector.emplace_back(hw_info);
	}
}

// 存储必要info到csv
void homeworkSet::updateInfoDatabase(std::string destinationPath, std::string databaseName) {
	std::ofstream outFile;
	outFile.open(destinationPath + databaseName, std::ios::out | std::ios::trunc);
	//д�������
	outFile 
		<< "student id" << ','
		<< "teacher id" << ','
		<< "course id" << ','
		<< "homework id" 
		<< std::endl;
	//д������
	for (int i = 0; i < this->infoVector.size(); ++i) {
		outFile
			<< this->infoVector[i].student_id << ','
			<< this->infoVector[i].teacher_id << ','
			<< this->infoVector[i].course_id << ','
			<< this->infoVector[i].hw_id
			<< std::endl;

	}
	outFile.close();

}

// 读取必要info从csv
void homeworkSet::loadInfoDatabase(std::string destinationPath, std::string databaseName) {


    // Open the file for reading
    std::ifstream csv_data(destinationPath + databaseName, std::ios::in);
    std::string line;
    this->infoVector.clear();

    if (!csv_data.is_open()) {
        std::cerr << "Error: opening file fail" << std::endl;
        std::exit(1);
    }

    // 读取标题行
    std::getline(csv_data, line);

    // 读取数据行
    while (std::getline(csv_data, line)) {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        std::istringstream lineStream;
        std::vector<std::string> words;
        std::string word;

        lineStream.clear();
        lineStream.str(line);
        words.clear();

        while (std::getline(lineStream, word, ',')) {
            words.push_back(word);
        }

        this->infoVector.emplace_back(initHomework(words));
    }

    csv_data.close();
}
/**
	* @brief ��ʼ����ҵ��Ϣ�ṹ��
	* @param elements ������ҵ��Ϣ���ַ�������
	* @return ��ʼ�������ҵ��Ϣ�ṹ��
	*/
homework homeworkSet::initHomework(std::vector<std::string> elements) {
	homework temp;
	temp.submitTime = localtime2__time(elements[0]);
	temp.student_name = elements[1];
	temp.student_id = elements[2];
	temp.teacher_name = elements[3];
	temp.teacher_id = elements[4];
	temp.course_name = elements[5];
	temp.course_id = elements[6];
	temp.title = elements[7];
	temp.request_id = elements[8];
	temp.hw_id = elements[9];
	temp.content = elements[10];
	if (elements[11] == "1")
		temp.isMarked = true;
	else if (elements[11] == "0")
		temp.isMarked = false;
	temp.grade = atoi(elements[12].c_str());
	return temp;
}

/**
	* @brief 初始化作业信息结构体
	* @param elements 包含作业信息的字符串向量
	* @return 初始化后的作业信息结构体
	*/
void homeworkSet::initDatabase(std::string destinationPath, std::string databaseName) {
	std::ofstream outFile;
	outFile.open(destinationPath + databaseName, std::ios::out | std::ios::trunc);
	// д�������
	outFile << "submit time" << ','
		<< "student name" << ','
		<< "student id" << ','
		<< "teacher name" << ','
		<< "teacher id" << ','
		<< "course name" << ','
		<< "course id" << ','
		<< "homework title" << ','
		<< "request_id" << ','
		<< "homework id" << ','
		<< "homework content" << ','
		<< "isMarked" << ','
		<< "grade" << std::endl;
	// д������
	outFile << "2023 10 05 15 25 47" << ','
		<< "zhangsan" << ','
		<< "20210700" << ','
		<< "Doc Lee" << ','
		<< "20010060" << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "OS_hw1" << ','
		<< "OShw_20210700_01" << ','
		<< "CS0710001" << ','
		<< "URL_1" << ','
		<< true << ','
		<< 89 << std::endl;
	outFile << "2023 11 21 08 41 02" << ','
		<< "lisi" << ','
		<< "20210800" << ','
		<< "Doc Lee" << ','
		<< "20010060" << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "OS_hw2" << ','
		<< "OShw_20210800_08" << ','
		<< "CS0710002" << ','
		<< "URL_2" << ','
		<< false << ','
		<< 0 << std::endl;
	outFile.close();
}

/**
	* @brief 从数据库文件中读取作业信息
	* @param destinationPath 目标路径
	* @param databaseName 数据库文件名
	*/
void homeworkSet::readDatabase(std::string destinationPath, std::string databaseName) {
	std::ifstream csv_data(destinationPath + databaseName, std::ios::in);
	std::string line;

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
		std::istringstream lineStream;         // �������ַ���line���뵽�ַ���istringstream��
		std::vector<std::string> words;        // ����һ���ַ�������
		std::string word;
		lineStream.clear();
		lineStream.str(line);
		words.clear();
		while (std::getline(lineStream, word, ','))  // ���ַ�����sin�е��ַ�����field�ַ����У��Զ���Ϊ�ָ���
		{
			words.push_back(word);  // ��ÿһword����
			// std::cout << word << " ";
			// std::cout << atol(word.c_str());
		}
		homeworkVector[count_hw] = initHomework(words);
		// std::cout << std::endl;

		count_hw++;
		if (count_hw >= capacity) {
			std::cerr << "Variable 'count_hw' exceeds capacity of homeworkVector!" << std::endl;
			exit(0);
		}
	}
	this->size = count_hw;
	csv_data.close();

}
/**
* @brief Updates the database with homework information.
*
* @param destinationPath The path where the database will be saved.
* @param databaseName The name of the database file.
* @return void
*/
void homeworkSet::updateDatabase(std::string destinationPath, std::string databaseName) {
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

	// Open the file for reading
	std::ofstream outFile;
	std::cout << "Update HomeworkSet DB path:" << destinationPath + databaseName << std::endl;
	outFile.open(destinationPath + databaseName, std::ios::out | std::ios::trunc);
		//д�������
	outFile << "submit time" << ','
		<< "student name" << ','
		<< "student id" << ','
		<< "teacher name" << ','
		<< "teacher id" << ','
		<< "course name" << ','
		<< "course id" << ','
		<< "homework title" << ','
		<< "request_id" << ','
		<< "homework id" << ','
		<< "homework content" << ','
		<< "isMarked" << ','
		<< "grade"
		<< std::endl;
		//д������
	for (int i = 0; i < this->size; ++i) {
		outFile << homeworkVector[i].submitTime << ','
			<< homeworkVector[i].student_name << ','
			<< homeworkVector[i].student_id << ','
			<< homeworkVector[i].teacher_name << ','
			<< homeworkVector[i].teacher_id << ','
			<< homeworkVector[i].course_name << ','
			<< homeworkVector[i].course_id << ','
			<< homeworkVector[i].title << ','
			<< homeworkVector[i].request_id << ','
			<< homeworkVector[i].hw_id << ','
			<< homeworkVector[i].content << ',';
		if (homeworkVector[i].isMarked)
			outFile << 1 << ',';
		else
			outFile << 0 << ',';
		outFile << homeworkVector[i].grade << std::endl;

	}
	outFile.close();

}
/**
 * @brief Add the database with homework vector.
 *
 * @param homework_vector The vector to be added to the database.
 * @param destinationPath The path where the database will be saved.
 * @param databaseName The name of the database file.
 * @return void
 */
void homeworkSet::appendDatabase(std::vector<homework> homework_vector, std::string destinationPath, std::string databaseName){
    std::ofstream outFile;
    outFile.open(destinationPath + databaseName, std::ios::out | std::ios::app); // Open in append mode

    for (const auto &hw : homework_vector) {
        outFile << hw.submitTime << ','
                << hw.student_name << ','
                << hw.student_id << ','
                << hw.teacher_name << ','
                << hw.teacher_id << ','
                << hw.course_name << ','
                << hw.course_id << ','
                << hw.title << ','
                << hw.request_id << ','
                << hw.hw_id << ','
                << hw.content << ',';

        if (hw.isMarked)
            outFile << 1 << ',';
        else
            outFile << 0 << ',';

        outFile << hw.grade << std::endl;
    }

    outFile.close();
}

void homeworkSet::writeVector(const std::vector<homework>& targetVector) {
	for (const auto& ele : targetVector) {
		int temp_index = this->searchRequestIndeByHomeworkID(ele.hw_id);
		if (temp_index == -1) {
			int count_hw = 0;
			if (this->size >= this->capacity) {
				std::cerr << "Variable 'count_hw' exceeds capacity of homeworkVector!" << std::endl;
				exit(0);
			}
			this->homeworkVector[this->size] = ele;
			this->size++;
		}
		else {
			this->homeworkVector[temp_index] = ele;
		}
	}
}
/**
 * @brief 根据作业ID删除作业信息
 * @param HW_id 作业ID
 * @return 如果成功删除返回true，否则返回false
 */
bool homeworkSet::deleteHomeworkFromVectorByHWID(std::string HW_id) {
	// 使用迭代器遍历容器
	bool tmp = false;
	for (auto it = this->homeworkVector.begin(); it != this->homeworkVector.end();) {
		// 检查关系式
		if (it->hw_id == HW_id) {
			// 删除满足条件的元素，并更新迭代器
			it = this->homeworkVector.erase(it);
			this->size--;
			tmp = true;
		} else {
			// 没有删除元素时，继续迭代
			++it;
		}
	}
	return tmp;
}
/**
 * @brief 根据用户ID删除作业信息
 * @param Student_id 学生ID
 * @return 如果成功删除返回true，否则返回false
 */
bool homeworkSet::deleteHomeworkFromVectorByStudentID(std::string Student_id){
	// 使用迭代器遍历容器
	bool tmp = false;
	for (auto it = this->homeworkVector.begin(); it != this->homeworkVector.end();) {
		// 检查关系式
		if (it->student_id == Student_id) {
			// 删除满足条件的元素，并更新迭代器
			it = this->homeworkVector.erase(it);
			this->size--;
			tmp = true;
		} else {
			// 没有删除元素时，继续迭代
			++it;
		}
	}
	return tmp;
}
/**
 * @brief 根据用户ID删除作业信息
 * @param Student_id 学生ID
 * @return 如果成功删除返回true，否则返回false
 */
bool homeworkSet::deleteHomeworkFromVectorByStudentIDandCourseID(std::string Student_id, std::string Course_id){
	// 使用迭代器遍历容器
	bool tmp = false;
	for (auto it = this->homeworkVector.begin(); it != this->homeworkVector.end();) {
		// 检查关系式
		if (it->student_id == Student_id && it->course_id == Course_id) {
			// 删除满足条件的元素，并更新迭代器
			it = this->homeworkVector.erase(it);
			this->size--;
			tmp = true;
		} else {
			// 没有删除元素时，继续迭代
			++it;
		}
	}
	return tmp;
}
/**
	* @brief Displays the homework vector.
	*
	* @return void
	*/
void homeworkSet::displayHomeworkVector() {
	std::cout << "homework Vector: " << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << homeworkVector[i];
	}
}
std::string homeworkSet::getGradeStats(std::string student_id) {
    int marked_hw_count = 0, grade_sum = 0, failed_count = 0;
    std::vector<homework> student_homework_vector = searchHomeworkByStudentID(student_id);
    std::string result;
	if (student_homework_vector.size()==0){
		result = "You have no homework!";
        return result;
	}

    for (int i = 0; i < student_homework_vector.size(); ++i) {
        if (student_homework_vector[i].isMarked == 1) {
            marked_hw_count++;
            grade_sum += student_homework_vector[i].grade;
            if (student_homework_vector[i].grade < 60) {
                failed_count++;
            }
        }
    }

    result += "You have submitted " + std::to_string(student_homework_vector.size()) +
              " homeworks, " + std::to_string(student_homework_vector.size() - marked_hw_count) +
              " of which have not been marked by your teacher by now.\n";

    if (marked_hw_count > 0) {
        result += "In the marked homeworks, you've got a mean score " +
                  std::to_string((marked_hw_count == 0) ? 0 : grade_sum / marked_hw_count) + ".\n";
    }

    if (failed_count == 0) {
        result += "None of your homework has failed.";
    } else {
        result += "The number of your homework which failed is " + std::to_string(failed_count);
    }

    return result;
}


/**
	* @brief Displays grade statistics for a given student.
	*
	* @param homework_id The ID of the student.
	* @return void
	*/
void homeworkSet::displayGradeStats(std::string student_id) {
	int marked_hw_count = 0, grade_sum = 0, failed_count = 0;
	std::vector<homework> student_homework_vector = searchHomeworkByStudentID(student_id);
	for (int i = 0; i < student_homework_vector.size(); ++i) {
		if (student_homework_vector[i].isMarked == 1) {
			marked_hw_count++;
			grade_sum += student_homework_vector[i].grade;
			if (student_homework_vector[i].grade < 60) {
				failed_count++;
			}
		}
	}
	printf("You have submitted %lld homeworks, %lld of which have not been marked by your teacher by now.\nIn the marked homeworks, you've got a mean score %d.\n",
		student_homework_vector.size(), student_homework_vector.size() - marked_hw_count, grade_sum / marked_hw_count);
	if (failed_count == 0) {
		printf("None of your homework has failed.");
	}
	else {
		printf("The number of your homework whichi failed is %d", failed_count);
	}

}
std::string homeworkSet::getGradeStats(std::string student_id, std::string course_id) {
    int marked_hw_count = 0, target_homework_num = 0, grade_sum = 0, failed_count = 0;
    std::string course_name;
    std::vector<homework> student_homework_vector = searchHomeworkByStudentID(student_id);
    for (int i = 0; i < student_homework_vector.size(); ++i) {
        if (student_homework_vector[i].course_id != course_id) {
            // Non-matching course, skip
            continue;
        }
        target_homework_num++;
        if (student_homework_vector[i].isMarked == 1) {
            marked_hw_count++;
            grade_sum += student_homework_vector[i].grade;
            if (student_homework_vector[i].grade < 60) {
                failed_count++;
            }
        }
        course_name = student_homework_vector[i].course_name;
    }
    if (target_homework_num == 0) {
        return "No homework submitted for the specified course.";
    }

    std::string result = "ID: " + course_id + " Name: " + course_name + "\n";

    if (marked_hw_count == 0) {
        result += "You have submitted " + std::to_string(target_homework_num) + " homeworks, none of which has already been marked!\n";
    } else {
        result += "You have submitted " + std::to_string(target_homework_num) +
                  " homeworks, " + std::to_string(target_homework_num - marked_hw_count) +
                  " of which have not been marked by your teacher by now.\n";

        if (marked_hw_count > 0) {
            result += "In the marked homeworks, you've got a mean score " +
                      std::to_string((grade_sum == 0) ? 0 : grade_sum / marked_hw_count) + ".\n";
        }

        if (failed_count == 0) {
            result += "None of your homework has failed.\n";
        } else {
            result += "The number of your homework which failed is " + std::to_string(failed_count) + ".\n";
        }
    }

    return result;
}

void homeworkSet::displayGradeStats(std::string student_id, std::string course_id){
	
	int marked_hw_count = 0, target_homework_num = 0, grade_sum = 0, failed_count = 0;
	std::string course_name;
	std::vector<homework> student_homework_vector = searchHomeworkByStudentID(student_id);
	for (int i = 0; i < student_homework_vector.size(); ++i) {
		if(student_homework_vector[i].course_id != course_id){
			// 非本课程跳过
			continue;
		}
		target_homework_num++;
		if (student_homework_vector[i].isMarked == 1) {
			marked_hw_count++;
			grade_sum += student_homework_vector[i].grade;
			if (student_homework_vector[i].grade < 60) {
				failed_count++;
			}
		}
		course_name = student_homework_vector[i].course_name;
	}
	if (target_homework_num==0) {return;}
	std::cout<<std::endl<<"ID: "<< course_id<<" Name: "<<course_name<<std::endl;

	if (marked_hw_count==0){
		printf("You have submitted %lld homeworks, none of which has already been marked!\n", target_homework_num);
		return;
	}
	printf("You have submitted %lld homeworks, %lld of which have not been marked by your teacher by now.\nIn the marked homeworks, you've got a mean score %d.\n",
		target_homework_num, target_homework_num - marked_hw_count, ((marked_hw_count==0)?0:grade_sum / marked_hw_count));
	if (failed_count == 0) {
		printf("None of your homework has failed.\n");
	}
	else {
		printf("The number of your homework whichi failed is %d.\n", failed_count);
	}

}
void homeworkSet::displayGradeStats(std::vector<homework> homework_course_vector){
	
	int marked_hw_count = 0, target_homework_num = 0, grade_sum = 0, failed_count = 0;
	std::string course_name, course_id;
	for (int i = 0; i < homework_course_vector.size(); ++i) {
		target_homework_num++;
		if (homework_course_vector[i].isMarked == 1) {
			marked_hw_count++;
			grade_sum += homework_course_vector[i].grade;
			if (homework_course_vector[i].grade < 60) {
				failed_count++;
			}
		}
		course_name = homework_course_vector[i].course_name;
		course_id = homework_course_vector[i].course_id;
	}
	if (target_homework_num==0) {return;}
	std::cout<<std::endl<<"ID: "<< course_id <<" Name: "<<course_name<<std::endl;

	if (marked_hw_count==0){
		printf("You have submitted %lld homeworks, none of which has already been marked!\n", target_homework_num);
		return;
	}
	printf("You have submitted %lld homeworks, %lld of which have not been marked by your teacher by now.\nIn the marked homeworks, you've got a mean score %d.\n",
		target_homework_num, target_homework_num - marked_hw_count, ((marked_hw_count==0)?0:grade_sum / marked_hw_count));
	if (failed_count == 0) {
		printf("None of your homework has failed.\n");
	}
	else {
		printf("The number of your homework whichi failed is %d.\n", failed_count);
	}

}

/**
	* @brief Searches for homework entries based on the course ID.
	*
	* @param course_id The ID of the course.
	* @return vector<homework> A vector containing homework entries for the specified course.
	*/
std::vector<homework> homeworkSet::searchHomeworkByCourseID(std::string course_id) {
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.course_id == course_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}
/**
	* @brief Searches for homework entries based on the teacher ID.
	*
	* @param request_id The ID of the teacher.
	* @return vector<homework> A vector containing homework entries for the specified teacher.
	*/
std::vector<homework> homeworkSet::searchHomeworkByTeacherID(std::string teacher_id) {
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.teacher_id == teacher_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}

/**
	* @brief Searches for homework entries based on the student ID.
	*
	* @param homework_id The ID of the student.
	* @return vector<homework> A vector containing homework entries for the specified student.
	*/
std::vector<homework> homeworkSet::searchHomeworkByStudentID(std::string student_id) {
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.student_id == student_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}
/**
	* @brief Searches for homework entries based on the homework ID.
	*
	* @param homework_id The ID of the student.
	* @return vector<homework> A vector containing homework entries for the specified student.
	*/
std::vector<homework> homeworkSet::searchHomeworkByHomeworkID(std::string homework_id) {
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.hw_id == homework_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}
/**
	* @brief Checks if a given homework ID exists in the database.
	*
	* @param homework_id The ID of the homework.
	* @return bool Returns true if the homework ID exists, false otherwise.
	*/
bool homeworkSet::isHomeworkIDexists(std::string homework_id) {
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.hw_id == homework_id) {
			return true;
		}
	}
	return false;
}
/**
 * @brief Checks if a given student ID exists in the database.
 *
 * @param student_id The ID of the student.
 * @return bool Returns true if the student ID exists, false otherwise.
 */
bool homeworkSet::isStudentIDexists(std::string student_id){
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.student_id == student_id) {
			return true;
		}
	}
	return false;
}

/**
 * @brief Checks if a given student ID and a given course ID exists in the database.
 *
 * @param student_id The ID of the student.
 * @param course_id The course ID
 * @return bool Returns true if the student ID exists, false otherwise.
 */
bool homeworkSet::isStudnetandCourseIDexists(std::string student_id, std::string course_id){
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		homework temp = homeworkVector[i];
		if (temp.student_id == student_id && temp.course_id == course_id) {
			return true;
		}
	}
	return false;
}

int homeworkSet::searchRequestIndeByHomeworkID(std::string homework_id) {
	std::vector<homework> result;
	for (int i = 0; i < size; ++i) {
		if (homeworkVector[i].hw_id == homework_id) {
			return i;
		}
	}
	return -1;
}


homework homeworkSet::queryHomeworkInFile(homework queryHomework, fileSystem &fs) {
	homework result_hw = get_stu_homework(queryHomework, fs);
	return result_hw;
}

std::vector<homework> homeworkSet::queryCourseHomeworkInFile(std::string student_id, std::string course_id, fileSystem &fs) {
	std::vector<homework> result_vec;
	result_vec = get_stu_homework_list(student_id, course_id, fs);
	return result_vec;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Request Definition~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
 * @brief requestSet类的构造函数。
 *
 * @param capacity 请求集的初始容量。
 */

requestSet::requestSet(int capacity) {
	this->capacity = capacity;
	this->requestVector = std::vector<Request>(capacity);
}
/**
 * @brief requestSet类的析构函数。
 */
requestSet::requestSet(int capacity, std::vector<Request>requestVector) {
	this->capacity = capacity;
	this->requestVector = requestVector;
}

/**
 * @brief requestSet�������������
 */
requestSet::~requestSet(){};
int requestSet::getCapacity() { return this->capacity; }
int requestSet::getSize() { return this->size; }
// 利用hw初始化info
//teacher_id course_id req_id
Request requestSet::initInfo(Request origin){
	Request temp;
    temp.id = origin.id;
    temp.course_id = origin.course_id;
    temp.teacher_id = origin.teacher_id;
    return temp;
}
// 利用req_vector初始化infovector
void requestSet::initInfoVector(){
	for (int i = 0; i < this->size; ++i ){ 
		Request req = this->requestVector[i];
		Request req_info = initInfo(req);
		this->infoVector.emplace_back(req_info);
	}
}

std::vector<Request> requestSet::getRequestVector(){
	std::vector<Request> return_request_vector;
	for(int i = 0; i < this->size; ++i){
		return_request_vector.emplace_back(this->requestVector[i]);
	}
	return return_request_vector;
}
/**
 * @brief 基于元素向量初始化Request对象。
 *
 * @param elements 包含初始化Request对象所需元素的向量。
 * @return Request 初始化后的Request对象。
 */
Request requestSet::initRequest(std::vector<std::string>elements) {
	Request temp;
	temp.submitTime = localtime2__time(elements[0]);
	temp.teacher_name = elements[1];
	temp.teacher_id = elements[2];
	temp.course_name = elements[3];
	temp.course_id = elements[4];
	temp.title = elements[5];
	temp.id = elements[6];
	temp.content = elements[7];
	return temp;

}

/**
 * @brief 使用预定义数据初始化数据库。
 *
 * @param destinationPath 数据库将保存的路径。
 * @param databaseName 数据库文件的名称。
 * @return void
 */
void requestSet::initDatabase(std::string destinationPath, std::string databaseName) {
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
	outFile << "request submitTime" << ','
		<< "request teacher_name" << ','
		<< "request teacher_id" << ','
		<< "request course_name" << ','
		<< "request course_id" << ','
		<< "request title" << ','
		<< "request id" << ','
		<< "request content"
		<< std::endl;

	outFile << "2023 09 27 12 00 00" << ','
		<< "Doc Lee" << ','
		<< "20010060" << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "OS_Homework_Intro" << ','
		<< "OS_hw1" << ','
		<< "URL_1560" << std::endl;
	outFile << "2023 10 20 12 00 00" << ','
		<< "Doc Lee" << ','
		<< "20010060" << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "OS_Homework_Chapter3_File Manangement" << ','
		<< "OS_hw2" << ','
		<< "URL_1880" << std::endl;
	outFile << "2023 11 10 12 00 00" << ','
		<< "Doc Lee" << ','
		<< "20010060" << ','
		<< "OS" << ','
		<< "CS0710" << ','
		<< "OS_Homework_Chapter6_Memory Manangement" << ','
		<< "OS_hw3" << ','
		<< "URL_1992" << std::endl;
	outFile.close();
}

/**
 * @brief 从数据库文件中读取数据并填充请求向量。
 *
 * @param destinationPath 数据库所在的路径。
 * @param databaseName 数据库文件的名称。
 * @return void
 */
void requestSet::readDatabase(std::string destinationPath, std::string databaseName) {
	std::ifstream csv_data(destinationPath + databaseName, std::ios::in);
	std::string line;

	if (!csv_data.is_open())
	{
		std::cout << "Error: opening file fail" << std::endl;
		std::exit(1);
	}


	// ��ȡ������
	std::getline(csv_data, line);
	// ��ȡ����
	int count_hw = 0;
	while (std::getline(csv_data, line))
	{
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		std::istringstream lineStream;         //�������ַ���line���뵽�ַ���istringstream��
		std::vector<std::string> words; //����һ���ַ�������
		std::string word;
		lineStream.clear();
		lineStream.str(line);
		words.clear();
		while (std::getline(lineStream, word, ',')) //���ַ�����sin�е��ַ�����field�ַ����У��Զ���Ϊ�ָ���
		{
			words.push_back(word); //��ÿһ���е��������push
			//std::cout << word << " ";
			//std::cout << atol(word.c_str());
		}
		// std::cout << "initRequest" << std::endl;
		requestVector[count_hw] = initRequest(words);
		// std::cout << std::endl;

		count_hw++;
		if (count_hw >= capacity) {
			std::cerr << "Variable 'count_hw' exceeds capacity of homeworkVector!" << std::endl;
			exit(0);
		}
	}
	this->size = count_hw;
	csv_data.close();

}
/**
 * @brief 使用请求信息更新数据库。
 *
 * @param destinationPath 数据库将保存的路径。
 * @param databaseName 数据库文件的名称。
 * @return void
 */
void requestSet::updateDatabase(std::string destinationPath, std::string databaseName) {
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
	// д�������
	outFile << "submit time" << ','
		<< "teacher name" << ','
		<< "teacher id" << ','
		<< "course name" << ','
		<< "course id" << ','
		<< "title" << ','
		<< "id" << ','
		<< "content"
		<< std::endl;
	// д������
	for (int i = 0; i < this->size; ++i) {
		outFile << requestVector[i].submitTime << ','
			<< requestVector[i].teacher_name << ','
			<< requestVector[i].teacher_id << ','
			<< requestVector[i].course_name << ','
			<< requestVector[i].course_id << ','
			<< requestVector[i].title << ','
			<< requestVector[i].id << ','
			<< requestVector[i].content
			<< std::endl;
	}
	outFile.close();

}

/**
 * @brief Add the database with request vector.
 *
 * @param request_vector The vector to be added to the database.
 * @param destinationPath The path where the database will be saved.
 * @param databaseName The name of the database file.
 * @return void
 */
void requestSet::appendDatabase(std::vector<Request> request_vector, std::string destinationPath, std::string databaseName){
	std::ofstream outFile;
    outFile.open(destinationPath + databaseName, std::ios::out | std::ios::app); // Open in append mode

    for (const auto &request : request_vector) {
        outFile << request.submitTime << ','
                << request.teacher_name << ','
                << request.teacher_id << ','
                << request.course_name << ','
                << request.course_id << ','
                << request.title << ','
                << request.id << ','
                << request.content
                << std::endl;
    }

    outFile.close();
}
/**
 * @brief 使用其他vector对已存在的内容进行更新
 *
 * @param targetVector 目标更改。
 * @return void
 */
void requestSet::writeVector(const std::vector<Request>& targetVector) {
	for (const auto& ele : targetVector) {
		int temp_index = this->searchRequestIndeByRequestID(ele.id);
		if (temp_index == -1) {
			int count_hw = 0;
			if (this->size >= this->capacity) {
				std::cerr << "Variable 'count_hw' exceeds capacity of homeworkVector!" << std::endl;
				exit(0);
			}
			this->requestVector[this->size] = ele;
			this->size++;
		}
		else {
			this->requestVector[temp_index] = ele;

		}
	}
	
}

/**
 * @brief 根据作业ID删除作业信息
 * @param HW_id 作业ID
 * @return 如果成功删除返回true，否则返回false
 */
bool requestSet::deleteRequestFromVectorByReqID(std::string request_id) {
	// 使用迭代器遍历容器
	bool tmp = false;
	for (auto it = this->requestVector.begin(); it != this->requestVector.end();) {
		// 检查关系式
		if (it->id == request_id) {
			// 删除满足条件的元素，并更新迭代器
			it = this->requestVector.erase(it);
			this->size--;
			tmp = true;
		} else {
			// 没有删除元素时，继续迭代
			++it;
		}
	}
	return tmp;
}
/**
 * @brief 根据用户ID删除作业信息
 * @param Student_id 学生ID
 * @return 如果成功删除返回true，否则返回false
 */
bool requestSet::deleteRequestFromVectorByTeacherID(std::string teacher_id){
	// 使用迭代器遍历容器
	bool tmp = false;
	for (auto it = this->requestVector.begin(); it != this->requestVector.end();) {
		// 检查关系式
		if (it->teacher_id == teacher_id) {
			// 删除满足条件的元素，并更新迭代器
			it = this->requestVector.erase(it);
			this->size--;
			tmp = true;
		} else {
			// 没有删除元素时，继续迭代
			++it;
		}
	}
	return tmp;
}
/**
 * @brief 根据老师ID&课程ID删除作业要求信息
 * @param teacher_id 老师ID	
 * @param course_id 课程ID
 * @return 如果成功删除返回true，否则返回false
 */
bool requestSet::deleteRequestFromVectorByTeacherIDandCourseID(std::string teacher_id, std::string course_id){
	// 使用迭代器遍历容器
	bool tmp = false;
	for (auto it = this->requestVector.begin(); it != this->requestVector.end();) {
		// 检查关系式
		if (it->teacher_id == teacher_id && it->course_id == course_id) {
			// 删除满足条件的元素，并更新迭代器
			it = this->requestVector.erase(it);
			this->size--;
			tmp = true;
		} else {
			// 没有删除元素时，继续迭代
			++it;
		}
	}
	return tmp;
}

/**
 * @brief 显示请求向量。
 *
 * @return void
 */
void requestSet::displayRequestsVector()  {
	std::cout << "request Vector: " << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << i << "-request" << requestVector[i] << std::endl << std::endl;
	}
}

/**
 * @brief 根据请求ID搜索请求条目。
 *
 * @param id 请求的ID。
 * @return std::vector<Request> 包含指定ID的请求条目的向量。
 */
std::vector<Request> requestSet::searchRequestByID(std::string id) {
	std::vector<Request> result;
	for (int i = 0; i < size; ++i) {
		Request temp = requestVector[i];
		if (temp.id == id) {
			result.emplace_back(temp);
		}
	}
	return result;
}

/**
 * @brief 根据课程ID搜索请求条目。
 *
 * @param course_id 课程的ID。
 * @return std::vector<Request> 包含指定课程的请求条目的向量。
 */
std::vector<Request> requestSet::searchRequestByCourseID(std::string course_id) {
	std::vector<Request> result;
	for (int i = 0; i < size; ++i) {
		Request temp = requestVector[i];
		if (temp.course_id == course_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}

/**
 * @brief 根据标题搜索请求条目。
 *
 * @param title 请求的标题。
 * @return std::vector<Request> 包含指定标题的请求条目的向量。
 */
std::vector<Request> requestSet::searchRequestBytitle(std::string title) {
	std::vector<Request> result;
	for (int i = 0; i < size; ++i) {
		Request temp = requestVector[i];
		if (temp.title == title) {
			result.emplace_back(temp);
		}
	}
	return result;
}

/**
 * @brief 根据标题搜索请求条目。
 *
 * @param request_id 老师的ID。
 * @return std::vector<Request> 包含指定标题的请求条目的向量。
 */
std::vector<Request> requestSet::searchRequestByTeacherID(std::string teacher_id) {
	std::vector<Request> result;
	for (int i = 0; i < size; ++i) {
		Request temp = requestVector[i];
		if (temp.teacher_id == teacher_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}
std::vector<Request> requestSet::searchRequestByRequestID(std::string request_id) {
	std::vector<Request> result;
	for (int i = 0; i < size; ++i) {
		Request temp = requestVector[i];
		if (temp.id == request_id) {
			result.emplace_back(temp);
		}
	}
	return result;
}
int requestSet::searchRequestIndeByRequestID(std::string request_id) {
	for (int i = 0; i < size; ++i) {
		if (requestVector[i].id == request_id) {
			return i;
		}
	}
	return -1;
}


/**
 * @brief Checks if a given request ID exists in the database.
 *
 * @param request_id The ID of the request.
 * @return bool Returns true if the request ID exists, false otherwise.
 */
bool requestSet::isRequestIDexists(std::string request_id) {
	for (int i = 0; i < size; ++i) {
		Request temp = this->requestVector[i];
		if (temp.id == request_id) {
			return true;
		}
	}
	return false;
}
bool requestSet::isTeacherIDexists(std::string teacher_id){
	for (int i = 0; i < size; ++i) {
		Request temp = this->requestVector[i];
		if (temp.teacher_id == teacher_id) {
			return true;
		}
	}
	return false;
}
bool requestSet::isTeacherandCourseIDexists(std::string teacher_id, std::string course_id){
	for (int i = 0; i < size; ++i) {
		Request temp = this->requestVector[i];
		if (temp.teacher_id == teacher_id && temp.course_id == course_id) {
			return true;
		}
	}
	return false;
}
Request requestSet::queryRequestInFile(Request queryRequest, fileSystem &fs) {
	Request result_hw = get_request(queryRequest, fs);
	return result_hw;
}

std::vector<Request> requestSet::queryCourseRequestInFile(std::string teacher_id, std::string course_id, fileSystem &fs) {
	std::vector<Request> result_vec;
	result_vec = get_stu_request_list(teacher_id, course_id, fs);
	return result_vec;
}