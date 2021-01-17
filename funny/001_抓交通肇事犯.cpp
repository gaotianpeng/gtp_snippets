/*
一辆卡车违反交通规则，撞人后逃跑。现场有三人目击该事件，但都没有记住车号，只记下了车号的一些特征
    甲说：牌照的前两位数字是相同的
    乙说：牌照的后两位数字是相同的，但与前两位不同
    丙是数学家，他说：4位的车号刚好是一个整数的平方
*/
#include <iostream> 
using namespace std;

int GetCarNum() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int k = 1000 * i + 100 * i + 10 * j + j;
            for (int temp = 31; temp < 100; ++temp) {
                if (temp * temp == k) {
                    return k;
                }
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    cout << "车牌号为:" << GetCarNum() << endl;
    return 0;
}