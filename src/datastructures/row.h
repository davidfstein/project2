//lang::CwC
// TODO: Should go in dataframe folder

#pragma once
#include "../objects/object.h"
#include "../objects/string.h"
#include "../dataframe/schema.h"
#include "../dataframe/fielder.h"
#include "../dataframe/data.h"

/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
    public:
        Schema* schema_;
        union Data** values_;
        size_t idx_in_df;
 
        /** Build a row following a schema. */
        Row(Schema& scm) {
            schema_ = new Schema(scm, false);
            values_ = new Data*[scm.width()];
        }

        ~Row() {
            for (int i = 0; i < schema_->width(); ++i) {
                delete values_[i];
            }
            delete[] values_;
            delete schema_;
        }

        void checkCorrectDataType(size_t col, char data_type) {
            char col_type = schema_->col_type(col);
            if (col_type != data_type) {
                printf("Column type does not match data type.\n");
                exit(1);
            }
        }
        
        /** Setters: set the given column with the given value. Setting a column with
            * a value of the wrong type is undefined. */
        void set(size_t col, int val) {
            checkCorrectDataType(col, 'I');
            values_[col] = new Data;
            values_[col]->i = val;
        }

        void set(size_t col, float val) {
            checkCorrectDataType(col, 'F');
            values_[col] = new Data;
            values_[col]->f = val;
        }

        void set(size_t col, bool val) {
            checkCorrectDataType(col, 'B');
            values_[col] = new Data;
            values_[col]->b = val;
        }

        void set(size_t col, double val) {
            checkCorrectDataType(col, 'D');
            values_[col] = new Data;
            values_[col]->d = val;
        }

        /** The string is external. */
        void set(size_t col, String* val) {
            checkCorrectDataType(col, 'S');
            values_[col] = new Data;
            values_[col]->s = val;
        }
         
        /** Set/get the index of this row (ie. its position in the dataframe. This is
         *  only used for informational purposes, unused otherwise */
        void set_idx(size_t idx) {
            idx_in_df = idx;
        }
        size_t get_idx() {
            return idx_in_df;
        }
        
        /** Getters: get the value at the given column. If the column is not
            * of the requested type, the result is undefined. */
        int get_int(size_t col) {
            checkCorrectDataType(col, 'I');
            return values_[col]->i;
        }

        bool get_bool(size_t col) {
            checkCorrectDataType(col, 'B');
            return values_[col]->b;
        }
        float get_float(size_t col) {
            checkCorrectDataType(col, 'F');
            return values_[col]->f;
        }
        double get_double(size_t col) {
            checkCorrectDataType(col, 'D');
            return values_[col]->d;
        }
        String* get_string(size_t col) {
            checkCorrectDataType(col, 'S');
            return values_[col]->s;
        }
        
        /** Number of fields in the row. */
        size_t width() {
            return schema_->width();
        }
         
        /** Type of the field at the given position. An idx >= width is  undefined. */
        char col_type(size_t idx) {
            return schema_->col_type(idx);
        }
        
        /** Given a Fielder, visit every field of this row. The first argument is
            * index of the row in the dataframe.
            * Calling this method before the row's fields have been set is undefined. */
        void visit(size_t idx, Fielder& f) {
            // TODO: go back to Fundies 2
            for (int index; index < width(); index++) {
                char typ = schema_->col_type(idx);
                if (typ == 'I') {
                    f.accept(values_[index]->i);
                }
                else if (typ == 'B') {
                    f.accept(values_[index]->b);
                }
                else if (typ == 'F') {
                    f.accept(values_[index]->f);
                }
                else if (typ == 'D') {
                    f.accept(values_[index]->d);
                }
                else if (typ == 'S') {
                    f.accept(values_[index]->s);
                }
            }
        }
};
