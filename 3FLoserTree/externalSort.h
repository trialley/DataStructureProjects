#ifndef EXTERNALSORT_H_INCLUDED
#define EXTERNALSORT_H_INCLUDED
#include"loserTree.h"
#include<limits.h>
#include<fstream>
#include<cstdio>
using namespace std;
/*
初始化顺串：
建立数据节点：元素值（关键字），顺串号
从源文件中读取p个元素，其顺串号为1.在这p个元素上初始化最小赢者树
不断从源文件读取元素：
    输出根节点(最终赢者)到相应顺串的文件
    如果元素值小于根节点元素值
        顺串号为根节点顺串加一（记录最大顺串号k）
    否则
        归并号为根节点归并号
    替换根节点
    重排


归并：
建立规模为k的最小赢者树，用k个顺串的第一个值初始化
循环n-k次:
    将赢者树冠军输出
    若相应顺串已空
        用最大数（int_max）代替
    否则
        从相应顺串读取下一个元素代替冠军的位置
    重排


*/

struct Node
{
    int value; //元素值
    int index;//顺串号

    operator int(){return value;}//将类转化为int类型
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
    void visitstime(){cout<<"访问磁盘次数: "<<visits<<endl;}
private:
    loserTree<Node> lt0;//最小竞赛树对象，可以用于初始化顺串
    loserTree<int> lt1;//用于顺串
    int p;//初始化顺串时最小竞赛树的规模,初始顺串的平均长度为2p
    int k;//顺串数
    int n;//储存元素数
    int visits;//访问磁盘次数
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
        out.open(a,ios::app);//追加模式写
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
        out.open(a,ios::app);//追加模式写
        out << players[lt0.winner()].value << " ";
        out.close();

        //替换元素
        int j = lt0.winner();
        players[j] = maxNode;
        lt0.replay(j);//重排
        //cout<<"finish replay"<<endl;
    }
    cout<<"产生顺串数："<<k<<endl;

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
    ifstream *in = new ifstream[k+1];//打开k个文件输入流
    //ofstream out("result.txt");//输出文件
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
    //读入k个首数据
    for(int i=1;i<=k;i++)
    {
        in[i] >> da[i];//数值表示key，下标表示顺串号

    }
    lt1.initialize(da, k);
    //cout << lt1.winner() << endl;

    freopen("result.txt", "w", stdout);
    for(int i=0; i<n; i++)
    {
        //剩余n个元素在各初始顺串中
        //每次从竞赛树中弹出冠军读取其顺串号，再输出key值到文件
        //根据顺串号从相应文件读取下一个数据，若不存在则用INT_MAX代替，替换冠军，重排
        int win = lt1.winner();//竞赛树中冠军的下标，顺串号
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
    fflush(fp);//将输出缓冲区清
    freopen( "CON", "w", stdout ); //定向输出到控制台
    for(int i=1;i<=k;i++)
    {
        in[i].close();
    }

    cout<<"归并成功"<<endl;
}
#endif // EXTERNALSORT_H_INCLUDED
