/*
 * This file contains declaration for functions used in mexgetarray.cpp.
 * 
 */
#include "mex.hpp"
#include "mexAdapter.hpp"

class MexFunction : public matlab::mex::Function {
  static int mex_count;
public:
  MexFunction()
  {
  }
  void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
};
