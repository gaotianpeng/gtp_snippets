//
// Created by gtp on 2020/10/27.
//

#ifndef _GEN_CSV_DATA_TYPE_H
#define _GEN_CSV_DATA_TYPE_H

#include <vector>

enum CSVDataType {
    CSV_INT,
    CSV_LONG,
    CSV_VARCHAR
};

struct DataTypeParam {
    CSVDataType type_ = CSV_INT;
    long long min_ = 0;
    long long max_ = 0;
    int str_len_ = 0;
};

#endif //_GEN_CSV_DATA_TYPE_H
