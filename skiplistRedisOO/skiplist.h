#pragma once
#include <cstdlib> 
#include <ctime> 
#include<iostream>
using namespace std;


/*
template<typename K, typename E>
class skiplistLevel {
public:
    skiplistNode<K, E>* forward;  // 前进指针
    unsigned long long span;      // 节点在该层和前向节点的距离。可用于获取元素排名
};

template<typename K, typename E>
class skiplistNode {
public:
    E data;     // 值
    K score;    // 关键词
    skiplistNode<K, E>* backward;       // 后退指针
    skiplistLevel<K, E> level[_SKIPLIST_MAXLEVEL];  // 各层指针
};

template<typename K, typename E>
class skiplist {
public:
    static const int _SKIPLIST_MAXLEVEL = 32;   //最大层数
protected:
    skiplistNode<K, E>* _header;//指向空头
    skiplistNode<K, E>* _tail;//指向有数据的尾部
    unsigned long _length;// 节点数量
    int _level;// 目前表内节点的最大层数
    int _getRandomLevel () ;
    skiplistNode<K, E>* _createNode (int  level, K score, E ele);
    skiplistNode<K, E>* _createNode (int  level);
    void _skiplistDeleteNode (skiplistNode<K, E>* x, skiplistNode<K, E>** update);

public:
    explicit skiplist ();    //构造函数
    bool empty ();           //返回是否为空
    K* getMinScore ();       //获取最大关键词
    K* getMaxScore ();       //获取最小关键词
    skiplistNode<K, E>* insert (double score, E data);//插入
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
    static const int _SKIPLIST_MAXLEVEL = 32;//最大层数，足够用
    static const int _SKIPLIST_P = 0.25;//随机数

    template<typename K, typename E>
    class skiplistNode {
    public:
        E data;// member 对象
        K score;// 分值
        skiplistNode<K, E>* backward;// 后退指针
        template<typename K, typename E>
        class skiplistLevel {
        public:
            skiplistNode<K, E>* forward; // 前进指针
            unsigned long long span; // 节点在该层和前向节点的距离。可用于获取元素排名
        };
        skiplistLevel<K, E> level[_SKIPLIST_MAXLEVEL];// 层
    };


protected:
    skiplistNode<K, E>* _header;//指向空头
    skiplistNode<K, E>* _tail;//指向有数据的尾部
    unsigned long _length;// 节点数量
    int _level;// 目前表内节点的最大层数


    /*获取随机层数，每个高度出现的概率都指数降低*/
    int _getRandomLevel () {
        int k = 1;
        while (rand () % 4)k++;//概率随高度指数降低
        k = (k < _SKIPLIST_MAXLEVEL) ? k : _SKIPLIST_MAXLEVEL-1;
        return k;
    }
    /*根据输入的层数和键值对构建节点*/
    skiplistNode<K, E>* _createNode (int  level, K score, E ele) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        zn->score = score;
        zn->data = ele;
        return zn;
    }
    /*根据层数构建空节点*/
    skiplistNode<K, E>* _createNode (int  level) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        return zn;
    }
    /*删除功能的辅助函数*/
    void _skiplistDeleteNode (skiplistNode<K, E>* x, skiplistNode<K, E>** update) {
        long long i;
        for (i = 0; i < this->_level; i++) {
            //如果待更新节点（待删除节点的前一节点）的后继节点是待删除节点，则需要处理待更新节点的后继指针
            if (update[i]->level[i].forward == x) {
                update[i]->level[i].span += x->level[i].span - 1;

                //这里有可能为nullptr，比如删除最后一个节点
                update[i]->level[i].forward = x->level[i].forward;

                //待删除节点没有出现在此层--跨度减1即可
            } else {
                update[i]->level[i].span -= 1;
            }
        }
        //处理待删除节点的后一节点（如果存在的话）
        if (x->level[0].forward) {
            x->level[0].forward->backward = x->backward;
        } else {
            this->_tail = x->backward;
        }
        //跳跃表的层数处理，如果表头层级的前向指针为空，说明这一层已经没有元素，层数要减一
        while (this->_level > 1 && this->_header->level[this->_level - 1].forward == nullptr)
            this->_level--;

        //跳跃表长度减一
        this->_length--;

        delete x;
    }

