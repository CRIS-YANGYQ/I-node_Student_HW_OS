#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <ctime>    // 用于 time_t
#include <sys/types.h>  // 用于 uid_t 和 gid_t
#include <sys/stat.h>   // 用于 mode_t
#include <string>
#include <cstring>
#include <unistd.h>


#define MANAGER 0
#define TEACHER 1
#define STUDENT 2
#define INODE_NUMBER 1024 //一共1024个inode
#define BLOCK_SIZE 1024
#define MAX_FILENAME_LENGTH 28//文件名长度最大为28
#define ENTRY_NUMBER 32
#define BLOCK_NUMBER INODE_NUMBER*3
#define USER_NUME_LEN 28
#define MAX_GROUP_NUM 100//用户组最多100人
#define MAX_PATH_LEN 128
#define BLOCKS_PER_INODE 3//每个INODE可以有的块
#define MAX_BACKUPFILE_SIZE 50//最多备份文件数目
#define FILE_NAME "filesystem.dat"

#define N 0 //没有权限
#define R 1 //读取权限
#define W 2 //读写权限


struct Inode {
    int blockIDs[BLOCKS_PER_INODE];     //block编号
    uint16_t blockNum;
    uint16_t inodeNumber;      // Inode 编号
    bool fileType;       // 文件类型（例如，普通文件、目录等）
    unsigned int fileSize;         // 文件大小32位
    time_t createdTime;        // 文件创建时间
    time_t modifiedTime;       // 文件最后修改时间
    size_t permission[3];//1是用户权限，2是用户组权限，3是其他权限
    char creator[USER_NUME_LEN];
    int groupNum;
    char group[MAX_GROUP_NUM][USER_NUME_LEN];

};

struct DirectoryBlock {
    char fileName[ENTRY_NUMBER][MAX_FILENAME_LENGTH];//32*28
    int inodeID[ENTRY_NUMBER];//32字节*4
};

struct FileBlock {
    char content[BLOCK_SIZE];//1024*1字节，可以写入1023个字符，最后一位要写入null表示结束
};




class fileSystem
{
public:
    /*
    初始化函数，载入硬盘上的文件系统信息
    */
    fileSystem();
   
    /*
    创建文件夹
    path:文件路径
    creator：创建者
    创建成功会返回1，失败返回0
    */
    bool createDirectory(const char *path,const char *creator);
    /*
    创建文件夹
    path:文件路径
    creator：创建者
    创建成功会返回1，失败返回0
    */
    bool createDirectoryIgnoeExit(const char *path,const char *creator);
    /*
    删除文件夹
    path:文件路径
    user：做出删除操作的用户id
    删除成功会返回1，失败返回0
    */
    bool deleteDirectory(const char *path,const char* user);  
    /*
    查看文件信息
    path:文件路径
    user：做出查看操作的用户id
    会打印文件信息：创建者，创建时间，修改时间等
    */
    void dispalyFile(const char *path,const char* user);  
    /*
    创建文件
    path:文件路径
    creator：创建者
    创建成功会返回1，失败返回0
    */
    bool createFile(const char *path,const char *creator);  
    /*
    删除文件
    path:文件路径
    user：做出删除操作的用户id
    删除成功会返回1，失败返回0
    */
    bool deleteFile(const char *path,const char* user);
    /*
    写入文件
    path:文件路径
    content:写入内容
    user：做出写入操作的用户id
    写入成功会返回1，失败返回0
    */
    bool writeFile(const char *path,char *content,const char* user);
    /*
    读取文件
    path:文件路径
    content:输入一个char*指针，用来存放读取的文件内容
    user：做出读取操作的用户id
    写入成功会返回1，失败返回0
    */
    bool readFile(const char *path,char *content,const char* user);
    /*
    在文件结尾写入内容
    path:文件路径
    content:写入内容
    user：做出写入操作的用户id
    写入成功会返回1，失败返回0
    */
    bool fileAddBack(const char *path,char *content,const char* user);  
    /**
    给文件组添加成员
    path：文件路径
    member：要添加的用户
    写入成功会返回1，失败返回0
    */
    bool addGroupMember(const char *path,const char *member);   
    /*
    更改文件权限
    path：文件路径
    user：要修改权限的用户
    A:创建者权限
    B:用户组权限
    C:其他用户权限
    返回1表示成功，0表示失败
    */
    bool modify_permission(const char *path,const char* user,size_t A,size_t B,size_t C);
    /*
    展示文件树
    path：文件路径
    layer：使用时不用填入，系统内部使用
    */
    void displayDirectory(const char *path, unsigned int layer=0);
    /*
    先不要用这个
    */
    void writeToFile(const char *filename) ;   
    // 备份文件3.0
    void backupFile(const char* sourceFile) ;

