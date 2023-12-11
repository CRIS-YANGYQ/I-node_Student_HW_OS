#include "modeApplication.h"
//1

// #define SUCCESS 0
// #define ERROR_CODE_DUPLICATE_USER 1
// #define ERROR_CODE_NO_USER 2
// #define ERROR_CODE_IDENTITY 3
// #define ERROR_CODE_NO_COURSE 4
// #define ERROR_CODE_NO_HOMEWORK 5
// #define ERROR_CODE_DUPLICATE_HOMEWORK 6
// #define ERROR_CODE_WRONG_HOMEWORK 7


void readHomeworkRequest(std::string student_id) {
	// 加载用户表
	
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应学生id
	std::vector<User> vec_student_user = users_table.searchUserByID(student_id);
	User student_user;
	if (vec_student_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_student_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else {//size == 1
		student_user = vec_student_user[0];
		if (student_user.kind != "student") {
			std::cout << "ERR: this corresponding user is not a student!";
			return;
		}
	}

	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	requests_table.readDatabase(databaseFolder, requestTablePath);
	// std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>student_selectection_table = selectionTable.searchSelectionByUserId(student_id);
	// 选课表对应的所有作业要求
	std::vector<Request>student_course_request_table;
	// 如果选课表size=0
	if (student_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	else { // 如果选课表size>0
		for (auto ele : student_selectection_table) {
			std::string temp_course_id = ele.course_id;
			std::vector<Request> temp_request_table = requests_table.searchRequestByCourseID(temp_course_id);
			for (auto temp_request : temp_request_table) {
				student_course_request_table.emplace_back(temp_request);
			}
		}
	}
	// 如果选课表size>0
	for (auto target_request : student_course_request_table) {
		std::cout << target_request << std::endl;
	}

	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
void readHomeworkRequestInFile(std::string student_id, fileSystem &myFS) {
	// 加载用户表
	
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询学生id
	std::vector<User> vec_student_user = users_table.searchUserByID(student_id);
	User student_user;
	if (vec_student_user.size() > 1) { // ��ѯ�����id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_student_user.size() == 0) { // �޶�Ӧid
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else {//size == 1
		student_user = vec_student_user[0];
		if (student_user.kind != "student") {
			std::cout << "ERR: this corresponding user is not a student!";
			return;
		}
	}



	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>student_selectection_table = selectionTable.searchSelectionByUserId(student_id);
	// 选课表对应的所有作业要求
	std::vector<Request>student_course_request_table;
	// 如果选课表size=0
	if (student_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	else { // 如果选课表size>0
		requestSet request_table(MAX_REQUEST_NUMBER);
		for (auto ele : student_selectection_table) {
			std::string temp_course_id = ele.course_id;
			// 对于每个课都载入
			Request query_list_condition_req;
			query_list_condition_req.teacher_id = selectionTable.searchTeacherIDByCourseID(temp_course_id);
			query_list_condition_req.course_id = temp_course_id;
			std::vector<Request> temp_request_table = request_table.queryCourseRequestInFile(query_list_condition_req.teacher_id, query_list_condition_req.course_id, myFS);

			for (auto temp_request : temp_request_table) {
				student_course_request_table.emplace_back(temp_request);
			}
		}
	}
	// 打印req
	for (auto target_request : student_course_request_table) {
		std::cout << target_request << std::endl;
	}

	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
passing_massage readHomeworkRequestInFileWithServer(std::string student_id, fileSystem &myFS) {
	// 传输信息
	passing_massage message_request;

	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询学生id
	std::vector<User> vec_student_user = users_table.searchUserByID(student_id);
	User student_user;
	if (vec_student_user.size() > 1) { // ��ѯ�����id
		std::cout << "Err: Duplicate user";
		message_request.err_code = ERROR_CODE_DUPLICATE_USER;
		return message_request;
		// exit(0);
	}
	else if (vec_student_user.size() == 0) { // �޶�Ӧid
		std::cout << "Err: No user returned to vector";
		message_request.err_code = ERROR_CODE_NO_USER;
		return message_request;
		// exit(0);
	}
	else {//size == 1
		student_user = vec_student_user[0];
		if (student_user.kind != "student") {
			std::cout << "ERR: this corresponding user is not a student!";
			message_request.err_code = ERROR_CODE_IDENTITY;
			return message_request;
		}
	}



	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>student_selectection_table = selectionTable.searchSelectionByUserId(student_id);
	// 选课表对应的所有作业要求
	std::vector<Request>student_course_request_table;
	// 如果选课表size=0
	if (student_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		message_request.err_code = ERROR_CODE_NO_COURSE;
		return message_request;
	}
	else { // 如果选课表size>0
		requestSet request_table(MAX_REQUEST_NUMBER);
		for (auto ele : student_selectection_table) {
			std::string temp_course_id = ele.course_id;
			// 对于每个课都载入
			Request query_list_condition_req;
			query_list_condition_req.teacher_id = selectionTable.searchTeacherIDByCourseID(temp_course_id);
			query_list_condition_req.course_id = temp_course_id;
			std::vector<Request> temp_request_table = request_table.queryCourseRequestInFile(query_list_condition_req.teacher_id, query_list_condition_req.course_id, myFS);

			for (auto temp_request : temp_request_table) {
				student_course_request_table.emplace_back(temp_request);
			}
		}
	}
	// 打印req
	for (auto target_request : student_course_request_table) {
		std::cout << target_request << std::endl;
	}
	message_request.err_code = SUCCESS;
	message_request.meassage_request_vector = student_course_request_table;

	return message_request;
	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
//2+1
void submitHomework(std::string student_id) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应学生id
	std::vector<User> vec_student_user = users_table.searchUserByID(student_id);
	User student_user;
	if (vec_student_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_student_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else {//size == 1
		student_user = vec_student_user[0];
		if (student_user.kind != "student") {
			std::cout << "ERR: this corresponding user is not a student!\n";
			return;
		}
	}

	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	requests_table.readDatabase(databaseFolder, requestTablePath);
	std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>student_selectection_table = selectionTable.searchSelectionByUserId(student_id);
	// 选课表对应的所有作业要求
	std::vector<Request>student_course_request_table;
	// 如果选课表size=0
	if (student_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	for (const auto& selection_ele : student_selectection_table) {
		std::vector<Request> tmep_requests = requests_table.searchRequestByCourseID(selection_ele.course_id);
		for (const auto& request_unit : tmep_requests) {
			student_course_request_table.emplace_back(request_unit);
		}
	}
	// 如果选课表size>0
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::vector<homework> homework_results;
	// std::cout << "Do you want to submit a homework corresponding to this Request?\n[y/n]:";
	char choice;
	std::cout << "Do you want to submit a homework?\n[y/n]:";

	std::cin >> choice;
	switch (choice) {
	case 'y': {
		std::cout << "ok";
		for (auto target_request : student_course_request_table) {
			std::cout << target_request << std::endl;
			char request_choice;
			std::cout << "Do you want to submit a homework corresponding to this request?\n[y/n/e for exit]:";
			std::cin >> request_choice;
			switch (request_choice) {
			case 'y': {
				while (1) {
					homework temp_hw;
					temp_hw.submitTime = localtime2__time(getLocaltime());
					temp_hw.student_name = student_user.name;
					temp_hw.student_id = student_user.id;
					temp_hw.teacher_name = target_request.teacher_name;
					temp_hw.teacher_id = target_request.teacher_id;
					temp_hw.course_name = target_request.course_name;
					temp_hw.course_id = target_request.course_id;
					std::cout << "Enter your homework title, press 'Enter' to end it.\nTitle:";
					std::cin >> temp_hw.title;
					temp_hw.request_id = target_request.id;
					std::string hw_id;
					while (1) {
						std::cout << "Enter your homework id, press 'Enter' to end it.\nMake sure its uniqueness or you need to repeat.\nHW ID:";
						std::cin >> hw_id;
						bool isLegal_local = true;
						for (auto& hw_to_be_submitted : homework_results) {
							if (hw_to_be_submitted.hw_id == hw_id) {
								isLegal_local = false;
								break;
							}
						}
						if (!homeworkTable.isHomeworkIDexists(hw_id) && isLegal_local) {
							break;
						}
					}
					std::cout << "Please enter the homework content!\nContent:";
					std::cin >> temp_hw.content;
					homework_results.emplace_back(temp_hw);


					std::cout << "Do you want to submit again?\n[y/n]:";
					char continue_choice;
					std::cin >> continue_choice;
					if (continue_choice == 'n') {
						break;
					}
				}
				break;
			}
			case 'n':{break; }

			case 'e':{
				homeworkTable.writeVector(homework_results);
				homeworkTable.updateDatabase(databaseFolder, homeworkSetPath);
				return; 
			}
			default: {break; }
			}
		}
	case 'n':
		std::cout << "Next one\n";
		break;
	default:
		break;
		}

		homeworkTable.writeVector(homework_results);
		homeworkTable.updateDatabase(databaseFolder, homeworkSetPath);
		// 可返回选课表对应的所有作业要求
		// return student_course_request_table; 
		break;
	}
}
void submitHomeworkInFile(std::string student_id, fileSystem& myFS) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应学生id
	std::vector<User> vec_student_user = users_table.searchUserByID(student_id);
	User student_user;
	if (vec_student_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";

		exit(0);
	}
	else if (vec_student_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else {//size == 1
		student_user = vec_student_user[0];
		if (student_user.kind != "student") {
			std::cout << "ERR: this corresponding user is not a student!\n";
			return;
		}
	}

	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	// requests_table.readDatabase(databaseFolder, requestTablePath);
	// std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>student_selectection_table = selectionTable.searchSelectionByUserId(student_id);
	// 选课表对应的所有作业要求
	std::vector<Request>student_course_request_table;
	// 如果选课表size=0
	if (student_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	for (const auto& selection_ele : student_selectection_table) {
		// 查询
		std::string teacher_id_temp = selectionTable.searchTeacherIDByCourseID(selection_ele.course_id);
		std::vector<Request> tmep_requests = requests_table.queryCourseRequestInFile(teacher_id_temp, selection_ele.course_id, myFS);
		for (const auto& request_unit : tmep_requests) {
			student_course_request_table.emplace_back(request_unit);
		}
	}
	// 如果选课表size>0
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	// homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::vector<homework> homework_results;
	// std::cout << "Do you want to submit a homework corresponding to this Request?\n[y/n]:";
	char choice;
	std::cout << "Do you want to submit a homework?\n[y/n]:";

	std::cin >> choice;
	switch (choice) {
	case 'y': {
		std::cout << "ok";
		for (auto target_request : student_course_request_table) {
			std::cout << target_request << std::endl;
			char request_choice;
			std::cout << "Do you want to submit a homework corresponding to this request?\n[y/n/e for exit]:";
			std::cin >> request_choice;
			switch (request_choice) {
			case 'y': {
				while (1) {
					homework temp_hw;
					temp_hw.submitTime = localtime2__time(getLocaltime());
					temp_hw.student_name = student_user.name;
					temp_hw.student_id = student_user.id;
					temp_hw.teacher_name = target_request.teacher_name;
					temp_hw.teacher_id = target_request.teacher_id;
					temp_hw.course_name = target_request.course_name;
					temp_hw.course_id = target_request.course_id;
					std::cout << "Enter your homework title, press 'Enter' to end it.\nTitle:";
					std::cin >> temp_hw.title;
					temp_hw.request_id = target_request.id;
					std::string hw_id;
					// check if hw_id is legal
					while (1) {
						std::cout << "Enter your homework id, press 'Enter' to end it.\nMake sure its uniqueness or you need to repeat.\nHW ID:";
						std::cin >> hw_id;
						bool isLegal_local = true;
						for (auto& hw_to_be_submitted : homework_results) {
							if (hw_to_be_submitted.hw_id == hw_id) {
								isLegal_local = false;
								break;
							}
						}
						if ((!homework_id_exist(hw_id, myFS)) && isLegal_local) {
							break;
						}
					}
					temp_hw.hw_id = hw_id;
					std::cout << "Please enter the homework content!\nContent:";
					std::cin >> temp_hw.content;
					homework_results.emplace_back(temp_hw);


					std::cout << "Do you want to submit again?\n[y/n]:";
					char continue_choice;
					std::cin >> continue_choice;
					if (continue_choice == 'n') {
						break;
					}
				}
				break;
			}
			case 'n':{break; }

			case 'e':{
				for(auto& hw_element : homework_results) {
					// 获取路径
					std::string submit_strings = pathStrings2Submit(hw_element);
					// std::vector<std::string> concated_strings = concatenateStrings2Init(hw_element);
					const char* hw_element_path = submit_strings.c_str();
					std::cout << hw_element_path << std::endl;

					if(!myFS.pathIsExist(hw_element_path)){
						// 首先创建文件夹路径
						// myFS.createDirectory(hw_element_path, "admin");
						// 提交作业
						submit_homework(hw_element, myFS);
					}
					else{
						std::cout << "The folowing homework has already existed in i-node directory." << std::endl;
						std::cout<<hw_element << std::endl;
					}
				}
				// homeworkTable.writeVector(homework_results);
				homeworkTable.appendDatabase(homework_results, databaseFolder, homeworkSetPath);
				return; 
			}
			default: {break; }
			}
		}
	case 'n':
		std::cout << "Next one\n";
		break;
	default:
		break;
		}

		// homeworkTable.writeVector(homework_results);
		homeworkTable.appendDatabase(homework_results, databaseFolder, homeworkSetPath);
		for(auto& hw_element : homework_results) {
			// 获取路径
			// std::vector<std::string> concated_strings = concatenateStrings2Init(hw_element);
			// const char* hw_element_path = concated_strings[2].c_str();
			std::string submit_strings = pathStrings2Submit(hw_element);
			// std::vector<std::string> concated_strings = concatenateStrings2Init(hw_element);
			const char* hw_element_path = submit_strings.c_str();
			std::cout << hw_element_path << std::endl;

			if(!myFS.pathIsExist(hw_element_path)){
				// 首先创建文件夹路径
				// myFS.createDirectory(hw_element_path, "admin");
				// 提交作业
				submit_homework(hw_element, myFS);
			}
			else{
				std::cout << "The folowing homework has already existed in i-node directory." << std::endl;
				std::cout<<hw_element << std::endl;
			}
		}


		// 可返回选课表对应的所有作业要求
		// return student_course_request_table; 
		break;
	}
}
passing_massage submitHomeworkInFileWithServer(std::string student_id, std::vector<homework> submit_homework_vector, fileSystem& myFS) {
	// 传输要求表，之后询问遍历询问是否对应提交

	// 传输信息
	passing_massage message_request;
	
	// 如果选课表size>0
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);

	// 检查id唯一性和存在性
	for(int i = 0; i < submit_homework_vector.size(); ++i) {
		homework submit_hw = submit_homework_vector[i];
		bool isLegal_local = true;
		for (int j = i + 1; j < submit_homework_vector.size(); ++j) {
			homework hw_to_be_submitted = submit_homework_vector[j];
			if (hw_to_be_submitted.hw_id == submit_hw.hw_id) {
                isLegal_local = false;
                break;
            }
		}
		if ((!homework_id_exist(submit_hw.hw_id, myFS)) && isLegal_local) {
			message_request.err_code = ERROR_CODE_DUPLICATE_HOMEWORK;
			return message_request;
		}
	}
	homeworkTable.appendDatabase(submit_homework_vector, databaseFolder, homeworkSetPath);
	for(auto& hw_element : submit_homework_vector) {
		// 获取路径
		// std::vector<std::string> concated_strings = concatenateStrings2Init(hw_element);
		// const char* hw_element_path = concated_strings[2].c_str();
		std::string submit_strings = pathStrings2Submit(hw_element);
		// std::vector<std::string> concated_strings = concatenateStrings2Init(hw_element);
		const char* hw_element_path = submit_strings.c_str();
		std::cout << hw_element_path << std::endl;

		if(!myFS.pathIsExist(hw_element_path)){
			// 首先创建文件夹路径
			// myFS.createDirectory(hw_element_path, "admin");
			// 提交作业
			submit_homework(hw_element, myFS);
		}
		else{
			std::cout << "The folowing homework has already existed in i-node directory." << std::endl;
			std::cout << hw_element << std::endl;
		}
	}
	message_request.err_code = SUCCESS;
	return message_request;
}
//1
void getGradeStats(std::string student_id) {
	// 检查是否为学生
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	if (users_table.searchUserByID(student_id).size() == 0) {
		std::cout << "ERR: this corresponding user is not a student!\n";
		return;
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	homeworkTable.displayGradeStats(student_id);
}
void getGradeStatsInFile(std::string student_id, fileSystem& myFS) {
	// 检查是否为学生
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	if (users_table.searchUserByID(student_id).size() == 0) {
		std::cout << "ERR: this corresponding user is not a student!\n";
		return;
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);

	Course2UserTable selection_table(MAX_SELECTION_NUMBER);
	selection_table.readDatabase(databaseFolder, selectionTablePath);
	std::vector<Course2User> student_selection_table = selection_table.searchSelectionByUserId(student_id);
	for(const auto& course_ele:student_selection_table){
		// 查找相应课程
		std::vector<homework>homework_course_vec =  homeworkTable.queryCourseHomeworkInFile(student_id, course_ele.course_id, myFS); 
		homeworkTable.displayGradeStats(homework_course_vec);
	}
	
}

passing_massage getGradeStatsInFileWithServer(std::string student_id, fileSystem& myFS) {
	// 传输信息
	passing_massage message_request;
	// 检查是否为学生
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	if (users_table.searchUserByID(student_id).size() == 0) {
		std::cout << "ERR: this corresponding user is not a student!\n";
		message_request.err_code = ERROR_CODE_NO_USER;
		return message_request;
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	message_request.meassage_grade_stats = homeworkTable.getGradeStats(student_id);
	message_request.err_code = SUCCESS;
	return message_request;

	
}
passing_massage getGradeStatsInFileWithServer(std::string student_id, std::string course_id, fileSystem& myFS) {
	// 传输信息
	passing_massage message_request;
	// 检查是否为学生
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	if (users_table.searchUserByID(student_id).size() == 0) {
		std::cout << "ERR: this corresponding user is not a student!\n";
		message_request.err_code = ERROR_CODE_NO_USER;
		return message_request;
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	message_request.meassage_grade_stats = homeworkTable.getGradeStats(student_id, course_id);
	message_request.err_code = SUCCESS;
	return message_request;
	
}
passing_massage getGradeStatsInFileWithServer(std::string student_id, std::string course_id, std::string hw_id, fileSystem& myFS) {
	// 传输信息
	passing_massage message_request;
	// 检查是否为学生
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	if (users_table.searchUserByID(student_id).size() == 0) {
		std::cout << "ERR: this corresponding user is not a student!\n";
		message_request.err_code = ERROR_CODE_NO_USER;
		return message_request;
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::vector<homework> returned_hw = homeworkTable.searchHomeworkByHomeworkID(hw_id);
	if(returned_hw.size() == 0){
		message_request.err_code = ERROR_CODE_NO_HOMEWORK;
        return message_request;
	}
	else if(returned_hw.size() > 1){
		message_request.err_code = ERROR_CODE_DUPLICATE_HOMEWORK;
		return message_request;
	}
	else if(returned_hw[0].student_id !=student_id || returned_hw[0].course_id !=course_id){
		message_request.err_code = ERROR_CODE_WRONG_HOMEWORK;
        return message_request;
	}
	homework target = returned_hw[0];
	if(target.isMarked){
		message_request.meassage_grade_stats = "Homework ID: " + hw_id + "The grade of Your homework is " + std::to_string(target.grade) + "\n";
	}
	else{
		message_request.meassage_grade_stats = "Homework ID: " + hw_id + "It has not been marked yet.\n" ;
	}
	message_request.err_code = SUCCESS;
	return message_request;
	
}
//2+1
void assignHomeworkRequest(std::string teacher_id) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应老师id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else { //size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			return;
		}
	}

	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	requests_table.readDatabase(databaseFolder, requestTablePath);
	std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业要求
	std::vector<Request>teacher_course_request_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	// 如果选课表size>0

	std::cout << "The number of course you are teaching is " << teacher_selectection_table.size() << std::endl;
	std::cout << "Do you want to submit a request?\n[y/n]";
	char choice;
	std::cin >> choice;
	switch (choice) {
	case 'y':
		std::cout << "ok\n";
		while (1) {

			for (int i = 0; i < teacher_selectection_table.size(); ++i) {
				std::cout << i << "-th course:\n";
				std::cout << teacher_selectection_table[i];

				std::cout << "Would you like to chooose the course and submit its new request for homeworks!\nOr choose n to search next course[y/n]\n";
				char course_choice;
				std::cin >> course_choice;
				if (course_choice == 'y') {
					Request temp_request;
					// std::cout << getLocaltime() << std::endl;
					// std::cout << localtime2__time(getLocaltime()) << std::endl;
					temp_request.submitTime = localtime2__time(getLocaltime());
					temp_request.teacher_name = teacher_user.name;
					temp_request.teacher_id = teacher_user.id;
					temp_request.course_name = teacher_selectection_table[i].course_name;
					temp_request.course_id = teacher_selectection_table[i].course_id;
					std::cout << "Enter the id of your request for homework , press 'Enter' to end it.\nID:";
					std::string temp_req_id;
					// check if temp_req_id is legal
					while (1) {
						std::cout << "Enter your homework id, press 'Enter' to end it.\nMake sure its uniqueness or you need to repeat.\nHW ID:";
						std::cin >> temp_req_id;
						bool isLegal_local = true;
						for (auto& req_to_be_submitted : teacher_course_request_table) {
							if (req_to_be_submitted.id == temp_req_id) {
								isLegal_local = false;
								break;
							}
						}
						if ((requests_table.searchRequestByID(temp_req_id).size() == 0) && isLegal_local) {
							break;
						}
					}
					// std::cin >> temp_req_id;
					// while (requests_table.searchRequestByID(temp_req_id).size() != 0) {
					// 	std::cout << "Re-Enter the id. The ID you entered just nowhas existed\nID:";
					// 	std::cin >> temp_req_id;
					// }
					temp_request.id = temp_req_id;
					std::cout << "Enter the title of your request for homework , press 'Enter' to end it.\nTitle:";
					std::cin >> temp_request.title;
					std::cout << "Please enter the homework content!\nContent:\n";
					std::cin >> temp_request.content;
					teacher_course_request_table.emplace_back(temp_request);
				}
				else {
					continue;
				}

			}

			std::cout << "Do you want to submit again?\n[y/n]:";
			char continue_choice;
			std::cin >> continue_choice;
			if (continue_choice == 'n') {
				break;
			}
		}
		break;
	case 'n':
		std::cout << "Next one";
		break;
	default:
		break;
	}

	requests_table.writeVector(teacher_course_request_table);
	requests_table.updateDatabase(databaseFolder, requestTablePath);


	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
void assignHomeworkRequestInFile(std::string teacher_id, fileSystem& myFS) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应老师id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else { //size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			return;
		}
	}

	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	requests_table.readDatabase(databaseFolder, requestTablePath);
	std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业要求
	std::vector<Request>teacher_course_request_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	// 如果选课表size>0

	std::cout << "The number of course you are teaching is " << teacher_selectection_table.size() << std::endl;
	std::cout << "Do you want to submit a request?\n[y/n]";
	char choice;
	std::cin >> choice;
	switch (choice) {
	case 'y':
		std::cout << "ok\n";
		while (1) {

			for (int i = 0; i < teacher_selectection_table.size(); ++i) {
				std::cout << i << "-th course:\n";
				std::cout << teacher_selectection_table[i];

				std::cout << "Would you like to chooose the course and submit its new request for homeworks![y]\nOr choose n to search next course[y/n]\n";
				char course_choice;
				std::cin >> course_choice;
				if (course_choice == 'y') {
					Request temp_request;
					// std::cout << getLocaltime() << std::endl;
					// std::cout << localtime2__time(getLocaltime()) << std::endl;
					temp_request.submitTime = localtime2__time(getLocaltime());
					temp_request.teacher_name = teacher_user.name;
					temp_request.teacher_id = teacher_user.id;
					temp_request.course_name = teacher_selectection_table[i].course_name;
					temp_request.course_id = teacher_selectection_table[i].course_id;
					std::cout << "Enter the id of your request for homework , press 'Enter' to end it.\nID:";
					std::string temp_req_id;
					// check if temp_req_id is legal
					while (1) {
						std::cout << "Enter your request id, press 'Enter' to end it.\nMake sure its uniqueness or you need to repeat.\nRequest ID:";
						std::cin >> temp_req_id;
						bool isLegal_local = true;
						for (auto& req_to_be_submitted : teacher_course_request_table) {
							if (req_to_be_submitted.id == temp_req_id) {
								isLegal_local = false;
								break;
							}
						}
						if ((!homework_id_exist(temp_req_id, myFS)) && isLegal_local) {
							break;
						}
					}
					// std::cin >> temp_req_id;
					// while (requests_table.searchRequestByID(temp_req_id).size() != 0) {
					// 	std::cout << "Re-Enter the id. The ID you entered just nowhas existed\nID:";
					// 	std::cin >> temp_req_id;
					// }
					temp_request.id = temp_req_id;
					std::cout << "Enter the title of your request for homework , press 'Enter' to end it.\nTitle:";
					std::cin >> temp_request.title;
					std::cout << "Please enter the homework content!\nContent:\n";
					std::cin >> temp_request.content;
					teacher_course_request_table.emplace_back(temp_request);
				}
				else {
					continue;
				}

			}

			std::cout << "Do you want to submit again?\n[y/n]:";
			char continue_choice;
			std::cin >> continue_choice;
			if (continue_choice == 'n') {
				break;
			}
		}
		break;
	case 'n':
		std::cout << "Next one";
		break;
	default:
		break;
	}

	// requests_table.writeVector(teacher_course_request_table);
	requests_table.appendDatabase(teacher_course_request_table, databaseFolder, requestTablePath);
	// 获取路径并更细inode
	for (auto& request_ele: teacher_course_request_table) {
		std::string submit_strings = pathStrings2Submit(request_ele);
		const char* req_element_path = submit_strings.c_str();
		// std::vector<std::string> concated_strings = concatenateStrings2Init(request_ele);
		// const char* req_element_path = concated_strings[2].c_str();
		std::cout << req_element_path << std::endl;

		if (!myFS.pathIsExist(req_element_path)){
			// 首先创建文件夹路径
			// myFS.createDirectory(req_element_path, "admin");
			// 提交作业
			submit_request(request_ele, myFS);
		}
		else{
			std::cout << "The folowing request has already existed in i-node directory." << std::endl;
			std::cout << request_ele << std::endl;
		}
	}
	

	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
passing_massage assignHomeworkRequestInFileWithServer(std::string teacher_id, std::vector<Request> submit_request_vector, fileSystem& myFS) {
	// 传输信息
	passing_massage message_request;

	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	requests_table.readDatabase(databaseFolder, requestTablePath);
	// 检查id唯一性和存在性
	for(int i = 0; i < submit_request_vector.size(); ++i) {
		Request submit_req = submit_request_vector[i];
		bool isLegal_local = true;
		for (int j = i + 1; j < submit_request_vector.size(); ++j) {
			Request req_to_be_submitted = submit_request_vector[j];
			if (req_to_be_submitted.id == submit_req.id) {
                isLegal_local = false;
                break;
            }
		}
		if ((request_id_exist(submit_req.id, myFS)) or !isLegal_local) {
			message_request.err_code = ERROR_CODE_DUPLICATE_REQUEST;
			return message_request;
		}
	}
	// requests_table.writeVector(teacher_course_request_table);
	requests_table.appendDatabase(submit_request_vector, databaseFolder, requestTablePath);
	// 获取路径并更细inode
	for (auto& request_ele: submit_request_vector) {
		std::string submit_strings = pathStrings2Submit(request_ele);
		const char* req_element_path = submit_strings.c_str();
		// std::vector<std::string> concated_strings = concatenateStrings2Init(request_ele);
		// const char* req_element_path = concated_strings[2].c_str();
		std::cout << req_element_path << std::endl;

		if (!myFS.pathIsExist(req_element_path)){
			// 首先创建文件夹路径
			// myFS.createDirectory(req_element_path, "admin");
			// 提交作业
			submit_request(request_ele, myFS);
		}
		else{
			std::cout << "The folowing request has already existed in i-node directory." << std::endl;
			std::cout << request_ele << std::endl;
		}
	}
	message_request.err_code = SUCCESS;
	return message_request;

	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
//1
void acceptHomework(std::string teacher_id) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应学生id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else {//size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			return;
		}
	}

	// 加载作业表
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业要求
	std::vector<homework>accepted_homework_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	else { // 如果选课表size>0
		for (auto ele : teacher_selectection_table) {
			std::string temp_course_id = ele.course_id;
			std::vector<homework> temp_homework_table = homeworkTable.searchHomeworkByCourseID(temp_course_id);
			for (auto temp_hw : temp_homework_table) {
				accepted_homework_table.emplace_back(temp_hw);
			}
		}
	}
	// 如果选课表size>0
	for (auto target_homework : accepted_homework_table) {
		std::cout << target_homework << std::endl;
	}

	// 可返回选课表对应的所有作业要求
	// return accepted_homework_table; 
}
void acceptHomeworkInFile(std::string teacher_id, fileSystem &myFS) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应学生id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else {//size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			return;
		}
	}

	// 加载作业表
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业要求
	std::vector<homework>accepted_homework_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	else { // 如果选课表size>0
		for (auto ele : teacher_selectection_table) {
			std::string temp_course_id = ele.course_id;
			// std::vector<homework> temp_homework_table = homeworkTable.searchHomeworkByCourseID(temp_course_id);
			// 查询选择这门课的所有学生
			std::vector<std::string> student_id_vec = selectionTable.searchStudentIDByCourseID(temp_course_id);
			// 对于每个学生，分别接收该课程的作业
			for(auto& student_id_element : student_id_vec){
				std::vector<homework> temp_homework_table = homeworkTable.queryCourseHomeworkInFile(student_id_element, temp_course_id, myFS);
				for (auto temp_hw : temp_homework_table) {
					accepted_homework_table.emplace_back(temp_hw);
				}
			}
		}
	}
	// 如果选课表size>0
	for (auto target_homework : accepted_homework_table) {
		std::cout << target_homework << std::endl;
	}

	// 可返回选课表对应的所有作业要求
	// return accepted_homework_table; 
}
passing_massage acceptHomeworkInFileWithServer(std::string teacher_id, fileSystem &myFS) {
	// 传输信息
	passing_massage message_request;
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应学生id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		message_request.err_code = ERROR_CODE_DUPLICATE_USER;
		return message_request;
		// exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		message_request.err_code = ERROR_CODE_NO_USER;
		return message_request;
		// exit(0);
	}
	else {//size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			message_request.err_code = ERROR_CODE_IDENTITY;
			return message_request;
		}
	}

	// 加载作业表
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业要求
	std::vector<homework>accepted_homework_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		message_request.err_code = ERROR_CODE_NO_COURSE;
		return message_request;
	}
	else { // 如果选课表size>0
		for (auto ele : teacher_selectection_table) {
			std::string temp_course_id = ele.course_id;
			// std::vector<homework> temp_homework_table = homeworkTable.searchHomeworkByCourseID(temp_course_id);
			// 查询选择这门课的所有学生
			std::vector<std::string> student_id_vec = selectionTable.searchStudentIDByCourseID(temp_course_id);
			// 对于每个学生，分别接收该课程的作业
			for(auto& student_id_element : student_id_vec){
				std::vector<homework> temp_homework_table = homeworkTable.queryCourseHomeworkInFile(student_id_element, temp_course_id, myFS);
				for (auto temp_hw : temp_homework_table) {
					accepted_homework_table.emplace_back(temp_hw);
				}
			}
		}
	}
	// 如果选课表size>0
	for (auto target_homework : accepted_homework_table) {
		std::cout << target_homework << std::endl;
	}
	message_request.err_code = SUCCESS;
	message_request.meassage_homework_vector = accepted_homework_table;
	return message_request;
	// 可返回选课表对应的所有作业要求
	// return accepted_homework_table; 
}
//2
void markHomework(std::string teacher_id) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应老师id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else { //size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			return;
		}
	}

	// 加载作业表
	homeworkSet homeworks_table = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworks_table.readDatabase(databaseFolder, homeworkSetPath);
	// std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业
	std::vector<homework>teacher_course_homework_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	// 如果选课表size>0

	for (int i = 0; i < teacher_selectection_table.size(); ++i) {
		std::vector<homework> temp_hw_vec = homeworks_table.searchHomeworkByCourseID(teacher_selectection_table[i].course_id);
		for (int j = 0; j < temp_hw_vec.size(); ++j) {
			teacher_course_homework_table.emplace_back(temp_hw_vec[j]);
		}
	}
	std::cout << "Do you want to mark a homework?\n[y/n]";
	char choice;
	std::cin >> choice;
	switch (choice) {
	case 'y':
		std::cout << "ok";
		for (int i = 0; i < teacher_course_homework_table.size(); ++i) {



			std::cout << i << "-th course:\n";
			std::cout << teacher_course_homework_table[i];

			std::cout << "Would you like to score homeworks of this course![y/n]\n";
			char course_choice;
			std::cin >> course_choice;
			if (course_choice == 'y') {
				std::cout << "Enter the score!(Interger)\n";
				std::cin >> teacher_course_homework_table[i].grade;
				teacher_course_homework_table[i].isMarked = true;
			}
			else {
				continue;
			}



		}
		break;
	default:
		break;

	}
	homeworks_table.writeVector(teacher_course_homework_table);
	homeworks_table.updateDatabase(databaseFolder, homeworkSetPath);


	// 可返回选课表对应的所有作业要求
	// return student_course_request_table; 
}
void markHomeworkInFile(std::string teacher_id, fileSystem& myFS) {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);

	// 查询对应老师id
	std::vector<User> vec_teacher_user = users_table.searchUserByID(teacher_id);
	User teacher_user;
	if (vec_teacher_user.size() > 1) { // 查询到多个id
		std::cout << "Err: Duplicate user";
		exit(0);
	}
	else if (vec_teacher_user.size() == 0) { // 无对应id
		std::cout << "Err: No user returned to vector";
		exit(0);
	}
	else { //size == 1
		teacher_user = vec_teacher_user[0];
		if (teacher_user.kind != "teacher") {
			std::cout << "ERR: this corresponding user is not a teacher!";
			return;
		}
	}

	// 加载作业表
	homeworkSet homeworks_table = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworks_table.readDatabase(databaseFolder, homeworkSetPath);
	// std::cout << "display" << std::endl;

	// 加载课程-用户映射表
	Course2UserTable selectionTable = Course2UserTable(MAX_SELECTION_NUMBER);
	selectionTable.readDatabase(databaseFolder, selectionTablePath);

	// 该生对应的选课表
	std::vector<Course2User>teacher_selectection_table = selectionTable.searchSelectionByUserId(teacher_id);
	// 选课表对应的所有作业
	std::vector<homework>teacher_course_homework_table;
	// 如果选课表size=0
	if (teacher_selectection_table.size() == 0) {
		std::cout << "No course corresponding!\n";
		return;
	}
	// 如果选课表size>0

	for (auto ele : teacher_selectection_table) {
		std::string temp_course_id = ele.course_id;
		// std::vector<homework> temp_homework_table = homeworks_table.searchHomeworkByCourseID(temp_course_id);
		// 查询选择这门课的所有学生
		std::vector<std::string> student_id_vec = selectionTable.searchStudentIDByCourseID(temp_course_id);
		// 对于每个学生，分别接收该课程的作业
		for(auto& student_id_element : student_id_vec){
			std::vector<homework> temp_homework_table = homeworks_table.queryCourseHomeworkInFile(student_id_element, temp_course_id, myFS);
			for (auto temp_hw : temp_homework_table) {
				teacher_course_homework_table.emplace_back(temp_hw);
			}
		}
	}
	std::cout << "Do you want to mark a homework?\n[y/n]";
	char choice;
	std::cin >> choice;
	switch (choice) {
	case 'y':
		std::cout << "ok";
		for (int i = 0; i < teacher_course_homework_table.size(); ++i) {



			std::cout << i << "-th course:\n";
			std::cout << teacher_course_homework_table[i];

			std::cout << "Would you like to score homeworks of this course![y/n]\n";
			char course_choice;
			std::cin >> course_choice;
			if (course_choice == 'y') {
				std::cout << "Enter the score!(Interger)\n";
				std::cin >> teacher_course_homework_table[i].grade;
				teacher_course_homework_table[i].isMarked = true;
			}
			else {
				continue;
			}



		}
		break;
	default:
		break;

	}
	// homeworks_table.writeVector(teacher_course_homework_table);
	homeworks_table.appendDatabase(teacher_course_homework_table, databaseFolder, homeworkSetPath);
	// 获取路径并更细inode
	for (auto& homework_ele: teacher_course_homework_table) {
		std::string submit_strings = pathStrings2Submit(homework_ele);
		const char* hw_element_path = submit_strings.c_str();
		// std::vector<std::string> concated_strings = concatenateStrings2Init(homework_ele);
		// const char* hw_element_path = concated_strings[2].c_str();
		std::cout << hw_element_path << std::endl;

		if (!myFS.pathIsExist(hw_element_path)){
			// 首先创建文件夹路径
			// myFS.createDirectory(hw_element_path, "admin");
			// 提交作业
			submit_homework(homework_ele, myFS);
		}
		else{
			std::cout << "The folowing homework has already existed in i-node directory." << std::endl;
			std::cout << homework_ele << std::endl;
		}
	}

	// 可返回选课表对应的所有作业要求
	// return teacher_course_homework_table; 
}
passing_massage markHomeworkInFileWithServer(std::string teacher_id, std::vector<homework> submit_homework_vector,fileSystem& myFS) {
	
	// 加载作业表
	homeworkSet homeworks_table = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworks_table.readDatabase(databaseFolder, homeworkSetPath);
	// std::cout << "display" << std::endl;

	
	// homeworks_table.writeVector(teacher_course_homework_table);
	homeworks_table.appendDatabase(submit_homework_vector, databaseFolder, homeworkSetPath);
	// 获取路径并更细inode
	for (auto& homework_ele: submit_homework_vector) {
		std::string submit_strings = pathStrings2Submit(homework_ele);
		const char* hw_element_path = submit_strings.c_str();
		// std::vector<std::string> concated_strings = concatenateStrings2Init(homework_ele);
		// const char* hw_element_path = concated_strings[2].c_str();
		std::cout << hw_element_path << std::endl;

		if (!myFS.pathIsExist(hw_element_path)){
			// 首先创建文件夹路径
			// myFS.createDirectory(hw_element_path, "admin");
			// 提交作业

			// TODO 更新已经提交的作业
			set_Score(homework_ele, myFS);
		}
		else{
			std::cout << "The folowing homework has already existed in i-node directory." << std::endl;
			std::cout << homework_ele << std::endl;
		}
	}

	// 可返回选课表对应的所有作业要求
	// return teacher_course_homework_table; 
}

void adminReadHomeworkReqest() {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	char choice = '\0';
	do {
		std::cout << "Do you want to display all user info?\n[y/n]" << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'y':
			users_table.displayUserVector();
			break;
		default:
			break;
		}
	} while (choice != 'y' && choice != 'n');
	// 输入教师ID后检索目标
	std::string seachingTeacherID = "";
	std::vector<User> target_users;
	while (1) {
		std::cout << "Enter the teacher ID of whom you wanna read.\n";
		std::cin>>seachingTeacherID;
		std::vector<User> returned_temp_users = users_table.searchUserByID(seachingTeacherID);
		if (returned_temp_users.size() == 1) {
			if (returned_temp_users[0].kind == "student") {
				std::cout<<"This is a student";
				return;
			}
			std::cout << "The result:\n" << returned_temp_users[0]<<std::endl;
			target_users.emplace_back(returned_temp_users[0]);
			// 更多目标？
			std::cout << "Do you wanna search for more teachers?\n[y/n]\n";
			char continue_choice = '\0';
			do {std::cin >> continue_choice;} 
			while (continue_choice != 'n' && continue_choice != 'y');

			if (continue_choice == 'n') {
				break;
			}
			else if (continue_choice == 'y') {
				std::cout << "Next teacher operation ->" << std::endl;
			}
		}
		else if (returned_temp_users.size() == 0) {// 无目标
			std::cout << "No corresponding user.\nPlease check the ID you just entered.\n" << std::endl;
		}
		else {// userTable返回多目标
			std::cerr << "Duplicate teachers of ID" << seachingTeacherID << std::endl;
		}
	}
	// 查询对应课程
	 
	// 加载作业要求表
	requestSet requests_table = requestSet(MAX_REQUEST_NUMBER);
	requests_table.readDatabase(databaseFolder, requestTablePath);

	std::vector<Request> target_request_vec;
	for (auto target_user : target_users) {
		std::vector<Request> temp_request_vec = requests_table.searchRequestByTeacherID(target_user.id);
		for (auto temp_request : temp_request_vec) {
			target_request_vec.emplace_back(temp_request);
		}
	}
	for (auto req: target_request_vec){
		std::cout<< req << std::endl;
	}

	// return target_request_vec
}
void adminGetGradeStats() {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	char choice = '\0';
	do {
		std::cout << "Do you want to display all user info?\n[y/n]: " << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'y':
			users_table.displayUserVector();
			break;
		default:
			break;
		}
	} while (choice != 'y' && choice != 'n');
	std::string seachingStudentID = "";
	std::vector<User> target_users;
	while (1) {
		std::cout << "Enter the student ID of whom you wanna read.\n";
		std::cin >> seachingStudentID;
		std::vector<User> returned_temp_users = users_table.searchUserByID(seachingStudentID);
		if (returned_temp_users.size() == 1) {
			if (returned_temp_users[0].kind == "teacher") {
				continue;
			}
			std::cout << "The result:\n" << returned_temp_users[0] << std::endl;
			target_users.emplace_back(returned_temp_users[0]);
			std::cout << "Do you wanna search for more students?\n[y/n]: ";
			char continue_choice = '\0';
			do {
				std::cin >> continue_choice;
			} while (continue_choice != 'n' && continue_choice != 'y');
			if (continue_choice == 'n') {
				break;
			}
			else if (continue_choice == 'y') {
				std::cout << "Next teacher operation ->" << std::endl;
			}
		}
		else if (returned_temp_users.size() == 0) {
			std::cout << "No corresponding user.\nPlease check the ID you just entered.\n" << std::endl;
		}
		else {
			std::cerr << "Duplicate teachers of ID" << seachingStudentID << std::endl;
		}
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	for (auto student_element : target_users) {
		homeworkTable.displayGradeStats(student_element.id);
		std::cout << std::endl;
	}
}
void adminAcceptHomework() {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	char choice = '\0';
	do {
		std::cout << "Do you want to display all user info?\n[y/n]" << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'y':
			users_table.displayUserVector();
			break;
		default:
			break;
		}
	} while (choice != 'y' && choice != 'n');
	std::string seachingStudentID = "";
	std::vector<User> target_users;
	while (1) {
		std::cout << "Enter the student ID of whom you wanna read.\n";
		std::cin >> seachingStudentID;
		std::vector<User> returned_temp_users = users_table.searchUserByID(seachingStudentID);
		if (returned_temp_users.size() == 1) {
			if (returned_temp_users[0].kind == "teacher") {
				continue;
			}
			std::cout << "The result:\n" << returned_temp_users[0] << std::endl;
			target_users.emplace_back(returned_temp_users[0]);
			std::cout << "Do you wanna search for more students?\n[y/n]: ";
			char continue_choice = '\0';
			do {std::cin >> continue_choice;} 
			while (continue_choice != 'n' && continue_choice != 'y');

			if (continue_choice == 'n') {
				break;
			}
			else if (continue_choice == 'y') {
				std::cout << "Next student operation ->" << std::endl;
			}
		}
		else if (returned_temp_users.size() == 0) {
			std::cout << "No corresponding user.\nPlease check the ID you just entered.\n" << std::endl;
		}
		else {
			std::cerr << "Duplicate teachers of ID" << seachingStudentID << std::endl;
		}
	}
	homeworkSet homeworkTable = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworkTable.readDatabase(databaseFolder, homeworkSetPath);
	std::vector <homework> student_hw_vector;
	for (const auto& student_element : target_users) {
		std::vector <homework> temp_student_hw = homeworkTable.searchHomeworkByStudentID(student_element.id);
		for (const auto& ele : temp_student_hw) {
			student_hw_vector.emplace_back(ele);
		}
	}
	for (const auto& ele : student_hw_vector) {
		std::cout << ele << "\n";
	}
}


