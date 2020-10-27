//
// Created by gtp on 2020/10/27.
//

#ifndef _GEN_CSV_CSV_IMPL_H
#define _GEN_CSV_CSV_IMPL_H

#include <string>
#include <vector>
#include "data_type.h"

class CSVImpl {
public:
    static void Execute(const DataTypeParam& data_param, std::string& out_str);
};


#endif // _GEN_CSV_CSV_IMPL_H
