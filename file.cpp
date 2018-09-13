#include "file.h"

void printdir(char *dir,char sp[SIZE],char dp[SIZE],Threadpool *pool)
{
    DIR *Dp;//打开目录指针
    struct dirent *entry;//文件结构体目录
    struct stat statbuf;//详细文件信息结构体
    char dircwd[SIZE],filecwd[SIZE];
    memset(dircwd,0,sizeof(dircwd));
    memset(filecwd,0,sizeof(filecwd));

    if(NULL==(Dp=opendir(dir)))//打开指定目录，获得目录指针
    {
        cerr<<"can't not open dir: "<<dir<<endl;
        return;
    }
    
    chdir(dir);//切换到这个目录

    while(NULL!=(entry=readdir(Dp)))//遍历目录下所有的文件
    {
        lstat(entry->d_name,&statbuf);//通过文件名得到文件详细信息
        
        if(S_ISDIR(statbuf.st_mode))//判断是不是目录
        {
            if(strcmp(".",entry->d_name)==0||strcmp("..",entry->d_name)==0)
                continue;//当前目录和上级目录过滤
            
            getcwd(dircwd,SIZE);
            strcat(dircwd,"/");
            strcat(dircwd,entry->d_name);//工作路径+目录名
            replace_string(dircwd,sp,rindex(sp,'/'));
            
            char dirdp[SIZE];
            sprintf(dirdp,"%s%s",dp,dircwd);//目录最终绝对路径
            mk_dir(dirdp);//创建目录
           
            printdir(entry->d_name,sp,dp,pool);//递归调用，直到找到文件
        }
        else
        {
            struct Filepath fpath;
            char temp_path[SIZE];
            getcwd(filecwd,SIZE);
            sprintf(fpath.filesp,"%s/%s",filecwd,entry->d_name);//源文件绝对路径
            strcpy(temp_path,fpath.filesp);//暂存源文件绝对路径
            replace_string(temp_path,sp,rindex(sp,'/'));
            sprintf(fpath.filedp,"%s%s",dp,temp_path);//目标文件绝对路径
            pool->addtask(copy_file,&fpath);
            usleep(9000);
        }
    }

    chdir("..");//切换到上级目录
    closedir(Dp);//关闭文件指针
}

//将字符串(source_str)中指定字符串(targ_str)用指定字符串(val)替换
void replace_string(char *source_str,char *targ_str,char *val)
{
    char temp_sstr[SIZE],result[SIZE];
    char *p=NULL,*q=NULL;
    int len=0;
    memset(result,0,sizeof(result));
    memset(temp_sstr,0,sizeof(temp_sstr));
    strcpy(temp_sstr,source_str);
    p=temp_sstr;
    q=temp_sstr;
    len=strlen(targ_str);

    while(q!=NULL)
    {
        if((q=strstr(p,targ_str))!=NULL)
        {
            strncat(result,p,q-p);
            strcat(result,val);
            strcat(result,"\0");
            q+=len;
            p=q;
        }
        else
            strcat(result,p);
    }
    strcpy(source_str,result);
}

//创建目录函数
void mk_dir(const char dpath[])
{
    mkdir(dpath,S_IRWXU|S_IRGRP|S_IROTH);
}

//复制文件
void copy_file(void *arg)
{
    Filepath *p=(Filepath *)arg;
    FILE *fb_from,*fb_to;

    int rd_cnt;
    char rd_buf[1024];
    fb_from=fopen(p->filesp,"r");
    if(fb_from==NULL)
    {
        cerr<<"read open file failure: "<<p->filesp<<endl;
        return;
    }

    fb_to=fopen(p->filedp,"w+");
    //perror("fopen error: ");//检测fopen为什么出错
    if(fb_to==NULL)
    {
        cerr<<"write opoen file failure: "<<p->filedp<<endl;
        return;
    }

    while((rd_cnt=fread(rd_buf,sizeof(char),1024,fb_from))>0)
    {
        fwrite(rd_buf,sizeof(char),rd_cnt,fb_to);
        fflush(fb_to);
    }

    cout<<"copy success : "<<p->filedp<<endl;
    fclose(fb_from);
    fclose(fb_to);
    return;
}

void rootdir(char sp[SIZE],char dp[SIZE])
{
    mkdir(dp,S_IRWXU|S_IRGRP|S_IROTH);//创建目标跟目录
    char dp1[SIZE];
    strcpy(dp1,dp);
    strcat(dp1,rindex(sp,'/'));
    mkdir(dp1,S_IRWXU|S_IRGRP|S_IROTH);//创建源根目录
}
