#include <stdio.h>
#include<memory>
using namespace std;
#define MINKEY -1  //�����������ݶ��������ֵ
#define MAXKEY 999 //�����������ݶ�С���ֵ
#define SWAP(a,b) {a=a^b;b=b^a;a=a^b;}


void adjust (int*, int*, int, int);
void createLoserTree (int* [], int*, int*, int);
void kMerge (int* [], int*, int, int*, int*);


int main () {
    //�ܹ���5��Ƭ����Ҫ�鲢
    int arr0[] = { 6, 15, 25 };
    int arr1[] = { 12, 37, 48, 50 };
    int arr2[] = { 10, 15, 16 };
    int arr3[] = { 9, 18, 20 };
    int arr4[] = { 10, 11, 40 };
    int* arr[] = { arr0, arr1, arr2, arr3, arr4 };

    //�ܹ�K·
    int k = sizeof (arr) / sizeof (arr[0]);
    //��¼ÿһ·�ĸ����������жϽ����ı��
    int* arrayCount=new int[k];
    arrayCount[0] = sizeof (arr0) / sizeof (arr0[0]);
    arrayCount[1] = sizeof (arr1) / sizeof (arr2[0]);
    arrayCount[2] = sizeof (arr2) / sizeof (arr2[0]);
    arrayCount[3] = sizeof (arr3) / sizeof (arr3[0]);
    arrayCount[4] = sizeof (arr4) / sizeof (arr4[0]);


    //��Ű�����
    int* ls=new int[k];
    //���ÿһ·����Ԫ��
    int* b=new int[k + 1];
    //����������
    createLoserTree (arr, b, ls, k);
    //���ж�·�鲢
    kMerge (arr, arrayCount, k, ls, b);
    delete ls;
    delete b;
}

/**
 * �������������ó�����ʤ��
 */
void adjust (int* b, int* ls, int s, int k) {
    //tΪb[s]�ڰ������еĸ����
    int t = (s + k) / 2;
    while (t > 0) {//��û�е��������������
        if (b[s] > b[ls[t]]) { //�븸���ָʾ�����ݽ��бȽ�
            //ls[t]��¼�������ڵ�������sָʾ�µ�ʤ�ߣ�ʤ�߽�ȥ�μӸ���һ��ıȽ�
            SWAP (s, ls[t]);
        }
        t /= 2;//��ȡ��һ�����ڵ�
    }
    //���յ�ʤ�߼�¼��ls[0]
    ls[0] = s;
}



/**
 * arry:��·�鲢����������
 * b:��Ŷ�·�鲢���׵�ַ����
 * ls:����������
 * k:��·�鲢
 */
void createLoserTree (int* arry[], int* b, int* ls, int k) {
    for (int i = 0; i < k; ++i) {
        b[i] = arry[i][0];//ÿһ·����Ԫ�ؽ��и���
    }
    //���һ��Ԫ�����ڴ��Ĭ�ϵ���Сֵ�����ڸտ�ʼ�ĶԱ�
    b[k] = MINKEY;


    //����ls�����а��ߵĳ�ʼֵ����b[k]��Сֵ
    for (int i = 0; i < k; i++) {
        ls[i] = k;
    }

    //��k��Ҷ�ӽڵ�
    //�����һ��Ҷ�ӽڵ㿪ʼ�����Ŵ�Ҷ�ӽڵ㵽���ڵ��·������
    for (int i = k - 1; i >= 0; --i) {
        adjust (b, ls, i, k);
    }

}

/**
 *��·�鲢����
 */
void kMerge (int* arr[], int* arrayCount, int k, int* ls, int* b) {

    //index�����¼ÿһ·��ȡ��������

    int *index=new int[k];
    for (int i = 0; i < k; i++) {
        index[i] = 0;
    }
    //���յ�ʤ�ߴ洢�� is[0]�У�����ֵΪ MAXKEYʱ��֤��5����ʱ�ļ��鲢����
    while (b[ls[0]] != MAXKEY) {

        //��ȡʤ������
        int s = ls[0];
        //�������ģ�������д�Ĳ���
        printf ("%d ", b[s]);
        //��Ӧ������·����++��¼
        ++index[s];

        //�ж��Ƿ��Ѿ�����
        if (index[s] < arrayCount[s]) {
            //û�ж��꣬�ӵ�s·�ж�ȡ����
            b[s] = arr[s][index[s]];
        } else {
            //�Ѿ����������ֵ����ʾ��·�Ѿ�����
            b[s] = MAXKEY;
        }
        //���е�����������ʤ�ߵ�������ŵ�ls[0]�� 
        adjust (b, ls, s, k);

    }
    delete index;
}
