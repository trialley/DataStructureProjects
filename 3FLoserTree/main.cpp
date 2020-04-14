#pragma warning(disable:4996)
#include"externalSort.h"

int main(){
    int disk_buf_size  = 200;
    externalsort es(disk_buf_size);


    //es.clearfile(300);//用以清空产生的k个归并段文件

    //初始化
    es.initTreeAndSplitFile();

    //进行归并
    es.merges();

    //展示时间
    es.visitstime();
    return 0;
}
