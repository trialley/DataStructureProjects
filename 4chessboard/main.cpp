/*
//4、残缺棋盘的问题:
// 问题描述
//残缺棋盘(defective chessboard)：
是一个有2k×2k个方格的棋盘，其中恰有一个方格残缺。
对于任意k，恰好存在22k种不同的残缺棋盘。

在残缺棋盘中，要求用三格板(triominoes)覆盖残缺棋盘。
在覆盖中，任意两个三格板不能重叠，任意一个三格板不能覆盖残缺方格，但三格板必须覆盖其他所有方格。
基本要求
输入棋盘大小和残缺方格的位置，输出覆盖后的棋盘，输出棋盘时要着色，共享同一边界的覆盖应着不同的颜色。棋盘是平面图，要求使用最少的颜色覆盖着色。
//
//
//说明：输入输出方式可自己设计并实现，但输入数据支持一定的规模，。不要键盘输入，程序的输出尽量清晰、直观(鼓励使用图形演示)
*/
//#pragma warning(disable:4996)
//#include<iostream>
//using namespace std;
//#include<stdio.h>
//int amount = 0, board[100][100];//规定棋盘大小
//void cover(int tr, int tc, int dr, int dc, int size)//用三格板覆盖棋盘
//{
//	int s, t;
//	if (size < 2)return;
//	amount += 1;
//	t = amount;
//	s = size / 2;
//	if (dr < tr + s && dc < tc + s)//残缺棋盘位于左上
//	{
//		cover(tr, tc, dr, dc, s);
//		board[tr + s - 1][tc + s] = t;
//		board[tr + s][tc + s - 1] = t;
//		board[tr + s][tc + s] = t;
//		cover(tr, tc + s, tr + s - 1, tc + s, s);
//		cover(tr + s, tc, tr + s, tc + s - 1, s);
//		cover(tr + s, tc + s, tr + s, tc + s, s);
//	}
//	else if (dr < tr + s && dc >= tc + s)//残缺方格位于右上
//	{
//		cover(tr, tc + s, dr, dc, s);
//		board[tr + s - 1][tc + s - 1] = t;
//		board[tr + s][tc + s - 1] = t;
//		board[tr + s][tc + s] = t;
//		cover(tr, tc, tr + s - 1, tc + s - 1, s);
//		cover(tr + s, tc, tr + s, tc + s - 1, s);
//		cover(tr + s, tc + s, tr + s, tc + s, s);
//	}
//	else if (dr >= tr + s && dc < tc + s)//残缺方格位于左下
//	{
//		cover(tr + s, tc, dr, dc, s);//覆盖三号版
//		board[tr + s - 1][tc + s - 1] = t;
//		board[tr + s - 1][tc + s] = t;
//		board[tr + s][tc + s] = t;
//		cover(tr, tc, tr + s - 1, tc + s - 1, s);//覆盖其余部分
//		cover(tr, tc + s, tr + s - 1, tc + s, s);
//		cover(tr + s, tc + s, tr + s, tc + s, s);
//	}
//	else if (dr >= tr + s && dc >= tc + s)//残缺方格位于右下
//	{
//		cover(tr + s, tc + s, dr, dc, s);
//		board[tr + s - 1][tc + s - 1] = t;//覆盖四号板
//		board[tr + s - 1][tc + s] = t;
//		board[tr + s][tc + s - 1] = t;
//		cover(tr, tc, tr + s - 1, tc + s - 1, s);//覆盖其余部分
//		cover(tr, tc + s, tr + s - 1, tc + s, s);
//		cover(tr + s, tc, tr + s, tc + s - 1, s);
//	}
//
//}
//void outputboard(int size)//输出棋盘
//{
//	int i, j;
//	for (i = 0; i < size; i++)	{
//		for (j = 0; j < size; j++)
//			printf("%d\t", board[i][j]);
//		cout << endl;
//	}
//}
//int main(){
//	int size = 1, x, y, i, j, k;
//	printf("输入K的值：");
//	scanf("%d", &k);
//	for (i = 1; i <= k; i++)
//		size *= 2;
//	printf("输入残缺棋盘中残缺的位置：");
//	cin >> x >> y;
//	cover(0, 0, x, y, size);
//	outputboard(size);
//	return 0;
//}

