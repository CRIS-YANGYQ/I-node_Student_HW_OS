/**
 *                    _ooOoo_
 *                   o8888888o
 *                   88" . "88
 *                   (| -_- |)
 *                    O\ = /O
 *                ____/`---'\____
 *              .   ' \\| |// `.
 *               / \\||| : |||// \
 *             / _||||| -:- |||||- \
 *               | | \\\ - /// | |
 *             | \_| ''\---/'' | |
 *              \ .-\__ `-` ___/-. /
 *           ___`. .' /--.--\ `. . __
 *        ."" '< `.___\_<|>_/___.' >'"".
 *       | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *         \ \ `-. \_ __\ /__ _/ .-` / /
 * ======`-.____`-.___\_____/___.-`____.-'======
 *                    `=---='
 *
 * .............................................
 *          佛祖保佑             永无BUG
 */

/*
* 本课程大作业要求学生以三到四人为一组，开发一个学生成绩管理系统。该系统基于操作系
统的基本文件管理与进程通讯原理，需要实现以下功能：
1. 用户和用户组功能：系统需要支持用户和用户组的创建、管理和权限设置，以便为系统
中的文件添加权限，并允许文件权限的查询与修改。
2. 远程访问：用户应能够通过网络端口进行远程访问系统，并对系统文件进行读写操作。
3. 文件快照：系统需要实现文件快照功能，以便管理员能够对系统进行快照操作，并在需
要时利用快照对系统进行恢复。
4. 其他功能：根据课程内容可以为系统添加额外功能，将根据难度与完成度酌情加分。
系统用户分为三类：管理员、教师和学生，分别拥有以下功能权限：
1. 教师：具有发布作业、接收学生作业并批改分数的权限。一位教师可以同时教授多门课
程。
2. 学生：具有查看作业内容、上传作业并获取课程分数汇总的权限。一名学生可以同时选
修多门课程。
3. 管理员：拥有教师和学生功能权限的同时，还可以对系统进行快照操作，并能够利用快
照对系统进行恢复操作。
*/


#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <fstream>						//文件的输出输出流操作
#include <iomanip>						//对齐输出
#include <vector>
#include <chrono>
#include <thread>
// #include "authenticator.h"
// #include "definedTime.h"
// #include "user.h"
// #include "course.h"
// #include "homework.h"
#include "modeApplication.h"
const char* __admin = "admin";

using namespace std;
fileSystem fs;

