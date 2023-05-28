#pragma once

namespace OpenAddress
{
	enum State
	{
		EMPTY,
		EXIST,
		DELETE
	};

	template<class K, class V>
	struct HashData
	{
		pair<K, V> _kv;
		State _state = EMPTY;
	};

	template<class K, class V>
	class HashTable
	{
	public:
		bool Insert(const pair<K, V>& kv)
		{
			if (Find(kv.first))
			{
				return false;
			}

			// ������˳���0.7�ͽ�������
			// ���ﻹ��ע��Ҫ��size == 0���ǽ��ж���
			if (_tables.size() == 0 || _n * 10 / _tables.size() >= 7) // ��������/�غ����� ��Ӧ���洢�������İٷֱ� // ���������ദ��û��С���ģ���Ҫת����double���߽�������*10
			{
				//_tables.reserve(_tables.capacity()) δ������Ϊ�յ���� ���������޷�����

				//size_t newsize = _tables.size() == 0 ? 10 : tables.size() * 2;
				//_tables.resize(newsize);

				// �����ظ���д��
				//size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				//vector<HashData<K, V>> newtable;
				//newtable.resize(newsize);
				//// �����ɱ�������ӳ�䵽�±�
				//for (size_t i = 0; i < _tables.size(); ++i)
				//{
				//	if (_tables[i]._state == EXIST)
				//	{

				//		size_t hashi = _tables[i]._kv.first % newtable.size();

				//		size_t j = 0;
				//		size_t index = hashi;
				//		while (newtable[index]._state == EXIST)
				//		{
				//			index = hashi + j;
				//			index %= newtable.size();
				//			++j;
				//		}
				//		newtable[index]._kv = _tables[i]._kv;
				//		newtable[index]._state = EXIST;
				//	}
				//}
				//_tables.swap(newtable);

				// �����µ�HashTable�����վɱ�
				size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
				HashTable<K, V> newht;
				newht._tables.resize(newsize);
				for (auto& data : _tables)
				{
					if (data._state == EXIST)
					{
						newht.Insert(data._kv);
					}
				}
				_tables.swap(newht._tables);
			}
			size_t hashi = kv.first % _tables.size();

			// ����̽��
			size_t i = 0;
			size_t index = hashi;
			while (_tables[index]._state != EMPTY)
			{
				index = hashi + i;
				//index = hashi + i*i; // ����̽��
				index %= _tables.size(); // ��ֹһֱ�ӳ���Խ����������
				++i;
			}
			_tables[index]._kv = kv;
			_tables[index]._state = EXIST;
			++_n;
			return true;
		}

		HashData<K, V>* Find(const K& key)
		{
			if (_tables.size() == 0)
			{
				return nullptr;
			}

			size_t hashi = key % _tables.size();

			// ����̽��
			size_t i = 1;
			size_t index = hashi;
			while (_tables[index]._state != EMPTY)
			{
				if (_tables[index]._state != DELETE
					&& key == _tables[index]._kv.first)
				{
					return &_tables[index];
				}

				index = hashi + i;
				index %= _tables.size();
				++i;

				// �Ѿ�������һȦ˵��ȫ�Ǵ�����ɾ��
				if (hashi == index)
				{
					break;
				}
			}
			return nullptr;
		}

		bool Erase(const K& key)
		{
			HashData<K, V>* ret = Find(key);
			if (ret)
			{
				ret->_state = DELETE;
				--_n;
				return true;
			}
			else
			{
				return false;
			}
		}

	private:
		vector<HashData<K, V>> _tables;
		size_t _n = 0; // �洢�����ݸ���
		//HashData* _tables; 
		//size_t size;
		//size_t capacity;
	};

	void TestHashTable1()
	{
		int a[] = { 3, 33, 2, 13, 5, 12, 1002 };
		HashTable<int, int> ht;
		for (auto e : a)
		{
			ht.Insert(make_pair(e, e));
		}

		ht.Insert(make_pair(15, 15));

		if (ht.Find(13))
		{
			cout << "13��" << endl;
		}
		else
		{
			cout << "13����" << endl;
		}

		ht.Erase(13);

		if (ht.Find(13))
		{
			cout << "13��" << endl;
		}
		else
		{
			cout << "13����" << endl;
		}
	}

}
template<class K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};

