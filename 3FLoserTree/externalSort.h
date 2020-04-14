#include"loserTree.h"
#include<limits.h>
#include<fstream>
#include<cstdio>
using namespace std;

//输者树的元素
struct node {
    int value; //元素值
    int index;//顺串号

    operator int () { return value; }//将类转化为int类型
    bool operator<=(node b) {
        if (index < b.index)
            return true;
        else if (index == b.index)
            return value <= b.value;
        else
            return false;
    }
};

class externalsort {
public:
    externalsort (int buf_size) :_bufferLength(buf_size){}
    ~externalsort () {}
    void initTreeAndSplitFile () {
        _clearfile (300);

        int num_per_file = 50;
        cout << ">>>分割文件开始" << endl;

        //打开文件
        ifstream in ("source.txt");
        //新建p+1个选手
        node* players = new node[num_per_file + 1];
        //
        _fileNum = 1;

        //读取文件，初始化所有选手
        int value;
        int q = 0;
        while (q < num_per_file && in >> value) {
            _countReadDiskOnce ();
            q++;

            players[q].value = value;
            players[q].index = 1;
            cout << players[q].value << endl;
        }
        q++;
        if (num_per_file > q)
            num_per_file = q;


        //使用这些选手初始化输者树
        LoserTree<node> _spliter;
        _spliter.initTree (players, num_per_file);

        //分割文件
        cout << "开始分割文件：" << endl;
        _totalSize = num_per_file;
        int temp_in_num;
        //将剩余数据添加到各小文件中
        while (in >> temp_in_num) {
            _totalSize++;
            _countReadDiskOnce ();

            node newNode;

            //输入的数字比原有的小
            if (temp_in_num < players[_spliter.winner ()].value) {

                newNode.index = players[_spliter.winner ()].index + 1;


                if (newNode.index > _fileNum)
                    _fileNum = newNode.index;
            } else
                newNode.index = players[_spliter.winner ()].index;


            newNode.value = temp_in_num;


            char temp_file_name[50];
            sprintf (temp_file_name, "temp/%d.txt", players[_spliter.winner ()].index);
            cout << players[_spliter.winner ()].value << "添加到文件 ";
            cout << temp_file_name << endl;

            ofstream out;
            out.open (temp_file_name, ios::app);//追加模式写
            out << players[_spliter.winner ()].value << " ";
            out.close ();
            _countReadDiskOnce ();



            int i = _spliter.winner ();
            players[i] = newNode;
            _spliter.replay (i);
        }

        //将所有数据清空
        for (int i = 0; i < num_per_file; i++) {
            _countReadDiskOnce ();

            char temp_file_name[50];
            sprintf (temp_file_name, "temp/%d.txt", players[_spliter.winner ()].index);

            cout << players[_spliter.winner ()].value << " ";
            cout << temp_file_name << endl;

            ofstream out_file_stream;
            out_file_stream.open (temp_file_name, ios::app);//追加模式写
            out_file_stream << players[_spliter.winner ()].value << "剩余数据写入到 ";
            out_file_stream.close ();

            //替换元素
            players[_spliter.winner ()] = { INT_MAX ,INT_MAX };

            //重排
            _spliter.replay (_spliter.winner ());//重排
        }
        delete[]players;
        cout << "分割文件数：" << _fileNum << "每个文件都已经排序" << endl;

        cout << ">>>分割文件完成" << endl;
    }


    void merges ();
    void visitstime () { cout << "访问磁盘次数: " << _readDiskCount << endl; }
private:
    LoserTree<int> _final_sorter;//用于文件归并排序
    //int num_per_file;//初始化顺串时最小竞赛树的规模,初始顺串的平均长度为2p
    int _fileNum;//输入顺串数
    int _totalSize;//待排序元素总数
    int _readDiskCount;//访问磁盘次数
    int _bufferLength;
    static int _bufferUsed;

    //清空所有文件
    void _clearfile (int n) {
        cout << ">>>正在清空临时文件：" << endl;

        char a[100];
        for (int i = 1; i <= n; i++) {
            sprintf (a, "temp/%d.txt", i);
            remove (a);
        }

        cout << ">>>临时文件清空完成：" << endl;
    }
    void _countReadDiskOnce () {
        _bufferUsed++;
        if (_bufferUsed > _bufferLength) {
            _readDiskCount++;
            _bufferUsed = 0;
        }
    }
};
int externalsort::_bufferUsed = 0;



void externalsort::merges () {
    cout << "开始合并：" << endl;
    cout << "分割文件个数: " << _fileNum << endl;
    cout << "总数据量: " << _totalSize << endl;

    ifstream* in_files = new ifstream[_fileNum + 1];//打开k个文件输入流

    //打开所有小文件，//出作用域自动关闭文件
    char a[50];
    for (int i = 1; i <= _fileNum; i++) {
        sprintf (a, "temp/%d.txt", i);
        in_files[i].open (a);
        if (!in_files[i].is_open ()) {
            cout << "open temp file error" << endl;
            return;
        }
    }


    //读入所有文件的头一个数据
    int* da = new int[_fileNum + 1];//头数据数组
    for (int i = 1; i <= _fileNum; i++) {
        in_files[i] >> da[i];//数值表示key，下标表示顺串号
    }



    _final_sorter.initTree (da, _fileNum);


    //遍历各文件中所有数据
    {
        ofstream out_file ("result.txt");//出作用域自动关闭文件
        for (int i = 0; i < _totalSize; i++) {
            //每次从竞赛树中弹出冠军读取其顺串号，再输出key值到文件
            int winner_file_id = _final_sorter.winner ();//竞赛树中冠军的下标，顺串号
            out_file << da[winner_file_id] << " ";

            //根据顺串号从相应文件读取下一个数据，若不存在则用INT_MAX代替，替换冠军，重排
            _countReadDiskOnce ();
            int x;
            if (in_files[winner_file_id] >> x)
                da[winner_file_id] = x;
            else
                da[winner_file_id] = INT_MAX;

            //输者树更新
            _final_sorter.replay (winner_file_id);
        }
    }
    delete[]in_files;
    cout << "合并完成" << endl;
}
