#ifndef FILE_H
#define FILE_H
#include "threadpool.h"

#define SIZE 512

struct Filepath
{
     char filesp[SIZE];
     char filedp[SIZE];

};

void rootdir(char sp[SIZE],char dp[SIZE]);
void printdir(char *dir,char sp[SIZE],char dp[SIZE],Threadpool *pool);
void replace_string(char *source_str,char *targ_str,char *val);
void mk_dir(const char dpath[]);
void copy_file(void *arg);

#endif
