//
// Created by gtp on 2020/10/27.
//

#include "generate_data.h"
#include "global_random_obj.h"
#include <random>
#include <limits>
#include <algorithm>

int GenerateData::GenerateInt() {
   std::uniform_int_distribution<int> di_int(std::numeric_limits<int>::min(),
                                             std::numeric_limits<int>::max());
   return di_int(GlobalRandomOBJ::GetInstance()->GetRandomEngine());
}

int GenerateData::GenerateInt(int r_min, int r_max) {
   std::uniform_int_distribution<int> di_int(r_min, r_max);
   return di_int(GlobalRandomOBJ::GetInstance()->GetRandomEngine());
}



int64_t GenerateData::GenerateLong() {
   std::uniform_int_distribution<int64_t> di_long(std::numeric_limits<long long>::min(),
                                                  std::numeric_limits<long long >::max());
   return di_long(GlobalRandomOBJ::GetInstance()->GetRandomEngine());
}

int64_t GenerateData::GenerateLong(int64_t r_min, int64_t r_max) {
   std::uniform_int_distribution<int64_t> di_long(r_min, r_max);
   return di_long(GlobalRandomOBJ::GetInstance()->GetRandomEngine());
}

std::string GenRandomString( size_t len ) {
    auto rand_char = []() -> char {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index ];
    };

    std::string ret_str(len, 0);
    std::generate_n(ret_str.begin(), len, rand_char);
    return ret_str;
}

std::string GenerateData::GenerateString(int str_len) {
   return GenRandomString(str_len);
}