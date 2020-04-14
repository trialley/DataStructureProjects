#pragma warning(disable:4996)
#include"externalSort.h"

int main(){
    externalsort es(200);

    //分割
    es.split();

    //进行归并
    es.merges();
    return 0;
}
