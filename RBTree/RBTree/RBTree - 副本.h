#pragma once

// 结点的颜色
enum Colour {
	RED,
	BLACK,
};

// 红黑树结点的定义
template<class K, class V>
struct RBTreeNode {
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	pair<K, V> _kv;
	Colour _col;

	RBTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _col(RED) // 新插入的节点如果时黑色一定会违反规则四，如果插入的是红色，有可能会违反规则三，因此在这里选择惩罚概率较小的规则三，选择新增插入的结点为红色
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;

public:
	~RBTree()
	{
		_Destroy(_root);
		_root = nullptr;
	}

	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first < key)
			{
				cur = cur->_right;
			}
			else if (cur->_kv.first > key)
			{
				cur = cur->_left;
			}
			else
			{
				return cur;
			}
		}

		return nullptr;
	}

	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			_root->_col = BLACK; // 将根节点设置为黑色
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(kv);
		if (parent->_kv.first > kv.first)
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}
		cur->_parent = parent;

		// 针对插入后的情况进行分析 
		// 如果插入的节点的父节点是黑色的该红黑树就不会产生问题，可以正常插入
		// 如果插入的节点的父节点是红色的就需要进行调整
		while (parent && parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				// 情况1：u存在且为红，变色处理，并继续往上处理
				if (uncle && uncle->_col == RED)
				{
						//     g
						//   p   u
						// c       
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					// 继续往上调整
					cur = grandfather;
					parent = cur->_parent;
				}
				else // 情况2+3：u不存在/u存在且为黑，旋转+变色 在这种情况下就无法满足最短路径的两倍是大于最长路径的这个条件，因此需要进行旋转操作，从AVL树中可以得知旋转是有四种情况，需要分别对其进行处理
				{
						//     g
						//   p   u
						// c 
					if (cur == parent->_left)
					{
						RotateR(grandfather);
						parent->_col = BLACK;
						grandfather->_col = RED;
					}
					else // cur == parent->_right
					{
						//     g
						//   p   u
						//     c
						RotateL(parent);
						RotateR(grandfather);
						cur->_col = BLACK;
						//parent->_col = RED;
						grandfather->_col = RED;
					}

					break;
				}
			}

			else // (grandfather->_right == parent)
			{
						//    g
						//  u   p
						//        c
				Node* uncle = grandfather->_left;
				// 情况1：u存在且为红，变色处理，并继续往上处理
				if (uncle && uncle->_col == RED)
				{
					parent->_col = BLACK;
					uncle->_col = BLACK;
					grandfather->_col = RED;

					// 继续往上调整
					cur = grandfather;
					parent = cur->_parent;
				}
				else // 情况2+3：u不存在/u存在且为黑，旋转+变色
				{
						//    g
						//  u   p
						//        c
					if (cur == parent->_right)
					{
						RotateL(grandfather);
						grandfather->_col = RED;
						parent->_col = BLACK;
					}
					else // cur == parent->_left
					{
						//    g
						//  u   p
						//    c
						RotateR(parent);
						RotateL(grandfather);
						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
		}

		_root->_col = BLACK;

		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	bool IsBalance()
	{
		// 检查根节点的颜色
		if (_root && _root->_col == RED) 
		{
			cout << "根节点颜色是红色" << endl;
			return false;
		}

		// 将最左路径作为基准值进行黑色节点的比较
		int benchmark = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_col == BLACK)
				++benchmark;
			cur = cur->_left;
		}

		// 连续红色节点
		return _Check(_root, 0, benchmark);
	}

	int Height()
	{
		return _Height(_root);
	}

private:
	void _Destroy(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
	}

	int _Height(Node* root)
	{
		if (root == NULL)
			return 0;

		int leftH = _Height(root->_left);
		int rightH = _Height(root->_right);

		return leftH > rightH ? leftH + 1 : rightH + 1;
	}

	bool _Check(Node* root, int blackNum, int benchmark)
	{
		// 检查黑色结点的数量，DFS走到空时将blackNum用来记录黑色结点的数量与benchmark进行比较
		if (root == nullptr)
		{
			if (benchmark != blackNum)
			{
				cout << "某条路径黑色节点的数量不相等" << endl;
				return false;
			}
			//cout << blackNum << endl; // 求每条路径黑色结点的数量
			return true;
		}

		if (root->_col == BLACK)
		{
			++blackNum;
		}

		// 检查连续的红色结点，从父亲开始检查儿子这种方法不是很好(孩子是有可能不存在的)，从儿子开始检查父亲更加的便捷
		if (root->_col == RED
			&& root->_parent
			&& root->_parent->_col == RED)
		{
			cout << "存在连续的红色节点" << endl;
			return false;
		}

		return _Check(root->_left, blackNum, benchmark)
			&& _Check(root->_right, blackNum, benchmark);
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << " ";
		_InOrder(root->_right);
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		Node* ppnode = parent->_parent;

		subR->_left = parent;
		parent->_parent = subR;

		if (ppnode == nullptr)
		{
			_root = subR;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppnode->_left == parent)
			{
				ppnode->_left = subR;
			}
			else
			{
				ppnode->_right = subR;
			}

			subR->_parent = ppnode;
		}
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node* ppnode = parent->_parent;

		subL->_right = parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppnode->_left == parent)
			{
				ppnode->_left = subL;
			}
			else
			{
				ppnode->_right = subL;
			}
			subL->_parent = ppnode;
		}
	}

private:
	Node* _root = nullptr;
};

void Test_RBTree1()
{
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	//int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14, 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	RBTree<int, int> t1;
	for (auto e : a)
	{
		/*	if (e == 14)
		{
		int x = 0;
		}*/
		//cout << e << endl;
		t1.Insert(make_pair(e, e));
		//cout << e << "插入：" << t1.IsBalance() << endl;
	}

	t1.InOrder();
	cout << t1.IsBalance() << endl;
}


void Test_RBTree2()
{
	srand(time(0));
	const size_t N = 5000000;
	RBTree<int, int> t;
	for (size_t i = 0; i < N; ++i)
	{
		size_t x = rand() + i;
		t.Insert(make_pair(x, x));
		//cout << t.IsBalance() << endl;
	}

	//t.Inorder();

	cout << t.IsBalance() << endl;
	cout << t.Height() << endl;
}