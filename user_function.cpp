#include "user_function.h"



void appendCharToString(char* str, char c) {
    // 找到字符串的末尾
    size_t len = strlen(str);
    // 将字符追加到字符串末尾
    str[len] = c;
    // 在新字符串的末尾添加 null 字符
    str[len + 1] = '\0';
}
//辅助函数，用来以标点符号分割字符串
size_t split(char *content, char s, char strings[100][USER_NUME_LEN]) {
        size_t length = strlen(content);
        size_t i;
        size_t j = 0;
        char str[100] = ""; // 使用字符数组而不是指针
        for (i = 0; i < length; i++) {
            if (content[i] == s && strcmp(str, "") != 0) {
                strcpy(strings[j],"/0");
                strcpy(strings[j],strdup(str));
                j++;
                str[0] = '\0'; // 重置字符串
            } else {
                appendCharToString(str, content[i]);
            }
            if (strcmp(str, "") != 0) {
            strcpy(strings[j],"/0");
            strcpy(strings[j],strdup(str));
            }
        }
        return j;
    }



bool submit_homework(homework hw,fileSystem &fs)
{
    char content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    char homeworkFilePath[MAX_PATH_LEN] = "\0";
    const char* student_id = hw.student_id.c_str();
    const char* course_id = hw.course_id.c_str();
    const char* hw_id = hw.hw_id.c_str();
    snprintf(homeworkFilePath, MAX_PATH_LEN, "/user/%s/%s/%s",student_id,course_id,hw_id);
    snprintf(content, (BLOCK_SIZE-1)*BLOCKS_PER_INODE, "%s;%s;%s;%s;%s;%s;%d;%d",hw.student_name.c_str(),hw.teacher_name.c_str(),hw.course_name.c_str(),hw.title.c_str(),hw.request_id.c_str(),hw.content.c_str(),hw.isMarked,hw.grade);
    if(fs.createFile(homeworkFilePath,"admin")&&fs.writeFile(homeworkFilePath,content,"admin"))
    {
        printf("提交作业成功\n");
    }
    else 
    {
        printf("提交作业失败\n");
        return 0;
    }
    return 1;
}

homework get_stu_homework(homework hw,fileSystem &fs)
{
    char content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    char homeworkFilePath[MAX_PATH_LEN] = "\0";
    const char* student_id = hw.student_id.c_str();
    const char* course_id = hw.course_id.c_str();
    const char* hw_id = hw.hw_id.c_str();
    snprintf(homeworkFilePath, MAX_PATH_LEN, "/user/%s/%s/%s",student_id,course_id,hw_id);

    if(fs.readFile(homeworkFilePath,content,"admin"))
    {
        char contents[100][USER_NUME_LEN];
        int num = split(content,';',contents);
        for (int i =0;i<num;i++)
        {
            std::string str(contents[i]);
        }
        hw.teacher_name = contents[0];
        hw.teacher_id = contents[1];
        hw.course_name = contents[2];
        hw.title = contents[3];
        hw.request_id = contents[4];
        hw.content = contents[5];
        hw.isMarked = std::stoi(contents[6]);
        hw.grade = std::stoi(contents[7]);
        printf("查询作业成功\n");
    }
    else 
    {
        printf("查询作业错误\n");
    }
    return hw;
}
bool set_Score(homework hw,fileSystem &fs)
{
    char content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    char homeworkFilePath[MAX_PATH_LEN] = "\0";
    const char* student_id = hw.student_id.c_str();
    const char* course_id = hw.course_id.c_str();
    const char* hw_id = hw.hw_id.c_str();
    hw.isMarked = 1;
    snprintf(homeworkFilePath, MAX_PATH_LEN, "/user/%s/%s/%s",student_id,course_id,hw_id);
    snprintf(content, (BLOCK_SIZE-1)*BLOCKS_PER_INODE, "%s;%s;%s;%s;%s;%s;%d;%d",hw.student_name.c_str(),hw.teacher_name.c_str(),hw.course_name.c_str(),hw.title.c_str(),hw.request_id.c_str(),hw.content.c_str(),hw.isMarked,hw.grade);
    if(fs.writeFile(homeworkFilePath,content,"admin")){
        printf("打分作业成功\n");
        return true;
    }
    else {
        printf("打分作业失败\n");
        return false;
    }
}
std::vector<homework> get_stu_homework_list(std::string student_id,std::string course_id,fileSystem &fs)
{
    std::vector<homework> homework_list;
    char courseFilePath[MAX_PATH_LEN] = "\0";
    char homeworks_content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    const char* stu_id = student_id.c_str();
    const char* c_id = course_id.c_str();
    snprintf(courseFilePath, MAX_PATH_LEN, "/user/%s/%s",stu_id,c_id);
    homework hw;
    hw.student_id = student_id;
    hw.course_id = course_id;
    if(fs.getFileNames(courseFilePath,homeworks_content))
    {
        char homework_group[100][USER_NUME_LEN];
        int file_num  = split(homeworks_content,';',homework_group);
        for (int i = 0;i<file_num;i++)
        {
            char homeworkPath[MAX_PATH_LEN] = "\0";
            snprintf(homeworkPath, MAX_PATH_LEN, "%s/%s",courseFilePath,homework_group[i]);
            hw.hw_id = homework_group[i];
            hw = get_stu_homework(hw,fs);
            homework_list.push_back(hw);
        }
    }
    return homework_list;
}