public:
    explicit skiplist () {
        long long j;

        // 初始化跳跃表属性,层数初始化为1，长度初始化为0
        this->_level = 1;
        this->_length = 0;

        // 创建一个层数为32，分值为0，成员对象为nullptr的表头结点
        this->_header = _createNode (_SKIPLIST_MAXLEVEL);
        for (j = 0; j < _SKIPLIST_MAXLEVEL; j++) {
            // 设定每层的forward指针指向nullptr
            this->_header->level[j].forward = nullptr;
            this->_header->level[j].span = 0;
        }
        // 设定backward指向nullptr
        this->_header->backward = nullptr;
        this->_tail = nullptr;
    }

    bool empty () { return _length == 0; }

    /*获取最小关键词*/
    K* getMinScore () {
        if (empty ())return nullptr;
        if (_header) {
            if (_header->level[0].forward) {
                return &_header->level[0].forward->score;
            }
        }
        return nullptr;
    }
    /*获取最大关键词*/
    K* getMaxScore () {
        if (empty ())return nullptr;
        if (_tail) {
            return &_tail->score;
        }
        return nullptr;
    }
    /*插入新的键值对*/
    skiplistNode<K, E>* insert (K score, E data) {
        // updata[]数组记录每一层位于插入节点的前一个节点
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL];

        // rank[]记录每一层位于插入节点的前一个节点的排名
        //在查找某个节点的过程中，将沿途访问过的所有层的跨度累计起来，得到的结果就是目标节点在跳跃表中的排位
        unsigned long long rank[_SKIPLIST_MAXLEVEL];

        long long i, level;

        // 表头节点
        skiplistNode<K, E>* x = this->_header;

        // 从最高层开始查找(最高层节点少，跳越快)
        for (i = this->_level - 1; i >= 0; i--) {
            /* store rank that is crossed to reach the insert position */

            //rank[i]用来记录第i层达到插入位置的所跨越的节点总数,也就是该层最接近(小于)给定score的排名  
            //rank[i]初始化为上一层所跨越的节点总数,因为上一层已经加过
            rank[i] = i == (this->_level - 1) ? 0 : rank[i + 1];

            //后继节点不为空，并且后继节点的score比给定的score小  
            while (x->level[i].forward &&
                ((x->level[i].forward)->score < score)) {
                //记录跨越了多少个节点  
                rank[i] += x->level[i].span;

                //查找下一个节点
                x = x->level[i].forward;
            }
            // 存储当前层上位于插入节点的前一个节点,找下一层的插入节点
            update[i] = x;
        }

         // 此处假设插入节点的成员对象不存在于当前跳跃表内，即不存在重复的节点
         // 随机生成一个level值
        level = _getRandomLevel ();


        // 如果level大于当前存储的最大level值
        // 设定rank数组中大于原level层以上的值为0--为什么设置为0
        // 同时设定update数组大于原level层以上的数据
        if (level > this->_level) {

            for (i = this->_level; i < level; i++) {

                //因为这一层没有节点，所以重置rank[i]为0
                rank[i] = 0;
                //因为这一层还没有节点，所以节点的前一个节点都是头节点
                update[i] = this->_header;

                //在未添加新节点之前，需要更新的节点跨越的节点数目自然就是skiplist->length---因为整个层只有一个头结点----->言外之意头结点的span都是链表长度
                update[i]->level[i].span = this->_length;
            }
            // 更新level值（max层数）
            this->_level = level;
        }

        // 创建插入节点
        x = _createNode (level, score, data);
        for (i = 0; i < level; i++) {

            // 针对跳跃表的每一层，改变其forward指针的指向
            x->level[i].forward = update[i]->level[i].forward;

            //插入位置节点的后继就是新节点  
            update[i]->level[i].forward = x;

            //rank[i]: 在第i层，update[i]->score的排名 
            //rank[0] - rank[i]: update[0]->score与update[i]->score之间间隔了几个数

            // A3 ----------------------------- [I] -> F3
            // A2 ----------------> D2 -------- [I] -> F2
            // A1 ---------> C1 --> D1 -------- [I] -> F1
            // A0 --> B0 --> C0 --> D0 --> E0 - [I] -> F0

            //x->level[i].span = 从x到update[i]->forword的span数目， 
            //原来的update[i]->level[i].span = 从update[i]到update[i]->level[i]->forward的span数目 
            //所以x->level[i].span = 原来的update[i]->level[i].span - (rank[0] - rank[i]); 
            x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);


            //对于update[i]->level[i].span值的更新由于在update[i]与update[i]->level[i]->forward之间又添加了x， 
            //update[i]->level[i].span = 从update[i]到x的span数目， 
            //由于update[0]后面肯定是新添加的x，所以自然新的update[i]->level[i].span = (rank[0] - rank[i]) + 1; 

            //提示： update[i]和x[i]之间肯定没有节点了
            update[i]->level[i].span = (rank[0] - rank[i]) + 1;
        }

        //另外需要注意当level > skiplist->level时，update[i] = skiplist->header的span处理 
        // 更新高层的span值
        for (i = level; i < this->_level; i++) {
            //因为下层中间插入了x，而高层没有，所以多了一个跨度
            update[i]->level[i].span++;
        }

        // 设定插入节点的backward指针
        //如果插入节点的前一个节点都是头节点，则插入节点的后向指针为nullptr？  
        x->backward = (update[0] == this->_header) ? nullptr : update[0];

        //如果插入节点的0层存前向节点则前向节点的后向指针为插入节点
        if (x->level[0].forward)
            x->level[0].forward->backward = x;
        else
            //否则该节点为跳跃表的尾节点
            this->_tail = x;

        // 跳跃表长度+1
        this->_length++;

        //返回插入的节点
        return x;
    }

    /*通过排名范围删除数据*/
    unsigned long deleteRangeByRank (unsigned long long start, unsigned long long end) {
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL], * x;
        unsigned long traversed = 0, removed = 0;
        long long i;

        x = this->_header;
        //寻找待更新的节点
        for (i = this->_level - 1; i >= 0; i--) {
            //指针前移的必要条件是前继指针不为空
            while (x->level[i].forward && (traversed + x->level[i].span) < start) {
                //排名累加
                traversed += x->level[i].span;
                x = x->level[i].forward;
            }
            update[i] = x;
        }

        //下面的节点排名肯定大于等于start
        traversed++;
        x = x->level[0].forward;
        while (x && traversed <= end) {
            //逐个删除后继节点,直到end为止
            skiplistNode<K, E>* next = x->level[0].forward;
            _skiplistDeleteNode (x, update);

            removed++;
            //每删除一个节点,排名加1
            traversed++;
            x = next;
        }
        return removed;
    }

    /*通过关键词删除数据*/
    bool deleteByScore (K score) {
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL], * x;
        long long i;

        x = this->_header;
        // 遍历所有层，记录删除节点后需要被修改的节点到 update 数组  
        for (i = this->_level - 1; i >= 0; i--) {
            //指针前移首要条件是前向节点指针不为空，次要条件是分数小于指定分数，或即使分数相等，节点成员对象也不相等------> 前向指针前移的必要条件：分数小于或等于指定分数
            while (x->level[i].forward && //前向指针不为空
                x->level[i].forward->score < score//前向节点分数小于指定分数
                )//前向节点成员对象不相同
                x = x->level[i].forward;
            //保存待删除节点的前一节点指针
            update[i] = x;
        }
        // 因为多个不同的 member 可能有相同的 score  
        // 所以要确保 x 的 member 和 score 都匹配时，才进行删除  

        x = x->level[0].forward;

        if (x && score == x->score) {
            _skiplistDeleteNode (x, update);
            return true;
        }
        return false; /* not found */
    }
    /*通过关键词获取数据*/
    E* getDataByScore (K score) {
        if (empty ())return nullptr;
        if (score < _header->level[0].forward->score)return nullptr;
        if (_tail && score > _tail->score)return nullptr;

        skiplistNode<K, E>* x;
        unsigned long rank = 0;
        long long i;

        x = this->_header;
        for (i = this->_level - 1; i >= 0; i--) {
            //指针前移的必要条件是后继指针不为空
            while (x->level[i].forward && x->level[i].forward->score <= score) {
                rank += x->level[i].span;
                //排名累加
                x = x->level[i].forward;
            }

            if (x->score == score) {
                return &x->data;
            }
        }
        return nullptr;
    }
    /*图形化输出跳表*/
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
    /*输出跳表的包装函数*/
    friend ostream& operator <<(ostream& out, skiplist<K, E>& item) {
        item.output (out);
        return out;
    }
};
