cmake_minimum_required(VERSION 3.20)
project(MYOWNSTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(MYOWNSTL main.cpp
        astring.h
        type_traits.h
        iterator.h
        construct.h
        allocator.h
        util.h
        memory.h
        algobase.h
        uninitialized.h
        exceptdef.h
        functional.h
        vector.h
        algo.h
        deque.h
        heap_algo.h
        list.h
        basic_string.h
        hashtable.h
        numeric.h
        stack.h
        queue.h
        unordered_map.h
        unordered_set.h
        algorithm.h
        rb_tree.h
        set.h
        map.h
        set_algo.h)
