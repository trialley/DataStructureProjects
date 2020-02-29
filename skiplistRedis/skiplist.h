#pragma once
#include<stdlib.h>

// C++���������VC program�� 
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
    // ǰ��ָ��
    struct zskiplistNode* forward;
    // �ڵ��ڸò��ǰ��ڵ�ľ���
    unsigned int span;
};
/* ZSETs use a specialized version of Skiplists */
typedef struct zskiplistNode {
    // member ����
    robj* obj;
    // ��ֵ
    double score;
    // ����ָ��
    struct zskiplistNode* backward;
    // ��
    struct zskiplistLevel level[];
} zskiplistNode;

typedef struct zskiplist {
    // ͷ�ڵ㣬β�ڵ�
    struct zskiplistNode* header, * tail;
    // �ڵ�����
    unsigned long length;
    // Ŀǰ���ڽڵ��������
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
 * �� �� ��  : zslCreate
 * ��������  : �����µ���Ծ��
 * �������  : void
 * �������  : ��
 * �� �� ֵ  : zskiplist
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2018��05��08��
 *   ��    ��:
 *   �޸�����: �����ɺ���
*****************************************************************************/
zskiplist* zslCreate (void) {
    int j;
    zskiplist* zsl;

    // �����ڴ�
    zsl = (zskiplist*)malloc (sizeof (*zsl));
    // ��ʼ����Ծ������,������ʼ��Ϊ1�����ȳ�ʼ��Ϊ0
    zsl->level = 1;
    zsl->length = 0;

    // ����һ������Ϊ32����ֵΪ0����Ա����ΪNULL�ı�ͷ���
    zsl->header = zslCreateNode (ZSKIPLIST_MAXLEVEL, 0, NULL);
    for (j = 0; j < ZSKIPLIST_MAXLEVEL; j++) {
        // �趨ÿ���forwardָ��ָ��NULL
        zsl->header->level[j].forward = NULL;
        zsl->header->level[j].span = 0;
    }
    // �趨backwardָ��NULL
    zsl->header->backward = NULL;
    zsl->tail = NULL;
    return zsl;
}

/*****************************************************************************
 * �� �� ��  : zslInsert
 * ��������  : �����½ڵ�
 * �������  : zskiplist *zsl  ��ͷ
               double score    �ڵ����
               robj *obj       ������ڵ����
 * �������  : ��
 * �� �� ֵ  : zskiplistNode
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2017��06��13��
 *   ��    ��:
 *   �޸�����: �����ɺ���
*****************************************************************************/
zskiplistNode* zslInsert (zskiplist* zsl, double score, robj* obj) {
    // updata[]�����¼ÿһ��λ�ڲ���ڵ��ǰһ���ڵ�
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL], * x;

    // rank[]��¼ÿһ��λ�ڲ���ڵ��ǰһ���ڵ������
    //�ڲ���ĳ���ڵ�Ĺ����У�����;���ʹ������в�Ŀ���ۼ��������õ��Ľ������Ŀ��ڵ�����Ծ���е���λ
    unsigned int rank[ZSKIPLIST_MAXLEVEL];

    int i, level;

    //serverAssert (!isnan (score));
    // ��ͷ�ڵ�
    x = zsl->header;

    // ����߲㿪ʼ����(��߲�ڵ��٣���Խ��)
    for (i = zsl->level - 1; i >= 0; i--) {
        /* store rank that is crossed to reach the insert position */

        //rank[i]������¼��i��ﵽ����λ�õ�����Խ�Ľڵ�����,Ҳ���Ǹò���ӽ�(С��)����score������  
        //rank[i]��ʼ��Ϊ��һ������Խ�Ľڵ�����,��Ϊ��һ���Ѿ��ӹ�
        rank[i] = i == (zsl->level - 1) ? 0 : rank[i + 1];

        //��̽ڵ㲻Ϊ�գ����Һ�̽ڵ��score�ȸ�����scoreС  
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                //score��ͬ�����ڵ��obj�ȸ�����objС  
            (x->level[i].forward->score == score &&
                compareObjects (x->level[i].forward->obj, obj) < 0))) {
            //��¼��Խ�˶��ٸ��ڵ�  
            rank[i] += x->level[i].span;

            //������һ���ڵ�
            x = x->level[i].forward;
        }
        // �洢��ǰ����λ�ڲ���ڵ��ǰһ���ڵ�,����һ��Ĳ���ڵ�
        update[i] = x;
    }

    /* we assume the key is not already inside, since we allow duplicated
     * scores, and the re-insertion of score and redis object should never
     * happen since the caller of zslInsert() should test in the hash table
     * if the element is already inside or not. */

     // �˴��������ڵ�ĳ�Ա���󲻴����ڵ�ǰ��Ծ���ڣ����������ظ��Ľڵ�
     // �������һ��levelֵ
    level = zslRandomLevel ();


    // ���level���ڵ�ǰ�洢�����levelֵ
    // �趨rank�����д���ԭlevel�����ϵ�ֵΪ0--Ϊʲô����Ϊ0
    // ͬʱ�趨update�������ԭlevel�����ϵ�����
    if (level > zsl->level) {

        for (i = zsl->level; i < level; i++) {

            //��Ϊ��һ��û�нڵ㣬��������rank[i]Ϊ0
            rank[i] = 0;
            //��Ϊ��һ�㻹û�нڵ㣬���Խڵ��ǰһ���ڵ㶼��ͷ�ڵ�
            update[i] = zsl->header;

            //��δ����½ڵ�֮ǰ����Ҫ���µĽڵ��Խ�Ľڵ���Ŀ��Ȼ����zsl->length---��Ϊ������ֻ��һ��ͷ���----->����֮��ͷ����span����������
            update[i]->level[i].span = zsl->length;
        }
        // ����levelֵ��max������
        zsl->level = level;
    }

    // ��������ڵ�
    x = zslCreateNode (level, score, obj);
    for (i = 0; i < level; i++) {

        // �����Ծ���ÿһ�㣬�ı���forwardָ���ָ��
        x->level[i].forward = update[i]->level[i].forward;

        //����λ�ýڵ�ĺ�̾����½ڵ�  
        update[i]->level[i].forward = x;

        /* update span covered by update[i] as x is inserted here */

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


    //
    //������Ҫע�⵱level > zsl->levelʱ��update[i] = zsl->header��span���� 


    /* increment span for untouched levels */

    // ���¸߲��spanֵ
    for (i = level; i < zsl->level; i++) {
        //��Ϊ�²��м������x�����߲�û�У����Զ���һ�����
        update[i]->level[i].span++;
    }

    // �趨����ڵ��backwardָ��
    //�������ڵ��ǰһ���ڵ㶼��ͷ�ڵ㣬�����ڵ�ĺ���ָ��ΪNULL��  
    x->backward = (update[0] == zsl->header) ? NULL : update[0];

    //�������ڵ��0���ǰ��ڵ���ǰ��ڵ�ĺ���ָ��Ϊ����ڵ�
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        //����ýڵ�Ϊ��Ծ���β�ڵ�
        zsl->tail = x;

    // ��Ծ����+1
    zsl->length++;

    //���ز���Ľڵ�
    return x;
}
/* Internal function used by zslDelete, zslDeleteByScore and zslDeleteByRank */
/*****************************************************************************
 * �� �� ��  : zslDeleteNode
 * ��������  : ���ù��ܺ�������zslDelete�Ⱥ�������
 * �������  : zskiplist *zsl          ����ͷָ��
               zskiplistNode *x        ��ɾ���ڵ�ָ��
               zskiplistNode **update  ��ɾ���ڵ��ǰһ�ڵ��ַ��ָ��
 * �������  : ��
 * �� �� ֵ  :
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2017��06��24��
 *   ��    ��:
 *   �޸�����: �����ɺ���
*****************************************************************************/
void zslDeleteNode (zskiplist* zsl, zskiplistNode* x, zskiplistNode** update) {
    int i;
    for (i = 0; i < zsl->level; i++) {
        //��������½ڵ㣨��ɾ���ڵ��ǰһ�ڵ㣩�ĺ�̽ڵ��Ǵ�ɾ���ڵ㣬����Ҫ��������½ڵ�ĺ��ָ��
        if (update[i]->level[i].forward == x) {
            update[i]->level[i].span += x->level[i].span - 1;

            //�����п���ΪNULL������ɾ�����һ���ڵ�
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
        zsl->tail = x->backward;
    }
    //��Ծ��Ĳ������������ͷ�㼶��ǰ��ָ��Ϊ�գ�˵����һ���Ѿ�û��Ԫ�أ�����Ҫ��һ
    while (zsl->level > 1 && zsl->header->level[zsl->level - 1].forward == NULL)
        zsl->level--;

    //��Ծ���ȼ�һ
    zsl->length--;
}

/* Delete all the elements with rank between start and end from the skiplist.
 * Start and end are inclusive. Note that start and end need to be 1-based */
 /*****************************************************************************
  * �� �� ��  : zslDeleteRangeByRank
  * ��������  : �����ṩ��������ʼ�ͽ�βɾ���ڵ�
  * �������  : zskiplist *zsl      ��Ծ��ָ��
                unsigned int start  ������ʼ
                unsigned int end    ������β
                dict *dict          ��
  * �������  : ��
  * �� �� ֵ  : removed
  * ���ù�ϵ  :
  * ��    ¼
  * 1.��    ��: 2017��06��29��
  *   ��    ��: zyz
  *   �޸�����: �����ɺ���
 *****************************************************************************/
unsigned long zslDeleteRangeByRank (zskiplist* zsl, unsigned int start, unsigned int end) {
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL], * x;
    unsigned long traversed = 0, removed = 0;
    int i;

    x = zsl->header;
    //Ѱ�Ҵ����µĽڵ�
    for (i = zsl->level - 1; i >= 0; i--) {
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
        zskiplistNode* next = x->level[0].forward;
        zslDeleteNode (zsl, x, update);
        free (x);
        removed++;
        //ÿɾ��һ���ڵ�,������1
        traversed++;
        x = next;
    }
    return removed;
}

