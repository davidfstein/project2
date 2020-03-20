#pragma once
#include "../client/kdstore.h"
#include "../datastructures/dataframe.h"
#include "../store/key.h"
#include "../dataframe/data.h"

class DFBuilder {
    public:

        static DataFrame* fromArray(Key key, KDStore store, size_t size, int* values) {
            Schema schema("I");
            DataFrame* df = new DataFrame(schema);
            IntColumn* column = new IntColumn();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            df->add_column(column, nullptr);
            store.put(&key, df);
            return df;
        }

        static DataFrame* fromArray(Key key, KDStore store, size_t size, float* values) {
            Schema schema("F");
            DataFrame* df = new DataFrame(schema);
            FloatColumn* column = new FloatColumn();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            df->add_column(column, nullptr);
            store.put(&key, df);
            return df;
        }

        static DataFrame* fromArray(Key key, KDStore store, size_t size, bool* values) {
            Schema schema("B");
            DataFrame* df = new DataFrame(schema);
            BoolColumn* column = new BoolColumn();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            df->add_column(column, nullptr);
            store.put(&key, df);
            return df;
        }

        static DataFrame* fromArray(Key key, KDStore store, size_t size, String** values) {
            Schema schema("S");
            DataFrame* df = new DataFrame(schema);
            StringColumn* column = new StringColumn();
            for (int i = 0; i < size; i++) {
                column->push_back(values[i]);
            }
            df->add_column(column, nullptr);
            store.put(&key, df);
            return df;
        }
};