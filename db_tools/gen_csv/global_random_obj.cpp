//
// Created by gtpsy on 2020/10/27.
//

#include "global_random_obj.h"

GlobalRandomOBJ* GlobalRandomOBJ::obj_ = nullptr;

GlobalRandomOBJ* GlobalRandomOBJ::GetInstance() {
   if (obj_) {
      return obj_;
   }

   obj_ = new GlobalRandomOBJ;
}

std::default_random_engine& GlobalRandomOBJ::GetRandomEngine() {
   return ran_eng_;
}