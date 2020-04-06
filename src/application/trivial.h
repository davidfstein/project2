#pragma once
#include "./application.h"
#include "../dataframe/dfbuilder.h"

class Trivial : public Application {
 public:
  Trivial(size_t idx) : Application(idx) { }
  void run_() {
    size_t SZ = 10*10;
    double* vals = new double[SZ];
    double sum = 0;
    for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
    Key key(new String("triv"), 0);
    DataFrame* df = DFBuilder::fromArray(&key, *store, SZ, vals);
    assert(df->get_double(0,1) == 1);
    DataFrame* df2 = store->get(&key);
    for (size_t i = 0; i < SZ; ++i) sum -= df2->get_double(0,i);
    assert(sum== (double) 0);
    delete df;
  }
};