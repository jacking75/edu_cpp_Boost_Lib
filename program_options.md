# Program Options
[공식 문서](https://www.boost.org/doc/libs/1_73_0/doc/html/program_options.html )  
    
## 명령 줄 인수를 boost :: program_options에 저장
[출처](http://tips.hecomi.com/entry/20110210/1297353366 )  
  
`boost::program_options`을 사용하면 명령어 라인 인수를 쉽게 분석 할 수 있다.   
  
### 사용법
한 가지 방법을 정리해 보았다.  
```
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

int main(int argc, char* argv[])
{
	// 옵션 정의를 한다
	// 뒤에서 합칠 수 있다.
	options_description options1("옵션 그룹 1");
	options_description options2("옵션 그룹 2");
	options_description options3("옵션 그룹 3");
	options_description options4("옵션 그룹 4");

	// 옵션은 add_options() 이후에 operator()를 연결시켜서 추가
	// 「,」의 앞에 정식 이름, 뒤에 줄임 이름을 적는다(생략 가능)
	options1.add_options()
		("option1",						"옵션 1")
		("option2,b",						"옵션 2")
	;

	// option=*** 으로 받는 값의 타입을 설정할 수 있다.
	// 또 default_value 로 기본 값도 설정 가능
	options2.add_options()
		("option3,c",	value<int>(),				"옵션3")
		("option4,d",	value<string>(),			"옵션4")
		("option5,e",	value<int>()->default_value(10),	"옵션5")
	;

	// option=*** 의 값을 받는 변수를 지정할 수도 있다
	int option6;
	string option7;
	options3.add_options()
		("option6,f",	value<int>(&option6),			"옵션6")
		("option7,g",	value<string>(&option7),		"옵션7")
	;

	// 같은 옵션을 복수 사용하여 저장할 수도 있다
	options4.add_options()
		("option8",	value<vector<string> >(),		"옵션8")
	;

	// 옵션 정의를 모을 수 있다
	options1.add(options2).add(options3);

	// 옵션 값을 읽는 변수
	variables_map values;

	// 다양한 예외를 잡을 수 있다(예는 catch 블럭 내에 기술한다)
	try {
		// parse_command_line 에서 명령어 라인 인수를 parse 하고,
		// 이 결과를 store에서 variable_maps 에 저장한다
		store(parse_command_line(argc, argv, options1), values);

		// 명령어 라인 인수뿐만 아닌 외부 파일에서도 옵션을 읽을 수 있다
		// 이 때는 parse_config_file을 사용하여 parse 한다
		ifstream ifs("config.txt");
		store(parse_config_file(ifs, options4), values);

		// notify를 호출하여 values에 값을 저장한다
		notify(values);

		// 옵션 유무는 count로 조사한다
		if (!values.count("option1") && !values.count("option2")) {
			// options_description는 표준 출력에 던질 수 있다
			cout << options1 << endl;
			cout << options4 << endl;
		}
		if (values.count("option1")) {
			cout << "option1 이 지정 되었다" << endl;
		}
		if (values.count("option2")) {
			cout << "option2 이 지정 되었다" << endl;
		}

		// as<타입 이름>() 으로 값을 얻을 수 있다
		cout << "option3: " << values["option3"].as<int>()	<< endl;
		cout << "option4: " << values["option4"].as<string>()	<< endl;
		cout << "option5: " << values["option5"].as<int>()	<< endl;

		cout << "option6: " << option6				<< endl;
		cout << "option7: " << option7				<< endl;

		// 복수의 데이터를 받은 것도 사용 가능
		cout << "option8: ";
		vector<string> option8( values["option8"].as<vector<string> >() );
		for (vector<string>::iterator it = option8.begin(); it != option8.end(); ++it) {
			cout << *it << " ";
		}
		cout << endl;

	} catch (exception &e) {
		// 저장 되지 않은 것을 values["***"] 를 호출하면 
		//	 boost::bad_any_cast: failed conversion using boost::any_cast
		// 가 표시된다.
		// 정의 되지 않은 옵션  --*** 을 호출하면 
        // unknown option ***
		// 가 표시된다
		cout << e.what() << endl;
	}

	return 0;
}
```
  
### 결과
<pre>
명령 줄 인수를 주지 않는 경우
옵션 그룹 1 :
  --option1 옵션 1
  -b [--option2 옵션 2

옵션 그룹 2 :
  -c [--option3] arg 옵션 3
  -d [--option4] arg 옵션 4
  -e [--option5] arg (= 10) 옵션 5

옵션 군 3 :
  -f [--option6] arg 옵션 6
  -g [--option7] arg 옵션 7

옵션 그룹 4 :
  --option8 arg 옵션 8
</pre>  
  
  
### 명령 라인 인수를 준 경우
명령 줄 인수  	
<pre>  
-b -c 1234 -d hoge -f 2345 -g fuga
</pre>  
    
입력 파일:  
<pre>  
# config.txt
# "#"는 주석 "//"에 해당
option8 = str1 # 1 번째
option8 = str2 # 두 번째도 ok
option8 = str3 # 3 번째
option8 = str4 # 4 번째
option8 = str5 # 몇개든지 추가 할 수 있다
</pre>  
  
결과:  
<pre>  
option2가 지정 되었다
option3 : 1234
option4 : hoge
option5 : 10
option6 : 2345
option7 : fuga
option8 : str1 str2 str3 str4 str5
</pre>  
      
  
