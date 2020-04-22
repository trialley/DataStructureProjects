#pragma once
#include <cstdlib> 
#include <ctime> 
#include<iostream>
using namespace std;


/*
template<typename K, typename E>
class skiplistLevel {
public:
    skiplistNode<K, E>* forward;  // ǰ��ָ��
    unsigned long long span;      // �ڵ��ڸò��ǰ��ڵ�ľ��롣�����ڻ�ȡԪ������
};

template<typename K, typename E>
class skiplistNode {
public:
    E data;     // ֵ
    K score;    // �ؼ���
    skiplistNode<K, E>* backward;       // ����ָ��
    skiplistLevel<K, E> level[_SKIPLIST_MAXLEVEL];  // ����ָ��
};

template<typename K, typename E>
class skiplist {
public:
    static const int _SKIPLIST_MAXLEVEL = 32;   //������
protected:
    skiplistNode<K, E>* _header;//ָ���ͷ
    skiplistNode<K, E>* _tail;//ָ�������ݵ�β��
    unsigned long _length;// �ڵ�����
    int _level;// Ŀǰ���ڽڵ��������
    int _getRandomLevel () ;
    skiplistNode<K, E>* _createNode (int  level, K score, E ele);
    skiplistNode<K, E>* _createNode (int  level);
    void _skiplistDeleteNode (skiplistNode<K, E>* x, skiplistNode<K, E>** update);

public:
    explicit skiplist ();    //���캯��
    bool empty ();           //�����Ƿ�Ϊ��
    K* getMinScore ();       //��ȡ���ؼ���
    K* getMaxScore ();       //��ȡ��С�ؼ���
    skiplistNode<K, E>* insert (double score, E data);//����
    unsigned long deleteRangeByRank (unsigned long long start, unsigned long long end);
    bool deleteByScore (double score);
    E* getDataByScore (double score);

    ostream& output (ostream& out);
    friend ostream& operator <<(ostream& out, skiplist<K, E>& item);
}

*/
template<typename K, typename E>
class skiplist {
public:
    static const int _SKIPLIST_MAXLEVEL = 32;//���������㹻��
    static const int _SKIPLIST_P = 0.25;//�����

    template<typename K, typename E>
    class skiplistNode {
    public:
        E data;// member ����
        K score;// ��ֵ
        skiplistNode<K, E>* backward;// ����ָ��
        template<typename K, typename E>
        class skiplistLevel {
        public:
            skiplistNode<K, E>* forward; // ǰ��ָ��
            unsigned long long span; // �ڵ��ڸò��ǰ��ڵ�ľ��롣�����ڻ�ȡԪ������
        };
        skiplistLevel<K, E> level[_SKIPLIST_MAXLEVEL];// ��
    };


protected:
    skiplistNode<K, E>* _header;//ָ���ͷ
    skiplistNode<K, E>* _tail;//ָ�������ݵ�β��
    unsigned long _length;// �ڵ�����
    int _level;// Ŀǰ���ڽڵ��������


