#pragma once
#include <stdlib.h> 
#include <time.h> 
#define skipLIST_MAXLEVEL 32
#define skipLIST_P 0.25 

typedef struct redisObject {
    void* ptr;
} robj;
bool compareObjects (robj* a, robj* b) {
    return a < b;
}
bool equalObjects (robj* a, robj* b) {
    return a < b;
}

template<typename K, typename E>
class skiplistNode;
template<typename K, typename E>
class skiplistLevel {
public:
    // 前进指针
    skiplistNode<K, E>* forward;
    // 节点在该层和前向节点的距离
    unsigned long long span;
};

/* ZSETs use a specialized version of Skiplists */
template<typename K, typename E>
class skiplistNode {
public:
    // member 对象
    E data;
    // 分值
    K score;
    // 后退指针
    skiplistNode<K, E>* backward;
    // 层
    skiplistLevel<K, E> level[skipLIST_MAXLEVEL];
};

template<typename K, typename E>
class skiplist {
protected:


    // 头节点，尾节点
    skiplistNode<K, E>* _header;//指向空头
    skiplistNode<K, E>* _tail;//指向有数据的尾部
    // 节点数量
    unsigned long _length;
    // 目前表内节点的最大层数
    long long _level;

    long long _skiplistRandomLevel () {
        srand ((unsigned)time (nullptr));
        long long level = 1;
        while ((rand () & 0xFFFF) < (skipLIST_P * 0xFFFF))
            level += 1;
        return (level < skipLIST_MAXLEVEL) ? level : skipLIST_MAXLEVEL;
    }
    skiplistNode<K, E>* _skiplistCreateNode (long long level, K score, E ele) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        //(skiplistNode<K,E>*)malloc (sizeof (*zn) + level * sizeof (struct skiplistLevel));
        zn->score = score;
        zn->data = ele;
        return zn;
    }

    skiplistNode<K, E>* _skiplistCreateNode (long long level) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        return zn;
    }
