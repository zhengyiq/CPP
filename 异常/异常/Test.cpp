#include <iostream>
#include <string>
#include <thread>
using namespace std;

//double Division(int a, int b)
//{
//	// 当b == 0时抛出异常
//	if (b == 0)
//		throw "Division by zero condition!";
//	else
//		return ((double)a / (double)b);
//}
//void Func()
//{
//	int len, time;
//	cin >> len >> time;
//	//cout << Division(len, time) << endl;
//	
//	try
//	{
//		cout << Division(len, time) << endl;
//	}
//	catch (const char* errmsg)
//	{
//		cout << "void Func()" << endl;
//		cout << errmsg << endl;
//	}
//}
//int main()
//{
//	try {
//		Func();
//	}
//	catch (const char* errmsg)
//	{
//		cout << errmsg << endl;
//	}
//	catch (...) 
//	{
//		cout << "unkown exception" << endl;
//	}
//	return 0;
//}

//class Exception
//{
//public:
//	Exception(int errid, const string& msg)
//		:_errid(errid)
//		,_errmsg(msg)
//	{}
//
//	const string& GetMsg() const
//	{
//		return _errmsg;
//	}
//
//	int GetErrid() const
//	{
//		return _errid;
//	}
//
//private:
//	int _errid; // 错误码
//	string _errmsg; // 错误描述
//};
//
//double Division(int a, int b)
//{
//	// 当b == 0时抛出异常
//	if (b == 0)
//	{
//		//Exception err(1, "除零错误");
//		//throw err;
//		throw Exception(1, "除零错误");
//	}
//	else
//		return ((double)a / (double)b);
//}
//void Func()
//{
//	int len, time;
//	cin >> len >> time;
//	//cout << Division(len, time) << endl;
//
//	try
//	{
//		cout << Division(len, time) << endl;
//	} 
//	catch (const Exception& e) // 这里捕获的是err的拷贝; 万能引用只存在于模版中
//	{
//		cout << "void Func()" << endl;
//		cout << e.GetErrid() << endl;
//		cout << e.GetMsg() << endl;
//	}
//	throw 1;
//}
//int main()
//{
//	while (1)
//	{
//		try {
//			Func();
//		}
//		catch (const char* errmsg)
//		{
//			cout << errmsg << endl;
//		}
//		catch (...) // 任意类型的异常都可以捕获, 放到最后，防止有一些异常没有捕获，导致程序终止
//		{
//			cout << "unkown exception" << endl;
//		}
//	}
//	return 0;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 可以抛出的派生类对象，使用基类捕获 -- 多态
//
class Exception
{
public:
	Exception(int errid, const string& msg)
		:_errid(errid)
		, _errmsg(msg)
	{}

	//const string& GetMsg() const
	//{
	//	return _errmsg;
	//}

	virtual string what() const
	{
		return _errmsg;
	}