// ģ����ػ�
template<>
struct HashFunc<string>
{
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (auto ch : s)
		{
			hash += ch;
			hash *= 31;
		}
		return hash;
	}
};
// �ַ�����ϣ�㷨
// https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html
struct HashStr
{
	// BKDR
	size_t operator()(const string& s)
	{
		// return s[0];

		size_t hash = 0;
		for (auto ch : s)
		{
			hash += ch; // ������������abcd��aadd������Ҳ���ͻ
			// ������� BKDRHash
			hash *= 31;
		}
		return hash;

	}
};
namespace HashBucket
{
	template<class T>
	struct HashNode
	{
		HashNode<T>* _next;
		T _data;

		HashNode(const T& data)
			:_next(nullptr)
			,_data(data)
		{}
 	};


	// ��������Ҫ��ϣ������Ҫ��һ��ǰ���������������һ���������õ�����
	template<class K, class T, class KeyOfT, class Hash>
	class HashTable;

	template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
	struct __HashIterator
	{
		typedef HashNode<T> Node;

		typedef HashTable<K, T, KeyOfT, Hash> HT;
		typedef __HashIterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;
		typedef __HashIterator<K, T, T&, T*, KeyOfT, Hash> Iterator;

		Node* _node;
		const HT* _ht;

		__HashIterator(Node* node, const HT* ht)
			:_node(node)
			,_ht(ht)
		{}

		__HashIterator(const Iterator& it)
			:_node(it._node)
			, _ht(it._ht)
		{}

		Ref operator*()
		{
			return _node->_data;
		}

		Ptr operator->()
		{
			return &_node->_data;
		}

		bool operator!=(const Self& s)
		{
			return _node != s._node;
		}

		Self& operator++() // ������Ҫ����HashTable�е�_tables��������Ҫgetsize()������Ԫ
		{
			if (_node->_next != nullptr)
			{
				_node = _node->_next;
			}
			else // _node->_next == nullptr // ����һ����Ͱ��λ��
			{
				KeyOfT kot;
				Hash hash;

				// �����ǰͰ��λ��
				size_t hashi = hash(kot(_node->_data)) % _ht->_tables.size();
				++hashi;

				while (hashi < _ht->_tables.size())
				{
					if (_ht->_tables[hashi])
					{
						_node = _ht->_tables[hashi];
						break;
					}
					else
					{
						++hashi;
					}
				}

				// û���ҵ���Ϊ�յ�Ͱ
				if (hashi == _ht->_tables.size())
				{
					_node = nullptr;
				}
			}


			return *this;
		}
	};

	template<class K, class T, class KeyOfT, class Hash>
	class HashTable 
	{
		// ��Ҫ������������Ϊ��Ԫ
		template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
		friend struct __HashIterator;

		typedef HashNode<T> Node;

	public:
		typedef  __HashIterator<K, T, T&, T*, KeyOfT, Hash> iterator;
		typedef  __HashIterator<K, T, const T&, const T*, KeyOfT, Hash> const_iterator;

		iterator begin()
		{
			Node* cur = nullptr;
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				cur = _tables[i];
				if (cur)
				{
					break;
				}
			}

			return iterator(cur, this);
		}

		iterator end()
		{
			return iterator(nullptr, this);
		}

		const_iterator begin() const
		{
			Node* cur = nullptr;
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				cur = _tables[i];
				if (cur)
				{
					break;
				}
			}

