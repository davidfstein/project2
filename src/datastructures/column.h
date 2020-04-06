//lang::CwC

#pragma once
#include "../objects/object.h"
#include "../objects/string.h"
#include "array.h"
#include "../utils/dataframe_utils.h"
#include "../dataframe/types.h"

class IntColumn;
class StringColumn;
class BoolColumn;
class FloatColumn;
class DoubleColumn;

/**************************************************************************
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class Column : public Object {
    public:
        char type;

        virtual ~Column() {}

        /** Type converters: Return same column under its actual type, or
         *  nullptr if of the wrong type.  */
        //TODO: figure out what the point of these is
        virtual IntColumn* as_int() {}
        virtual BoolColumn*  as_bool() {}
        virtual FloatColumn* as_float() {}
        virtual StringColumn* as_string() {}
        virtual DoubleColumn* as_double() {}

        /** Type appropriate push_back methods. Calling the wrong method is
         * undefined behavior. **/
        virtual void push_back(int val) {}
        virtual void push_back(bool val) {}
        virtual void push_back(float val) {}
        virtual void push_back(String* val) {}
        virtual void push_back(double val) {}

        /** Returns the number of elements in the column. */
        virtual size_t size() {}

        /** Return the type of this column as a char: 'S', 'B', 'I', 'D' and 'F'. */
        char get_type() {
            return type;
        }

        virtual void appendMissing() {
            push_back(nullptr);
        }
};

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 */
class StringColumn : public Column {
    public:
        StringArrayArray* values_;

        StringColumn() {
            type = 'S';
            values_ = new StringArrayArray();
        }

        ~StringColumn() {
            delete values_;
        }

        StringColumn(int n, ...) {
            va_list ap;
            va_start(ap, n);
            values_ = dynamic_cast<StringArrayArray*>(Utils::buildArray(STRING, n, ap));
        }

        StringColumn* as_string() {
            return this;
        }

        /** Returns the string at idx; undefined on invalid idx.*/
        String* get(size_t idx) {
            return dynamic_cast<String*>(values_->get(idx));
        }

        /** Out of bound idx is undefined. */
        void set(size_t idx, String* val) {
            values_->set(idx, val->clone());
        }

        void push_back(String* val) {
            if (val == nullptr) {
                values_->pushBack(val);
            }
            else {
                values_->pushBack(val->clone());
            }
        }

        size_t size() {
            return values_->length();
        }
};

/*************************************************************************
 * FloatColumn::
 * Holds float values.
 */
class FloatColumn : public Column {
    public:
        FloatArrayArray* values_;

        FloatColumn() {
            type = 'F';
            values_ = new FloatArrayArray();
        }

        FloatColumn(int n, ...) {
            va_list ap;
            va_start(ap, n);
            values_ = dynamic_cast<FloatArrayArray*>(Utils::buildArray(FLOAT, n, ap));
        }

        ~FloatColumn() {
            delete values_;
        }

        float get(size_t idx) {
            return values_->get(idx);
        }

        FloatColumn* as_float() {
            return this;
        }

        /** Set value at idx. An out of bound idx is undefined.  */
        void set(size_t idx, float val) {
            values_->set(idx, val);
        }

        void push_back(float val) {
            values_->pushBack(val);
        }

        size_t size() {
            return values_->length();
        }
};

/*************************************************************************
 * DoubleColumn::
 * Holds double values.
 */
class DoubleColumn : public Column {
    public:
        DoubleArrayArray* values_;

        DoubleColumn() {
            type = 'D';
            values_ = new DoubleArrayArray();
        }

        DoubleColumn(int n, ...) {
            va_list ap;
            va_start(ap, n);
            values_ = dynamic_cast<DoubleArrayArray*>(Utils::buildArray(DOUBLE, n, ap));
        }

        ~DoubleColumn() {
            delete values_;
        }

        double get(size_t idx) {
            return values_->get(idx);
        }

        DoubleColumn* as_double() {
            return this;
        }

        /** Set value at idx. An out of bound idx is undefined.  */
        void set(size_t idx, double val) {
            values_->set(idx, val);
        }

        void push_back(double val) {
            values_->pushBack(val);
        }

        size_t size() {
            return values_->length();
        }
};

/*************************************************************************
 * IntColumn::
 * Holds int values.
 */
class IntColumn : public Column {
    public:
        IntArrayArray* values_;

        IntColumn() {
            type = 'I';
            values_ = new IntArrayArray();
        }

        IntColumn(int n, ...) {
            va_list ap;
            va_start(ap, n);
            values_ = dynamic_cast<IntArrayArray*>(Utils::buildArray(INT, n, ap));
        }

        ~IntColumn() {
            delete values_;
        }

        int get(size_t idx) {
            return values_->get(idx);
        }

        IntColumn* as_int() {
            return this;
        }

        /** Set value at idx. An out of bound idx is undefined.  */
        void set(size_t idx, int val) {
            values_->set(idx, val);
        }

        void push_back(int val) {
            values_->pushBack(val);
        }

        size_t size() {
            return values_->length();
        }
};

/*************************************************************************
 * BoolColumn::
 * Holds bool values.
 */
class BoolColumn : public Column {

    public:
        BoolArrayArray* values_;

        BoolColumn() {
            type = 'B';
            values_ = new BoolArrayArray();
        }

        BoolColumn(int n, ...) {
            va_list ap;
            va_start(ap, n);
            values_ = dynamic_cast<BoolArrayArray*>(Utils::buildArray(BOOL, n, ap));
        }

        ~BoolColumn() {
            delete values_;
        }

        bool get(size_t idx) {
            return values_->get(idx);
        }

        BoolColumn* as_bool() {
            return this;
        }

        /** Set value at idx. An out of bound idx is undefined.  */
        void set(size_t idx, bool val) {
            values_->set(idx, val);
        }

        void push_back(bool val) {
            values_->pushBack(val);
        }

        size_t size() {
            return values_->length();
        }
};

class ColumnArray: public Array {
  public:
    Column** values_;

    // Creates an array of size length, where each object is data_length long
    ColumnArray(size_t length) : Array(length) {
      values_ = new Column*[length]();
    }

    ~ColumnArray() {
      for (int i = 0; i < this->length(); ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the string at index
    Column* get(size_t index) {
      checkOutOfBounds(index);
      return values_[index];
    }

    //sets the string at index to be o, returns former object
    Column* set(size_t index, Column* s) {
      checkOutOfBounds(index);
      Column* former = this->get(index);
      values_[index] = s;
      return former;
    } 

    //Replaces string at index with nullptr, returns string
    Column* remove(size_t index) {
      checkOutOfBounds(index);
      Column* former = get(index);
      this->set(index, nullptr);
      for (int i = index+1; i < length(); i++) {
        this->set(i-1, this->get(i));
      }
      size_ = length() - 1;
      return former;
    } 

    // replaces all objects with nullpttr
    void clear() {
      for (int i = length()-1; i >= 0; i--) {
        remove(i);
      }
    } 

    //add o to end of array
    void pushBack(Column* s) {
      int old_length = length();
      if (this->length() < this->capacity_ && !this->get(this->length())) {
        this->set(this->length(), s);
      } else {
        Column** new_data = new Column*[capacity_ * 2]();
        for (int i = 0; i < length(); i++) {
            new_data[i] = this->get(i);
        }
        delete[] values_;
        values_ = new_data;
        capacity_ = capacity_ * 2;
        Column* old = this->set(old_length, s);
        delete old;
      }
      size_ = size_ + 1;
      return;
    } 
};