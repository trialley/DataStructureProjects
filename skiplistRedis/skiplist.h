#pragma once
#include<stdlib.h>

// C++随机函数（VC program） 
#include <stdlib.h> 
#include <time.h> 
#define ZSKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define ZSKIPLIST_P 0.25      /* Skiplist P = 1/4 */
/* Returns a random level for the new skiplist node we are going to create.
 * The return value of this function is between 1 and ZSKIPLIST_MAXLEVEL
 * (both inclusive), with a powerlaw-alike distribution where higher
 * levels are less likely to be returned. */
int zslRandomLevel (void) {
    srand ((unsigned)time (NULL));
    int level = 1;
    while ((rand () & 0xFFFF) < (ZSKIPLIST_P * 0xFFFF))
        level += 1;
    return (level < ZSKIPLIST_MAXLEVEL) ? level : ZSKIPLIST_MAXLEVEL;
}

typedef struct redisObject {
    void* ptr;
} robj;
bool compareObjects (robj* a, robj* b) {
    return a < b;
}
bool equalObjects (robj* a, robj* b) {
    return a < b;
}
struct zskiplistLevel {
    // 前进指针
    struct zskiplistNode* forward;
    // 节点在该层和前向节点的距离
    unsigned int span;
};
/* ZSETs use a specialized version of Skiplists */
typedef struct zskiplistNode {
    // member 对象
    robj* obj;
    // 分值
    double score;
    // 后退指针
    struct zskiplistNode* backward;
    // 层
    struct zskiplistLevel level[];
} zskiplistNode;

typedef struct zskiplist {
    // 头节点，尾节点
    struct zskiplistNode* header, * tail;
    // 节点数量
    unsigned long length;
    // 目前表内节点的最大层数
    int level;
} zskiplist;
zskiplistNode* zslCreateNode (int level, double score, robj* ele) {
    zskiplistNode* zn = 
        (zskiplistNode*)malloc (sizeof (*zn) + 
            level * sizeof (struct zskiplistLevel));
    zn->score = score;
    zn->obj = ele;
    return zn;
}
/*****************************************************************************
 * 函 数 名  : zslCreate
 * 函数功能  : 创建新的跳跃表
 * 输入参数  : void
 * 输出参数  : 无
 * 返 回 值  : zskiplist
 * 调用关系  :
 * 记    录
 * 1.日    期: 2018年05月08日
 *   作    者:
 *   修改内容: 新生成函数
*****************************************************************************/
zskiplist* zslCreate (void) {
    int j;
    zskiplist* zsl;

    // 申请内存
    zsl = (zskiplist*)malloc (sizeof (*zsl));
    // 初始化跳跃表属性,层数初始化为1，长度初始化为0
    zsl->level = 1;
    zsl->length = 0;

    // 创建一个层数为32，分值为0，成员对象为NULL的表头结点
    zsl->header = zslCreateNode (ZSKIPLIST_MAXLEVEL, 0, NULL);
    for (j = 0; j < ZSKIPLIST_MAXLEVEL; j++) {
        // 设定每层的forward指针指向NULL
        zsl->header->level[j].forward = NULL;
        zsl->header->level[j].span = 0;
    }
    // 设定backward指向NULL
    zsl->header->backward = NULL;
    zsl->tail = NULL;
    return zsl;
}

