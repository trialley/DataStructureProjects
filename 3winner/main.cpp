// test winner tree class

#include <iostream>

#include "completeWinnerTree.cpp"
#include "player.h"
#include <cstdlib>
using namespace std;

int main () {
	while (1) {
		cout << endl;
		cout << "������a ����Ӯ����     ��  b ����Ӯ����K·�鲢����,    ��  ����cֹͣ����" << endl;
		char code;
		while (!(cin >> code) || (code != 'a' && code != 'b' && code != 'c')) {
			cin.clear (); cin.sync (); cout << "������a��b��c" << endl;
		}
		switch (code) {
		case 'a': {


			int n;
			cout << "����μӾ���������, ��������>= 2   (����0 ǿ�н�������)  :" << endl;


			while (!(cin >> n) || (n < 2 && n != 0)) {
				cin.clear (); cin.sync (); cout << "��������ڵ���2������" << endl;
			}
			if (n == 0) { cout << "�������"; return 0; }
			cout << "�����Ӯ����������СӮ������1 for ��С��2 for ���   (����0 ǿ�н�������)  :" << endl;
			int flag = 0;

			while (!(cin >> flag) || (flag != 1 && flag != 2 && flag != 0)) {
				cin.clear (); cin.sync (); cout << "�������������1����СӮ��������2�����Ӯ������" << endl;
			}
			if (flag == 0) { cout << "�������"; return 0; }
			double* Player = new double[n + 1];
			cout << "��˳������" << n << "�������ߵ�ֵ" << endl;
			for (int i = 1; i <= n; i++) {
				while (!(cin >> Player[i])) {
					cin.clear (); cin.sync (); cout << "�����������������" << endl;
				}

			}
			completeWinnerTree<double>* w = new completeWinnerTree<double> (Player, n, flag);
			cout << "Ӯ����Ϊ��" << endl;
			w->output ();

			while (1) {
				cout << "��Ҫ�ı�ĳѡ�ֵ�ֵ��0 for ����Ҫ�ı� �� 1 for ��Ҫ�ı�" << endl;

				int flag2 = 0;

				while (!(cin >> flag2) || (flag2 != 0 && flag2 != 1)) {
					cin.clear (); cin.sync (); cout << "�������������0�����ı䣩��1���ı䣩" << endl;
				}

				if (flag2 == 0) { cout << "����Ӯ��������"; break; }

				if (flag2 == 1) {
					cout << "��������Ҫ�ı��ѡ�֣��͸ı���ֵ��" << endl;
					int N = 0;
					double value = 0;
					while (!(cin >> N >> value) || N > n) {
						cin.clear (); cin.sync (); cout << "����������������롣" << endl;
					}
					Player[N] = value;
					cout << "�Ƿ�Ҫ���½��б�����1 for ���½��У�0 for �����½��У��Ƽ����½��б�����" << endl;
					int flag3 = 0;

					while (!(cin >> flag3) || (flag3 != 0 && flag3 != 1)) {
						cin.clear (); cin.sync (); cout << "�������������0�������±�������1�����±�����" << endl;
					}
					if (flag3 == 0) break;
					if (flag3 == 1) {

						if (flag == 1) {
							w->smallWinRePlay (N);
							cout << "�� " << N << " �ž����ߵ�ֵ��� " << value << " ������СӮ�����ǣ�" << endl;
							w->output ();
						}
						if (flag == 2) {
							w->bigWinRePlay (N);
							cout << "�� " << N << " �ž����ߵ�ֵ��� " << value << " �������Ӯ�����ǣ�" << endl;
							w->output ();
						}

					}

				}

			}

		}
				break;

		case 'b': {
			cout << "����Ҫ������������г��ȣ�";
			int nn;
			cin >> nn;
			double* array = new double[nn];
			cout << "����Ҫ���������" << nn << "������";
			for (int i = 0; i < nn; i++) cin >> array[i];
			//cout<<"indwauod";
			//for(int i=0;i<nn;i++) cout<<array[i]<<endl;
			completeWinnerTree<double>* ww = new completeWinnerTree<double> ();

			cout << "����������ѡ��p������";//(p��ö�һ��)
			int pp;
			cin >> pp;
			ww->Kway_mergerwithWT (array, nn, pp, 1);
			cout << "�������Ϊ��";
			for (int i = 0; i < nn; i++) cout << array[i] << ",";

			break;
		}
		case 'c': cout << "�������"; return 0;

		default:cout << "�������";
		}

	}

}