	int GetErrid() const
	{
		return _errid;
	}

protected: 
	int _errid; // 错误码
	string _errmsg; // 错误描述
};
//
//class SqlException : public Exception
//{
//public:
//	SqlException(int errid, const string& msg, const string& sql)
//		:Exception(errid, msg)
//		,_sql(sql)
//	{}
//
//	virtual string what() const
//	{
//		string msg = "SqlException：";
//		msg += _errmsg;
//		msg += "->";
//		msg += _sql;
//
//		return msg;
//	}
//
//protected:
//	string _sql;
//};
//
//class CacheException : public Exception
//{
//public:
//	CacheException(const string& errmsg, int id)
//		:Exception(id, errmsg)
//	{}
//
//	virtual string what() const
//	{
//		string msg = "CacheException：";
//		msg += _errmsg;
//
//		return msg;
//	}
//};
//
//class HttpServerException : public Exception
//{
//public:
//	HttpServerException(const string& errmsg, int id, const string& type)
//		:Exception(id, errmsg)
//		, _type(type)
//	{}
//
//	virtual string what() const
//	{
//		string msg = "HttpServerException：";
//		msg += _errmsg;
//		msg += "->";
//		msg += _type;
//
//		return msg;
//	}
//
//private:
//	const string _type;
//};
//
//void SQLMgr()
//{
//	srand(time(0));
//	if (rand() % 7 == 0)
//	{
//		throw SqlException(100, "权限不足", "select * from name = '张三'");
//	}
//
//	cout << "调用成功" << endl;
//}
//
//void CacheMgr()
//{
//	srand(time(0));
//	if (rand() % 5 == 0)
//	{
//		throw CacheException("权限不足", 100);
//	}
//	else if (rand() % 6 == 0)
//	{
//		throw CacheException("数据不存在", 101);
//	}
//
//	SQLMgr();
//}
//
//void HttpServer()
//{
//	// 模拟服务出错
//	srand(time(0));
//	if (rand() % 3 == 0)
//	{
//		throw HttpServerException("请求资源不存在", 100, "get");
//	}
//	else if (rand() % 4 == 0)
//	{
//		throw HttpServerException("权限不足", 101, "post");
//	}
//
//	CacheMgr();
//}

//int main()
//{
//	while (1)
//	{
//		this_thread::sleep_for(chrono::seconds(1));
//
//		try
//		{
//			HttpServer();
//		}
//		catch (const HttpServerException& e) // 谁近就选择谁
//		{
//			cout << "子类:";
//			cout << e.what() << endl;
//		}
//		catch (const Exception& e) // 这里捕获父类对象就可以 -- 通过多态捕获父类对象
//		{
//			// 多态
//			cout << e.what() << endl;
//		}
//		catch (...)
//		{
//			cout << "Unkown Exception" << endl;
//		}
//	}
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////////////////
double Division(int a, int b) // noexcept 此时异常无法捕获// throw()//声明这些函数不会抛出异常
{
	// 当b == 0时抛出异常
	if (b == 0)
	{
		throw "Division by zero condition!";
	}

	return (double)a / (double)b;
}

void Func() // noexcept // 这里如果是间接的抛异常是不会显示警告
{
	// 这里可以看到如果发生除0错误抛出异常，另外下面的array没有得到释放。
	// 所以这里捕获异常后并不处理异常，异常还是交给外面处理，这里捕获了再
	// 重新抛出去。
	int* array = new int[10];

	int len, time;
	cin >> len >> time;

	try
	{
		cout << Division(len, time) << endl;
		//HttpServer();
	}
	catch (...)   // 异常的重新抛出，代码不支持解析对象
	{
		cout << "delete []" << array << endl;
		delete[] array;

		throw;   // 捕到什么抛什么 期望在主函数中统一处理异常，坐在这里捕获到了重新抛出
	}

	cout << "delete []" << array << endl;
	delete[] array;
}

int main()
{
	while (1)
	{
		try
		{
			Func();
		}
		catch (const char* errmsg)
		{
			cout << errmsg << endl;
		}
		catch (const Exception& e) // 这里捕获父类对象就可以
		{
			// 多态
			cout << e.what() << endl;
		}
		catch (...)
		{
			cout << "Unkown Exception" << endl;
		}
		
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 异常规范
// 这个函数异常声明并不是必须的并且比较繁琐
// 这里表示这个函数会抛出A/B/C/D中的某种类型的异常
// void fun() throw(A，B，C，D);
// // 这里表示这个函数只会抛出bad_alloc的异常
// void* operator new (std::size_t size) throw (std::bad_alloc);
// // 这里表示这个函数不会抛出异常
// void* operator delete (std::size_t size, void* ptr) throw();
// // C++11 中新增的noexcept，表示不会抛异常
// thread() noexcept;
// thread(thread&& x) noexcept;

//new/malloc/fopen/lock
//delete/free/fclose/unlock

//异常的缺点，执行流乱跳：没有用异常之前，跳出循环或者函数；有异常之后，可以跳跃多个函数