//
// Created by gy gao on 10/11/22.
//

#ifndef MYOWNSTL_EXCEPTDEF_H
#define MYOWNSTL_EXCEPTDEF_H

#include <stdexcept>
#include <cassert>

namespace myownstl
{
#define myownstl_DEBUG(expr) \
  assert(expr)

#define THROW_LENGTH_ERROR_IF(expr, what) \
  if ((expr)) throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what) \
  if ((expr)) throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr, what) \
  if ((expr)) throw std::runtime_error(what)

}


#endif //MYOWNSTL_EXCEPTDEF_H
