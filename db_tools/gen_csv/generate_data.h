//
// Created by gtp on 2020/10/27.
//

#ifndef _GEN_CSV_GENERATE_DATA_H
#define _GEN_CSV_GENERATE_DATA_H

#include <vector>
#include <string>

class GenerateData {
public:

    /*
     *
     */
    static int GenerateInt();
    /*
     *
     */
    static int GenerateInt(int r_min, int r_max);



    /*
     *
     */
    static int64_t GenerateLong();
    /*
     *
     */
    static int64_t GenerateLong(int64_t r_min, int64_t r_max);

    /*
     *
     */
    static std::string GenerateString(int str_len);
};
#endif //_GEN_CSV_GENERATE_DATA_H
