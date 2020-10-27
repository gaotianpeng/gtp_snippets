//
// Created by gtp on 2020/10/27.
//

#include "csv_impl.h"
#include "generate_data.h"

void CSVImpl::Execute(const DataTypeParam& data_param, std::string& out_str) {
  switch (data_param.type_) {
    case CSV_INT:
       if (data_param.min_ != data_param.max_) {
          out_str = std::to_string(GenerateData::GenerateInt(data_param.min_, data_param.max_));
       } else {
          out_str = std::to_string(GenerateData::GenerateInt());
       }
    break;
    case CSV_LONG:
       if (data_param.min_ != data_param.max_) {
          out_str = std::to_string(GenerateData::GenerateLong(data_param.min_, data_param.max_));
       } else {
          out_str = std::to_string(GenerateData::GenerateLong());
       }
    break;
    case CSV_VARCHAR:
       out_str = GenerateData::GenerateString(data_param.str_len_);
    break;
  }
}