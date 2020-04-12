// test winner tree class

#include <iostream>

#include "completeWinnerTree.cpp"
#include "player.h"
#include <cstdlib>
using namespace std;

int main () {
	while (1) {
		cout << endl;
		cout << "请输入a 测试赢者树     或  b 测试赢者树K路归并排序,    或  输入c停止程序" << endl;
		char code;
		while (!(cin >> code) || (code != 'a' && code != 'b' && code != 'c')) {
			cin.clear (); cin.sync (); cout << "请输入a或b或c" << endl;
		}
		switch (code) {
		case 'a': {


			int n;
			cout << "输入参加竞赛者人数, 竞赛者数>= 2   (输入0 强行结束程序)  :" << endl;


			while (!(cin >> n) || (n < 2 && n != 0)) {
				cin.clear (); cin.sync (); cout << "请输入大于等于2的数字" << endl;
			}
			if (n == 0) { cout << "程序结束"; return 0; }
			cout << "用最大赢者树还是最小赢者树？1 for 最小，2 for 最大   (输入0 强行结束程序)  :" << endl;
			int flag = 0;

			while (!(cin >> flag) || (flag != 1 && flag != 2 && flag != 0)) {
				cin.clear (); cin.sync (); cout << "输入错误，请输入1（最小赢者树）或2（最大赢者树）" << endl;
			}
			if (flag == 0) { cout << "程序结束"; return 0; }
			double* Player = new double[n + 1];
			cout << "按顺序输入" << n << "个竞赛者的值" << endl;
			for (int i = 1; i <= n; i++) {
				while (!(cin >> Player[i])) {
					cin.clear (); cin.sync (); cout << "输入错误，请输入数字" << endl;
				}

			}
			completeWinnerTree<double>* w = new completeWinnerTree<double> (Player, n, flag);
			cout << "赢者树为：" << endl;
			w->output ();

			while (1) {
				cout << "需要改变某选手的值？0 for 不需要改变 ， 1 for 需要改变" << endl;

				int flag2 = 0;

				while (!(cin >> flag2) || (flag2 != 0 && flag2 != 1)) {
					cin.clear (); cin.sync (); cout << "输入错误，请输入0（不改变）或1（改变）" << endl;
				}

				if (flag2 == 0) { cout << "结束赢者树测试"; break; }

				if (flag2 == 1) {
					cout << "请输入需要改变的选手，和改变后的值：" << endl;
					int N = 0;
					double value = 0;
					while (!(cin >> N >> value) || N > n) {
						cin.clear (); cin.sync (); cout << "输入错误，请重新输入。" << endl;
					}
					Player[N] = value;
					cout << "是否要重新进行比赛？1 for 重新进行，0 for 不重新进行（推荐重新进行比赛）" << endl;
					int flag3 = 0;

					while (!(cin >> flag3) || (flag3 != 0 && flag3 != 1)) {
						cin.clear (); cin.sync (); cout << "输入错误，请输入0（不重新比赛）或1（重新比赛）" << endl;
					}
					if (flag3 == 0) break;
					if (flag3 == 1) {

						if (flag == 1) {
							w->smallWinRePlay (N);
							cout << "把 " << N << " 号竞赛者的值变成 " << value << " 后，新最小赢者树是：" << endl;
							w->output ();
						}
						if (flag == 2) {
							w->bigWinRePlay (N);
							cout << "把 " << N << " 号竞赛者的值变成 " << value << " 后，新最大赢者树是：" << endl;
							w->output ();
						}

					}

				}

			}

		}
				break;

		case 'b': {
			cout << "输入要进行排序的序列长度：";
			int nn;
			cin >> nn;
			double* array = new double[nn];
			cout << "输入要进行排序的" << nn << "个数：";
			for (int i = 0; i < nn; i++) cin >> array[i];
			//cout<<"indwauod";
			//for(int i=0;i<nn;i++) cout<<array[i]<<endl;
			completeWinnerTree<double>* ww = new completeWinnerTree<double> ();

			cout << "请输入所用选手p数量：";//(p最好多一点)
			int pp;
			cin >> pp;
			ww->Kway_mergerwithWT (array, nn, pp, 1);
			cout << "排序后结果为：";
			for (int i = 0; i < nn; i++) cout << array[i] << ",";

			break;
		}
		case 'c': cout << "程序结束"; return 0;

		default:cout << "输入错误！";
		}

	}

}


