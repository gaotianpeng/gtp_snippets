#include <iostream>
#include <string>
using namespace std;

class SalesItem {
public:
    SalesItem(std::string isbn, float price=0.0f): isbn_(isbn), price_(price){
    }

    std::string Isbn() {
        return isbn_;
    }

    float operator+(const SalesItem& item) {
        return this->price_ +item.GetPrice();
    }

    float GetPrice() const {
        return price_;
    }
private:
    std::string isbn_;
    float price_ = 0.0f;
};

int main(int argc, char* argv[]) {
    // throw 示例
    SalesItem item1("isbn-1234", 10.4f);
    SalesItem item2("isbn-1234", 200.8f);
    if (item1.Isbn() != item2.Isbn()) {
        throw runtime_error("Data must refer to same ISBN");
    }
    cout << item1 + item2 << endl;

    // try 示例
    // 如果try语句块的代码抛出了runtime_error异常，则接下来执行catch块内的语句
    try {
        SalesItem item1("isbn-1234", 10.4f);
        SalesItem item2("isbn-12345", 200.8f);
        if (item1.Isbn() != item2.Isbn()) {
            throw runtime_error("Data must refer to same ISBN");
        }
        cout << item1 + item2 << endl;
    }
    catch(runtime_error err) {
        cout << err.what()
            << "\n You can put this code in while loop!!!" << endl;
    }
    return 0;
}