    /*��ȡ���������ÿ���߶ȳ��ֵĸ��ʶ�ָ������*/
    int _getRandomLevel () {
        int k = 1;
        while (rand () % 4)k++;//������߶�ָ������
        k = (k < _SKIPLIST_MAXLEVEL) ? k : _SKIPLIST_MAXLEVEL-1;
        return k;
    }
    /*��������Ĳ����ͼ�ֵ�Թ����ڵ�*/
    skiplistNode<K, E>* _createNode (int  level, K score, E ele) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        zn->score = score;
        zn->data = ele;
        return zn;
    }
    /*���ݲ��������սڵ�*/
    skiplistNode<K, E>* _createNode (int  level) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        return zn;
    }
    /*ɾ�����ܵĸ�������*/
    void _skiplistDeleteNode (skiplistNode<K, E>* x, skiplistNode<K, E>** update) {
        long long i;
        for (i = 0; i < this->_level; i++) {
            //��������½ڵ㣨��ɾ���ڵ��ǰһ�ڵ㣩�ĺ�̽ڵ��Ǵ�ɾ���ڵ㣬����Ҫ��������½ڵ�ĺ��ָ��
            if (update[i]->level[i].forward == x) {
                update[i]->level[i].span += x->level[i].span - 1;

                //�����п���Ϊnullptr������ɾ�����һ���ڵ�
                update[i]->level[i].forward = x->level[i].forward;

                //��ɾ���ڵ�û�г����ڴ˲�--��ȼ�1����
            } else {
                update[i]->level[i].span -= 1;
            }
        }
        //�����ɾ���ڵ�ĺ�һ�ڵ㣨������ڵĻ���
        if (x->level[0].forward) {
            x->level[0].forward->backward = x->backward;
        } else {
            this->_tail = x->backward;
        }
        //��Ծ��Ĳ������������ͷ�㼶��ǰ��ָ��Ϊ�գ�˵����һ���Ѿ�û��Ԫ�أ�����Ҫ��һ
        while (this->_level > 1 && this->_header->level[this->_level - 1].forward == nullptr)
            this->_level--;

        //��Ծ���ȼ�һ
        this->_length--;

        delete x;
    }

