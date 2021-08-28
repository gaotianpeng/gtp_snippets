#include <iostream>
using namespace std;

template <typename T, size_t N>
class RangeBaseTest {
public:
    RangeBaseTest() {
        for (size_t i = 0; i < N; ++i) {
            element_[i] = i;
        }
    }

    ~RangeBaseTest() {}

    T* begin() {
        return elements_ + 0;
    }

    T* end() {
        return elements_ + N;
    }
protected:

private:
    T element_[N];
};

int main(int argc, char* argv[]) {
    RangeBaseTest<int, 10> a;
	for (auto iter : a) {
		cout << iter << endl;
	}
    return 0;
}