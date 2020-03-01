#pragma once
#include <stdlib.h> 
#include <time.h> 

template<typename K, typename E>
class skiplist {
public:

    static const int _SKIPLIST_MAXLEVEL = 32;
    static const int _SKIPLIST_P = 0.25;

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

    int _getRandomLevel () {
        srand ((unsigned)time (nullptr));
        int  level = 1;
        while ((rand () & 0xFFFF) < (_SKIPLIST_P * 0xFFFF))
            level += 1;
        return (level < _SKIPLIST_MAXLEVEL) ? level : _SKIPLIST_MAXLEVEL;
    }

    skiplistNode<K, E>* _createNode (int  level, K score, E ele) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        zn->score = score;
        zn->data = ele;
        return zn;
    }

    skiplistNode<K, E>* _createNode (int  level) {
        skiplistNode<K, E>* zn = new skiplistNode<K, E>;
        return zn;
    }
    /* long longernal function used by skiplistDelete, deleteByScore and skiplistDeleteByRank */
/*****************************************************************************
 * �� �� ��  : skiplistDeleteNode
 * ��������  : ���ù��ܺ�������skiplistDelete�Ⱥ�������
 * �������  : skiplist *skiplist          ����ͷָ��
               skiplistNode *x        ��ɾ���ڵ�ָ��
               skiplistNode **update  ��ɾ���ڵ��ǰһ�ڵ��ַ��ָ��
 * �������  : ��
 * �� �� ֵ  :
 * ���ù�ϵ  :
 * ��    ¼
 * 1.��    ��: 2017��06��24��
 *   ��    ��:
 *   �޸�����: �����ɺ���
*****************************************************************************/
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


        //free (x);
        delete x;
    }

public:

    //explicit skiplist (K MIN, E NODATA) {
    //    long long j;

    //    // ��ʼ����Ծ������,������ʼ��Ϊ1�����ȳ�ʼ��Ϊ0
    //    this->_level = 1;
    //    this->_length = 0;

    //    // ����һ������Ϊ32����ֵΪ0����Ա����Ϊnullptr�ı�ͷ���
    //    this->_header = _createNode (_SKIPLIST_MAXLEVEL, MIN, NODATA);
    //    for (j = 0; j < _SKIPLIST_MAXLEVEL; j++) {
    //        // �趨ÿ���forwardָ��ָ��nullptr
    //        this->_header->level[j].forward = nullptr;
    //        this->_header->level[j].span = 0;
    //    }
    //    // �趨backwardָ��nullptr
    //    this->_header->backward = nullptr;
    //    this->_tail = nullptr;
    //}
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
    //E* getMin () {
    //    if (empty ())return nullptr;
    //    if (_header) {
    //        if (_header->level[0].forward) {
    //            return &_header->level[0].forward->data;
    //        }
    //    }
    //    return nullptr;
    //}
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
        }
        return nullptr;
    }
    //E* getMax () {
    //    if (empty ())return nullptr;
    //    if (_tail) {
    //        return &_tail->data;
    //    }
    //    return nullptr;
    //}
    /*****************************************************************************
 * �� �� ��  : insert
 * ��������  : �����½ڵ�
 * �������  : skiplist *skiplist  ��ͷ
               double score    �ڵ����
               robj *data       ������ڵ����
 * �������  : ��
 * �� �� ֵ  : skiplistNode
*****************************************************************************/
    skiplistNode<K, E>* insert (double score, E data) {
        // updata[]�����¼ÿһ��λ�ڲ���ڵ��ǰһ���ڵ�
        skiplistNode<K, E>* update[_SKIPLIST_MAXLEVEL];

        // rank[]��¼ÿһ��λ�ڲ���ڵ��ǰһ���ڵ������
        //�ڲ���ĳ���ڵ�Ĺ����У�����;���ʹ������в�Ŀ���ۼ��������õ��Ľ������Ŀ��ڵ�����Ծ���е���λ
        unsigned long long rank[_SKIPLIST_MAXLEVEL];

        long long i, level;

        //serverAssert (!isnan (score));
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

        /* we assume the key is not already inside, since we allow duplicated
         * scores, and the re-insertion of score and redis object should never
         * happen since the caller of insert() should test in the hash table
         * if the element is already inside or not. */

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
        //������Ҫע�⵱level > skiplist->levelʱ��update[i] = skiplist->header��span���� 


        /* increment span for untouched levels */

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



    /* Delete all the elements with rank between start and end from the skiplist.
     * Start and end are inclusive. Note that start and end need to be 1-based */
     /*****************************************************************************
      * �� �� ��  : deleteRangeByRank
      * ��������  : �����ṩ��������ʼ�ͽ�βɾ���ڵ�
      * �������  : skiplist *skiplist      ��Ծ��ָ��
                    unsigned long long start  ������ʼ
                    unsigned long long end    ������β
                    dict *dict          ��
      * �������  : ��
      * �� �� ֵ  : removed
      * ���ù�ϵ  :
      * ��    ¼
      * 1.��    ��: 2017��06��29��
      *   ��    ��: zyz
      *   �޸�����: �����ɺ���
     *****************************************************************************/
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

    bool deleteByScore (double score) {
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
    /* Find the rank for an element by both score and key.
    * Returns 0 when the element cannot be found, rank otherwise.
    * Note that the rank is 1-based due to the span of skiplist->header to the
    * first element. */
    /*****************************************************************************
     * �� �� ��  : skiplistGetRank
     * ��������  : ��ȡָ�������ͳ�Ա���ݶ���ȷ���ڵ������
     * �������  : skiplist *skiplist  ��Ծ��ָ��
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
    E* getDataByScore (double score) {
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

            /* x might be equal to skiplist->header, so test if data is non-nullptr */
            if (x->score == score) {
                return &x->data;
            }
        }
        return nullptr;
    }
};
