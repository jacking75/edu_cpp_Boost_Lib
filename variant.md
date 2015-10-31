### 다양한 타입 사용하기

```
// 출처: https://sites.google.com/site/boostjp/tips/variant
#include <iostream>
#include <string>
#include <boost/variant.hpp>

struct var_printer : boost::static_visitor<void> {
	void operator()(int x) const
	{
		std::cout << x << std::endl;
	}

	void operator()(std::string& s) const
	{
		std::cout << s << std::endl;
	}

	void operator()(double x) const
	{
		std::cout << x << std::endl;
	}
};

int main()
{
	std::cout << "저장" << std::endl;
	{
		boost::variant<int, std::string, double> v;

		// 빈 상태
		std::cout << v.which() << std::endl;

		v = 1; // int 타입의 값 저장
		std::cout << v.which() << std::endl;

		v = 3.14; // double 타입의 값 저장
		std::cout << v.which() << std::endl;
	}

	std::cout << "타입 판정" << std::endl;
	{
		boost::variant<int, std::string, double> v;

		v = 1; // int 타입의 값 저장
		if (v.type() == typeid(int)) {
			std::cout << "int" << std::endl;
		}

		v = 3.14; // double 타입의 값 저장
		if (v.type() == typeid(double)) {
			std::cout << "double" << std::endl;
		}
	}

	std::cout << "값 얻기" << std::endl;
	{
		boost::variant<int, std::string, double> v;
		v = 1;

		// 참조
		try {
			int& x = boost::get<int>(v);
			std::cout << x << std::endl;
		}
		catch (boost::bad_get& e) {
			std::cout << e.what() << std::endl;
		}

		// 포인터
		if (int* x = boost::get<int>(&v)) {
			std::cout << *x << std::endl;
		}
		else {
			std::cout << "int 값은 저장 되어 있지 않다." << std::endl;
		}
	}

	std::cout << "값 비우기" << std::endl;
	{
		boost::variant<boost::blank, int, std::string> v = boost::blank();
		v = 3;

		v = boost::blank();

		if (v.type() == typeid(boost::blank)) {
			std::cout << "blank" << std::endl;
		}
		else {
			std::cout << "no blank" << std::endl;
		}
	}

	std::cout << "타입에 맞는 함수 호출하기" << std::endl;
	{
		// int, string, double
		boost::variant<int, std::string, double> v;

		v = 3; // int
		boost::apply_visitor(var_printer(), v); // visitor으로 타입에 맞는 처리를 한다.

		v = "hello"; // string
		boost::apply_visitor(var_printer(), v);
	}


	return 0;
}
```


### Boost.Variant로 상속을 대체
- 출처 https://sites.google.com/site/boostjp/tips/variant
```
#include <iostream>
#include <memory>
#include <vector>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/variant.hpp>


struct Background {
	void update()
	{
		std::cout << "background" << std::endl;
	}
};

struct Character {
	void update()
	{
		std::cout << "character" << std::endl;
	}
};

struct Effect {
	void update()
	{
		std::cout << "effect" << std::endl;
	}
};

struct UpdateVisitor {
	using result_type = void;

	template <class T>
	void operator()(T& x)
	{
		x.update();
	}
};

class TaskList {
	using TaskType = boost::variant<Background, Character, Effect>;
	std::vector<TaskType> taskList_;
public:
	void update()
	{
		for (TaskType& x : taskList_) {
			UpdateVisitor vis;
			boost::apply_visitor(vis, x);
		}
	}

	template <class Task, class... Args>
	void add(Args... args)
	{
		taskList_.push_back(Task(args...));
	}
};

int main()
{
	TaskList task;
	task.add<Background>();
	task.add<Character>();
	task.add<Effect>();

	task.update();

	return 0;
}
```

위 구현을 기존에는 아래처럼 인터페이스를 상속해서 구현하였다.
```
struct UpdateInterface {
	virtual void update() = 0;
};

struct Background : public UpdateInterface {
	void update() override
	{
		std::cout << "background" << std::endl;
	}
};

struct Character : public UpdateInterface {
	void update() override
	{
		std::cout << "character" << std::endl;
	}
};

struct Effect : public UpdateInterface {
	void update() override
	{
		std::cout << "effect" << std::endl;
	}
};

class TaskList {
	std::vector<std::unique_ptr<UpdateInterface>> taskList_;
public:
	void update()
	{
		for (UpdateInterface& x : taskList_ | boost::adaptors::indirected) {
			x.update();
		}
	}

	template <class Task, class... Args>
	void add(Args... args)
	{
		taskList_.push_back(std::unique_ptr<UpdateInterface>(new Task(args...)));
	}
};

int main()
{
	TaskList task;
	task.add<Background>();
	task.add<Character>();
	task.add<Effect>();

	task.update();
}
```
