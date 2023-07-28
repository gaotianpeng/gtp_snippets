#include <iostream>
#include <string>
#include <set>
#include <random>

using namespace std;

class Math {
public:
    static double random() {
        static std::random_device rd;
        static std::default_random_engine engine(rd());
        static std::uniform_real_distribution<> distribution(0.0, 1.0);
        return distribution(engine);
    }
};

static int RandomVal(int min_val, int max_val) {
    return (int)(Math::random()*(max_val - min_val)) + min_val;
}


int main(int arg, char* argv[]) {
    int min = 0; int max = 10000;
    std::set<int> numbers;
    for (int i = 0; i < 1000; i++) {
        numbers.insert(RandomVal(min, max));
    } 
    
    for (const auto& element : numbers) {
        std::cout << element << " ";
    }
    cout << endl << endl;
    auto it = numbers.begin();
    for (int i = 0; i < 1000; ++i) {
        int index = RandomVal(0, numbers.size());
        std::advance(it, index);
        cout << *it << " ";
    }

    return 0;
}