/*****************************************************************************
 * 函 数 名  : zslInsert
 * 函数功能  : 插入新节点
 * 输入参数  : zskiplist *zsl  表头
               double score    节点分数
               robj *obj       待插入节点分数
 * 输出参数  : 无
 * 返 回 值  : zskiplistNode
 * 调用关系  :
 * 记    录
 * 1.日    期: 2017年06月13日
 *   作    者:
 *   修改内容: 新生成函数
*****************************************************************************/
zskiplistNode* zslInsert (zskiplist* zsl, double score, robj* obj) {
    // updata[]数组记录每一层位于插入节点的前一个节点
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL], * x;

    // rank[]记录每一层位于插入节点的前一个节点的排名
    //在查找某个节点的过程中，将沿途访问过的所有层的跨度累计起来，得到的结果就是目标节点在跳跃表中的排位
    unsigned int rank[ZSKIPLIST_MAXLEVEL];

    int i, level;

    //serverAssert (!isnan (score));
    // 表头节点
    x = zsl->header;

    // 从最高层开始查找(最高层节点少，跳越快)
    for (i = zsl->level - 1; i >= 0; i--) {
        /* store rank that is crossed to reach the insert position */

        //rank[i]用来记录第i层达到插入位置的所跨越的节点总数,也就是该层最接近(小于)给定score的排名  
        //rank[i]初始化为上一层所跨越的节点总数,因为上一层已经加过
        rank[i] = i == (zsl->level - 1) ? 0 : rank[i + 1];

        //后继节点不为空，并且后继节点的score比给定的score小  
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                //score相同，但节点的obj比给定的obj小  
            (x->level[i].forward->score == score &&
                compareObjects (x->level[i].forward->obj, obj) < 0))) {
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
     * happen since the caller of zslInsert() should test in the hash table
     * if the element is already inside or not. */

     // 此处假设插入节点的成员对象不存在于当前跳跃表内，即不存在重复的节点
     // 随机生成一个level值
    level = zslRandomLevel ();


    // 如果level大于当前存储的最大level值
    // 设定rank数组中大于原level层以上的值为0--为什么设置为0
    // 同时设定update数组大于原level层以上的数据
    if (level > zsl->level) {

        for (i = zsl->level; i < level; i++) {

            //因为这一层没有节点，所以重置rank[i]为0
            rank[i] = 0;
            //因为这一层还没有节点，所以节点的前一个节点都是头节点
            update[i] = zsl->header;

            //在未添加新节点之前，需要更新的节点跨越的节点数目自然就是zsl->length---因为整个层只有一个头结点----->言外之意头结点的span都是链表长度
            update[i]->level[i].span = zsl->length;
        }
        // 更新level值（max层数）
        zsl->level = level;
    }

    // 创建插入节点
    x = zslCreateNode (level, score, obj);
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
    //另外需要注意当level > zsl->level时，update[i] = zsl->header的span处理 


    /* increment span for untouched levels */

    // 更新高层的span值
    for (i = level; i < zsl->level; i++) {
        //因为下层中间插入了x，而高层没有，所以多了一个跨度
        update[i]->level[i].span++;
    }

    // 设定插入节点的backward指针
    //如果插入节点的前一个节点都是头节点，则插入节点的后向指针为NULL？  
    x->backward = (update[0] == zsl->header) ? NULL : update[0];

    //如果插入节点的0层存前向节点则前向节点的后向指针为插入节点
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        //否则该节点为跳跃表的尾节点
        zsl->tail = x;

    // 跳跃表长度+1
    zsl->length++;

    //返回插入的节点
    return x;
}
/* Internal function used by zslDelete, zslDeleteByScore and zslDeleteByRank */
/*****************************************************************************
 * 函 数 名  : zslDeleteNode
 * 函数功能  : 内置功能函数，被zslDelete等函数调用
 * 输入参数  : zskiplist *zsl          链表头指针
               zskiplistNode *x        待删除节点指针
               zskiplistNode **update  带删除节点的前一节点地址的指针
 * 输出参数  : 无
 * 返 回 值  :
 * 调用关系  :
 * 记    录
 * 1.日    期: 2017年06月24日
 *   作    者:
 *   修改内容: 新生成函数
*****************************************************************************/
void zslDeleteNode (zskiplist* zsl, zskiplistNode* x, zskiplistNode** update) {
    int i;
    for (i = 0; i < zsl->level; i++) {
        //如果待更新节点（待删除节点的前一节点）的后继节点是待删除节点，则需要处理待更新节点的后继指针
        if (update[i]->level[i].forward == x) {
            update[i]->level[i].span += x->level[i].span - 1;

            //这里有可能为NULL，比如删除最后一个节点
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
        zsl->tail = x->backward;
    }
    //跳跃表的层数处理，如果表头层级的前向指针为空，说明这一层已经没有元素，层数要减一
    while (zsl->level > 1 && zsl->header->level[zsl->level - 1].forward == NULL)
        zsl->level--;

    //跳跃表长度减一
    zsl->length--;
}

/* Delete all the elements with rank between start and end from the skiplist.
 * Start and end are inclusive. Note that start and end need to be 1-based */
 /*****************************************************************************
  * 函 数 名  : zslDeleteRangeByRank
  * 函数功能  : 根据提供的排名起始和结尾删除节点
  * 输入参数  : zskiplist *zsl      跳跃表指针
                unsigned int start  排名起始
                unsigned int end    排名结尾
                dict *dict          ？
  * 输出参数  : 无
  * 返 回 值  : removed
  * 调用关系  :
  * 记    录
  * 1.日    期: 2017年06月29日
  *   作    者: zyz
  *   修改内容: 新生成函数
 *****************************************************************************/
unsigned long zslDeleteRangeByRank (zskiplist* zsl, unsigned int start, unsigned int end) {
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL], * x;
    unsigned long traversed = 0, removed = 0;
    int i;

    x = zsl->header;
    //寻找待更新的节点
    for (i = zsl->level - 1; i >= 0; i--) {
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
        zskiplistNode* next = x->level[0].forward;
        zslDeleteNode (zsl, x, update);
        free (x);
        removed++;
        //每删除一个节点,排名加1
        traversed++;
        x = next;
    }
    return removed;
}