/*****************************************************************************
 * �� �� ��  : zslDelete
 * ��������  : ���ݸ�����ֵ�ͳ�Ա��ɾ���ڵ�
 * �������  : zskiplist *zsl  ��ͷָ��
               double score    �ڵ����
               robj *obj       �ڵ�����ָ��
 * �������  : ��
 * �� �� ֵ  :
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2017��06��13��
 *   ��    ��: zyz
 *   �޸�����: �����ɺ���
*****************************************************************************/
int zslDelete (zskiplist* zsl, double score, robj* obj) {
    zskiplistNode* update[ZSKIPLIST_MAXLEVEL], * x;
    int i;

    x = zsl->header;
    // �������в㣬��¼ɾ���ڵ����Ҫ���޸ĵĽڵ㵽 update ����  
    for (i = zsl->level - 1; i >= 0; i--) {
        //ָ��ǰ����Ҫ������ǰ��ڵ�ָ�벻Ϊ�գ���Ҫ�����Ƿ���С��ָ����������ʹ������ȣ��ڵ��Ա����Ҳ�����------> ǰ��ָ��ǰ�Ƶı�Ҫ����������С�ڻ����ָ������
        while (x->level[i].forward && //ǰ��ָ�벻Ϊ��
            (x->level[i].forward->score < score || //ǰ��ڵ����С��ָ������
            (x->level[i].forward->score == score &&  //ǰ��ڵ��������ָ������
                compareObjects (x->level[i].forward->obj, obj) < 0)))//ǰ��ڵ��Ա������ͬ
            x = x->level[i].forward;
        //�����ɾ���ڵ��ǰһ�ڵ�ָ��
        update[i] = x;
    }
    // ��Ϊ�����ͬ�� member ��������ͬ�� score  
    // ����Ҫȷ�� x �� member �� score ��ƥ��ʱ���Ž���ɾ��  

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
 * �� �� ��  : zslGetRank
 * ��������  : ��ȡָ�������ͳ�Ա���ݶ���ȷ���ڵ������
 * �������  : zskiplist *zsl  ��Ծ��ָ��
               double score    �ڵ����
               robj *o         ��Ա���ݶ���ָ��
 * �������  : ��
 * �� �� ֵ  : unsigned
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2017��06��29��
 *   ��    ��: zyz
 *   �޸�����: �����ɺ���
*****************************************************************************/
unsigned long zslGetRank (zskiplist* zsl, double score, robj* o) {
    zskiplistNode* x;
    unsigned long rank = 0;
    int i;

    x = zsl->header;
    for (i = zsl->level - 1; i >= 0; i--) {
        //ָ��ǰ�Ƶı�Ҫ�����Ǻ��ָ�벻Ϊ��
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
            (x->level[i].forward->score == score &&
                compareObjects (x->level[i].forward->obj, o) <= 0))) {
            rank += x->level[i].span;
            //�����ۼ�
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
 * �� �� ��  : zslGetElementByRank
 * ��������  : ����������ȡ��Ծ��Ԫ��
 * �������  : zskiplist *zsl      ��Ծ�������ṹָ��
               unsigned long rank  ����
 * �������  : ��
 * �� �� ֵ  :
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2018��05��10��
 *   ��    ��:
 *   �޸�����: �����ɺ���
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

