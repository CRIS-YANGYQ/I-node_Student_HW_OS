void test()
{
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
    submit_homework(myHomework);
    myHomework.hw_id = "HW002";
    submit_homework(myHomework);
    myHomework.hw_id = "HW003";
    submit_homework(myHomework);
    fs.displayDirectory("/");
    homework myHk;
    myHk.student_id = "s123456";
    myHk.teacher_id = "t789012";
    myHk.course_id = "CS101";
    myHk.hw_id = "HW001";
    myHk = get_stu_homework(myHomework);
    std::cout << myHk.content<<std::endl;
    std::vector<homework> stu_homeworks;
    stu_homeworks = get_stu_homework_list("s123456","CS101");
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

    request myRequest;
    myRequest.teacher_name = "John Doe";
    myRequest.teacher_id = "T123";
    myRequest.course_name = "Computer Science";
    myRequest.course_id = "CS101";
    myRequest.title = "Request Title";
    myRequest.id = "R001";
    myRequest.content = "This is the content of the request.";
    submit_request(myRequest);
    myRequest.id = "R002";
    submit_request(myRequest);
    myRequest.id = "R003";
    submit_request(myRequest);
    fs.displayDirectory("/");

    request hwRequest;
    hwRequest.teacher_id = "T123";
    hwRequest.course_id = "CS101";
    hwRequest.id = "R001";
    hwRequest = get_request(hwRequest);

    std::vector<request> request_list;
    request_list = get_stu_request_list("T123","CS101");
    for (int i =0;i<request_list .size();i++)
    {
        std::cout<<"教师id:"<<request_list[i].teacher_id<<"需求id:"<<request_list[i].id<<std::endl;
    }
}