#include"loserTree.h"
#include<limits.h>
#include<fstream>
#include<cstdio>
using namespace std;

//��������Ԫ��
struct node {
    int value; //Ԫ��ֵ
    int index;//˳����

    operator int() { return value; }//����ת��Ϊint����
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
    string _filepath;
    externalsort(int buf_size, string filepath) :_bufferLength(buf_size), _filepath(filepath) {}
    ~externalsort() {}

    //�����ļ��ָ�
    void split() {
        _clearfile(300);

        cout << ">>>�ָ��ļ���ʼ" << endl;

        //���ļ�
        ifstream in(_filepath);
        if (in.fail()) {
            cout << "no such file!";
            exit(0);
        }


        //�½�p+1��ѡ��
        node* players = new node[_bufferLength + 1];
        //

        //��ȡ�ļ�����ʼ������ѡ��
        int value;
        int q = 0;
        while (q < _bufferLength && in >> value) {
            _countReadDiskOnce();
            q++;

            players[q].value = value;
            players[q].index = 1;
            cout << "������������" << players[q].value << endl;
        }
        q++;
        if (_bufferLength > q)
            _bufferLength = q;


        //ʹ����Щѡ�ֳ�ʼ��������
        LoserTree<node> _spliter;
        _spliter.initTree(players, _bufferLength);

        _fileNum = 1;
        //�ָ��ļ�
        cout << "��ʼ�ָ��ļ���" << endl;
        _totalSize = _bufferLength;
        int temp_in_num;
        //��ʣ��������ӵ���С�ļ���
        while (in >> temp_in_num) {
            _totalSize++;
            _countReadDiskOnce();

            node newNode;

            //��������ֱ�ԭ�е�С
            if (temp_in_num < players[_spliter.winner()].value) {

                newNode.index = players[_spliter.winner()].index + 1;


                if (newNode.index > _fileNum)
                    _fileNum = newNode.index;
            } else
                newNode.index = players[_spliter.winner()].index;


            newNode.value = temp_in_num;


            char temp_file_name[50];
            sprintf(temp_file_name, "winners/%d.txt", players[_spliter.winner()].index);

            cout << players[_spliter.winner()].value << "��ӵ��ļ� " << temp_file_name << endl;

            ofstream out;
            out.open(temp_file_name, ios::app);//׷��ģʽд
            out << players[_spliter.winner()].value << " ";
            out.close();
            _countReadDiskOnce();



            int i = _spliter.winner();
            players[i] = newNode;
            _spliter.reBuild(i);
        }

        //������ʣ������������ļ���
        for (int i = 0; i < _bufferLength; i++) {
            _countReadDiskOnce();

            char temp_file_name[50];
            sprintf(temp_file_name, "winners/%d.txt", players[_spliter.winner()].index);

            cout << players[_spliter.winner()].value << "���ʣ�����ݵ��ļ�" << temp_file_name << endl;

            ofstream out_file_stream;
            out_file_stream.open(temp_file_name, ios::app);//׷��ģʽд
            out_file_stream << players[_spliter.winner()].value << " ";
            out_file_stream.close();

            //�滻Ԫ��
            players[_spliter.winner()] = { INT_MAX ,INT_MAX };

            //����
            _spliter.reBuild(_spliter.winner());//����
        }
        delete[]players;
        cout << "�ָ��ļ�����" << _fileNum << "ÿ���ļ����Ѿ�����" << endl;

        cout << ">>>�ָ��ļ����" << endl;
    }


    void merges();
    void visitstime() { cout << "���ʴ��̴���: " << _readDiskCount << endl; }
private:
    //int _bufferLength;//��ʼ��˳��ʱ��С�������Ĺ�ģ,��ʼ˳����ƽ������Ϊ2p
    int _fileNum;//����˳����
    int _totalSize;//������Ԫ������
    int _readDiskCount;//���ʴ��̴���
    int _bufferLength;
    static int _bufferUsed;

    //��������ļ�
    void _clearfile(int n) {
        cout << ">>>���������ʱ�ļ���" << endl;

        char a[100];
        for (int i = 1; i <= n; i++) {
            sprintf(a, "winners/%d.txt", i);
            remove(a);
        }

        cout << ">>>��ʱ�ļ������ɣ�" << endl;
    }

    //ģ���Ӳ��
    void _countReadDiskOnce() {
        _bufferUsed++;
        if (_bufferUsed > _bufferLength) {
            _readDiskCount++;
            _bufferUsed = 0;
        }
    }
};
int externalsort::_bufferUsed = 0;



//�������
void externalsort::merges() {
    //�ָ�
    split();

    //�����ļ��鲢����
    LoserTree<int> _final_sorter;
    cout << "��ʼ�ϲ���" << endl;
    cout << "�ָ��ļ�����: " << _fileNum << endl;
    cout << "��������: " << _totalSize << endl;


    //��k���ļ�������
    char a[50];
    ifstream* in_files = new ifstream[_fileNum + 1];
    for (int i = 1; i <= _fileNum; i++) {
        sprintf(a, "winners/%d.txt", i);
        in_files[i].open(a);
        if (!in_files[i].is_open()) {
            cout << "open winners file error" << endl;
            return;
        }
    }

    //���������ļ���ͷһ������
    int* da = new int[_fileNum + 1];//ͷ��������
    for (int i = 1; i <= _fileNum; i++) {
        in_files[i] >> da[i];//��ֵ��ʾkey���±��ʾ˳����
    }


    //��ʼ���ϲ�������
    _final_sorter.initTree(da, _fileNum);


    //�������ļ�����������
    {
        ofstream out_file("result.txt");//���������Զ��ر��ļ�
        for (int i = 0; i < _totalSize; i++) {
            //ÿ�δӾ������е����ھ���ȡ��˳���ţ������keyֵ���ļ�
            int winner_file_id = _final_sorter.winner();//�������йھ����±꣬˳����
            out_file << da[winner_file_id] << " ";

            //����˳���Ŵ���Ӧ�ļ���ȡ��һ�����ݣ�������������INT_MAX���棬�滻�ھ�������
            _countReadDiskOnce();

            //��ȡ��һ������+˳�����ж�
            int x;
            if (in_files[winner_file_id] >> x)
                da[winner_file_id] = x;
            else
                da[winner_file_id] = INT_MAX;

            //���������±���
            _final_sorter.reBuild(winner_file_id);
        }
    }
    delete[] in_files;//ɾ��˳������������
    cout << "�ϲ����" << endl;
}