    void writeBackupFile(char* filename);

    void displayBackupFile();

    void backupRecover();
    /*
    查看路径是否存在
    path：路径
    返回值：1存在，0不存在
    */
    bool pathIsExist(const char *path);
    /*
    获取文件夹下的文件数量
    path：路径
    返回值：文件夹下的文件数量
    */
    int getFileNum(const char *path);
    /*
    获取文件夹下层文件名称
    path：路径
    content：文件夹下层文件名称，用；（分号）隔开，可以用split函数分割
    返回值：是否成功
    */
    bool getFileNames(const char *path,char content[BLOCK_NUMBER]);
    /*
    是否能访问这个文件
    path：路径
    user：用户名
    返回值：是否成功
    */
    bool canAccess(const char *path,const char* user);
private:
    Inode inodeMem[INODE_NUMBER];
    FileBlock blockMem[BLOCK_NUMBER];
    char blockBitmap[BLOCK_NUMBER/8];
    char* backupFileName[MAX_BACKUPFILE_SIZE];
    const char* baseDir = "./backup/";//存放备份文件的目录
    void readFromFile(const char *filename,bool &flag) {
            FILE *file = fopen(filename, "rb");

            if (file == NULL) {
                perror("There is no file");
                flag = 1;
                return;
            }
            fread(inodeMem, sizeof(struct Inode), INODE_NUMBER, file);
            fread(blockMem, sizeof(struct FileBlock), BLOCK_NUMBER, file);
            fread(blockBitmap, 1, BLOCK_NUMBER / 8, file);

            if (flag==1)//不要改变备份文件的值，是备份恢复
            {
                fclose(file);
                return;
            }
            else//是读取文件,要读备份
            {
                //fread(backupFileName,sizeof(char),MAX_BACKUPFILE_SIZE,file);
                // 分配内存并读取字符到数组中
                int numElements=sizeof(backupFileName)/sizeof(backupFileName[0]);
                char format[]="./backup/YYYY-MM-DD HH:MM:SS.dat";
                long fileLen=strlen(format);
                for (int i = 0; i < numElements; i++) {
                    backupFileName[i] = (char*)malloc(fileLen + 1); // 分配足够的内存来存储文件内容
                    fread(backupFileName[i], sizeof(char), fileLen, file);
                    backupFileName[i][fileLen] = '\0'; // 在结尾添加null字符
                }
                printf("已成功读入\n");
                fclose(file);
                return;
            }

        }
    /*
    删除目录
    功能函数，不用管
    */
    bool deleteDirectoryByName( const char *path ,const char *user,bool isFolder =0){
        //isFolder用来记录是不是当前要删除的文件夹，如果是当前是文件，输入的是文件时删除会失败
        char temp[MAX_PATH_LEN];
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *directory, *parent, *target;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        parent = NULL;
        directory = strtok(temp, delimiter);
        flag  =1;
        if (directory == NULL) {
            printf("It is the root directory!\n");
            return 0;
        }
        Inode* pointerGroup[10];
        int num = 0;
        while (directory != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointerGroup[num] = pointer;
                            num++;
                            pointer = &inodeMem[block->inodeID[i]];                          
                            k=pointer->blockNum+1;
                            break;                          
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = directory;
            directory = strtok(NULL, delimiter);
            
        }
        int entry = -1;
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }

