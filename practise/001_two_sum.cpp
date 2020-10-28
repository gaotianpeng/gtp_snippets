#include <iostream>
#include <vector>
#include <map>
using namespace std;

/*
给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那 两个 整数，并返回他们的数组下标
*/
class Solution {
 public:
    vector<int> twoSum(vector<int>& nums, int target) {
      if (nums.size() == 0 || nums.size() == 1) {
          return vector<int>();
      }

      std::map<int, int> val_loc_mapper;
      size_t n_elem = nums.size();
      for (int i = 0; i < n_elem; ++i) {
          val_loc_mapper.insert(std::pair<int, int>(nums.at(i), i));
      }

      for (int i = 0; i < n_elem; ++i) {
          auto iter = val_loc_mapper.find(target - nums[i]);
          if (iter != val_loc_mapper.end()) {
              if (iter->second != i) {
                  if (iter->second < i) {
                      return vector<int>{iter->second, i};
                  }
                  return vector<int>{i, iter->second};
              }
          }
      }

      return vector<int>();
    }
};

void RunTest() {
    Solution sol;
    vector<int> nums = {2, 4, 5, 9};
    vector<int> res = sol.twoSum(nums, 9);
    if (res.size() == 2) {
        cout << "[" << res.at(0) << ", " << res.at(1) << "]";
    } else {
        cout << "no element that fit.";
    }
}

int main(int argc, char* argv[]) {
    RunTest();
    return 0;
}