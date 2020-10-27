//
// Created by gtpsy on 2020/10/27.
//

#ifndef _GEN_CSV_GLOBAL_RANDOM_OBJ_H_
#define _GEN_CSV_GLOBAL_RANDOM_OBJ_H_
#include <random>

class GlobalRandomOBJ {
public:
   static GlobalRandomOBJ* GetInstance();

   std::default_random_engine& GetRandomEngine();
private:
   GlobalRandomOBJ() {}

private:
   static GlobalRandomOBJ* obj_;

   std::default_random_engine ran_eng_;
};

#endif //_GEN_CSV_GLOBAL_RANDOM_OBJ_H_