public:

    /*****************************************************************************
    * 函 数 名  : skiplistCreate
    * 函数功能  : 创建新的跳跃表
    * 输入参数  : void
    * 输出参数  : 无
    * 返 回 值  : skiplist
   *****************************************************************************/
    explicit skiplist (K MIN, E NODATA) {
        long long j;

        // 初始化跳跃表属性,层数初始化为1，长度初始化为0
        this->_level = 1;
        this->_length = 0;

        // 创建一个层数为32，分值为0，成员对象为nullptr的表头结点
        this->_header = _skiplistCreateNode (skipLIST_MAXLEVEL, MIN, NODATA);
        for (j = 0; j < skipLIST_MAXLEVEL; j++) {
            // 设定每层的forward指针指向nullptr
            this->_header->level[j].forward = nullptr;
            this->_header->level[j].span = 0;
        }
        // 设定backward指向nullptr
        this->_header->backward = nullptr;
        this->_tail = nullptr;
    }
    explicit skiplist () {
        long long j;

        // 初始化跳跃表属性,层数初始化为1，长度初始化为0
        this->_level = 1;
        this->_length = 0;

        // 创建一个层数为32，分值为0，成员对象为nullptr的表头结点
        this->_header = _skiplistCreateNode (skipLIST_MAXLEVEL);
        for (j = 0; j < skipLIST_MAXLEVEL; j++) {
            // 设定每层的forward指针指向nullptr
            this->_header->level[j].forward = nullptr;
            this->_header->level[j].span = 0;
        }
        // 设定backward指向nullptr
        this->_header->backward = nullptr;
        this->_tail = nullptr;
    }

    bool empty () {
        return _length == 0;
    }
    E* getMin () {
        if (empty ())return nullptr;

        if (_header) {
            if (_header->level[0].forward) {
                return &_header->level[0].forward->data;
            }
        }
        return nullptr;
    }
    K* getMinScore () {
        if (empty ())return nullptr;

        if (_header) {
            if (_header->level[0].forward) {
                return &_header->level[0].forward->score;
            }
        }
        return nullptr;
    }
    K* getMaxScore () {
        if (empty ())return nullptr;
        if (_tail) {
            return &_tail->score;
            //if (_tail->backward) {
            //    return &_tail->backward->data;
            //}
        }
        return nullptr;
    }
    E* getMax () {
        if (empty ())return nullptr;
        if (_tail) {
            return &_tail->data;
            //if (_tail->backward) {
            //    return &_tail->backward->data;
            //}
        }
        return nullptr;
    }
    /*****************************************************************************
 * 函 数 名  : insert
 * 函数功能  : 插入新节点
 * 输入参数  : skiplist *skiplist  表头
               double score    节点分数
               robj *data       待插入节点分数
 * 输出参数  : 无
 * 返 回 值  : skiplistNode
*****************************************************************************/
    skiplistNode<K, E>* insert (double score, E data) {
        // updata[]数组记录每一层位于插入节点的前一个节点
        skiplistNode<K, E>* update[skipLIST_MAXLEVEL];

        // rank[]记录每一层位于插入节点的前一个节点的排名
        //在查找某个节点的过程中，将沿途访问过的所有层的跨度累计起来，得到的结果就是目标节点在跳跃表中的排位
        unsigned long long rank[skipLIST_MAXLEVEL];

        long long i, level;

        //serverAssert (!isnan (score));
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

        /* we assume the key is not already inside, since we allow duplicated
         * scores, and the re-insertion of score and redis object should never
         * happen since the caller of insert() should test in the hash table
         * if the element is already inside or not. */

         // 此处假设插入节点的成员对象不存在于当前跳跃表内，即不存在重复的节点
         // 随机生成一个level值
        level = _skiplistRandomLevel ();


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
        x = _skiplistCreateNode (level, score, data);
        for (i = 0; i < level; i++) {

            // 针对跳跃表的每一层，改变其forward指针的指向
            x->level[i].forward = update[i]->level[i].forward;

            //插入位置节点的后继就是新节点  
            update[i]->level[i].forward = x;

            /* update span covered by update[i] as x is inserted here */

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


        //
        //另外需要注意当level > skiplist->level时，update[i] = skiplist->header的span处理 


        /* increment span for untouched levels */

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

    /* long longernal function used by skiplistDelete, skiplistDeleteByScore and skiplistDeleteByRank */
    /*****************************************************************************
     * 函 数 名  : skiplistDeleteNode
     * 函数功能  : 内置功能函数，被skiplistDelete等函数调用
     * 输入参数  : skiplist *skiplist          链表头指针
                   skiplistNode *x        待删除节点指针
                   skiplistNode **update  带删除节点的前一节点地址的指针
     * 输出参数  : 无
     * 返 回 值  :
     * 调用关系  :
     * 记    录
     * 1.日    期: 2017年06月24日
     *   作    者:
     *   修改内容: 新生成函数
    *****************************************************************************/
    void _skiplistCutDownNode (skiplist<K, E>* skiplist, skiplistNode<K, E>* x, skiplistNode<K, E>** update) {
        long long i;
        for (i = 0; i < skiplist->_level; i++) {
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
            skiplist->_tail = x->backward;
        }
        //跳跃表的层数处理，如果表头层级的前向指针为空，说明这一层已经没有元素，层数要减一
        while (skiplist->_level > 1 && skiplist->_header->level[skiplist->_level - 1].forward == nullptr)
            skiplist->_level--;

        //跳跃表长度减一
        skiplist->_length--;
    }


    /* Delete all the elements with rank between start and end from the skiplist.
     * Start and end are inclusive. Note that start and end need to be 1-based */
     /*****************************************************************************
      * 函 数 名  : skiplistDeleteRangeByRank
      * 函数功能  : 根据提供的排名起始和结尾删除节点
      * 输入参数  : skiplist *skiplist      跳跃表指针
                    unsigned long long start  排名起始
                    unsigned long long end    排名结尾
                    dict *dict          ？
      * 输出参数  : 无
      * 返 回 值  : removed
      * 调用关系  :
      * 记    录
      * 1.日    期: 2017年06月29日
      *   作    者: zyz
      *   修改内容: 新生成函数
     *****************************************************************************/
    unsigned long skiplistDeleteRangeByRank (unsigned long long start, unsigned long long end) {
        skiplistNode<K, E>* update[skipLIST_MAXLEVEL], * x;
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
            _skiplistCutDownNode (this, x, update);
            //free (x);
            delete x;
            removed++;
            //每删除一个节点,排名加1
            traversed++;
            x = next;
        }
        return removed;
    }

    long long skiplistDeleteByScore (double score) {
        skiplistNode<K, E>* update[skipLIST_MAXLEVEL], * x;
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
            _skiplistCutDownNode (this, x, update);
            //free (x);
            delete x;
            return 1;
        }
        return 0; /* not found */
    }
    /* Find the rank for an element by both score and key.
    * Returns 0 when the element cannot be found, rank otherwise.
    * Note that the rank is 1-based due to the span of skiplist->header to the
    * first element. */
    /*****************************************************************************
     * 函 数 名  : skiplistGetRank
     * 函数功能  : 获取指定分数和成员数据对象确定节点的排名
     * 输入参数  : skiplist *skiplist  跳跃表指针
                   double score    节点分数
                   robj *o         成员数据对象指针
     * 输出参数  : 无
     * 返 回 值  : unsigned
     * 调用关系  :
     * 记    录
     * 1.日    期: 2017年06月29日
     *   作    者: zyz
     *   修改内容: 新生成函数
    *****************************************************************************/
    E* skiplistGetDataByScore (double score) {
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

            /* x might be equal to skiplist->header, so test if data is non-nullptr */
            if (x->score == score) {
                return &x->data;
            }
        }
        return nullptr;
    }
};