/*****************************************************************************
 * 函 数 名  : zslDelete
 * 函数功能  : 根据给定分值和成员来删除节点
 * 输入参数  : zskiplist *zsl  表头指针
               double score    节点分数
               robj *obj       节点数据指针
 * 输出参数  : 无
 * 返 回 值  :
 * 调用关系  :
 * 记    录
 * 1.日    期: 2017年06月13日
 *   作    者: zyz
 *   修改内容: 新生成函数
*****************************************************************************/
int zslDelete (zskiplist* zsl, double score, robj* obj) {
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL], * x;
    int i;

    x = zsl->header;
    // 遍历所有层，记录删除节点后需要被修改的节点到 update 数组  
    for (i = zsl->level - 1; i >= 0; i--) {
        //指针前移首要条件是前向节点指针不为空，次要条件是分数小于指定分数，或即使分数相等，节点成员对象也不相等------> 前向指针前移的必要条件：分数小于或等于指定分数
        while (x->level[i].forward && //前向指针不为空
            (x->level[i].forward->score < score || //前向节点分数小于指定分数
            (x->level[i].forward->score == score &&  //前向节点分数等于指定分数
                compareObjects (x->level[i].forward->obj, obj) < 0)))//前向节点成员对象不相同
            x = x->level[i].forward;
        //保存待删除节点的前一节点指针
        update[i] = x;
    }
    // 因为多个不同的 member 可能有相同的 score  
    // 所以要确保 x 的 member 和 score 都匹配时，才进行删除  

    x = x->level[0].forward;

    if (x && score == x->score && equalObjects (x->obj, obj)) {
        zslDeleteNode (zsl, x, update);
        free (x);
        return 1;
    }
    return 0; /* not found */
}

/* Find the rank for an element by both score and key.
* Returns 0 when the element cannot be found, rank otherwise.
* Note that the rank is 1-based due to the span of zsl->header to the
* first element. */
/*****************************************************************************
 * 函 数 名  : zslGetRank
 * 函数功能  : 获取指定分数和成员数据对象确定节点的排名
 * 输入参数  : zskiplist *zsl  跳跃表指针
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
unsigned long zslGetRank (zskiplist* zsl, double score, robj* o) {
    zskiplistNode* x;
    unsigned long rank = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--) {
        //指针前移的必要条件是后继指针不为空
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
            (x->level[i].forward->score == score &&
                compareObjects (x->level[i].forward->obj, o) <= 0))) {
            rank += x->level[i].span;
            //排名累加
            x = x->level[i].forward;
        }

        /* x might be equal to zsl->header, so test if obj is non-NULL */
        if (x->obj && equalObjects (x->obj, o)) {
            return rank;
        }
    }
    return 0;
}

/* Finds an element by its rank. The rank argument needs to be 1-based. */
/*****************************************************************************
 * 函 数 名  : zslGetElementByRank
 * 函数功能  : 根据排名获取跳跃表元素
 * 输入参数  : zskiplist *zsl      跳跃表描述结构指针
               unsigned long rank  排名
 * 输出参数  : 无
 * 返 回 值  :
 * 调用关系  :
 * 记    录
 * 1.日    期: 2018年05月10日
 *   作    者:
 *   修改内容: 新生成函数
*****************************************************************************/
zskiplistNode* zslGetElementByRank (zskiplist* zsl, unsigned long rank) {
    zskiplistNode* x;
    unsigned long traversed = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--) {
        while (x->level[i].forward && (traversed + x->level[i].span) <= rank) {
            traversed += x->level[i].span;
            x = x->level[i].forward;
        }
        if (traversed == rank) {
            return x;
        }
    }
    return NULL;
}

