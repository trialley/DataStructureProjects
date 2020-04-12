#pragma warning(disable:4996)
#include"externalSort.h"

int main(){
    int p = 100;
    int buf  = 200;
    externalsort es(p, buf);
    es.clearfile(300);//用以清空产生的k个归并段文件
    es.initialize();
    es.merges();
    es.visitstime();
    return 0;
}
