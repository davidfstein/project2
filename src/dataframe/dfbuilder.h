#pragma once
#include "../client/kdstore.h"
#include "../datastructures/dataframe.h"
#include "../store/key.h"
#include "../dataframe/data.h"
#include "../utils/helper.h"

class DFBuilder {
    public:

        static DataFrame* fromArray(Key* key, KDStore store, size_t size, int* values) {
            Schema schema("I");
            DataFrame* df = new DataFrame(schema);
            IntColumn* column = df->get_col(0)->as_int();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            store.put(key, df);
            return df;
        }

        static DataFrame* fromArray(Key* key, KDStore store, size_t size, float* values) {
            Schema schema("F");
            DataFrame* df = new DataFrame(schema);
            FloatColumn* column = df->get_col(0)->as_float();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            store.put(key, df);
            return df;
        }

        static DataFrame* fromArray(Key* key, KDStore store, size_t size, bool* values) {
            Schema schema("B");
            DataFrame* df = new DataFrame(schema);
            BoolColumn* column = df->get_col(0)->as_bool();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            store.put(key, df);
            return df;
        }

        static DataFrame* fromArray(Key* key, KDStore store, size_t size, double* values) {
            Schema* s = new Schema("D");
            DataFrame* df = new DataFrame(*s);
            DoubleColumn* column = df->get_col(0)->as_double();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            store.put(key, df);
            return df;
        }

        static DataFrame* fromArray(Key* key, KDStore store, size_t size, String** values) {
            Schema schema("S");
            DataFrame* df = new DataFrame(schema);
            StringColumn* column = df->get_col(0)->as_string();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            store.put(key, df);
            return df;
        }
};