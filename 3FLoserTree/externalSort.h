#ifndef EXTERNALSORT_H_INCLUDED
#define EXTERNALSORT_H_INCLUDED
#include"loserTree.h"
#include<limits.h>
#include<fstream>
#include<cstdio>
using namespace std;
/*
��ʼ��˳����
�������ݽڵ㣺Ԫ��ֵ���ؼ��֣���˳����
��Դ�ļ��ж�ȡp��Ԫ�أ���˳����Ϊ1.����p��Ԫ���ϳ�ʼ����СӮ����
���ϴ�Դ�ļ���ȡԪ�أ�
    ������ڵ�(����Ӯ��)����Ӧ˳�����ļ�
    ���Ԫ��ֵС�ڸ��ڵ�Ԫ��ֵ
        ˳����Ϊ���ڵ�˳����һ����¼���˳����k��
    ����
        �鲢��Ϊ���ڵ�鲢��
    �滻���ڵ�
    ����


�鲢��
������ģΪk����СӮ��������k��˳���ĵ�һ��ֵ��ʼ��
ѭ��n-k��:
    ��Ӯ�����ھ����
    ����Ӧ˳���ѿ�
        ���������int_max������
    ����
        ����Ӧ˳����ȡ��һ��Ԫ�ش���ھ���λ��
    ����


*/

struct Node
{
    int value; //Ԫ��ֵ
    int index;//˳����

    operator int(){return value;}//����ת��Ϊint����
    bool operator<=(Node b)
    {
        if(index<b.index)
            return true;
        else if(index==b.index)
            return value<=b.value;
        else
            return false;
    }
};

class externalsort{
public:
    externalsort(int thep,int buf)
    {
        p = thep;
        k = 0;
        n = 0;
        visits = 0;
        buffing = buf;
    }
    ~externalsort(){}
    void initialize();
    void clearfile(int);
    void merges();
    void visitstime(){cout<<"���ʴ��̴���: "<<visits<<endl;}
private:
    loserTree<Node> lt0;//��С���������󣬿������ڳ�ʼ��˳��
    loserTree<int> lt1;//����˳��
    int p;//��ʼ��˳��ʱ��С�������Ĺ�ģ,��ʼ˳����ƽ������Ϊ2p
    int k;//˳����
    int n;//����Ԫ����
    int visits;//���ʴ��̴���
    int buffing;
    static int times;
};
int externalsort::times=0;
void externalsort::initialize()
{
    freopen("source2.txt", "r", stdin);
    Node *players = new Node[p+1];
    int q = 0;
    int value;
    k = 1;
    cout << "initialize" << endl;
    while(q<p && scanf("%d",&value) == 1){
        q++;
        times++;
        if(times>buffing){
            visits++;
            times=0;
        }
        players[q].value = value;
        players[q].index = 1;
        cout << players[q].value << endl;
    }
    q++;
    if(p>q)
        p = q;
    lt0.initialize(players, p);
    cout << "end of initialize" << endl;
    cout << players[lt0.winner()] << endl;
    lt0.output();

    n = p;
    int x;
    char a[50];
    while(scanf("%d",&x) == 1){
        n++;
        times++;
        if(times>buffing){
            visits++;
            times=0;
        }
        Node newNode;
        //cout << x << endl;
        if(x < players[lt0.winner()].value){
            newNode.index = players[lt0.winner()].index + 1;
            if(newNode.index > k)
                k = newNode.index;
        }
        else
            newNode.index = players[lt0.winner()].index;
        newNode.value = x;
        sprintf(a,"ini/%d.txt",players[lt0.winner()].index);
        cout << players[lt0.winner()].value << " ";
        cout << a << endl;

        ofstream out;
        out.open(a,ios::app);//׷��ģʽд
        out << players[lt0.winner()].value << " ";
        out.close();
        times++;
        if(times>buffing){
            visits++;
            times=0;
        }


        int i = lt0.winner();
        players[i] = newNode;
        lt0.replay(i);
    }
    Node maxNode;
    maxNode.index = INT_MAX;
    maxNode.value = INT_MAX;
    for(int i=0; i<p; i++)
    {
        times++;
        if(times>buffing){
            visits++;
            times=0;
        }

        ofstream out;
        sprintf(a,"ini/%d.txt",players[lt0.winner()].index);
        cout << players[lt0.winner()].value << " ";
        cout << a << endl;
        out.open(a,ios::app);//׷��ģʽд
        out << players[lt0.winner()].value << " ";
        out.close();

        //�滻Ԫ��
        int j = lt0.winner();
        players[j] = maxNode;
        lt0.replay(j);//����
        //cout<<"finish replay"<<endl;
    }
    cout<<"����˳������"<<k<<endl;

}
void externalsort::clearfile(int n)
{
    char a[100];
    for(int i=1; i<=n; i++)
    {
        sprintf(a,"ini/%d.txt",i);
        remove(a);
    }
}
void externalsort::merges()
{
    cout << "k: " << k << endl;
    cout << "n: " << n << endl;
    ifstream *in = new ifstream[k+1];//��k���ļ�������
    //ofstream out("result.txt");//����ļ�
    char a[50];

    for(int i=1; i<=k; i++)
    {
        sprintf(a,"ini/%d.txt",i);
        in[i].open(a);
        if(!in[i].is_open())
        {
            cout<<"open ini file error"<<endl;
            return;
        }
    }

    int *da = new int[k+1];
    //����k��������
    for(int i=1;i<=k;i++)
    {
        in[i] >> da[i];//��ֵ��ʾkey���±��ʾ˳����

    }
    lt1.initialize(da, k);
    //cout << lt1.winner() << endl;

    freopen("result.txt", "w", stdout);
    for(int i=0; i<n; i++)
    {
        //ʣ��n��Ԫ���ڸ���ʼ˳����
        //ÿ�δӾ������е����ھ���ȡ��˳���ţ������keyֵ���ļ�
        //����˳���Ŵ���Ӧ�ļ���ȡ��һ�����ݣ�������������INT_MAX���棬�滻�ھ�������
        int win = lt1.winner();//�������йھ����±꣬˳����
        cout << da[win] << " ";
        int x;
        times++;
        if(times>buffing)
        {
            visits++;
            times=0;
        }
        if(in[win]>>x)
            da[win] = x;
        else
            da[win] = INT_MAX;
        //cout<<"new:"<<da[win]<<endl;

        lt1.replay(win);
    }
    FILE *fp=freopen("a.out","w",stdout);
    fflush(fp);//�������������
    freopen( "CON", "w", stdout ); //�������������̨
    for(int i=1;i<=k;i++)
    {
        in[i].close();
    }

    cout<<"�鲢�ɹ�"<<endl;
}
#endif // EXTERNALSORT_H_INCLUDED
