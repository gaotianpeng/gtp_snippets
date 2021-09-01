#include <iostream>
#include <boost/type_index.hpp>
using namespace std;

template <typename T>
void myfunc(T&& tmp_var) {
	cout << "--------------------begin----------------------" << std::endl;
	using boost::typeindex::type_id_with_cvr;
	cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
	cout << "tmp_var=" << type_id_with_cvr<decltype(tmp_var)>().pretty_name() << endl;
	cout << "---------------------end-----------------------" << std::endl;
}

int main(int argc, char* argv[]) {
	int i = 0;
	myfunc(i);
	return 0;
}