public:
    explicit skiplist () {
        long long j;

        // ��ʼ����Ծ������,������ʼ��Ϊ1�����ȳ�ʼ��Ϊ0
        this->_level = 1;
        this->_length = 0;

        // ����һ������Ϊ32����ֵΪ0����Ա����Ϊnullptr�ı�ͷ���
        this->_header = _createNode (_SKIPLIST_MAXLEVEL);
        for (j = 0; j < _SKIPLIST_MAXLEVEL; j++) {
            // �趨ÿ���forwardָ��ָ��nullptr
            this->_header->level[j].forward = nullptr;
            this->_header->level[j].span = 0;
        }
        // �趨backwardָ��nullptr
        this->_header->backward = nullptr;
        this->_tail = nullptr;
    }

    bool empty () { return _length == 0; }

    /*��ȡ��С�ؼ���*/
    K* getMinScore () {
        if (empty ())return nullptr;
        if (_header) {
            if (_header->level[0].forward) {
                return &_header->level[0].forward->score;
            }
        }
        return nullptr;
    }
    /*��ȡ���ؼ���*/
    K* getMaxScore () {
        if (empty ())return nullptr;
        if (_tail) {
            return &_tail->score;
        }
        return nullptr;
    }
    /*�����µļ�ֵ��*/
    skiplistNode<K, E>* insert (K score, E data) {
        // updata[]�����¼ÿһ��λ�ڲ���ڵ��ǰһ���ڵ�
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL];

        // rank[]��¼ÿһ��λ�ڲ���ڵ��ǰһ���ڵ������
        //�ڲ���ĳ���ڵ�Ĺ����У�����;���ʹ������в�Ŀ���ۼ��������õ��Ľ������Ŀ��ڵ�����Ծ���е���λ
        unsigned long long rank[_SKIPLIST_MAXLEVEL];

        long long i, level;

        // ��ͷ�ڵ�
        skiplistNode<K, E>* x = this->_header;

        // ����߲㿪ʼ����(��߲�ڵ��٣���Խ��)
        for (i = this->_level - 1; i >= 0; i--) {
            /* store rank that is crossed to reach the insert position */

            //rank[i]������¼��i��ﵽ����λ�õ�����Խ�Ľڵ�����,Ҳ���Ǹò���ӽ�(С��)����score������  
            //rank[i]��ʼ��Ϊ��һ������Խ�Ľڵ�����,��Ϊ��һ���Ѿ��ӹ�
            rank[i] = i == (this->_level - 1) ? 0 : rank[i + 1];

            //��̽ڵ㲻Ϊ�գ����Һ�̽ڵ��score�ȸ�����scoreС  
            while (x->level[i].forward &&
                ((x->level[i].forward)->score < score)) {
                //��¼��Խ�˶��ٸ��ڵ�  
                rank[i] += x->level[i].span;

                //������һ���ڵ�
                x = x->level[i].forward;
            }
            // �洢��ǰ����λ�ڲ���ڵ��ǰһ���ڵ�,����һ��Ĳ���ڵ�
            update[i] = x;
        }

         // �˴��������ڵ�ĳ�Ա���󲻴����ڵ�ǰ��Ծ���ڣ����������ظ��Ľڵ�
         // �������һ��levelֵ
        level = _getRandomLevel ();


        // ���level���ڵ�ǰ�洢�����levelֵ
        // �趨rank�����д���ԭlevel�����ϵ�ֵΪ0--Ϊʲô����Ϊ0
        // ͬʱ�趨update�������ԭlevel�����ϵ�����
        if (level > this->_level) {

            for (i = this->_level; i < level; i++) {

                //��Ϊ��һ��û�нڵ㣬��������rank[i]Ϊ0
                rank[i] = 0;
                //��Ϊ��һ�㻹û�нڵ㣬���Խڵ��ǰһ���ڵ㶼��ͷ�ڵ�
                update[i] = this->_header;

                //��δ����½ڵ�֮ǰ����Ҫ���µĽڵ��Խ�Ľڵ���Ŀ��Ȼ����skiplist->length---��Ϊ������ֻ��һ��ͷ���----->����֮��ͷ����span����������
                update[i]->level[i].span = this->_length;
            }
            // ����levelֵ��max������
            this->_level = level;
        }

        // ��������ڵ�
        x = _createNode (level, score, data);
        for (i = 0; i < level; i++) {

            // �����Ծ���ÿһ�㣬�ı���forwardָ���ָ��
            x->level[i].forward = update[i]->level[i].forward;

            //����λ�ýڵ�ĺ�̾����½ڵ�  
            update[i]->level[i].forward = x;

            //rank[i]: �ڵ�i�㣬update[i]->score������ 
            //rank[0] - rank[i]: update[0]->score��update[i]->score֮�����˼�����

            // A3 ----------------------------- [I] -> F3
            // A2 ----------------> D2 -------- [I] -> F2
            // A1 ---------> C1 --> D1 -------- [I] -> F1
            // A0 --> B0 --> C0 --> D0 --> E0 - [I] -> F0

            //x->level[i].span = ��x��update[i]->forword��span��Ŀ�� 
            //ԭ����update[i]->level[i].span = ��update[i]��update[i]->level[i]->forward��span��Ŀ 
            //����x->level[i].span = ԭ����update[i]->level[i].span - (rank[0] - rank[i]); 
            x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);


            //����update[i]->level[i].spanֵ�ĸ���������update[i]��update[i]->level[i]->forward֮���������x�� 
            //update[i]->level[i].span = ��update[i]��x��span��Ŀ�� 
            //����update[0]����϶�������ӵ�x��������Ȼ�µ�update[i]->level[i].span = (rank[0] - rank[i]) + 1; 

            //��ʾ�� update[i]��x[i]֮��϶�û�нڵ���
            update[i]->level[i].span = (rank[0] - rank[i]) + 1;
        }

        //������Ҫע�⵱level > skiplist->levelʱ��update[i] = skiplist->header��span���� 
        // ���¸߲��spanֵ
        for (i = level; i < this->_level; i++) {
            //��Ϊ�²��м������x�����߲�û�У����Զ���һ�����
            update[i]->level[i].span++;
        }

        // �趨����ڵ��backwardָ��
        //�������ڵ��ǰһ���ڵ㶼��ͷ�ڵ㣬�����ڵ�ĺ���ָ��Ϊnullptr��  
        x->backward = (update[0] == this->_header) ? nullptr : update[0];

        //�������ڵ��0���ǰ��ڵ���ǰ��ڵ�ĺ���ָ��Ϊ����ڵ�
        if (x->level[0].forward)
            x->level[0].forward->backward = x;
        else
            //����ýڵ�Ϊ��Ծ���β�ڵ�
            this->_tail = x;

        // ��Ծ����+1
        this->_length++;

        //���ز���Ľڵ�
        return x;
    }

    /*ͨ��������Χɾ������*/
    unsigned long deleteRangeByRank (unsigned long long start, unsigned long long end) {
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL], * x;
        unsigned long traversed = 0, removed = 0;
        long long i;

        x = this->_header;
        //Ѱ�Ҵ����µĽڵ�
        for (i = this->_level - 1; i >= 0; i--) {
            //ָ��ǰ�Ƶı�Ҫ������ǰ��ָ�벻Ϊ��
            while (x->level[i].forward && (traversed + x->level[i].span) < start) {
                //�����ۼ�
                traversed += x->level[i].span;
                x = x->level[i].forward;
            }
            update[i] = x;
        }

        //����Ľڵ������϶����ڵ���start
        traversed++;
        x = x->level[0].forward;
        while (x && traversed <= end) {
            //���ɾ����̽ڵ�,ֱ��endΪֹ
            skiplistNode<K, E>* next = x->level[0].forward;
            _skiplistDeleteNode (x, update);

            removed++;
            //ÿɾ��һ���ڵ�,������1
            traversed++;
            x = next;
        }
        return removed;
    }

    /*ͨ���ؼ���ɾ������*/
    bool deleteByScore (K score) {
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL], * x;
        long long i;

        x = this->_header;
        // �������в㣬��¼ɾ���ڵ����Ҫ���޸ĵĽڵ㵽 update ����  
        for (i = this->_level - 1; i >= 0; i--) {
            //ָ��ǰ����Ҫ������ǰ��ڵ�ָ�벻Ϊ�գ���Ҫ�����Ƿ���С��ָ����������ʹ������ȣ��ڵ��Ա����Ҳ�����------> ǰ��ָ��ǰ�Ƶı�Ҫ����������С�ڻ����ָ������
            while (x->level[i].forward && //ǰ��ָ�벻Ϊ��
                x->level[i].forward->score < score//ǰ��ڵ����С��ָ������
                )//ǰ��ڵ��Ա������ͬ
                x = x->level[i].forward;
            //�����ɾ���ڵ��ǰһ�ڵ�ָ��
            update[i] = x;
        }
        // ��Ϊ�����ͬ�� member ��������ͬ�� score  
        // ����Ҫȷ�� x �� member �� score ��ƥ��ʱ���Ž���ɾ��  

        x = x->level[0].forward;

        if (x && score == x->score) {
            _skiplistDeleteNode (x, update);
            return true;
        }
        return false; /* not found */
    }
    /*ͨ���ؼ��ʻ�ȡ����*/
    E* getDataByScore (K score) {
        if (empty ())return nullptr;
        if (score < _header->level[0].forward->score)return nullptr;
        if (_tail && score > _tail->score)return nullptr;

        skiplistNode<K, E>* x;
        unsigned long rank = 0;
        long long i;

        x = this->_header;
        for (i = this->_level - 1; i >= 0; i--) {
            //ָ��ǰ�Ƶı�Ҫ�����Ǻ��ָ�벻Ϊ��
            while (x->level[i].forward && x->level[i].forward->score <= score) {
                rank += x->level[i].span;
                //�����ۼ�
                x = x->level[i].forward;
            }

            if (x->score == score) {
                return &x->data;
            }
        }
        return nullptr;
    }
    /*ͼ�λ��������*/
    ostream& output (ostream& out) {
        char map[1000][32] = { 0 };
        for (int y = 0; y < 1000; y++) {
            for (int x = 0; x < 32; x++) {
                map[y][x] = ' ';
            }
        }
        skiplistNode<K, E>* p = _header;
        int i = 0;
        while (p) {//
            int j = -1;
            while (p->level[++j].forward) {
                map[i][j] = '|';
            }

            i++;

            p = p->level[0].forward;
        }

        for (int y = 0; y < 30; y++) {
            for (int x = 0; x < 1000; x++) {
                out << map[x][y];
            }
            out << "\n";
        }
        return out;
    }
    /*�������İ�װ����*/
    friend ostream& operator <<(ostream& out, skiplist<K, E>& item) {
        item.output (out);
        return out;
    }
};
