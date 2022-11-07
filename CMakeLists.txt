cmake_minimum_required(VERSION 3.20)
project(MyTinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyTinySTL main.cpp astring.h type_traits.h iterator.h construct.h allocator.h util.h memory.h algobase.h uninitialized.h exceptdef.h functional.h vector.h algo.h deque.h heap_algo.h list.h)
