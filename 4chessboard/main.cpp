#pragma warning(disable:4996)
#include <stdio.h>
#include<iostream>
//#define OS_TYPE_WINDOWS_CC
#include"ColorfulConsoleIO.h"
#include <math.h>
#include <iomanip>
void ChessBoard(int tr, int tc, int dr, int dc, int size);
void OutputBoard(int size);

int  tile = 1;
int  Board[1025][1025];
int main(){
    int n, a, b;
    std::cout << R"(  _____ _                   ____                      _ 
 / ____| |                 |  _ \                    | |
| |    | |__   ___  ___ ___| |_) | ___   __ _ _ __ __| |
| |    | '_ \ / _ \/ __/ __|  _ < / _ \ / _` | '__/ _` |
| |____| | | |  __/\__ \__ \ |_) | (_) | (_| | | | (_| |
 \_____|_| |_|\___||___/___/____/ \___/ \__,_|_|  \__,_|
)";
    std::cout << "请输入递归次数：";
    scanf("%d", &n);//输入2的幂次

    int sum;
    sum = pow(2, n);
    std::cout << "请输入两个数字代表残缺点：";

    scanf("%d %d", &a, &b);
    Board[sum - a - 1][b - 1] = 0;
    
    
    std::cout << "开始递归计算。\n";
    ChessBoard(0, 0, a - 1, b - 1, sum);
    std::cout << "递归计算完成。\n";
    OutputBoard(sum);
    std::cout << "输出完成。";

    return 0;
}

//残缺棋盘的递归程序
void ChessBoard(int tr, int tc, int dr, int dc, int size){
    if (size == 1) return;
    int num = tile++;    //所使用的三格板的编号
    int sub_size = size / 2;//象限大小
    
    //对于左上象限
    if (dr < tr + sub_size && dc < tc + sub_size) {//残缺方格位于本象限
        ChessBoard(tr, tc, dr, dc, sub_size);
    }else{//本象限中没有残缺方格, 把三格板num号放在右下角，这样四个象限均有一个残缺
        Board[tr + sub_size - 1][tc + sub_size - 1] = num;
        ChessBoard(tr, tc, tr + sub_size - 1, tc + sub_size - 1, sub_size);
    }
    
    //右上象限
    if (dr < tr + sub_size && dc >= tc + sub_size) {//残缺方格位于本象限
        ChessBoard(tr, tc + sub_size, dr, dc, sub_size);
    }else {    //本象限中没有残缺方格, 把三格板t放在左下角
        Board[tr + sub_size - 1][tc + sub_size] = num;
        ChessBoard(tr, tc + sub_size, tr + sub_size - 1, tc + sub_size, sub_size);
    }
    
    //左下象限
    if (dr >= tr + sub_size && dc < tc + sub_size) {
        ChessBoard(tr + sub_size, tc, dr, dc, sub_size);
    }else{
        Board[tr + sub_size][tc + sub_size - 1] = num;
        ChessBoard(tr + sub_size, tc, tr + sub_size, tc + sub_size - 1, sub_size);
    }
    
    //右下象限
    if (dr >= tr + sub_size && dc >= tc + sub_size) {
        ChessBoard(tr + sub_size, tc + sub_size, dr, dc, sub_size);
    }else{
        Board[tr + sub_size][tc + sub_size] = num;
        ChessBoard(tr + sub_size, tc + sub_size, tr + sub_size, tc + sub_size, sub_size);
    }
}


//彩色显示输出
void OutputBoard(int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++) {
            std::wcout << (ConsoleBackgroundColor)(Board[i][j]%10) <<(ConsoleColor)((Board[i][j]+1) % 10)<<std::setw(2) << Board[i][j];
        }
        std::wcout << ConsoleBackgroundColor::None;
        std::wcout << ConsoleColor::None;
        printf("\n");
    }
    	std::wcout << ConsoleBackgroundColor::None;
	std::wcout << ConsoleColor::None;
}