void adminMarkHomework() {
	// 加载用户表
	userTable users_table = userTable(MAX_USER_NUMBER);
	users_table.readDatabase(databaseFolder, userTablePath);
	char choice = '\0';
	do {
		std::cout << "Do you want to display all user info?\n[y/n]" << std::endl;
		std::cin >> choice;
		switch (choice) {
		case 'y':
			users_table.displayUserVector();
			break;
		default:
			break;
		}
	} while (choice != 'y' && choice != 'n');
	std::string seachingStudentID = "";
	std::vector<User> target_users;
	while (1) {
		std::cout << "Enter the student ID of whom you wanna read.\n";
		std::cin >> seachingStudentID;
		std::vector<User> returned_temp_users = users_table.searchUserByID(seachingStudentID);
		if (returned_temp_users.size() == 1) {
			if (returned_temp_users[0].kind == "teacher") {
				std::cout << "This user is a teacher.\n" << std::endl;
				continue;
			}
			std::cout << "The result:\n" << returned_temp_users[0] << std::endl;
			target_users.emplace_back(returned_temp_users[0]);
			std::cout << "Do you wanna search for more students?\n[y/n]\n";
			char continue_choice = '\0';
			do {
				std::cin >> continue_choice;
				if (continue_choice == 'n') {
					break;
				}
				else if (continue_choice == 'y') {
					std::cout << "Next student operation ->" << std::endl;

				}
			} while (continue_choice != 'n' && continue_choice != 'y');

		}
		else if (returned_temp_users.size() == 0) {
			std::cout << "No corresponding user.\nPlease check the ID you just entered.\n" << std::endl;
		}
		else {
			std::cerr << "Duplicate teachers of ID" << seachingStudentID << std::endl;
		}
	}

	// ��ѯ��Ӧ��ʦid
	std::vector<User> vec_student_user = users_table.searchUserByID(seachingStudentID);


	// ������ҵ��
	homeworkSet homeworks_table = homeworkSet(MAX_HOMEWORK_NUMBER);
	homeworks_table.readDatabase(databaseFolder, homeworkSetPath);
	// std::cout << "display" << std::endl;

	// ѡ�α���Ӧ��������ҵ
	std::vector<homework>student_homework_table = homeworks_table.searchHomeworkByStudentID(seachingStudentID);


	std::cout << "Do you want to mark a homework?\n[y/n]";
	choice = '\0';
	std::cin >> choice;
	switch (choice) {
	case 'y':
		std::cout << "ok";
		for (int i = 0; i < student_homework_table.size(); ++i) {
			std::cout << student_homework_table[i] << std::endl;
			std::cout << "Would you like to mark a score for the above homework![y/n]\n";
			char course_choice;
			std::cin >> course_choice;
			if (course_choice == 'y') {
				std::cout << "Enter the score!(Interger)\n";
				std::cin >> student_homework_table[i].grade;
				student_homework_table[i].isMarked = true;
				break;
			}
			else {
				return;
			}
		}
		break;
	default:
		break;

	}
	homeworks_table.writeVector(student_homework_table);
	homeworks_table.updateDatabase(databaseFolder, homeworkSetPath);
}