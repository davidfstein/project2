//lang::CwC
// TODO: Should go in dataframe folder

#pragma once
#include <stdio.h>
#include <thread>
#include "../dataframe/rower.h"
#include "../objects/object.h"
#include "../objects/string.h"
#include "../dataframe/schema.h"
#include "column.h"
#include "../dataframe/fielder.h"
#include "row.h"
#include "../dataframe/rower.h"

/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {
    public:
        Schema* schema;
        ObjectArrayArray* columns;
 
        /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
        DataFrame(DataFrame& df) : DataFrame(df.get_schema()){
            for (int i = 0; i < df.get_schema().length(); i++) {
                Row* r = new Row(*schema);
                df.fill_row(i, *r);
                add_row(*r);
                delete r;
            }
        }
        
        /** Create a data frame from a schema and columns. All columns are created
            * empty. */
        DataFrame(Schema& schema) {
            this->schema = &schema;
            columns = new ObjectArrayArray();
            for (int i = 0; i < schema.width(); ++i) {
                columns->pushBack(make_column(schema.col_type(i)));
            }
        }

        // Not sure if we should own the schema or not.
        ~DataFrame() {
            delete columns;
            delete schema;
        }
        
        /** Returns the dataframe's schema. Modifying the schema after a dataframe
            * has been created in undefined. */
        Schema& get_schema() {
            return *schema;
        }

        Column* make_column(char type) {
            if (type == 'I') {
                return new IntColumn();
            }
            else if (type == 'B') {
                return new BoolColumn();
            }
            else if (type == 'F') {
                return new FloatColumn();
            }
            else if (type == 'D') {
                return new DoubleColumn();
            }
            else if (type == 'S') {
                return new StringColumn();
            }
            else {
                return nullptr;
            }
        }
        
        /** Adds a column this dataframe, updates the schema, the new column
            * is external, and appears as the last column of the dataframe, the
            * name is optional and external. A nullptr colum is undefined. */
        void add_column(Column* col, String* name) {
            char t[] = {col->get_type()};
            if (name == nullptr) {
                schema->add_column(t, nullptr);
            } else {
                schema->add_column(t, name->clone());
            }
            columns->pushBack(col);
        }
        
        void checkTypeAtPosition(size_t col, char type) {
            char true_type = schema->col_type(col);
            if (true_type != type) {
                printf("Incorrect column type in data query.\n");
                exit(1);
            }
        }

        /** Return the value at the given column and row. Accessing rows or
         *  columns out of bounds, or request the wrong type is undefined.*/
        int get_int(size_t col, size_t row) {
            checkTypeAtPosition(col, 'I');
            return this->get_col(col)->as_int()->get(row);
        }

        bool get_bool(size_t col, size_t row) {
            checkTypeAtPosition(col, 'B');
            return this->get_col(col)->as_bool()->get(row);
        }

        float get_float(size_t col, size_t row) {
            checkTypeAtPosition(col, 'F');
            return this->get_col(col)->as_float()->get(row);
        }

        String* get_string(size_t col, size_t row) {
            checkTypeAtPosition(col, 'S');
            return this->get_col(col)->as_string()->get(row);
        }

        double get_double(size_t col, size_t row) {
            checkTypeAtPosition(col, 'D');
            return this->get_col(col)->as_double()->get(row);
        }
        
        Column* get_col(int i) {
            return dynamic_cast<Column*>(columns->get(i));
        }

        /** Return the offset of the given column name or -1 if no such col. */
        int get_col(String& col) {
            return schema->col_idx(col.c_str());
        }
        
        /** Return the offset of the given row name or -1 if no such row. */
        int get_row(String& col) {
            return schema->row_idx(col.c_str());
        }
        
        /** Set the value at the given column and row to the given value.
            * If the column is not  of the right type or the indices are out of
            * bound, the result is undefined. */
        void set(size_t col, size_t row, int val) {
            checkTypeAtPosition(col, 'I');
            this->get_col(col)->as_int()->set(row, val);
        }

        void set(size_t col, size_t row, bool val) {
            checkTypeAtPosition(col, 'B');
            this->get_col(col)->as_bool()->set(row, val);
        }

        void set(size_t col, size_t row, float val) {
            checkTypeAtPosition(col, 'F');
            this->get_col(col)->as_float()->set(row, val);
        }

        void set(size_t col, size_t row, String* val) {
            checkTypeAtPosition(col, 'S');
            this->get_col(col)->as_string()->set(row, val);
        }

        void set(size_t col, size_t row, double val) {
            checkTypeAtPosition(col, 'D');
            this->get_col(col)->as_double()->set(row, val);
        }
        
        /** Set the fields of the given row object with values from the columns at
            * the given offset.  If the row is not form the same schema as the
            * dataframe, results are undefined.
            */
        void fill_row(size_t idx, Row& row) {
            row.idx_in_df = idx;
            for (int i = 0; i < ncols(); i ++) {
                char col_type = schema->col_type(i);
                if (col_type == 'I') {
                    int val = get_int(i, idx);
                    row.set(i, val);
                } else if (col_type == 'B') {
                    bool val = get_bool(i, idx);
                    row.set(i, val);
                } else if (col_type == 'F') {
                    float val = get_float(i, idx);
                    row.set(i, val);
                } else if (col_type == 'S') {
                    String* val = get_string(i, idx);
                    row.set(i, val);
                } else if (col_type == 'D') {
                    double val = get_double(i, idx);
                    row.set(i, val);
                } else {
                    printf("Column type not recognized.\n");
                    exit(1);
                }
            }
        }
        
        /** Add a row at the end of this dataframe. The row is expected to have
         *  the right schema and be filled with values, otherwise undedined.  */
        void add_row(Row& row) {
            for (int i = 0; i < ncols(); i++) {
                char col_type = schema->col_type(i);
                if (col_type == 'I') {
                    int val = row.get_int(i);
                    this->get_col(i)->as_int()->push_back(val);
                } else if (col_type == 'B') {
                    bool val = row.get_bool(i);
                    this->get_col(i)->as_bool()->push_back(val);
                } else if (col_type == 'F') {
                    float val = row.get_float(i);
                    this->get_col(i)->as_float()->push_back(val);
                } else if (col_type == 'S') {
                    String* val = row.get_string(i);
                    this->get_col(i)->as_string()->push_back(val);
                } else if (col_type == 'D') {
                    double val = row.get_double(i);
                    this->get_col(i)->as_double()->push_back(val);
                } else {
                    printf("Column type not recognized.\n");
                    exit(1);
                }
            }
            String* rowName = new String("");
            schema->add_row(rowName);
            delete rowName;
        }
        
        /** The number of rows in the dataframe. */
        size_t nrows() {
            if (schema == nullptr) {
                return 0;
            } 
            return schema->length();
        }
        
        /** The number of columns in the dataframe.*/
        size_t ncols() {
            if (schema == nullptr) {
                return 0;
            } 
            return schema->width();
        }
        
        /** Visit rows in order */
        void map(Rower& r) {
            Row* row = new Row(*schema);
            for (int i = 0; i < nrows(); i++) {
                fill_row(i, *row);
                r.accept(*row);
            }
        }

        /** Visit rows in order at given bounds */
        void submap(Rower* r, size_t start, size_t end) {
            Row* row = new Row(*schema);
            for (int i = start; i < end; i++) {
                this->fill_row(i, *row);
                r->accept(*row);
            }
        }

        /** This method clones the Rower and executes the map in parallel. Join is
         *   * used at the end to merge the results. */
        void pmap(Rower& r) {
            Rower* s = r.clone();
            Rower* t = r.clone();
            std::thread t1(&DataFrame::submap, this, &r, (size_t) 0, nrows() / 3);
            std::thread t2(&DataFrame::submap, this, s, nrows() / 3, 2 * nrows() / 3);
            std::thread t3(&DataFrame::submap, this, t, 2 * nrows() / 3, nrows());
            t1.join();
            t2.join();
            t3.join();
            r.join_delete(s);
            r.join_delete(t);
        }
        
        /** Create a new dataframe, constructed from rows for which the given Rower
            * returned true from its accept method. */
        DataFrame* filter(Rower& r) {
            Schema* s = new Schema(*schema, false);
            DataFrame* filtered = new DataFrame(*s);
            for (int i = 0; i < nrows(); i++) {
                Row* row = new Row(*schema);
                fill_row(i, *row);
                bool keep = r.accept(*row);
                if (keep) {
                    String* name = schema->row_name(i);
                    filtered->add_row(*row);
                    s->set_row_name(i, name);
                }
                delete row;
            }
            delete s;
            return filtered;
        }
        
        /** Print the dataframe in SoR format to standard output. */
        void print() {
            for (int i = 0; i < nrows(); i++) {
                for (int j = 0; j < ncols(); j++) {
                    char col_type = schema->col_type(j);
                    if (col_type == 'I') {
                        int val = get_int(j, i);
                        printf("%d", val);
                    } else if (col_type == 'B') {
                        bool val = get_bool(j, i);
                        printf("%d", val);
                    } else if (col_type == 'F') {
                        float val = get_float(j, i);
                        printf("%f", val);
                    } else if (col_type == 'S') {
                        String* val = get_string(j, i);
                        printf("%s", val->c_str());
                    } else if (col_type == 'D') {
                        double val = get_double(j, i);
                        printf("%f", val);
                    } else {
                        printf("Column type not recognized.\n");
                        exit(1);
                    }
                }
                printf("\n");
            }
        }
};
