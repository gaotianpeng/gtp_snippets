cmake_minimum_required(VERSION 3.17)
project(gen_csv)

set(CMAKE_CXX_STANDARD 11)

add_executable(gen_csv main.cpp generate_data.cpp generate_data.h csv_impl.cpp csv_impl.h data_type.h gen_csv.cpp gen_csv.h global_random_obj.cpp global_random_obj.h)