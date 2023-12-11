#include "file.h"

template <typename T>
T my_min(T a, T b) {
    return a < b ? a : b;
}


    fileSystem::fileSystem()
    {   
        bool flag = 0;
        readFromFile("filesystem.dat",flag);
        if(flag)
            {int i,j;
            for (i = 0; i < INODE_NUMBER; i++) {
            inodeMem[i].inodeNumber = i;
            //inodeMem[i].blockID = -1;
            for (j=0;j<BLOCKS_PER_INODE;j++)
            {
                inodeMem[i].blockIDs[j] = -1;
            }
            }
            //inodeMem[0].blockID = 0;
            inodeMem[0].blockIDs[0] = 0;
            blockBitmap[0] |= 1;
            struct DirectoryBlock *root = (struct DirectoryBlock *) &blockMem[0];
            for (i = 0; i < ENTRY_NUMBER; i++) {
                root->inodeID[i] = -1;
            }
            //对备份文件数组初始化
            for (i=0;i<MAX_BACKUPFILE_SIZE;i++)
            {
                backupFileName[i]="\0";
            }
            char user[100] = "/user";
            createDirectory(user,"admin");
            char user_data[100] = "/user_data.csv";
            createFile(user_data,"admin");
            char *data_head = "user,password;";
            writeFile(user_data,data_head,"admin");
            char *admin = "admin,123;";
            fileAddBack(user_data,admin,"admin");
            printf("系统第一次开启\n");
        }
        printf("初始化完成\n");
    }
 
    bool fileSystem::createDirectory(const char *path,const char *creator)
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));

        int i, j, flag,k;
        char* directory, * parent, * target;
        const char delimiter[2] = "/";
        Inode* pointer = inodeMem;
        DirectoryBlock* block;
        parent = NULL;
        directory = strtok(temp, delimiter);

        if (directory == NULL) {
            printf("It is the root directory!\n");
            return 0;
        }
        Inode* pointerGroup[10];
        int num = 0;
        while (directory != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++)
                {
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            //判断创建者是否有创建文件的权限
                            if(strcmp(pointer->creator,creator)==0||(isInGroup(pointer->group,creator)&&pointer->permission[1]==W)||pointer->permission[2]==W)
                            {
                                pointerGroup[num] = pointer;
                                num++;
                                k = pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist!\n");
                    return 0;
                }
            }
            parent = directory;
            directory = strtok(NULL, delimiter);
        }

        // Create the target directory
        int entry = -1, n_block = -1, n_inode = -1;
        target = parent;
        if (sizeof(*target)>=28)
        {
            printf("文件名超出限制长度\n");
            return 0;
        }
        block = (struct DirectoryBlock*)&blockMem[pointer->blockIDs[pointer->blockNum]];

        for (i = 0; i < ENTRY_NUMBER; i++) {
            if (strcmp(block->fileName[i], target) == 0 && block->inodeID[i] != -1) {
                printf("The directory already exists!\n");
                return 0;
            }
            if (block->inodeID[i] == -1) {
                entry = i;
                break;
            }
        }
        //当前的block满了，需要扩容，找到一个新的空block来存放
        if(entry == -1&&pointer->blockNum<BLOCKS_PER_INODE-1)
        {
            int new_block_id = -1;
            for (i = 0; i < BLOCK_NUMBER / 8; i++) {
                for (j = 0; j < 8; j++) {
                    if ((blockBitmap[i] & (1 << j)) == 0) {
                        new_block_id = i * 8 + j;
                        break;
                    }
                }
            }
            if (new_block_id == -1) {
                printf("The block is full!\n");
                return 0;
            }
            blockBitmap[new_block_id / 8] |= 1 << (new_block_id % 8);
            pointer->blockNum =pointer->blockNum+1;
            pointer->blockIDs[pointer->blockNum] = new_block_id;
            block = (struct DirectoryBlock*)&blockMem[new_block_id];
            for (i = 0; i < ENTRY_NUMBER; i++) {
                block->inodeID[i] = -1;
            }
            entry = 0;
        }
        if (entry >= 0) {
            // Find a unused block
            for (i = 0; i < BLOCK_NUMBER / 8; i++) {
                for (j = 0; j < 8; j++) {
                    if ((blockBitmap[i] & (1 << j)) == 0) {
                        n_block = i * 8 + j;
                        break;
                    }
                }
                if (n_block != -1) {
                    break;
                }
            }
            if (n_block == -1) {
                printf("The block is full!\n");
                return 0;
            }
            // 找到没有使用的inode，标记为使用过
            flag = 0;
            for (i = 0; i < INODE_NUMBER; i++) {

                if (inodeMem[i].blockIDs[0] == -1) {
                    flag = 1;
                    time_t currentTime;
                    time(&currentTime);
                    inodeMem[i].blockIDs[0] = n_block;
                    inodeMem[i].fileType = 0;
                    inodeMem[i].fileSize = 0;
                    inodeMem[i].createdTime = currentTime;
                    inodeMem[i].modifiedTime = currentTime;
                    inodeMem[i].permission[0] = W;
                    inodeMem[i].permission[1] = W;
                    inodeMem[i].permission[2] = W;
                    strncpy(inodeMem[i].creator , creator,sizeof(inodeMem[i].creator));
                    inodeMem[i].groupNum = 0;
                    inodeMem[i].blockNum = 0;
                    n_inode = i;
                    if (num >0)
                    {
                        for (num = num-1;num>=0;num--)
                        {
                            pointerGroup[num]->modifiedTime = currentTime;
                        }
                    }
                    break;
                }
            }
            if (n_inode == -1) {
                printf("The inode is full!\n");
                return 0;
            }
            block->inodeID[entry] = n_inode;
            strncpy(block->fileName[entry], target, sizeof(block->fileName[entry]) - 1);
            block->fileName[entry][sizeof(block->fileName[entry]) - 1] = '\0';
            blockBitmap[n_block / 8] |= 1 << (n_block % 8);
            block = (struct DirectoryBlock*)&blockMem[n_block];
            num--;
            
            for (i = 0; i < ENTRY_NUMBER; i++) {
                block->inodeID[i] = -1;
            }
        }
        else {
            printf("The directory is full!\n");
            return 0;
        }

        return 1;
    }
    
    bool fileSystem::deleteDirectory(const char *path,const char* user) {
        bool isdelete = 0;
        isdelete = deleteDirectoryByName(path,user,1);
        if(isdelete)return 1;
        else printf("delete unsuccessfully\n");
        return 0;
    }

    void fileSystem::dispalyFile(const char *path,const char* user)
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *directory, *parent, *target;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        parent = NULL;
        directory = strtok(temp, delimiter);
        if (directory == NULL) {
            printf("It is the root directory!\n");
            return;
        }
        while (directory != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++)
                {
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) { 
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if(strcmp(pointer->creator,user) == 0||(isInGroup(pointer->group,user))&&pointer->permission[1]==R||pointer->permission[2]==R)
                            {
                                k = pointer->blockNum+1;
                                break;
                            }
                        else 
                            {
                                printf("you can't access this directory\n");
                                return ;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    printf("The path does not exist!\n");
                    return;
                }
            }
            
            parent = directory;
            directory = strtok(NULL, delimiter);
        }
        int entry = -1;
        target = parent;
        block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[pointer->blockNum]];
        for (i = 0; i < ENTRY_NUMBER; i++) {
            if (strcmp(block->fileName[i], target) == 0 && block->inodeID[i]!=-1) {
                entry = i;
                break;
            }
        }
        if (entry >= 0)
        {
            int n_inode = block->inodeID[entry];
            if (!(strcmp(inodeMem[n_inode].creator,user)==0 ||(isInGroup(inodeMem[n_inode].group,user)&&pointer->permission[1]>=R)||inodeMem[n_inode].permission[2]>=R))
                {
                    printf("You can't access this file %s\n",target);
                    return ;
                }
            printf("---------------------------------------\n");
            printf("file name:%s\n",block->fileName[i]);
            if (inodeMem[n_inode].fileType==0) printf("Type:Directory\n");
            else printf("Type:File\n");
            printf("file size:%dbytes\n",inodeMem[n_inode].fileSize);
            printf("file creator:%s\n",inodeMem[n_inode].creator);
            char *createTime = ctime(&inodeMem[n_inode].createdTime);
            createTime[strlen(createTime) - 1] = '\0'; // 去掉末尾的换行符
            printf("create time: %s\n", createTime);
            char *modifiedTime= ctime(&inodeMem[n_inode].modifiedTime);
            modifiedTime[strlen(modifiedTime) - 1] = '\0'; // 去掉末尾的换行符
            printf("modified time: %s\n", modifiedTime);
            printf("---------------------------------------\n");
        }
        else 
        {
            printf("The file doesn't exist\n");
        }
        return;
    }
  
    bool fileSystem::createFile(const char *path,const char *creator) 
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *directory, *parent, *target;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        parent = NULL;
        directory = strtok(temp, delimiter);
        Inode* pointerGroup[10];
        int num = 0;
        if (directory == NULL) {
            printf("It is the root directory!\n");
            return 0;
        }
        while (directory != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++)
                {
                    block = (DirectoryBlock*)&blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            //判断创建者是否有创建文件的权限
                            if(strcmp(pointer->creator,creator)==0||(isInGroup(pointer->group,creator)&&pointer->permission[1]==W)||pointer->permission[2]==W)
                            {
                                pointerGroup[num] = pointer;
                                num++;
                                k = pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist!\n");
                    return 0;
                }
            }
            parent = directory;
            directory = strtok(NULL, delimiter);
        }
        int entry = -1, n_block = -1, n_inode = -1;
        target = parent;
        if (sizeof(*target)>=28)
        {
            printf("文件名超出限制长度\n");
            return 0;
        }
        block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[pointer->blockNum]];
        for (i = 0; i < ENTRY_NUMBER; i++) {
            if (strcmp(block->fileName[i], target) == 0 && block->inodeID[i]!=-1) {
                printf("The directory already exists!\n");
                return 0;
            }
            if (block->inodeID[i] == -1) {
                entry = i;
                break;
            }
        }
        if(entry == -1&&pointer->blockNum<BLOCKS_PER_INODE-1)
        {
            int new_block_id = -1;
            // Find a unused block
            for (i = 0; i < BLOCK_NUMBER / 8; i++) {
                for (j = 0; j < 8; j++) {
                    if ((blockBitmap[i] & (1 << j)) == 0) {
                        new_block_id = i * 8 + j;
                        break;
                    }
                }
            }
            if (new_block_id == -1) {
                printf("The block is full!\n");
                return 0;
            }
            blockBitmap[new_block_id / 8] |= 1 << (new_block_id % 8);
            pointer->blockNum =pointer->blockNum+1;
            pointer->blockIDs[pointer->blockNum] = new_block_id;
            block = (struct DirectoryBlock*)&blockMem[new_block_id];
            for (i = 0; i < ENTRY_NUMBER; i++) {
                block->inodeID[i] = -1;
            }
            entry = 0;
        }
        if (entry >= 0) {
            // Find a unused block
            for (i = 0; i < BLOCK_NUMBER/8; i++) {
                for (j = 0; j < 8; j++) {
                    if ((blockBitmap[i] & (1 << j)) == 0) {
                        n_block = i * 8 + j;
                        break;
                    }
                }
                if (n_block != -1) {
                    break;
                }
            }
            if (n_block == -1) {
                printf("The block is full!\n");
                return 0;
            }

            // 找到没有使用的inode，标记为使用过
            flag = 0;
            for (i = 0; i < INODE_NUMBER; i++) {
                if (inodeMem[i].blockIDs[0] == -1) {
                    flag = 1;
                    time_t currentTime;
                    time(&currentTime);
                    inodeMem[i].blockIDs[0] = n_block;
                    inodeMem[i].fileType = 1;
                    inodeMem[i].fileSize = 0;
                    inodeMem[i].createdTime = currentTime;
                    char *formattedTime = ctime(&currentTime);
                    formattedTime = ctime(&inodeMem[i].createdTime);
                    inodeMem[i].modifiedTime = currentTime;
                    inodeMem[i].permission[0] = W;
                    inodeMem[i].permission[1] = W;
                    inodeMem[i].permission[2] = W;
                    inodeMem[i].groupNum = 0;
                    inodeMem[i].blockNum = 0;
                    strncpy(inodeMem[i].creator , creator,sizeof(inodeMem[i].creator));
                    n_inode = i;
                    if(num>0)
                    {
                        for(num = num-1;num>=0;num--)
                        {
                            pointerGroup[num]->modifiedTime = currentTime; 
                        }
                    }
                    break;
                }
            }
            if (n_inode == -1) {
                printf("The inode is full!\n");
                return 0;
            }
            block->inodeID[entry] = n_inode;
            strncpy(block->fileName[entry], target,sizeof(block->fileName[entry]));
            blockBitmap[n_block/8] |= 1 << (n_block % 8);
            block = (struct DirectoryBlock *) &blockMem[n_block];
        }
        else {
            printf("The directory is full!\n");
            return 0;
        }
        return 1;
        }

    bool fileSystem::deleteFile(const char *path,const char* user)
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        Inode* pointerGroup[10];
        int num = 0;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if(strcmp(pointer->creator,user)==0||(isInGroup(pointer->group,user)&&pointer->permission[1]==W)||pointer->permission[2]==W||strcmp("admin",user)==0)
                            {
                                pointerGroup[num] = pointer;
                                num++;
                                k = pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
        }
        
        //查找到目标文件
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry =i;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if (pointer->fileType != 1) {
            printf("The specified path does not point to a file!\n");
            return 0;
        }
        int  n_block = -1, n_inode = -1;
        if (entry >= 0) {
            n_inode = block->inodeID[entry];
            if (!(strcmp(inodeMem[n_inode].creator,user)==0 ||(isInGroup(inodeMem[n_inode].group,user)&&pointer->permission[1]==W)||inodeMem[n_inode].permission[2]==W))
            {
                printf("You can't access this file %s\n",target);
                return 0;
            }
            //找blockid
            size_t size = inodeMem[n_inode].fileSize;
            for (int num = 0;num<=inodeMem[n_inode].blockNum;num++)
            {
            n_block = inodeMem[n_inode].blockIDs[num];
            if (num==0)size = inodeMem[n_inode].fileSize;
            block->inodeID[entry]=-1;
            inodeMem[n_inode].blockIDs[0] = -1;
            blockBitmap[n_block/8] &= ~(1 << (n_block % 8));
            }
            inodeMem[n_inode].blockNum = 0;
            time_t currentTime;
            time(&currentTime);
            if (num >0)
                
                {
                    for (num = num-1;num>=0;num--)
                    {
                        pointerGroup[num]->fileSize -= size;
                        pointerGroup[num]->modifiedTime = currentTime;
                    }
                }
            printf("delete successfully\n");
        }
        else {
            printf("delete unsuccessfully\n");
            return 0;
        }
        return 1;
    }
  
    bool fileSystem::writeFile(const char *path,char *content,const char* user) {
        
        if (strlen(content)>(BLOCK_SIZE-1)*BLOCKS_PER_INODE)
        {
            printf("字符长度：%zu输入内容过长,请控制字数!\n",strlen(content));
            return 0;
        }
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        Inode *pointer = inodeMem;
        DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        Inode* pointerGroup[10];
        int num = 0;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if(strcmp(pointer->creator,user)==0||(isInGroup(pointer->group,user)&&pointer->permission[1]==W)||pointer->permission[2]==W)
                            {
                                pointerGroup[num] = pointer;
                                num++;
                                k = pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
        }
        //查找到目标文件
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry =i;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if (pointer->fileType != 1) {
            printf("The specified path does not point to a file!\n");
            return 0;
        }
        int  n_block = -1, n_inode = -1;
        if (entry >= 0) {
            int fileInodeID = block->inodeID[entry];
            if (!(strcmp(inodeMem[fileInodeID].creator,user)==0 ||(isInGroup(inodeMem[fileInodeID].group,user)&&pointer->permission[1]==W)||inodeMem[fileInodeID].permission[2]==W))
            {
                printf("You can't access this file %s\n",target);
                return 0;
            }
            int m;
            for (m = 0;m<int(strlen(content)/BLOCK_SIZE);m++)
            {
                int new_block_id = -1;
                for (i = 0; i < BLOCK_NUMBER / 8; i++) {
                    for (j = 0; j < 8; j++) {
                        if ((blockBitmap[i] & (1 << j)) == 0) {
                            new_block_id = i * 8 + j;
                            break;
                        }
                    }
                }
                if (new_block_id == -1) {
                    printf("The block is full!\n");
                    return 0;
                }
                inodeMem[fileInodeID].blockIDs[m+1] = new_block_id;//给下一个块指针赋值
                blockBitmap[new_block_id / 8] |= 1 << (new_block_id % 8);
            }
            j = 0;
            unsigned int size = 0;
            for (i = 0; i < int(strlen(content) / BLOCK_SIZE) + 1; i++) {
                struct FileBlock *fileBlock = (struct FileBlock *) &blockMem[inodeMem[fileInodeID].blockIDs[i]];
                int contentLength = strlen(content + j);
                int copyLength = my_min(contentLength, BLOCK_SIZE-1);
                if (copyLength!=(BLOCK_SIZE-1)) copyLength = copyLength +1;
                strncpy(fileBlock->content, content + j, copyLength);
                fileBlock->content[copyLength] = '\0';
                j += copyLength;
                size += copyLength;
            }
            struct FileBlock *fileBlock = (struct FileBlock *) &blockMem[inodeMem[fileInodeID].blockIDs[0]];
            inodeMem[fileInodeID].fileSize = size;
            time_t currentTime;
            time(&currentTime);
            inodeMem[fileInodeID].modifiedTime = currentTime;
            inodeMem[fileInodeID].blockNum = int(strlen(content)/BLOCK_SIZE);

            if (num >0)
            {
                for (num = num-1;num>=0;num--)
                {
                    pointerGroup[num]->modifiedTime = currentTime;
                    pointerGroup[num]->fileSize +=size;
                }
            }
            size_t contentSize = 0;
        }
        else {
            printf("Fail to write the file!\n");
        }
        return 1;
    }
  
    bool fileSystem::readFile(const char *path,char *content,const char* user) {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if(strcmp(pointer->creator,user) == 0||(isInGroup(pointer->group,user))&&pointer->permission[1]>=R||pointer->permission[2]>=R)
                            {
                                k=pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
            
        }
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry = i;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if (pointer->fileType != 1) {
            printf("The specified path does not point to a file!\n");
            return 0;
        }
        int n_block = -1, n_inode = -1;

        
        if (entry >= 0) {
            int fileInodeID = block->inodeID[entry];
            if (!(strcmp(inodeMem[fileInodeID].creator,user)==0 ||(isInGroup(inodeMem[fileInodeID].group,user)&&pointer->permission[1]>=R)||inodeMem[fileInodeID ].permission[2]>=R))
            {
                printf("You can't access this file %s\n",target);
                return 0;
            }
        size_t contentSize = 0;
        for (i = 0; i <= inodeMem[fileInodeID].blockNum; i++) {
            FileBlock *fileBlock = (struct FileBlock *)&blockMem[inodeMem[fileInodeID].blockIDs[i]];

            size_t blockContentSize = strlen(fileBlock->content);
            memcpy(content + contentSize, fileBlock->content, blockContentSize);
            contentSize += blockContentSize;  
        }

        content[contentSize] = '\0';
            
        }
        else {
            printf("Fail to read the file!\n");
            return 0;
        }
        return 1;
}
  
    bool fileSystem::fileAddBack(const char *path,char *content,const char* user)
    {
        if (strlen(content)>(BLOCK_SIZE-1)*BLOCKS_PER_INODE)
        {
            printf("字符长度：%zu输入内容过长,请控制字数!\n",strlen(content));
            return 0;
        }
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        Inode* pointerGroup[10];
        int num = 0;
        flag =1;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if(strcmp(pointer->creator,user)==0||(isInGroup(pointer->group,user)&&pointer->permission[1]==W)||pointer->permission[2]==W)
                            {
                                pointerGroup[num] = pointer;
                                num++;
                                k = pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
        }
        
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry =i;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if (pointer->fileType != 1) {
            printf("The specified path does not point to a file!\n");
            return 0;
        }
        int  n_block = -1, n_inode = -1;
        if (entry >= 0) {
            int fileInodeID = block->inodeID[entry];
            if (!(strcmp(inodeMem[fileInodeID].creator,user)==0 ||(isInGroup(inodeMem[fileInodeID].group,user)&&pointer->permission[1]==W)||inodeMem[fileInodeID].permission[2]==W))
            {
                printf("You can't access this file %s\n",target);
                return 0;
            }
            if (strlen(content)+inodeMem[fileInodeID].fileSize>=(BLOCK_SIZE-1)*BLOCKS_PER_INODE)
            {
                printf("文件大小超出限制\n");
                return 0;
            }
            //先把文件读入，然后
            char allContent[(BLOCK_SIZE-1)*BLOCKS_PER_INODE];
            size_t contentSize = 0;
            for (i = 0; i <= inodeMem[fileInodeID].blockNum; i++) {
            FileBlock *fileBlock = (struct FileBlock *)&blockMem[inodeMem[fileInodeID].blockIDs[i]];

            size_t blockContentSize = strlen(fileBlock->content);
            memcpy(allContent + contentSize, fileBlock->content, blockContentSize);
            contentSize += blockContentSize;  
            }
            strncat(allContent,content,strlen(content));
            contentSize+=strlen(content);
            allContent[contentSize] = '\0';
            int m;
            for (m = inodeMem[fileInodeID].blockNum+1;m<int((strlen(content)+inodeMem[fileInodeID].fileSize)/BLOCK_SIZE);m++)
            {
                if (m==BLOCKS_PER_INODE) break;
                int new_block_id = -1;
                for (i = 0; i < BLOCK_NUMBER / 8; i++) {
                    for (j = 0; j < 8; j++) {
                        if ((blockBitmap[i] & (1 << j)) == 0) {
                            new_block_id = i * 8 + j;
                            break;
                        }
                    }
                }
                if (new_block_id == -1) {
                    printf("The block is full!\n");
                    return 0;
                }
                inodeMem[fileInodeID].blockIDs[m+1] = new_block_id;//给下一个块指针赋值
                blockBitmap[new_block_id / 8] |= 1 << (new_block_id % 8); 
            }

            j = 0;
            unsigned int size = 0;
            for (i = 0; i < int(strlen(allContent) / BLOCK_SIZE) + 1; i++) {
                struct FileBlock *fileBlock = (struct FileBlock *) &blockMem[inodeMem[fileInodeID].blockIDs[i]];
                int contentLength = strlen(allContent + j);
                int copyLength = my_min(contentLength, BLOCK_SIZE-1);
                if (copyLength!=(BLOCK_SIZE-1)) copyLength = copyLength +1;
                strncpy(fileBlock->content, allContent + j, copyLength);
                fileBlock->content[copyLength] = '\0';
                j += copyLength;
                size += copyLength;
            }
            
            struct FileBlock *fileBlock = (struct FileBlock *) &blockMem[inodeMem[fileInodeID].blockIDs[0]];
            inodeMem[fileInodeID].fileSize = size;
            time_t currentTime;
            time(&currentTime);
            inodeMem[fileInodeID].modifiedTime = currentTime;
            inodeMem[fileInodeID].blockNum = int(strlen(allContent)/BLOCK_SIZE);

            if (num >0)
            {
                for (num = num-1;num>=0;num--)
                {
                    pointerGroup[num]->modifiedTime = currentTime;
                    pointerGroup[num]->fileSize += strlen(content);
                }
            }
        }
        else {
            printf("Fail to write the file!\n");
        }
        return 1;
    }
   
    bool fileSystem::addGroupMember(const char *path,const char *member)
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    pointer = &inodeMem[block->inodeID[i]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
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
            parent = filename;
            filename = strtok(NULL, delimiter);
            
        }
        
        if(flag)
        {
            for (k = 0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            break;
                        }
                    }
            }
        }
        int entry = -1, n_block = -1, n_inode = -1;
        target = parent;//target是要写入的文件
        for (i = 0; i < ENTRY_NUMBER; i++) {
            if(strcmp(block->fileName[i], target) == 0)//找到目标文件的位置
            {
                entry =i;
                break;
            }
        }
        if (entry >= 0) {
            int fileInodeID = block->inodeID[entry];
            if (inodeMem[fileInodeID].groupNum>=MAX_GROUP_NUM)
            {
                printf("group is full");
                return 0;
            }
            //添加成员
            strncpy(inodeMem[fileInodeID].group[inodeMem[fileInodeID].groupNum],member,sizeof(inodeMem[fileInodeID].group[inodeMem[fileInodeID].groupNum]));
            inodeMem[fileInodeID].groupNum = inodeMem[fileInodeID].groupNum+1;
        }
        else {
            printf("Fail to add member to group!\n");
        }
        return 1;
    }
 
    bool fileSystem::modify_permission(const char *path,const char* user,size_t A,size_t B,size_t C)//修改文件权限（只有文件所有者和管理员可以使用）
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        Inode* pointerGroup[10];
        int num = 0;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if(strcmp(pointer->creator,user)==0||(isInGroup(pointer->group,user)&&pointer->permission[1]==W)||pointer->permission[2]==W)
                            {
                                pointerGroup[num] = pointer;
                                num++;
                                k = pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
        }
        int entry = -1;
        if(flag)
        {
            for (k=0;k<=pointer->blockNum;k++){
            block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
            for (i = 0; i < ENTRY_NUMBER; i++) {
                    if (strcmp(block->fileName[i], parent) == 0) {
                        flag = 1;
                        entry =i;
                        pointer = &inodeMem[block->inodeID[i]];
                        k = pointer->blockNum+1;
                        break;
                    }
                }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        
        
        if (entry >= 0) {
            int fileInodeID = block->inodeID[entry];
            char admin[28] = "admin";
            if (!(strcmp(inodeMem[fileInodeID].creator,user)==0 ||(strcmp(user,admin)==0)))
            //只有文件创建者和管理员可以修改文件权限
            {
                printf("You can't access this file %s\n",target);
                return 0;
            }
            inodeMem[fileInodeID].permission[0] = A;
            inodeMem[fileInodeID].permission[1] = B;
            inodeMem[fileInodeID].permission[2] = C;
        }
        else {
            printf("Fail to modify the permission!\n");
        }
        return 1;
    }
  
    void fileSystem::displayDirectory(const char *path, unsigned int layer)
    {
        int i, flag,k;
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        char* directory, * parent, * filename;
        const char delimiter[2] = "/";
        struct Inode* pointer = inodeMem;
        struct DirectoryBlock* block;
        directory = strtok(temp, delimiter);
        if (directory == NULL)
        {
            printf("root\n");
        }
        
        else {
            while (directory != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock*)&blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], directory) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist!\n");
                    return;
                }
                filename = directory;
                directory = strtok(NULL, delimiter);
                
            }
            int l;
            for (l = 0; l < layer; ++l) {
                std::cout << "----";
            }
            printf("%s\n", filename);
        }
        
        for (k=0;k<=pointer->blockNum;k++)
            {
            block = (struct DirectoryBlock*)&blockMem[pointer->blockIDs[k]];
            for (i = 0; i < ENTRY_NUMBER; i++) {
                if (block->inodeID[i] != -1) {
                    if (inodeMem[block->inodeID[i]].fileType == 0)
                    {
                        char filepath[100];
                        strcpy(filepath, path);
                        if (filepath[strlen(filepath) - 1] != '/') {
                            strcat(filepath, "/");
                        }
                        char filename[50];
                        strcpy(filename, block->fileName[i]);
                        strcat(filepath, filename);
                        displayDirectory(filepath, layer + 1);
                    }
                    else
                    {
                        int l;
                        for (l = 0; l < layer + 1; ++l) {
                            std::cout << "----";
                        }
                        printf("%s\n", block->fileName[i]);
                    }
                }

            }
            }
        return;
    }
 
    void fileSystem::writeToFile(const char *filename) {
        FILE *file = fopen(filename, "wb");

        if (file == NULL) {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }

        fwrite(inodeMem, sizeof(struct Inode), INODE_NUMBER, file);
        fwrite(blockMem, sizeof(struct FileBlock), BLOCK_NUMBER, file);
        fwrite(blockBitmap, 1, BLOCK_NUMBER / 8, file);
        // 写入char*数据到文件
        for (int i = 0; i < MAX_BACKUPFILE_SIZE; i++) {
                size_t numBytes = strlen(backupFileName[i]); // 获取字符串的字节数
                fwrite(backupFileName[i], sizeof(char), numBytes, file);
            }

        //fwrite(backupFileName,sizeof(char),MAX_BACKUPFILE_SIZE,file);
        std::cout<<"成功完成写入源文件"<<std::endl;

        fclose(file);
    }
   
    void fileSystem::backupFile(const char* sourceFile) {
        FILE* source, * destination;
        char buffer[1024];
        size_t bytesRead;
        char destinationFile[1024];

        // 打开源文件以读取数据
        source = fopen(sourceFile, "rb");
        if (source == NULL) {
            perror("Error opening source file??");
            return;
        }
        std::cout<<"成功打开源文件"<<std::endl;

        // 创建备份文件夹
        if (access(baseDir, 0) != 0) {
            mkdir(baseDir, 0700); // 创建储存备份文件的目录
        }

        std::cout<<"检查完是否创建了备份文件夹"<<std::endl;


        // 生成备份文件名
        time_t t = time(NULL);
        strftime(destinationFile, sizeof(destinationFile), "%Y-%m-%d %H:%M:%S.dat", localtime(&t));
        std::cout<<"生成备份文件名"<<std::endl;
        
        char path[1024];
        snprintf(path, sizeof(path), "%s%s", baseDir, destinationFile);
        std::cout<<"生成路径"<<path<<std::endl;
        //printf(path);
        //写入备份文件数组
        writeBackupFile(path);
        //写入源文件的数组
        writeToFile(FILE_NAME);
        // //重新初始化
        // bool flag;
        // readFromFile(FILE_NAME,flag);
        // 创建备份文件以写入数据
        destination = fopen(path, "wb");
        if (destination == NULL) {
            perror("Error creating destination file");
            fclose(source);
            return;
        }

        // 逐块读取源文件内容并写入备份文件
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
            fwrite(buffer, 1, bytesRead, destination);
        }

        // 关闭文件
        fclose(source);
        fclose(destination);

        printf("Backup copy success.\n");
    }

    void fileSystem::writeBackupFile(char* filename) {
        bool inserted = false;
         std::cout<<"准备插入数组"<<std::endl;

        for (int i = 0; i < MAX_BACKUPFILE_SIZE; i++) {
            if (strcmp(backupFileName[i], "\0") == 0) {
                strcpy(backupFileName[i], filename);
                inserted = true;
                break;
            }
        }
        //如果已经满了，替代最老的
        if (!inserted) {
            for (int i = 0; i < MAX_BACKUPFILE_SIZE - 1; i++) {
                strcpy(backupFileName[i], backupFileName[i + 1]);
            }
            strcpy(backupFileName[MAX_BACKUPFILE_SIZE - 1], filename);
        }
        std::cout<<"成功插入数组"<<std::endl;
    }


    void fileSystem::displayBackupFile()//列出目前所有的备份文件
    {
        printf("success run displayBackfile\n");
        int i;
        for (i = 0; backupFileName[i] != NULL; i++) {
            if (strcmp(backupFileName[i], "\0") == 0) {
                break;
            }
            printf("%s\n", backupFileName[i]);
        }
        //printf(backupFileName[0]);
    }
   
    void fileSystem::backupRecover()//启用备份文件
    {
        char filename[100];//先用char，不要用char*
        printf("input the backup file you want:\n");
        scanf("%99[^\n]%*c", filename); // 限制读取的字符数为99，并丢弃剩余的字符和换行符，会忽略空白字符
        //getchar(); // 读取并丢弃换行符


        const char* path=filename;

        FILE *source, *destination;
        char buffer[1024];
        size_t bytesRead;

        // char* basepath;
        // strcpy(basepath, baseDir);
        // const char* path=strcat(basepath,filename);

        // 打开源文件以读取数据
        printf("%s\n", path);
        source = fopen(path, "rb");
        printf("成功读取备份文件\n");
        if (source == NULL) {
            perror("Error opening source file");
            return;
        }

        destination = fopen(FILE_NAME, "wb");//打开源文件
        // fseek(destination, 0, SEEK_SET);// 将文件指针位置设置到文件开头
        // ftruncate(fileno(destination), 0);// 清空文件内

        // 逐块读取备份文件内容并写入目标文件
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
            fwrite(buffer, 1, bytesRead, destination);
        }

        printf("成功替换备份文件\n");

        //调用初始化函数，重新初始化
        bool flag=1;
        readFromFile(FILE_NAME,flag);
        //重新写一次dat文件
        writeToFile(FILE_NAME);
    }
  
    bool fileSystem::pathIsExist(const char *path)
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        Inode *pointer = inodeMem;
        DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        Inode* pointerGroup[10];
        int num = 0;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            pointerGroup[num] = pointer;
                            num++;
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
        }
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            k = pointer->blockNum+1;
                            return 1;
                            
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path %s does not exist!\n", path);
            return 0;
        }
        if(!flag)
        {
            printf("The specified path %s does not exist!\n", path);
            return 0;
        }
    }
   
    int fileSystem::getFileNum(const char *path)
    {
        int filenum = 0;
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return -1;
        }
        flag =1;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            k=pointer->blockNum+1;
                            break;
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist or is not a directory!\n");
                    return -1;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
            
        }
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry = i;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return -1;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return -1;
        }
        if (pointer->fileType != 0) {
            printf("The specified path does not point to a directory!\n");
            return -1;
        }
        for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (block->inodeID[i]!=-1) {
                            filenum++;
                        }
                        else break;
                    }
            }

        return filenum;
    }
 
    bool fileSystem::getFileNames(const char *path,char content[BLOCK_NUMBER])
    {
        int filenum = 0;
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
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
            parent = filename;
            filename = strtok(NULL, delimiter);
            
        }
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry = i;
                            pointer = &inodeMem[block->inodeID[i]];
                            k = pointer->blockNum+1;
                            break;
                        }
                    }
            }
        }
        else 
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if(!flag)
        {
            printf("The specified path does not exist!\n");
            return 0;
        }
        if (pointer->fileType != 0) {
            printf("The specified path does not point to a directory!\n");
            return 0;
        }
        for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (block->inodeID[i]!=-1) {
                            filenum++;
                            strcat(content,block->fileName[i]);
                            strcat(content,";");
                        }
                        else break;
                    }
            }

        return 1;
    }
    
    bool fileSystem::canAccess(const char *path,const char* user)
    {
        char temp[MAX_PATH_LEN] = "\0";
        strncpy(temp,path,sizeof(temp));
        int i, j, flag,k;
        char *filename, *parent, *target;
        parent = NULL;
        const char delimiter[2] = "/";
        struct Inode *pointer = inodeMem;
        struct DirectoryBlock *block;
        filename = strtok(temp, delimiter);
        if (filename == NULL) {
            printf("Invalid file path!\n");
            return 0;
        }
        flag =1;
        while (filename != NULL) {
            if (parent != NULL) {
                flag = 0;
                for (k=0;k<=pointer->blockNum;k++){
                    block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                    for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            pointer = &inodeMem[block->inodeID[i]];
                            if((isInGroup(pointer->group,user))&&pointer->permission[1]>=R||pointer->permission[2]>=R)
                            {
                                k=pointer->blockNum+1;
                                break;
                            }
                            else 
                            {
                                printf("you can't access this directory\n");
                                return 0;
                            }
                        }
                    }
                }
                if (flag == 0 || pointer->fileType == 1) {
                    // If the parent directory does not exist or is not a directory file
                    printf("The path does not exist or is not a directory!\n");
                    return 0;
                }
            }
            parent = filename;
            filename = strtok(NULL, delimiter);
            
        }
        int entry = -1;
        if(flag)
        {
            flag = 0;
            for (k=0;k<=pointer->blockNum;k++){
                block = (struct DirectoryBlock *) &blockMem[pointer->blockIDs[k]];
                for (i = 0; i < ENTRY_NUMBER; i++) {
                        if (strcmp(block->fileName[i], parent) == 0) {
                            flag = 1;
                            entry = i;
                            pointer = &inodeMem[block->inodeID[i]];
                            if((isInGroup(pointer->group,user))&&pointer->permission[1]>=R)
                            {
                                k=pointer->blockNum+1;
                                return 1;
                            }
                        }
                    }
            }
        }
        return 0;
    }



  
    