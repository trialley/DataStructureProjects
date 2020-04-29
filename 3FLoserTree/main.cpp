#pragma warning(disable:4996)
#include"externalSort.h"
#include<string>
#include<iostream>

int main(){

    cout << R"( _                      _______            
| |                    |__   __|           
| |     ___  ___  ___ _ __| |_ __ ___  ___ 
| |    / _ \/ __|/ _ \ '__| | '__/ _ \/ _ \
| |___| (_) \__ \  __/ |  | | | |  __/  __/
|______\___/|___/\___|_|  |_|_|  \___|\___|
)";
    cout << "*LoserTree external sort"<<endl;
    cout << "* file path:";
    string filepath;
    cin >> filepath;
    cout << "* buffer size:";
    int size;
    cin >> size;
    externalsort es(size,filepath);

    //分割
    es.split();

    //进行归并
    es.merges();
    return 0;
}
