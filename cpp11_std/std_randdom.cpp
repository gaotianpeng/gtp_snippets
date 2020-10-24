#include <iostream>
#include <random>
#include <algorithm>

int main(int argc, char* argv[]) {
    std::default_random_engine dre;
    std::uniform_int_distribution<int64_t> di(10, 20000000);
    
    for(int i = 0; i < 20; ++i) {
        std::cout << di(dre) << " ";
    }
    std::cout << di(dre) << std::endl;
    std::cout << std::endl;
    return 0;
}
