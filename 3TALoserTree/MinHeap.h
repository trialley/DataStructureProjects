#pragma once
template<class T>
/*分布式排序*/
class minHeap {
public:
	typedef enum { min_head_empty }err;
private:
	int _size;
	int _length;
	T* _head;
	void _extLength () {
		T* temp = new T[_length * 2];
		copy (_head, _head + _length, temp);
		delete[] _head;
		_length *= 2;
		_head = temp;
	}
	void _clear () {
		/*delete[] _head;*/
	}
public:
	minHeap (int lengthi = 10) {
		_length = lengthi + 1;
		_head = new T[_length];
		_size = 0;
	}
	void initialize (T* arri, int sizei) {
		_clearAndInit ();
		for (int i = 0; i < sizei; i++) {
			push (arri[i]);
		}
	}
	~minHeap () { _clear (); }
	void _clearAndInit () {
		_clear ();
		_length = 11;
		_head = new T[11];
		_size = 0;
	}
	const T& top () {
		if (_size == 0)
			throw min_head_empty;
		return _head[1];
	}
	void pop () {
		if (_size == 0) {
			throw min_head_empty;
		}
		_head[1].~T ();
		T to_be_insert = _head[_size--];
		int insert_index = 1,
			child_index = 2;     // child_index of current_node

		//将新的头部元素逐层向下移动，向下移动到左子还是右子？这里需要判断
		//起码有一个左子树，所以要<=
		while (child_index <= _size) {
			//如果左子比右子大，则根应当与右子交换，使新根小，这样可以保持最小堆特性
			//如果左子树卡到了size位置，说明没有右子树，不必寻找左右中最小的元素
			if (child_index < _size && _head[child_index] > _head[child_index + 1]) {
				child_index++;
			}
			//如果根比两个子都小，那直接退出就行了，不必再交换
			if (to_be_insert <= _head[child_index]) {
				break;
			}
			_head[insert_index] = _head[child_index];
			insert_index = child_index;
			child_index *= 2;
		}
		_head[insert_index] = to_be_insert;
	}
	void push (const T& datai) {
		//进行越界检查
		if (_size == _length - 1) {
			_extLength ();
		}

		int insert_index = ++_size;

		while (insert_index != 1 && _head[insert_index / 2] > datai) {//插入元素的父元素不小于插入元素，说明需要调整
			_head[insert_index] = _head[insert_index / 2]; //该父元素放到子节点位置
			insert_index /= 2;//子节点位置指向原父节点那里去，也即发生父子交换，只不过子元素还没有插入

			//继续循环查看新的父节点
		}

		_head[insert_index] = datai;
	}
	void pushShow (const T& datai) {
		//进行越界检查
		if (_size == _length - 1) {
			_extLength ();
		}

		int insert_index = ++_size;
		_head[insert_index] = datai;
		for (int i = 1; i <= _size; i++) {
			cout << _head[i] << " ";
		}
		cout << "\n";

		while (insert_index != 1 && _head[insert_index / 2] > datai) {//插入元素的父元素不小于插入元素，说明需要调整
			_head[insert_index] = _head[insert_index / 2]; //该父元素放到子节点位置
			insert_index /= 2;//子节点位置指向原父节点那里去，也即发生父子交换，只不过子元素还没有插入
			_head[insert_index] = datai;

			//继续循环查看新的父节点
			for (int i = 1; i <= _size; i++) {
				cout << _head[i] << " ";
			}
			cout << "\n";
		}



		_head[insert_index] = datai;
	}
	ostream& out (ostream& out) {
		for (int i = 1; i <= _size; i++) {
			out << _head[i] << " ";
		}

		return out;
	}
	bool empty () const { return _size == 0; }
	int size () const { return _size; }
};


/*


*/