			return iterator(cur, this);
		}

		const_iterator end() const 
		{
			return iterator(nullptr, this);
		}

		~HashTable()
		{
			for (auto& cur : _tables)
			{
				while (cur)
				{
					Node* next = cur->_next;
					delete cur;
					cur = next;
				}

				cur = nullptr;
			}
		}

		iterator Find(const K& key)
		{
			if (_tables.size() == 0)
				return end();

			KeyOfT kot;
			Hash hash;
			size_t hashi = hash(key) % _tables.size();
			Node* cur = _tables[hashi];
			while (cur)
			{
				if (kot(cur->_data) == key)
				{
					return iterator(cur, this);
				}

				cur = cur->_next;
			}

			return end();
		}

		//Node* Find(const K& key)
		//{
		//	if (_tables.size() == 0)
		//		return nullptr;
		//	
		//	KeyOfT kot;
		//	Hash hash;
		//	size_t hashi = hash(key) % _tables.size();
		//	Node* cur = _tables[hashi];
		//	while (cur)
		//	{
		//		if (kot(cur->_data) == key)
		//		{
		//			return cur;
		//		}

		//		cur = cur->_next;
		//	}

		//	return nullptr;
		//}

		bool Erase(const K& key)
		{
			Hash hash;
			size_t hashi = hash(key) % _tables.size();
			Node* prev = nullptr;
			Node* cur = _tables[hashi];
			while (cur)
			{
				if (kot(cur->_data) == key)
				{
					if (prev == nullptr)
					{
						_tables[hashi] = cur->_next;
					}
					else
					{
						prev->_next = cur->_next;
					}
					delete cur;

					return true;
				}
				else
				{
					prev = cur;
					cur = cur->_next;
				}
			}

			return false;
		}

		size_t GetNextPrime(size_t prime)
		{
			// SGI
			static const int __stl_num_primes = 28;
			static const unsigned long __stl_prime_list[__stl_num_primes] =
			{
				53, 97, 193, 389, 769,
				1543, 3079, 6151, 12289, 24593,
				49157, 98317, 196613, 393241, 786433,
				1572869, 3145739, 6291469, 12582917, 25165843,
				50331653, 100663319, 201326611, 402653189, 805306457,
				1610612741, 3221225473, 4294967291
			};

			size_t i = 0;
			for (; i < __stl_num_primes; ++i)
			{
				if (__stl_prime_list[i] > prime)
					return __stl_prime_list[i];
			}

			return __stl_prime_list[i];
		}

		pair<iterator, bool> Insert(const T& data)
		{
			KeyOfT kot;
			iterator it = Find(kot(data));
			if (it != end())
			{
				return make_pair(it, false);
			}

			Hash hash;
			// ����
			// ��������==1ʱ����ƽ��ÿ��Ͱ�����һ��
			if (_n == _tables.size())
			{
				size_t newsize = GetNextPrime(_tables.size());
				vector<Node*> newtable(newsize, nullptr);

				for (auto& cur : _tables)
				{
					while (cur)
					{
						Node* next = cur->_next; // ��¼��һ�����
						size_t hashi = hash(kot(cur->_data)) % newtable.size();

						// ����
						cur->_next = newtable[hashi]; // Ͱ�ĵ�һ���ڵ��Ǵ洢��Ͱ�е�
						newtable[hashi] = cur;
						cur = next;
					}
				}
				_tables.swap(newtable);
			}


			size_t hashi = hash(kot(data)) % _tables.size();

			// ����
			Node* newnode = new Node(data);
			newnode->_next = _tables[hashi];
			_tables[hashi] = newnode;
			++_n;

			return make_pair(iterator(newnode, this), true);
		}

		//bool Insert(const T& data)
		//{
		//	KeyOfT kot;
		//	if (Find(kot(data)))
		//	{
		//		return false;
		//	}

		//	Hash hash;
		//	// ����
		//	// ��������==1ʱ����ƽ��ÿ��Ͱ�����һ��
		//	if (_n == _tables.size())
		//	{
		//		//size_t newsize = _tables.size() == 0 ? 10 : _tables.size()*2;
		//		//HashTable<K, V> newht;
		//		//newht.resize(newsize);
		//		//for (auto cur : _tables)
		//		//{
		//		//	while (cur)
		//		//	{
		//		//		newht.Insert(cur->_kv);
		//		//		cur = cur->_next;
		//		//	}
		//		//}

		//		//_tables.swap(newht._tables);

		//		
		//		size_t newsize = GetNextPrime(_tables.size());
		//		//size_t newsize = _tables.size() == 0 ? 10 : _tables.size() * 2;
		//		vector<Node*> newtable(newsize, nullptr);
		//		
		//		//for (Node*& cur : _tables)
		//		//for (size_t i = 0; i < _tables.size(); ++i)
		//		//{
		//		//	Node* cur = _tables[i];
		//		for (auto& cur : _tables) 
		//		{
		//			while (cur)
		//			{
		//				Node* next = cur->_next; // ��¼��һ�����
		//				size_t hashi = hash(kot(cur->_data)) % newtable.size();

		//				// ����
		//				cur->_next = newtable[hashi]; // Ͱ�ĵ�һ���ڵ��Ǵ洢��Ͱ�е�
		//				newtable[hashi] = cur;
		//				cur = next;
		//			}
		//		}
		//		_tables.swap(newtable);
		//	}


		//	size_t hashi = hash(kot(data)) % _tables.size();
		//	
		//	// ����
		//	Node* newnode = new Node(data);
		//	newnode->_next = _tables[hashi];
		//	_tables[hashi] = newnode;
		//	++_n;

		//	return true;
		//}

		size_t MaxBucketSize()
		{
			size_t max = 0;
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				auto cur = _tables[i];
				size_t size = 0;
				while (cur)
				{
					++size;
					cur = cur->_next;
				}

				//printf("[%d]->%d\n", i, size);
				if (size > max)
				{
					max = size;
				}
			}

			return max;
		}

	private:
		vector<Node*> _tables;
		size_t _n = 0; // �洢����Ч���ݸ���
	};
} 