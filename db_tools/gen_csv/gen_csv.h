//
// Created by gtp on 2020/10/27.
//

#ifndef _GEN_CSV_GEN_CSV_H
#define _GEN_CSV_GEN_CSV_H

#include "data_type.h"
#include <vector>
#include <string>

class GenCSV {
public:
    static void GenCSVData(const std::vector<DataTypeParam>& data_types, int row_n, const std::string& file_name);

private:

};


#endif //_GEN_CSV_GEN_CSV_H