bool submit_request(Request myRequest,fileSystem &fs)
{
    char homeworkFilePath[MAX_PATH_LEN] = "\0";
    char content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    const char* teacher_id = myRequest.teacher_id.c_str();
    const char* course_id = myRequest.course_id.c_str();
    const char* id = myRequest.id.c_str();
    snprintf(homeworkFilePath, MAX_PATH_LEN, "/user/%s/%s/%s",teacher_id,course_id,id);
    snprintf(content, (BLOCK_SIZE-1)*BLOCKS_PER_INODE, "%s;%s;%s;%s",myRequest.teacher_name.c_str(),myRequest.course_name.c_str(),myRequest.title.c_str(),myRequest.content.c_str());
    if (fs.createFile(homeworkFilePath,"admin")&&fs.writeFile(homeworkFilePath,content,"admin"))
    {   
        printf("上传作业要求成功\n");
    }
    else 
    {
        printf("上传作业要求失败\n");
        return 0;
    }
    return 1;
}

Request get_request(Request myRequest,fileSystem &fs)
{
    char content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    char homeworkFilePath[MAX_PATH_LEN] = "\0";
    const char* teacher_id = myRequest.teacher_id.c_str();
    const char* course_id = myRequest.course_id.c_str();
    const char* id = myRequest.id.c_str();
    snprintf(homeworkFilePath, MAX_PATH_LEN, "/user/%s/%s/%s",teacher_id,course_id,id);

    if(fs.readFile(homeworkFilePath,content,"admin"))
    {
        char contents[100][USER_NUME_LEN];
        int num = split(content,';',contents);
        for (int i =0;i<num;i++)
        {
            std::string str(contents[i]);
        }
        myRequest.teacher_name =contents[0];
        myRequest.course_name = contents[1];
        myRequest.title = contents[2];
        myRequest.content = contents[3];
        printf("查询作业要求成功\n");
    }
    else 
    {
        printf("查询作业要求错误\n");
        
    }
    return myRequest;
}