        int n_block = -1, n_inode = -1;
        target = parent;
        char admin[28] = "admin";
        if (isFolder&&!((strcmp(user, pointer->creator) == 0)||(strcmp(user,admin) == 0)))
        {
            printf("无权限删除这个文件夹\n");
            return 0;
        }
        for (k = 0;k<=pointer->blockNum;k++)
        {
            block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
            if(pointer->fileType!=0&&isFolder == 0)
            {
                for (i = 0; i < ENTRY_NUMBER; i++) {  
                if (strcmp(block->fileName[i], target) == 0 && block->inodeID[i]!=-1) {
                    entry = i;
                    break;
                    }
                }
                if(entry>=0)
                {
                    n_inode = block->inodeID[entry];
                    for (int num = 0;num<inodeMem[n_inode].blockNum;num++)
                    {
                        n_block = inodeMem[n_inode].blockIDs[num];
                        block->inodeID[entry]=-1;
                        inodeMem[n_inode].blockIDs[0] = -1;
                        inodeMem[n_inode].blockNum = 0;
                        blockBitmap[n_block/8] &= ~(1 << (n_block % 8));
                    }
                    return 0;
                }
                else
                {
                    printf("file doesn't exist\n");
                    return 0;
                }
            }
            else if(pointer->fileType==0)
            {
                
                //当前是文件夹
                for (i = 0; i < ENTRY_NUMBER; i++) {
                    
                if (strcmp(block->fileName[i], target) == 0 && block->inodeID[i]!=-1) {
                    
                    entry = i;
                    break;
                    }
                }
                if (entry>=0)
                {
                    printf(target);
                    printf("------------\n");
                    n_inode = block->inodeID[entry];
                    for (int num=0;num<=inodeMem[n_inode].blockNum;num++)
                    {
                        n_block = inodeMem[n_inode].blockIDs[num];
                        struct DirectoryBlock *temp = (struct DirectoryBlock *) &blockMem[n_block];
                        for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (temp->inodeID[i] != -1) {
                            int inode_id =temp->inodeID[i];
                            int type = inodeMem[temp->inodeID[i]].fileType;
                            char filename[50];
                            strncpy(filename, temp->fileName[i],sizeof(filename));
                            if(type==0)
                            {
                                char filepath[150];
                                strncpy(filepath, path,sizeof(filepath));
                                if (filepath[strlen(filepath) - 1] != '/') {
                                    strcat(filepath, "/");
                                    }
                                strcat(filepath, filename);
                                deleteDirectoryByName(filepath,0);
                                temp->inodeID[i]=-1;
                                }
                            }
                        }
                        block->inodeID[entry]=-1;
                        inodeMem[n_inode].blockIDs[0] = -1;
                        blockBitmap[n_block/8] &= ~(1 << (n_block % 8));
                        inodeMem[n_inode].blockNum = 0;
                    }
                    if (isFolder ==1)
                    {
                        int size  =inodeMem[n_inode].fileSize;
                        time_t currentTime;
                        time(&currentTime);
                        if (num >0)
                        {
                            for (num = num-1;num>=0;num--)
                            {
                                pointerGroup[num]->modifiedTime = currentTime;
                                pointerGroup[num]->fileSize -= size;
                            }
                        }
                    }
                }
                else
                {
                    printf("folder doesn't exist\n");
                    return 0 ;
                }
            }
            else
            {
                printf("folder doesn't exist\n");
                return 0;
            }
        }
        return 1;
}
    /*
    查看用户是否在用户组中
    功能函数，不用管
    */
    bool isInGroup(char group[MAX_GROUP_NUM][USER_NUME_LEN],const char* user )//辅助函数，查看是否在用户组中
    {
        int i;
        for (i=0;i<MAX_GROUP_NUM;i++)
        {
            if (strcmp(group[i],user)==0) return 1;
        }
        return 0;
    }
};

#endif
