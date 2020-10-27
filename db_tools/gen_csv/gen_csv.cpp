//
// Created by gtp on 2020/10/27.
//

#include "gen_csv.h"
#include "csv_impl.h"
#include <fstream>

void GenCSV::GenCSVData(const std::vector<DataTypeParam>& data_types, int row_n, const std::string& file_name) {
   std::ofstream file(file_name);
   if (!file.is_open()) {
      return;
   }

   int col_n = data_types.size();
   for (int i = 0; i < row_n; ++i) {
      std::string line_str;
      for (int j = 0; j < col_n; ++j) {
         std::string out_str;
         CSVImpl::Execute(data_types[j], out_str);
         if (j != col_n - 1) {
            line_str += (out_str + ",");
         } else {
            line_str += out_str;
         }
      }
      file << line_str << std::endl;
   }
}