std::vector<Request> get_stu_request_list(std::string teacher_id,std::string course_id,fileSystem &fs)
{
    std::vector<Request> request_list;
    char courseFilePath[MAX_PATH_LEN] = "\0";
    char request_content[(BLOCK_SIZE-1)*BLOCKS_PER_INODE] = "\0";
    const char* tea_id = teacher_id.c_str();
    const char* c_id = course_id.c_str();
    snprintf(courseFilePath, MAX_PATH_LEN, "/user/%s/%s",tea_id,c_id);
    Request re;
    re.teacher_id = teacher_id;
    re.course_id = course_id;
    if(fs.getFileNames(courseFilePath,request_content))
    {
        char request_group[100][USER_NUME_LEN];
        int file_num  = split(request_content,';',request_group);
        for (int i = 0;i<file_num;i++)
        {
            char homeworkPath[MAX_PATH_LEN] = "\0";
            snprintf(homeworkPath, MAX_PATH_LEN, "%s/%s",courseFilePath,request_group[i]);
            re.id = request_group[i];
            re = get_request(re,fs);
            request_list.push_back(re);
        }
    }
    return request_list;
}

bool homework_id_exist(std::string homework_id,fileSystem &fs)
{   
    const char* id = homework_id.c_str();
    const char* user_path = "/user";
    char user_content[BLOCKS_PER_INODE*(BLOCK_SIZE-1)] = "\0";
    if(fs.getFileNames(user_path,user_content))
    {
        char user_group[100][USER_NUME_LEN];
        int user_num = split(user_content,';',user_group);
        for(int i=0;i<user_num;i++)
        {
            char course_group[BLOCKS_PER_INODE*(BLOCK_SIZE-1)] = "\0";
            char user_path[MAX_PATH_LEN] = "\0";
            snprintf(user_path, MAX_PATH_LEN, "/user/%s",user_group[i]);
            if(fs.getFileNames(user_path,course_group))
            {
                char course_group_id[100][USER_NUME_LEN];
                int course_num = split(course_group,';',course_group_id);
                for(int j=0;j<course_num;j++)
                {
                    char homework_group[BLOCKS_PER_INODE*(BLOCK_SIZE-1)] = "\0";
                    char course_path[MAX_PATH_LEN] = "\0";
                    snprintf(course_path, MAX_PATH_LEN, "/user/%s/%s",user_group[i],course_group_id[j]);
                    if(fs.getFileNames(course_path,course_group))
                    {
                        char homework_group_id[100][USER_NUME_LEN];
                        int homework_num = split(course_group,';',homework_group_id);
                        for(int k=0;k<homework_num;k++)
                        {
                            if(strcmp(homework_group_id[k],id)==0)
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

bool request_id_exist(std::string request_id,fileSystem &fs)
{   
    const char* id = request_id.c_str();
    const char* user_path = "/user";
    char user_content[BLOCKS_PER_INODE*(BLOCK_SIZE-1)] = "\0";
    if(fs.getFileNames(user_path,user_content))
    {
        char user_group[100][USER_NUME_LEN];
        int user_num = split(user_content,';',user_group);
        for(int i=0;i<user_num;i++)
        {
            char course_group[BLOCKS_PER_INODE*(BLOCK_SIZE-1)] = "\0";
            char user_path[MAX_PATH_LEN] = "\0";
            snprintf(user_path, MAX_PATH_LEN, "/user/%s",user_group[i]);
            if(fs.getFileNames(user_path,course_group))
            {
                char course_group_id[100][USER_NUME_LEN];
                int course_num = split(course_group,';',course_group_id);
                for(int j=0;j<course_num;j++)
                {
                    char request_group[BLOCKS_PER_INODE*(BLOCK_SIZE-1)] = "\0";
                    char course_path[MAX_PATH_LEN] = "\0";
                    snprintf(course_path, MAX_PATH_LEN, "/user/%s/%s",user_group[i],course_group_id[j]);
                    if(fs.getFileNames(course_path,course_group))
                    {
                        char request_group_id[100][USER_NUME_LEN];
                        int request_num = split(course_group,';',request_group_id);
                        for(int k=0;k<request_num;k++)
                        {
                            if(strcmp(request_group_id[k],id)==0)
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}