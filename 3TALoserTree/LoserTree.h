#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <stdexcept>


template< class ValueType,
    class ContainerType,
    const ValueType& forever_lose_value,
    class Compare = std::less<ValueType> >
    class LoserTree {
    public:
        //传入容器数组与容器数量
        LoserTree (const ContainerType* ways, size_t num) :
            _num (num), _ways (ways), _indexes (new size_t[_num]),
            _data (new ValueType[_num]), _losers (new int[_num]) {
            if (ways == NULL || num == 0) {
                delete[] _indexes;
                delete[] _data;
                delete[] _losers;
                throw std::invalid_argument ("invalid ways or number of ways");
            }
            std::fill (_indexes, _indexes + _num, 0);
            std::fill (_losers, _losers + _num, -1);
            for (int way_idx = _num - 1; way_idx >= 0; --way_idx) {
                if (_indexes[way_idx] == _ways[way_idx].size ()) {
                    _data[way_idx] = forever_lose_value;
                } else {
                    _data[way_idx] = _ways[way_idx][_indexes[way_idx]];
                }
                _adjust (way_idx);
            }
        }

        ~LoserTree () {
            delete[] _indexes;
            delete[] _losers;
            delete[] _data;
        }

        //拿出一个最大或最小一个元素
        bool extract_one (ValueType& v) {
            int way_idx = _losers[0];
            if (_data[way_idx] == forever_lose_value)
                return false;

            //目标容器的值传入v
            v = _data[way_idx];

            if (++_indexes[way_idx] == _ways[way_idx].size ()) {
                _data[way_idx] = forever_lose_value;
            } else {
                _data[way_idx] = _ways[way_idx][_indexes[way_idx]];
            }

            //拿完数据后进行调整
            _adjust (way_idx);
            return true;
        }

    private:
        size_t _num;//容器数量，代表几道排序
        const ContainerType* _ways;//道数组们
        size_t* _indexes;//数组头部元素索引
        ValueType* _data;//输者树头部
        int* _losers;//输者树的元素所在容器序号


        //调整函数
        void _adjust (int winner_idx) {
            using std::swap;

            // _losers[loser_idx_idx] is the index of the loser in _data
            int loser_idx_idx = (winner_idx + _num) / 2;//获得输者id
            
            //
            while (loser_idx_idx != 0 && winner_idx != -1) {//
                if (_losers[loser_idx_idx] == -1 || !Compare ()(_data[winner_idx], _data[_losers[loser_idx_idx]]))
                    swap (winner_idx, _losers[loser_idx_idx]);
                loser_idx_idx /= 2;
            }
            _losers[0] = winner_idx;
        }
};

/*
 * input format:
 * 1 10 100 1000
 * 2 20 200 2000
 * 3 30 300
 * 4 40 400 4000 40000
 */
std::vector<std::vector<int> > get_input () {
    std::vector<std::vector<int> > data;
    std::string line;
    while (std::getline (std::cin, line)) {
        std::vector<int> tmp_data;
        std::istringstream iss (line);
        std::copy (std::istream_iterator<int> (iss), std::istream_iterator<int> (), std::back_inserter (tmp_data));
        data.push_back (tmp_data);
    }

    for (size_t i = 0; i < data.size (); ++i) {
        std::copy (data[i].begin (), data[i].end (), std::ostream_iterator<int> (std::cout, ", "));
        std::cout << std::endl;
    }

    return data;
}

template<class ValueType, class ContainerType>
std::vector<ContainerType> generate_data () {
    const int way_num = 20;
    std::vector<ContainerType> data (way_num);
    for (int num = 0; num < 10/*100000*/; ++num) {
        data[rand () % way_num].push_back (ValueType (num));
    }

    return data;
}