void sleepMilliseconds(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
void GodBlessUs() {
	std::cout << "                    _ooOoo_" << std::endl;
	std::cout << "                   o8888888o" << std::endl;
	std::cout << "                   88\" . \"88" << std::endl;
	std::cout << "                   (| -_- |)" << std::endl;
	std::cout << "                    O\\ = /O" << std::endl;
	std::cout << "                ____/`---'\\____" << std::endl;
	std::cout << "              .   ' \\\\| |// `." << std::endl;
	std::cout << "               / \\||| : |||// \\" << std::endl;
	std::cout << "             / _||||| -:- |||||- \\" << std::endl;
	std::cout << "               | | \\\\ - /// | |" << std::endl;
	std::cout << "             | \\_| ''\\---/'' | |" << std::endl;
	std::cout << "              \\ .-\\__ `-` ___/-. /" << std::endl;
	std::cout << "           ___`. .' /--.--\\ `. . __" << std::endl;
	std::cout << "        .\"\" '< `.___\\_<|>_/___.' >'\"\"." << std::endl;
	std::cout << "| | : `- \\`.;`\\ _ /`;.`/ - ` : | |" << std::endl;
	std::cout << " \\ \\ `-. \\_ __\\ /__ _/ .-` / /" << std::endl;
	std::cout << "======`-.____`-.___\\_____/___.-`____.-'======" << std::endl;
	std::cout << "                    `=---='" << std::endl;
	std::cout << std::endl;
	std::cout << "............................................." << std::endl;
	std::cout << "      佛祖保佑             永无BUG" << std::endl;
}

void studentMode() {
	// Input password
	string enteredStudentID, enteredStudentPwd = "";
	while (1) {
		cout << "\n\n\t\t\tEnter user ID:";
		cin >> enteredStudentID;
		cout << "\t\t\tEnter user password:";
		cin >> enteredStudentPwd;
		if (userAuthentic(enteredStudentID, enteredStudentPwd)) {
			break;
		}
	}
	while (1) {
		// Task selection
		cout << "                                  Student Mode" << endl;
		cout << "                                   			  Hi, User "<< enteredStudentID << endl;
		cout << "                           ★    1. View Homework Request Content\t\t\t★" << endl;
		cout << "                           ★    2. Submit Homework\t\t\t★" << endl;
		cout << "                           ★    3. Get Course Grade Summary\t\t\t★" << endl;
		cout << "                           ★    4. Clear Screen\t\t\t★" << endl;
		cout << "                           ★    B. Bless Us\t\t\t★" << endl;
		cout << "                           ★    0. Return to Previous Menu\t\t\t★" << endl;
		char studentChoice;
		cin >> studentChoice;
		switch (studentChoice) {
		case '0': return;
		case '1':
			readHomeworkRequestInFile(enteredStudentID, fs);
			break;
		case '2':
			submitHomeworkInFile(enteredStudentID, fs);
			break;
		case '3':
			getGradeStatsInFile(enteredStudentID, fs);
			break;
		case '4':
			system("clear");
			break;
		case 'B':
		case 'b':
			GodBlessUs();
			sleepMilliseconds(5000);
			break;
		}
	}
}

void teacherMode() {
	// Input password
	string enteredTeacherID, enteredTeacherPwd = "";
	while (1) {
		cout << "\n\n\t\t\tEnter user ID:";
		cin >> enteredTeacherID;
		cout << "\t\t\tEnter user password:";
		cin >> enteredTeacherPwd;
		if (userAuthentic(enteredTeacherID, enteredTeacherPwd)) {
			break;
		}
	}
	while (1) {
		// Task selection
		cout << "                                   Teacher Mode" << endl;
		cout << "                                   			  Hi, User "<< enteredTeacherID << endl;
		cout << "                           ★    1. Publish Homework Requirements\t\t\t★" << endl;
		cout << "                           ★    2. Receive Student Homework\t\t\t★" << endl;
		cout << "                           ★    3. Grade Homework\t\t\t★" << endl;
		cout << "                           ★    4. Clear Screen\t\t\t★" << endl;
		cout << "                           ★    B. Bless Us\t\t\t★" << endl;
		cout << "                           ★    0. Return to Previous Menu\t\t\t★" << endl;
		char teacherChoice;
		cin >> teacherChoice;
		switch (teacherChoice) {
		case '0': return;
		case '1':
			assignHomeworkRequestInFile(enteredTeacherID, fs);
			break;
		case '2':
			acceptHomeworkInFile(enteredTeacherID, fs);
			break;
		case '3':
			markHomeworkInFile(enteredTeacherID, fs);
			break;
		case '4':
			system("clear");
			break;
		case 'B':
		case 'b':
			GodBlessUs();
			sleepMilliseconds(5000);
			break;
		}
	}
}
void administratorMode() {
	PasswordAuthenticator authenticator;

	// Input password
	string enteredAdminPwd = "";
	while (1) {
		cout << "\t\t\tEnter administrator password:";
		cin >> enteredAdminPwd;
		if (authenticator.authenticate("admin", enteredAdminPwd)) {
			break;
		}
	}

	// Download password book
	authenticator.loadPasswordMapFromFile(passwordsPath);

	while (1) {
		// Task selection
		cout << "                                Administrator Mode" << endl;
		cout << "                           ★  1. List Passwords\t\t\t★" << endl;
		cout << "                           ★  2. Add User & Password\t\t\t★" << endl;
		cout << "                           ★  3. Verify Password\t\t\t★" << endl;
		cout << "                           ★  4. Upload Password Book\t\t\t★" << endl;
		cout << "                           ★  5. Read Homework Requirements\t\t\t★" << endl;
		cout << "                           ★  6. Get Grade Statistics\t\t\t★" << endl;
		cout << "                           ★  7. Receive Student Homework\t\t\t★" << endl;
		cout << "                           ★  8. Grade Student Homework\t\t\t★" << endl;
		cout << "                           ★  0. Return to Previous Menu\t\t\t★" << endl;

		char adminChoice;
		cin >> adminChoice;

switch (adminChoice) 
		{
		case '0': {
			authenticator.savePasswordMapToFile(passwordsPath);
			return;
		}
		case '1': {
			authenticator.listEntries();
			break;
		}
		case '2': {
			userTable users_table(MAX_USER_NUMBER);
			User new_user;
			
			std::string addUserName, addUserId, addPassword ;
			cout << "Enter Name: ";
			cin >> addUserName;
			cout << "Enter ID: ";
			std::vector<User> searched_users;
			while(true){
				cin >> addUserId;
				searched_users = users_table.searchUserByID(addUserId);
				if(searched_users.size() == 0){
					break;
                }
				else{
                    std::cout << "ID already exists. Try another one.\nEnter ID: " << std::endl;
                }
        
			}

			cout << "Enter Kind(0 for student and 1 for teacher): ";
			char kind_choice;
			cin>>kind_choice;
			while(true) {
				if(kind_choice == '0'){
					new_user.kind = "student";
					new_user.priority = 1;
					break;
				}
				else if(kind_choice == '1'){
					new_user.kind = "teacher";
					new_user.priority = 2;
					break;
				}
				else{
					cout << "Your answer is illegal.\nPlease enter 0 for student and 1 for teacher: ";
                    cin>>kind_choice;
				}
			}
			cout << "Enter Gender(0 for male and 1 for female): ";
			char gender_choice;
			cin>>gender_choice;
			while(true) {
				if(gender_choice == '0'){
					new_user.gender = "Male";
					break;
				}
				else if(gender_choice == '1'){
					new_user.gender = "Female";
					break;
				}
				else{
					cout << "Your answer is illegal.\nPlease enter 0 for male and 1 for female: ";
                    cin>>gender_choice;
				}
			}
			cout << "Enter Password: ";
			cin >> addPassword;
			authenticator.addEntry(addUserId, addPassword);
			authenticator.savePasswordMapToFile(passwordsPath);
			cout << "Successfull update for password: " << endl;
			new_user.name = new_user.name;
			new_user.id = new_user.id;
			if(users_table.addDatabase(new_user,databaseFolder,userTablePath)){
				cout << "Successfull update for user table: " << endl;
			}
			else{
				cout << "Failed update for user table: " << endl;
			}

		}
		case '3': {
			string verifyUserId, verifyPassword;
			cout << "Authenticate User and his password\nEnter ID: ";
			cin >> verifyUserId;
			cout << "Enter Password: ";
			cin >> verifyPassword;
			if (authenticator.authenticate(verifyUserId, verifyPassword)) {
				cout << "Correct" << endl;
			}
			else {
				cout << "Incorrect" << endl;
			}
			break;
		}
		case '4': {
			authenticator.savePasswordMapToFile(passwordsPath);
			cout << "Commit the changes successfully" << endl;
			break;
		}
				/*			
			void adminReadHomeworkReqest();
			void adminGetGradeStats();
			void adminAcceptHomework();
			void adminMarkHomework();
			*/
		case '5': {
			adminReadHomeworkReqest();
			cout << "Read Homework Reqest successfully" << endl;
			break;
		}
		case '6':{
			// ��ȡ�ɼ�ͳ��
			adminGetGradeStats();
			cout << "Get Grade Stats successfully" << endl;
			break;
		}
		case '7': {
			// ����ѧ����ҵ
			adminAcceptHomework();
			cout << "Accept Homework successfully" << endl;
			break;
		}
		case '8': {
			// ��ҵ���
			adminMarkHomework();
			cout << "Mark Homework successfully" << endl;
			break;
		}
		default:
			cout << "Illegal operation" << endl;
			break;
		}
	}
}
void Welcome() {
	system("clear");
	cout << "                            ☆ Student Grade Management System ☆" << endl;
	cout << "                                              @ Yang Yuqin" << endl;
	cout << "                           ★    1. Teacher Login\t\t\t★" << endl;
	cout << "                           ★    2. Student Login\t\t\t★" << endl;
	cout << "                           ★    3. Administrator Mode\t\t\t★" << endl;
    cout << "                           ★    4. Clear Screen\t\t\t★" << endl;
	cout << "                           ★    B. Bless Us\t\t\t★" << endl;
	cout << "                           ★    0. Exit\t\t\t★" << endl;

	char globalChoice;
	cin >> globalChoice;
	switch (globalChoice) {
	case '0': exit(0);
	case '1':
		teacherMode();
		break;
	case '2':
		studentMode();
		break;
	case '3':
		administratorMode();
		break;
	case '4':
		system("clear");
		break;
	case 'B':
		GodBlessUs();
		sleepMilliseconds(5000);
		break;
	}
}





// void test() {
// 	homeworkSet set = homeworkSet(10);
// 	set.initDatabase(databaseFolder,  homeworkSetPath);
// 	//set.readDatabase(databaseFolder, homeworkSetPath);
// 	//set.displayHomeworkVector();
// 	userTable users_table = userTable(5);
// 	users_table.initDatabase(databaseFolder,  userTablePath);
// 	//users_table.readDatabase(databaseFolder, userTablePath);
// 	//users_table.displayUserVector();
// 	requestSet requests_table = requestSet(5);
// 	requests_table.initDatabase(databaseFolder,  requestTablePath);
// 	//requests_table.readDatabase(databaseFolder, requestTablePath);
// 	//cout << "display" << endl;
// 	//requests_table.displayRequestsVector();
// 	Course2UserTable selectionTable = Course2UserTable(10);
// 	selectionTable.initDatabase(databaseFolder, selectionTablePath);
// 	//selectionTable.readDatabase(databaseFolder, selectionTablePath);
// 	//selectionTable.displaySelectionVector();
// }
// void testPasswordAuthenticator() {
// 	PasswordAuthenticator authenticator;
// 	authenticator.loadPasswordMapFromFile(passwordsPath);
// 	std::cout << "All Entries:" << std::endl;
// 	authenticator.listEntries();
// 	if (authenticator.authenticate("20010060", "200160")) {
// 		std::cout << "authentication successful." << std::endl;
// 	}
// 	else {
// 		std::cout << "authentication failed." << std::endl;
// 	}
// }
// void testTimestamp(){
// 	tm tm = {};
// 	std::istringstream iss("2021:01:07:21:21:21");
// 	//cout << iss << endl;
// 	iss >> tm.tm_year >> tm.tm_mon >> tm.tm_mday >> tm.tm_hour >> tm.tm_min >> tm.tm_sec;
// 	cout << tm.tm_year <<"''" << tm.tm_mon << "''" << tm.tm_mday << "''" << tm.tm_hour << "''" << tm.tm_min << "''" << tm.tm_sec;
// 	__time temp = localtime2__time("2021 01 07 21 21 21");
// 	cout << temp;
// }



// // 定义拼接Homework 查询字符串的函数
// std::vector<std::string> concatenateStrings4Homework(const homework& hw) {
//     std::vector<std::string> result;
//     // 第一个字符串拼接
//     std::string firstString = "/user/" + hw.student_id;
//     result.push_back(firstString);
//     // 第二个字符串拼接
//     std::string secondString = firstString + "/" + hw.course_id;
//     result.push_back(secondString);
//     // 第三个字符串拼接
//     std::string thirdString = secondString + "/" + hw.hw_id;
//     result.push_back(thirdString);
//     return result;
// }
// // 定义拼接Request 查询字符串的函数
// std::vector<std::string> concatenateStrings4Request(const Request& req) {
//     std::vector<std::string> result;
//     // 第一个字符串拼接
//     std::string firstString = "/user/" + req.teacher_id;
//     result.push_back(firstString);
//     // 第二个字符串拼接
//     std::string secondString = firstString + "/" + req.course_id;
//     result.push_back(secondString);
//     // 第三个字符串拼接
//     std::string thirdString = secondString + "/" + req.id;
//     result.push_back(thirdString);
//     return result;
// }
// // 创建homework i-node
// void createHomeworkDirByInfoVector(const std::vector<homework>& infoVectorInstance, fileSystem &myFS){
// 	for (const auto& hw_info_ele: infoVectorInstance){
// 		std::vector<std::string>returned_path = concatenateStrings4Homework(hw_info_ele);
// 		for(const std::string& path_ele: returned_path){
// 			myFS.createDirectory(path_ele.c_str(), __admin);
// 		}
// 	}
// }
// void createRequestDirByInfoVector(const std::vector<Request>& infoVectorInstance, fileSystem &myFS){
// 	for (const auto& hw_info_ele: infoVectorInstance){
// 		std::vector<std::string>returned_path = concatenateStrings4Request(hw_info_ele);
// 		for(const std::string& path_ele: returned_path){
// 			myFS.createDirectory(path_ele.c_str(), __admin);
// 		}
// 	}
// }


// 创建目录的通用函数 Homework and Request
template<typename T>
void createDirByInfoVector(const std::vector<T>& infoVectorInstance, fileSystem& myFS) {
    for (const auto& info_ele : infoVectorInstance) {
		// 获取查询字符串concatenateStrings()
        std::vector<std::string> returned_path = concatenateStrings2Init(info_ele);
        for (const std::string& path_ele : returned_path) {
			std::cout << path_ele<<std::endl;
			if(!myFS.pathIsExist(path_ele.c_str())){
            	myFS.createDirectory(path_ele.c_str(), __admin);
			}
        }
    }
}

void createUserDirByInfoVector(const std::vector<User> user_info_vector, fileSystem& myFS){
	for (const auto& user_info_ele : user_info_vector) {
        std::string path = "/user/" + user_info_ele.id;
		if (!myFS.pathIsExist(path.c_str())){
			myFS.createDirectory(path.c_str(), __admin);
		}
		
    }
}

void createCourseDirByInfoVector(const std::vector<Course2User> selection_info_vector, fileSystem& myFS){
	for (const auto& selection_info_ele : selection_info_vector) {
        std::string path = "/user/" + selection_info_ele.user_id + "/"+ selection_info_ele.course_id;
        if (!myFS.pathIsExist(path.c_str())){
            myFS.createDirectory(path.c_str(), __admin);
        }
        
    }
}

// CSV到I-node
void transferCSV2INODE(fileSystem &myFS){
	userTable user_table(MAX_USER_NUMBER);
	user_table.readDatabase(databaseFolder, userTablePath);
	user_table.initInfoVector();
	createUserDirByInfoVector(user_table.infoVector, myFS);

	Course2UserTable selection_table(MAX_SELECTION_NUMBER);
	selection_table.readDatabase(databaseFolder, selectionTablePath);
	selection_table.initInfoVector();
	createCourseDirByInfoVector(selection_table.infoVector, myFS);

	homeworkSet homework_table(MAX_HOMEWORK_NUMBER);
	homework_table.readDatabase(databaseFolder, homeworkSetPath);
	homework_table.initInfoVector();
	createDirByInfoVector(homework_table.infoVector, myFS);
	std::vector<homework> homework_vector_copy = homework_table.getHomeworkVector();
	for(auto& hw_copy: homework_vector_copy){
		submit_homework(hw_copy, myFS);
	}


	requestSet request_table(MAX_REQUEST_NUMBER);
	request_table.readDatabase(databaseFolder, requestTablePath);
	request_table.initInfoVector();
	createDirByInfoVector(request_table.infoVector, myFS);
	std::vector<Request> request_vector_copy = request_table.getRequestVector();
	std::cout<<"Request vector size: "<<request_vector_copy.size()<<std::endl;
	int index = 0;
	for(auto& req_copy: request_vector_copy){
		index++;
		std:cout<<index<<std::endl;
		submit_request(req_copy, myFS);
	}

}


void testWLFILE() {
    //提交获取作业内容测试
    homework myHomework;
    myHomework.student_name = "John Doe";
    myHomework.student_id = "s123456";
    myHomework.teacher_name = "Dr. Smith";
    myHomework.teacher_id = "t789012";
    myHomework.course_name = "Computer Science";
    myHomework.course_id = "CS101";
    myHomework.title = "Homework Assignment 1";
    myHomework.request_id = "R123";
    myHomework.hw_id = "HW001";
    myHomework.content = "Write a program that...";
    fs.createDirectory("/user/s123456","admin");
    fs.createDirectory("/user/s123456/CS101","admin");
    fs.createDirectory("/user/s123456/CS101/HW001","admin");
    fs.createDirectory("/user/s123456/CS101/HW002","admin");
    fs.createDirectory("/user/s123456/CS101/HW003","admin");
    submit_homework(myHomework, fs);
    myHomework.hw_id = "HW002";
    submit_homework(myHomework, fs);
    myHomework.hw_id = "HW003";
    submit_homework(myHomework, fs);
    fs.displayDirectory("/");
    homework myHk;
    myHk.student_id = "s123456";
    myHk.teacher_id = "t789012";
    myHk.course_id = "CS101";
    myHk.hw_id = "HW001";
    myHk = get_stu_homework(myHomework, fs);
    std::cout << myHk.content<<std::endl;
    std::vector<homework> stu_homeworks;
    stu_homeworks = get_stu_homework_list("s123456","CS101", fs);
    for (int i =0;i<stu_homeworks.size();i++)
    {
        std::cout<<"学生id:"<<stu_homeworks[i].student_id<<"作业id:"<<stu_homeworks[i].hw_id<<std::endl;
    }
    //---------------------------------------------------------------------------------------------------
    //提交和获取作业要求测试
    fs.createDirectory("/user/T123","admin");
    fs.createDirectory("/user/T123/CS101","admin");
    fs.createDirectory("/user/T123/CS101/R001","admin");
    fs.createDirectory("/user/T123/CS101/R002","admin");
    fs.createDirectory("/user/T123/CS101/R003","admin");
    Request myRequest;
    myRequest.teacher_name = "John Doe";
    myRequest.teacher_id = "T123";
    myRequest.course_name = "Computer Science";
    myRequest.course_id = "CS101";
    myRequest.title = "Request Title";
    myRequest.id = "R001";
    myRequest.content = "This is the content of the request.";
    submit_request(myRequest, fs);
    myRequest.id = "R002";
    submit_request(myRequest, fs);
    myRequest.id = "R003";
    submit_request(myRequest, fs);
    fs.displayDirectory("/");
    Request hwRequest;
    hwRequest.teacher_id = "T123";
    hwRequest.course_id = "CS101";
    hwRequest.id = "R001";
    hwRequest = get_request(hwRequest, fs);
    std::vector<Request> request_list;
    request_list = get_stu_request_list("T123","CS101", fs);
    for (int i =0;i<request_list.size();i++)
    {
        std::cout<<"教师id:"<<request_list[i].teacher_id<<"需求id:"<<request_list[i].id<<std::endl;
    }


// -------------------------------YYQ TEST-----------------------------------
	transferCSV2INODE(fs);

	std::cout<<"\n-----test req-----"<<std::endl;
	std::cout<<std::endl<<"20010060 CS0710"<<std::endl;
    request_list = get_stu_request_list("20010060","CS0710", fs);
    for (int i =0;i<request_list.size();i++)
    {
        std::cout<<"教师id:"<<request_list[i].teacher_id<<" 需求id:"<<request_list[i].id<<std::endl;
    }


	std::cout<<"\n-----test hw------"<<std::endl;


	std::cout<<std::endl<<"20210700 CS0710"<<std::endl;
	std::cout<<"duplicate"<<std::endl;
	stu_homeworks = get_stu_homework_list("20210700","CS0710", fs);
    for (int i =0;i<stu_homeworks.size();i++)
    {
        std::cout<<"学生id:"<<stu_homeworks[i].student_id<<" 作业id:"<<stu_homeworks[i].hw_id<<std::endl;
    }


	std::cout<<std::endl<<"20210800 WEB0715"<<std::endl;
	stu_homeworks = get_stu_homework_list("20210800","WEB0715", fs);
    for (int i =0;i<stu_homeworks.size();i++)
    {
        std::cout<<"学生id:"<<stu_homeworks[i].student_id<<" 作业id:"<<stu_homeworks[i].hw_id<<std::endl;
    }


	std::cout<<std::endl<<"20212000 SE0718"<<std::endl;
	stu_homeworks = get_stu_homework_list("20212000","SE0718", fs);
    for (int i =0;i<stu_homeworks.size();i++)
    {
        std::cout<<"学生id:"<<stu_homeworks[i].student_id<<" 作业id:"<<stu_homeworks[i].hw_id<<std::endl;
    }
}


void testYYQFILE(){
	transferCSV2INODE(fs);

	while (1)
		Welcome();
}
void testAppendDB(){
	homeworkSet homeworkTable(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	homework hw;
	hw.student_name = "haha";
	hw.student_id = "s123456";
	hw.teacher_name = "hahaha";
	hw.teacher_id = "t789012";
	hw.course_name = "Computer Science Introduction";
	hw.course_id = "CS101";
	hw.hw_id = "testHWID";
	hw.title = "Homework Assignment 1";
	hw.request_id = "testRequest";
	hw.content = "Append Test";
	hw.submitTime = localtime2__time(getLocaltime());
	std::vector<homework> test_append;
	test_append.push_back(hw);
	homeworkTable.appendDatabase(test_append, databaseFolder, homeworkSetPath);
}
void testFileInit(){
	cout<<"1"<<endl;
	userTable user_table(MAX_USER_NUMBER);
	user_table.readDatabase(databaseFolder, userTablePath);
	user_table.initInfoVector();
	createUserDirByInfoVector(user_table.infoVector, fs);
	cout<<"2"<<endl;

	Course2UserTable selection_table(MAX_SELECTION_NUMBER);
	selection_table.readDatabase(databaseFolder, selectionTablePath);
	selection_table.initInfoVector();
	createCourseDirByInfoVector(selection_table.infoVector, fs);

	std::cout<<fs.pathIsExist("/user/20010064")<<std::endl;;
	string searchPath ;
	while(1){
		cout<<"input your search path"<<std::endl;
		cin>>searchPath;
		std::cout<<"Exist: "<<fs.pathIsExist(searchPath.c_str())<<std::endl;;
	}
}
int main()
{
	// testAppendDB();
	// testYYQFILE();
	while (1)
		//testPasswordAuthenticator();
		testYYQFILE();
		//test();
	//teacherMode();
	//test();

}
