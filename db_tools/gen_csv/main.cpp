#include <iostream>
#include <vector>
#include "gen_csv.h"

using namespace std;

int main(int argc, char* argv[]) {

   std::vector<DataTypeParam> data_types;
   data_types.push_back({CSV_INT, 10, 100});
   data_types.push_back({CSV_INT, 10, 100});
   data_types.push_back({CSV_LONG, 10, 1000000});
   data_types.push_back({CSV_VARCHAR, 0, 0, 10});

   GenCSV::GenCSVData(data_types, 100, "e:/values.csv");

   return 0;
}
