#include <iostream>
#include <exception>
#include <system_error>
#include <future>
using namespace std;
/*
所有被语言本向或标准库抛出的异常都派生自基类 exception, 定义于<exception>,是若干标准异常类的基类
标准异常类分为三组
    1 语言本身支持的异常
    2 C++标准库发出的异常
    3 程序作用域之外发出的异常

exception:
    logic_error
        domain_error
        invalid_argument
        length_error
        out_of_range
        future_error(code)
    runtime_error
        system_error(code)
            ios_base::failure
        range_error
        overflow_error
        underflow_error
    bad_cast
    bad_typeid
    bad_exception
    bad_alloc
        bad_array_new_length
    bad_weak_ptr
    bad_function_call

逻辑错误通常可以避免，因为其错误原因发生在程序作用域（scope of a program）内，例如先决条件被违反
运行期异常则是由一个位于程序作用域之外的原因触发，例如资源不足
*/

/*
1. 针对语言支持(Language Support)而设计的异常类
    此类异常用以支撑某些语言特性，是核心语言的一部分。以下操作如果失败会抛出此类异常
        a) 运行期间，当一个针对reference身上的dynamic_cast失败时，dynamic_cast会抛出bad_cast异常
        b) RTTI过程中，如果交给 typeid 操作符的实参为0或为空指针，typeid操作符会抛出bad_typeid异常
        c) 定义于<exception>内的bad_exception异常用来处理非预期的异常
2. 针对逻辑差错(Logic Error)而设计的异常类
    总是派生自logic_error。逻辑差错就是可在程序中避免的错误。C++标准库提供以下异常类
        a) invalid_argument
        b) length_error
        c) out_of_range
        d) domain_error
        e) future_error:当使用非同步调用(asynchronous system call) 时发生的逻辑差错
3. 针对运行差错(Runtime Error)而设计的异常类
	总是派生自runtime_error, 用来指出不在程序作用域内且不容易回避的事件
		a) range_error
		b) overflow_error
		c) underflow_error
		d) system_error:因底层操作系统而发生的差错
		e) 只要全局操作符new失败，定义于＜new＞的bad_alloc就会被抛出，除非用的是new的nothrow版本
        f) “根据一个 shared pointer 创建出一个weak pointer”的操作失败，定义于＜memory＞中的bad_weak_ptr会被抛出
        g)  当一个function的wrapper object被调用但其实没有目标（target）时，定义于＜functional＞中的bad_function_call会被抛出
4. 异常类的头文件
    exception       // for class exception and bad_exception
    stdexcept       // for most logic and runtime error classes
    system_error    // for system errors(since c++11)
    new             // for out-of-memory exceptions
    ios             // for I/O exceptions
    future          // for errors wyth async() and futures(since c++11)
    typeinfo        // for bad_cast and bad_typeid
*/

/*
异常类(Exception Class)的成员
    为了在catch子句中处理异常，你必须采用该异常类提供的接口。所有标准异常都提供what()，某些异常类还提供了code()
1. 成员函数 what()
	namespace std {
		class exception {
		  public:
			virtual const char* what() const noexcept;
		};
	}

2. 差错码vs差错状态(Error Code vs Error Condition)
    差错码：是一种轻型对象，用来封装差错码值(error code value),差错值由编译器指定
    差错值：是一种提供差错描述之可移植性抽象层的对象

3. 处理差错码(Error Code)和差错状态(Error Condition)
    std::error_code
    std::error_condition
		namespace std {
			class system_error: public range_error {
			  public:
				virtual const char* what() const noexcept;
				const error_code& code() const noexcept;
			};

			class future_error: public logic_error {
				virtual const char* what() const noexcept;
				const error_code& code() const noexcept;
			};
        }
*/

/*
示例：使用一个范型函数来打印不同的异常
*/
template <typename T>
void ProcessCodeException(const T& e) {
    auto c = e.code();
    cerr << "- category:        " << c.category().name() << endl;
    cerr << "- value:           " << c.value() << endl;
    cerr << "- def category:    " << c.default_error_condition().category().name() << endl;
    cerr << "- def value:       " << c.default_error_condition().value() << endl;
}

void ProcessException() {
    try {
        throw std::system_error(std::make_error_code(std::errc::invalid_argument), "argument ... is not valid");
    }
    catch (const ios_base::failure& e) {
        cerr << "I/O EXCEPTION: " << e.what() << endl;
        ProcessCodeException(e);
    }
    catch (const system_error& e) {
        cerr << "SYSTEM EXCEPTION: " << e.what() << endl;
        ProcessCodeException(e);
    }
    catch (const future_error& e) {
        cerr << "FUTURE EXCEPTION: " << e.what() << endl;
        ProcessCodeException(e);
    }
    catch (...) {
        cerr << "EXCEPTION (unknow)" << endl;
    }
}

/*
以 class exception_ptr 传递异常
    将异常存储于类型为exception_ptr的对象中，稍后才在其他情境（context）中处理
*/

std::exception_ptr eptr;	// object to hold exceptions
void foo() {
    try {
        throw std::out_of_range("out_of_range (somewhere, somehow)");
    }
    catch (...) {
        eptr = std::current_exception();
    }
}

void bar() {
    if (eptr != nullptr) {
        std::rethrow_exception(eptr);
    }
}

int main(int argc, char* argv[]) {
    ProcessException();
    bar();
    return 0;
}