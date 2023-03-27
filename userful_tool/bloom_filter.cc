#include <iostream>
#include <bitset>
#include <string>

using namespace std;

const int FILTER_SIZE = 1000000;

class BloomFilter {
public:
    void add(string key) {
        int hash1_val = hash1(key);
        int hash2_val = hash2(key);
        int hash3_val = hash3(key);
        filter[hash1_val] = 1;
        filter[hash2_val] = 1;
        filter[hash3_val] = 1;
    }

    bool contains(string key) {
        int hash1_val = hash1(key);
        int hash2_val = hash2(key);
        int hash3_val = hash3(key);
        return filter[hash1_val] && filter[hash2_val] && filter[hash3_val];
    }

private:
    bitset<FILTER_SIZE> filter;
    int hash1(string key) {
        int hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash = (hash * 31 + key[i]) % FILTER_SIZE;
        }
        return hash;
    }

    int hash2(string key) {
        int hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash = (hash * 17 + key[i]) % FILTER_SIZE;
        }
        return hash;
    }

    int hash3(string key) {
        int hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash = (hash * 13 + key[i]) % FILTER_SIZE;
        }
        return hash;
    }
};

int main(int argc, char* argv[]) {
    BloomFilter filter;
    filter.add("hello");
    filter.add("world");
    cout << filter.contains("hello") << endl;
    cout << filter.contains("world") << endl;
    cout << filter.contains("foo") << endl;
    return 0;
}
