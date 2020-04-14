#include"loserTree.h"
#include<limits.h>
#include<fstream>
#include<cstdio>
using namespace std;

//��������Ԫ��
struct node {
    int value; //Ԫ��ֵ
    int index;//˳����

    operator int () { return value; }//����ת��Ϊint����
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
        cout << ">>>�ָ��ļ���ʼ" << endl;

        //���ļ�
        ifstream in ("source.txt");
        //�½�p+1��ѡ��
        node* players = new node[num_per_file + 1];
        //
        _fileNum = 1;

        //��ȡ�ļ�����ʼ������ѡ��
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


        //ʹ����Щѡ�ֳ�ʼ��������
        LoserTree<node> _spliter;
        _spliter.initTree (players, num_per_file);

        //�ָ��ļ�
        cout << "��ʼ�ָ��ļ���" << endl;
        _totalSize = num_per_file;
        int temp_in_num;
        //��ʣ��������ӵ���С�ļ���
        while (in >> temp_in_num) {
            _totalSize++;
            _countReadDiskOnce ();

            node newNode;

            //��������ֱ�ԭ�е�С
            if (temp_in_num < players[_spliter.winner ()].value) {

                newNode.index = players[_spliter.winner ()].index + 1;


                if (newNode.index > _fileNum)
                    _fileNum = newNode.index;
            } else
                newNode.index = players[_spliter.winner ()].index;


            newNode.value = temp_in_num;


            char temp_file_name[50];
            sprintf (temp_file_name, "temp/%d.txt", players[_spliter.winner ()].index);
            cout << players[_spliter.winner ()].value << "��ӵ��ļ� ";
            cout << temp_file_name << endl;

            ofstream out;
            out.open (temp_file_name, ios::app);//׷��ģʽд
            out << players[_spliter.winner ()].value << " ";
            out.close ();
            _countReadDiskOnce ();



            int i = _spliter.winner ();
            players[i] = newNode;
            _spliter.replay (i);
        }

        //�������������
        for (int i = 0; i < num_per_file; i++) {
            _countReadDiskOnce ();

            char temp_file_name[50];
            sprintf (temp_file_name, "temp/%d.txt", players[_spliter.winner ()].index);

            cout << players[_spliter.winner ()].value << " ";
            cout << temp_file_name << endl;

            ofstream out_file_stream;
            out_file_stream.open (temp_file_name, ios::app);//׷��ģʽд
            out_file_stream << players[_spliter.winner ()].value << "ʣ������д�뵽 ";
            out_file_stream.close ();

            //�滻Ԫ��
            players[_spliter.winner ()] = { INT_MAX ,INT_MAX };

            //����
            _spliter.replay (_spliter.winner ());//����
        }
        delete[]players;
        cout << "�ָ��ļ�����" << _fileNum << "ÿ���ļ����Ѿ�����" << endl;

        cout << ">>>�ָ��ļ����" << endl;
    }


    void merges ();
    void visitstime () { cout << "���ʴ��̴���: " << _readDiskCount << endl; }
private:
    LoserTree<int> _final_sorter;//�����ļ��鲢����
    //int num_per_file;//��ʼ��˳��ʱ��С�������Ĺ�ģ,��ʼ˳����ƽ������Ϊ2p
    int _fileNum;//����˳����
    int _totalSize;//������Ԫ������
    int _readDiskCount;//���ʴ��̴���
    int _bufferLength;
    static int _bufferUsed;

    //��������ļ�
    void _clearfile (int n) {
        cout << ">>>���������ʱ�ļ���" << endl;

        char a[100];
        for (int i = 1; i <= n; i++) {
            sprintf (a, "temp/%d.txt", i);
            remove (a);
        }

        cout << ">>>��ʱ�ļ������ɣ�" << endl;
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
    cout << "��ʼ�ϲ���" << endl;
    cout << "�ָ��ļ�����: " << _fileNum << endl;
    cout << "��������: " << _totalSize << endl;

    ifstream* in_files = new ifstream[_fileNum + 1];//��k���ļ�������

    //������С�ļ���//���������Զ��ر��ļ�
    char a[50];
    for (int i = 1; i <= _fileNum; i++) {
        sprintf (a, "temp/%d.txt", i);
        in_files[i].open (a);
        if (!in_files[i].is_open ()) {
            cout << "open temp file error" << endl;
            return;
        }
    }


    //���������ļ���ͷһ������
    int* da = new int[_fileNum + 1];//ͷ��������
    for (int i = 1; i <= _fileNum; i++) {
        in_files[i] >> da[i];//��ֵ��ʾkey���±��ʾ˳����
    }



    _final_sorter.initTree (da, _fileNum);


    //�������ļ�����������
    {
        ofstream out_file ("result.txt");//���������Զ��ر��ļ�
        for (int i = 0; i < _totalSize; i++) {
            //ÿ�δӾ������е����ھ���ȡ��˳���ţ������keyֵ���ļ�
            int winner_file_id = _final_sorter.winner ();//�������йھ����±꣬˳����
            out_file << da[winner_file_id] << " ";

            //����˳���Ŵ���Ӧ�ļ���ȡ��һ�����ݣ�������������INT_MAX���棬�滻�ھ�������
            _countReadDiskOnce ();
            int x;
            if (in_files[winner_file_id] >> x)
                da[winner_file_id] = x;
            else
                da[winner_file_id] = INT_MAX;

            //����������
            _final_sorter.replay (winner_file_id);
        }
    }
    delete[]in_files;
    cout << "�ϲ����" << endl;
}