#pragma warning(disable:4996)
#include <stdio.h>
#include<iostream>
//#define OS_TYPE_WINDOWS_CC
#include"ColorfulConsoleIO.h"
#include <math.h>
#include <iomanip>
void TileBoard(int tr, int tc, int dr, int dc, int size);
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
    TileBoard(0, 0, a - 1, b - 1, sum);
    std::cout << "递归计算完成。\n";
    OutputBoard(sum);
    std::cout << "输出完成。";

    return 0;
}

//残缺棋盘的递归程序
void TileBoard(int tr, int tc, int dr, int dc, int size){
    if (size == 1) return;
    int t = tile++;    //所使用的三格板的数目
      int   s = size / 2;//象限大小
        //覆盖左上象限
    if (dr < tr + s && dc < tc + s) {
        //残缺方格位于本象限
        TileBoard(tr, tc, dr, dc, s);
    }else{    //本象限中没有残缺方格, 把三格板t放在右下角
        Board[tr + s - 1][tc + s - 1] = t;
        //覆盖其余部分
        TileBoard(tr, tc, tr + s - 1, tc + s - 1, s);
    }
    //覆盖右上象限
    if (dr < tr + s && dc >= tc + s) {
        //残缺方格位于本象限
        TileBoard(tr, tc + s, dr, dc, s);
    }else {    //本象限中没有残缺方格, 把三格板t放在左下角
        Board[tr + s - 1][tc + s] = t;
        //覆盖其余部分
        TileBoard(tr, tc + s, tr + s - 1, tc + s, s);
    }
    //覆盖左下象限
    if (dr >= tr + s && dc < tc + s) {
        //残缺方格位于本象限
        TileBoard(tr + s, tc, dr, dc, s);
    }else{    //把三格板t放在右上角
        Board[tr + s][tc + s - 1] = t;
        //覆盖其余部分
        TileBoard(tr + s, tc, tr + s, tc + s - 1, s);
    }
    //覆盖右下象限
    if (dr >= tr + s && dc >= tc + s) {
        //残缺方格位于本象限
        TileBoard(tr + s, tc + s, dr, dc, s);
    }else{    //把三格板t放在左上角
        Board[tr + s][tc + s] = t;
        //覆盖其余部分
        TileBoard(tr + s, tc + s, tr + s, tc + s, s);
    }
}


//彩色显示输出
void OutputBoard(int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++) {
            std::wcout << (ConsoleBackgroundColor)(Board[i][j]%10) <<(ConsoleColor)((Board[i][j]+1) % 10)<<std::setw(2) << Board[i][j];
        }
        printf("\n");
    }
    	std::wcout << ConsoleBackgroundColor::None;
	std::wcout << ConsoleColor::None;
}

//#include<iostream>

//
//using std::endl;
//int main(){
//
//	std::cout << "what color it is ?" << endl;
//
//	std::wcout << ConsoleColor::Green << L"1abc" << endl;
//	std::wcout << ConsoleColor::Red << L"2red red is fire" << endl;
//	std::wcout << ConsoleColor::Blue << L"3blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::White << L"4blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::Black << L"5blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::Purple << L"6blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::Yellow << L"7blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::Cyan << L"8blue blue love is blue" << endl;
//
//	std::wcout << ConsoleColor::GreenIntensity << L"1abc" << endl;
//	std::wcout << ConsoleColor::RedIntensity << L"2red red is fire" << endl;
//	std::wcout << ConsoleColor::BlueIntensity << L"3blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::WhiteIntensity << L"4blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::BlackIntensity << L"5blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::PurpleIntensity << L"6blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::YellowIntensity << L"7blue blue love is blue" << endl;
//	std::wcout << ConsoleColor::CyanIntensity << L"8blue blue love is blue" << endl;
//
//
//	std::wcout << ConsoleBackgroundColor::Green << L"1abc" << endl;
//	std::wcout << ConsoleBackgroundColor::Red << L"2red red is fire" << endl;
//	std::wcout << ConsoleBackgroundColor::Blue << L"3blue blue love is blue" << endl;
//	std::wcout << ConsoleBackgroundColor::Black << L"4abc" << endl;
//	std::wcout << ConsoleBackgroundColor::White << L"5red red is fire" << endl;
//	std::wcout << ConsoleBackgroundColor::Purple << L"6blue blue love is blue" << endl;
//	std::wcout << ConsoleBackgroundColor::Yellow << L"7blue blue love is blue" << endl;
//	std::wcout << ConsoleBackgroundColor::Cyan << L"8blue blue love is blue" << endl;
//	std::wcout << ConsoleBackgroundColor::None;
//	std::wcout << ConsoleColor::None;
//	return 0;
//}