//
///*****************************************************************************
// * 函 数 名  : skiplistDelete
// * 函数功能  : 根据给定分值和成员来删除节点
// * 输入参数  : skiplist *skiplist  表头指针
//               double score    节点分数
//               robj *data       节点数据指针
// * 输出参数  : 无
// * 返 回 值  :
// * 调用关系  :
// * 记    录
// * 1.日    期: 2017年06月13日
// *   作    者: zyz
// *   修改内容: 新生成函数
//*****************************************************************************/
//long long skiplistDelete (skiplist* skiplist, double score, robj* data) {
//    skiplistNode<K,E>* update[skipLIST_MAXLEVEL], * x;
//    long long i;
//
//    x = skiplist->_header;
//    // 遍历所有层，记录删除节点后需要被修改的节点到 update 数组  
//    for (i = skiplist->_level - 1; i >= 0; i--) {
//        //指针前移首要条件是前向节点指针不为空，次要条件是分数小于指定分数，或即使分数相等，节点成员对象也不相等------> 前向指针前移的必要条件：分数小于或等于指定分数
//        while (x->level[i].forward && //前向指针不为空
//            (x->level[i].forward->score < score || //前向节点分数小于指定分数
//            (x->level[i].forward->score == score &&  //前向节点分数等于指定分数
//                compareObjects (x->level[i].forward->data, data) < 0)))//前向节点成员对象不相同
//            x = x->level[i].forward;
//        //保存待删除节点的前一节点指针
//        update[i] = x;
//    }
//    // 因为多个不同的 member 可能有相同的 score  
//    // 所以要确保 x 的 member 和 score 都匹配时，才进行删除  
//
//    x = x->level[0].forward;
//
//    if (x && score == x->score && equalObjects (x->data, data)) {
//        skiplistDeleteNode (skiplist, x, update);
//        free (x);
//        return 1;
//    }
//    return 0; /* not found */
//}


#include<string>

#include<iostream>
using namespace std;

/*
apple
-1
5 oops
1 mi
10 apple
*/

/*输入包含 1+n+m 行数据
第一行包含两个数字 n, m (0 \leq n \leq 1000, 0 \leq m \leq 10000≤n≤1000,0≤m≤1000)
之后的 n 行，每行包含一个数字与一个字符串，表示一个元素，数字为跳表的关键字(0\leq key \leq 1,000,000,000)(0≤key≤1,000,000,000)，字符串为元素的值。
之后的 m 行表示 m 个跳表ADT操作。每行第一个数字 a 用来区别操作。

a 为 1 时，表示查找操作，其后的一个数字 b 为要查询的元素的关键字， 输出 该元素的值，不存在则输出 -1。
a 为 2 时，表示插入操作，其后的一个数字 b 为要插入的元素，一个字符串 c 为对应的值， 不输出 。
a 为 3 时，表示删除操作，其后的一个数字 b 为要删除的关键字， 输出 该关键字与其对应的元素值，若删除关键字不存在或删除失败，输出 -1。
a 为 4 时，表示删除最小元素操作。 输出 跳表内最小关键字 c 与其对应的元素值，并将对应元素在跳表内部删除
a 为 5 时，表示删除最大元素操作。 输出 跳表内最大关键字 c 与其对应的元素值，并将对应元素在跳表内部删除*/
int main () {
#pragma warning(disable:4996)
  // freopen ("in.txt", "r", stdin);

    skiplist<long long, string> z;

    long long n, m;
    cin >> n >> m;

    for (long long i = 0; i < n; i++) {
        long long  a;
        cin >> a;
        string b;
        cin >> b;
        z.insert (a, b);

    }


    for (long long i = 0; i < m; i++) {
        long long a;
        cin >> a;
        if (a == 1) {
            long long b;
            cin >> b;
            string* p = z.skiplistGetDataByScore (b);
            if (p) {
                cout << *p;
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 2) {
            long long b;
            cin >> b;
            string c;
            cin >> c;
            z.insert (b, c);
        } else if (a == 3) {
            long long b;
            cin >> b;
            string* p = z.skiplistGetDataByScore (b);
            if (p) {
                cout << b << " " << *p;
                z.skiplistDeleteByScore (b);
            } else {
                cout << "-1";
            }
            cout << "\n";
        } else if (a == 4) {
            string* e = z.getMin ();
            long long* k = z.getMinScore ();
            if (e) {
                cout << *k << " " << *e;
            } else {
                cout << "-1";
            }
            z.skiplistDeleteByScore (*k);

            cout << "\n";
        } else if (a == 5) {
            string* e = z.getMax ();
            long long* k = z.getMaxScore ();

            if (e) {
                cout << *k << " " << *e;
            } else {
                cout << "-1";
            }
            z.skiplistDeleteByScore (*k);
            cout << "\n";
        }
    }


}