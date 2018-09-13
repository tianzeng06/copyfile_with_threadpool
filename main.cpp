#include "header.h"
#include <string.h>
#include "file.h"

int main()
{
    int numberthread;
    cout<<"Please input the MAX number to start thread: ";
    cin>>numberthread;
    Threadpool *pool=new Threadpool(numberthread);//初始化线程池中线程数量，默认36
 
    char s1[50],s2[50];
    memset(s1,0,sizeof(s1));
    memset(s2,0,sizeof(s2));
    cout<<"Please input the source_dir and dest_dir: ";
    cin>>s1>>s2;
    
    rootdir(s1,s2);
    printdir(s1,s1,s2,pool);

    return 0;
}
