//lang::CwC
/*
* Authors: David Stein (stein.davi@husky.neu.edu), Adin Vashi 
* CS4500 A2 (Array definition)
*/

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "../objects/string.h"
#include "../objects/object.h"

//CwC implementation of an Array like object. The class is istantiated with an initial length
//but is also resizeable. Provides efficient storage and lookup.
class Array: public Object {
  public:
    size_t capacity_;
    size_t size_;

    Array() {};

    // Creates an array of size length, where each object is data_length long
    Array(size_t length) {
      size_ = 0;
      capacity_ = length;
    }

    virtual ~Array() {}
    
    //checks if there are any items in the array
    bool empty() {
      return size_ == 0;
    } 
    
    void checkOutOfBounds(size_t index) {
        if (index > this->length() || index < 0) {
          printf("Index Out of Bounds\n");
          exit(1);
        }
      }
    
    //returns the length of the array
    size_t length() {
      return size_;
    }
};


//CwC implementation of an Array like object. The class is istantiated with an initial length
//but is also resizeable. Provides efficient storage and lookup of objects.
class objArray: public Array {
  public:

    Object** values_;

    // Creates an array of size length, where each object is data_length long
    objArray(size_t length) : Array(length) {
      values_ = new Object*[length]();
    }

    ~objArray() {
      for (int i = 0; i < this->length(); ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the hash of the array
    size_t hash() {
      size_t average = 0;
      for (int i = 1; i <= length(); i++) {
        average = (size_t) (average + values_[i-1]->hash()) / i;
      }
      return average;
    }

    bool equals(Array* other) {
      try {
        objArray* arr = dynamic_cast<objArray*>(other);
        if (arr->length() != this->length()) {
          return false;
        }
        for (int i = 0; i < this->length(); i++) {
          if (!values_[i]->equals(arr->values_[i])) {
            return false;
          }
        }
        return true;
      } catch (char* exception) {
        return false;
      }
    }

    //returns the object at index
    Object* get(size_t index) {
      checkOutOfBounds(index);
      return values_[index];
    }

    //sets the object at index to be o, returns former object
    Object* set(size_t index, Object* o) {
      checkOutOfBounds(index);
      Object* former = get(index);
      values_[index] = o;
      return former;
    } 

    //Replaces object at index with nullptr, returns objec
    Object* remove(size_t index) {
      checkOutOfBounds(index);
      Object* former = get(index);
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
    void* pushBack(Object* o) {
      int old_length = length();
      if (this->length() < capacity_ && !this->get(this->length())) {
        this->set(this->length(), o);
      } else {
        objArray* new_data = new objArray(capacity_ * 2);
        for (int i = 0; i < length(); i++) {
            Object* item = get(i); 
            new_data->pushBack(item);
        }
        values_ = new_data->values_;
        size_ = new_data->size_;
        capacity_ = new_data->capacity_;
        this->set(old_length, o);
      }
      size_ = size_ + 1;
    } 
};


//CwC implementation of an Array like object. The class is istantiated with an initial length
//but is also resizeable. Provides efficient storage and lookup of ints.
class intArray: public Array {
public:

  int* values_;

  // Creates an array of size length, where each object is data_length long
  intArray(size_t length) : Array(length) {
    values_ = new int[length]();
  }

  ~intArray() {
    delete[] values_;
  };

  //returns the hash of the array
  size_t hash() {
    size_t average = 0;
    for (int i = 1; i <= length(); i++) {
      average = (size_t) (average + values_[i-1]) / i;
    }
    return average;
  }

  bool equals(Array* other) {
    try {
      intArray* arr = dynamic_cast<intArray*>(other);
      if (arr->length() != this->length()) {
        return false;
      }
      for (int i = 0; i < this->length(); i++) {
        if (!(values_[i] == arr->values_[i])) {
          return false;
        }
      }
      return true;
    } catch (char* exception) {
      return false;
    }
  }

  //returns the object at index
  int get(size_t index) {
    checkOutOfBounds(index);
    return values_[index];
  }

  //sets the object at index to be o, returns former object
  int set(size_t index, int o) {
    checkOutOfBounds(index);
    int former = get(index);
    values_[index] = o;
    return former;
  } 

  //Replaces object at index with nullptr, returns objec
  int remove(size_t index) {
    checkOutOfBounds(index);
    int former = get(index);
    this->set(index, 0);
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
  void pushBack(int o) {
    int old_length = length();
    if (this->length() < capacity_ && !this->get(this->length())) {
      this->set(this->length(), o);
    } else {
      intArray* new_data = new intArray(capacity_ * 2);
      for (int i = 0; i < length(); i++) {
          int item = get(i); 
          new_data->set(i, item);
      }
      values_ = new_data->values_;
      this->set(old_length, o);
    }
    size_ = size_ + 1;
  } 
};

class DoubleArray: public Array{
public:
  double* values_;

  DoubleArray(size_t size) : Array(size) {
    values_ = new double[size]();
  }

  ~DoubleArray() {
    delete[] values_;
  };

  bool contains(double f) {
    for (int i = 0; i < length(); i++) {
      if (get(i) == f) {
        return true;
      }
    }
    return false;
  }

  //returns the hash of the array
  size_t hash() {
    size_t average = 0;
    for (int i = 1; i <= length(); i++) {
      average = (size_t) (average + values_[i-1]) / i;
    }
    return average;
  }

  bool equals(Array* other) {
    try {
      DoubleArray* arr = dynamic_cast<DoubleArray*>(other);
      if (arr->length() != this->length()) {
        return false;
      }
      for (int i = 0; i < this->length(); i++) {
        if (!(values_[i] == arr->values_[i])) {
          return false;
        }
      }
      return true;
    } catch (char* exception) {
      return false;
    }
  }

  //returns the object at index
  double get(size_t index) {
    checkOutOfBounds(index);
    return values_[index];
  }

  //sets the object at index to be o, returns former object
  double set(size_t index, double o) {
    checkOutOfBounds(index);
    double former = get(index);
    values_[index] = o;
    return former;
  } 

  //Replaces object at index with nullptr, returns objec
  double remove(size_t index) {
    checkOutOfBounds(index);
    double former = get(index);
    this->set(index, 0);
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
  void pushBack(double o) {
    int old_size = length();
    if (this->length() < capacity_ && !this->get(this->length())) {
      this->set(this->length(), o);
    } else {
      DoubleArray* new_data = new DoubleArray(capacity_ * 2);
      for (int i = 0; i < length(); i++) {
          double item = get(i); 
          new_data->set(i, item);
      }
      values_ = new_data->values_;
      this->set(old_size, o);
    }
    size_ = size_ + 1;
  } 
};


//CwC implementation of an Array like object. The class is istantiated with an initial length
//but is also resizeable. Provides efficient storage and lookup of strings.
class strArray: public Array{
  public:
    String** values_;

    // Creates an array of size length, where each object is data_length long
    strArray(size_t length) : Array(length) {
      values_ = new String*[length]();
    }

    ~strArray() {
      for (int i = 0; i < this->length(); ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    bool contains(String* s) {
      for (int i = 0; i < length(); i++) {
        if (get(i)->equals(s)) {
          return true;
        }
      }
      return false;
    }

    //returns the hash of the array
    size_t hash() {
      size_t average = 0;
      for (int i = 1; i <= length(); i++) {
        average = (size_t) (average + values_[i-1]->hash()) / i;
      }
      return average;
    }

    bool equals(Array* other) {
      try {
        strArray* arr = dynamic_cast<strArray*>(other);
        if (arr->length() != this->length()) {
          return false;
        }
        for (int i = 0; i < this->length(); i++) {
          if (!values_[i]->equals(arr->values_[i])) {
            return false;
          }
        }
        return true;
      } catch (char* exception) {
        return false;
      }
    }

    //returns the string at index
    String* get(size_t index) {
      checkOutOfBounds(index);
      return values_[index];
    }

    //sets the string at index to be o, returns former object
    String* set(size_t index, String* s) {
      checkOutOfBounds(index);
      String* former = this->get(index);
      values_[index] = s;
      return former;
    } 

    //Replaces string at index with nullptr, returns string
    String* remove(size_t index) {
      checkOutOfBounds(index);
      String* former = get(index);
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
    void pushBack(String* s) {
      int old_length = length();
      if (this->length() < this->capacity_ && !this->get(this->length())) {
        this->set(this->length(), s);
      } else {
        String** new_data = new String*[capacity_ * 2]();
        for (int i = 0; i < length(); i++) {
            new_data[i] = this->get(i);
        }
        delete[] values_;
        values_ = new_data;
        capacity_ = capacity_ * 2;
        String* old = this->set(old_length, s);
        delete old;
      }
      size_ = size_ + 1;
      return;
    } 
};


//CwC implementation of an Array like object. The class is istantiated with an initial length
//but is also resizeable. Provides efficient storage and lookup of booleans.
class boolArray: public Array{
public:
  bool* values_;

  boolArray(size_t length) : Array(length) {
    values_ = new bool[length]();
  }

  ~boolArray() {
    delete[] values_;
  };

  //returns the hash of the array
  size_t hash() {
    // https://stackoverflow.com/questions/17398578/hash-algorithm-for-variable-size-boolean-array
    int h = (1 << this->length());
    for (int i = 0; i <this->length(); i++)
    {
        h = h | (values_[i] << (this->length() - i - 1));
    }
    return h;
  }

  bool equals(Array* other) {
    try {
      boolArray* arr = dynamic_cast<boolArray*>(other);
      if (arr->length() != this->length()) {
        return false;
      }
      for (int i = 0; i < this->length(); i++) {
        if (!(values_[i] == arr->values_[i])) {
          return false;
        }
      }
      return true;
    } catch (char* exception) {
      return false;
    }
  }

  //returns the object at index
  bool get(size_t index) {
    checkOutOfBounds(index);
    return values_[index];
  }

  //sets the object at index to be o, returns former object
  bool set(size_t index, bool o) {
    checkOutOfBounds(index);
    bool former = get(index);
    values_[index] = o;
    return former;
  } 

  //Replaces object at index with nullptr, returns objec
  bool remove(size_t index) {
    checkOutOfBounds(index);
    bool former = get(index);
    this->set(index, 0);
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
  void pushBack(bool o) {
    int old_length = length();
    if (this->length() < capacity_ && !this->get(this->length())) {
      this->set(this->length(), o);
    } else {
      boolArray* new_data = new boolArray(capacity_ * 2);
      for (int i = 0; i < length(); i++) {
          bool item = get(i); 
          new_data->set(i, item);
      }
      values_ = new_data->values_;
      this->set(old_length, o);
    }
    size_ = size_ + 1;
  } 
};


//CwC implementation of an Array like object. The class is istantiated with an initial length
//but is also resizeable. Provides efficient storage and lookup of floats.
class floatArray: public Array{
public:
  float* values_;

  floatArray(size_t length) : Array(length) {
    values_ = new float[length]();
  }

  ~floatArray() {
    delete[] values_;
  };

  //returns the hash of the array
  size_t hash() {
    size_t average = 0;
    for (int i = 1; i <= length(); i++) {
      average = (size_t) (average + values_[i-1]) / i;
    }
    return average;
  }

  bool equals(Array* other) {
    try {
      floatArray* arr = dynamic_cast<floatArray*>(other);
      if (arr->length() != this->length()) {
        return false;
      }
      for (int i = 0; i < this->length(); i++) {
        if (!(values_[i] == arr->values_[i])) {
          return false;
        }
      }
      return true;
    } catch (char* exception) {
      return false;
    }
  }

  //returns the object at index
  float get(size_t index) {
    checkOutOfBounds(index);
    return values_[index];
  }

  //sets the object at index to be o, returns former object
  float set(size_t index, float o) {
    checkOutOfBounds(index);
    float former = get(index);
    values_[index] = o;
    return former;
  } 

  //Replaces object at index with nullptr, returns objec
  float remove(size_t index) {
    checkOutOfBounds(index);
    float former = get(index);
    this->set(index, 0);
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
  void pushBack(float o) {
    int old_length = length();
    if (this->length() < capacity_ && !this->get(this->length())) {
      this->set(this->length(), o);
    } else {
      floatArray* new_data = new floatArray(capacity_ * 2);
      for (int i = 0; i < length(); i++) {
          float item = get(i); 
          new_data->set(i, item);
      }
      values_ = new_data->values_;
      this->set(old_length, o);
    }
    size_ = size_ + 1;
  } 
};

class IntArrayArray: public Array {
  public: 
    intArray** values_;

    // Creates an array of size length, where each object is data_length long
    IntArrayArray(): Array(100) {
      values_ = new intArray*[1];
      values_[0] = new intArray(100);
    }

    ~IntArrayArray() {
      size_t num_array_pointers = this->length() / 100 > 0 ? this->length() / 100 : 1;
      for (int i = 0; i < num_array_pointers; ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the object at index
    int get(size_t index) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      return values_[array_idx]->get(value_idx);
    }

    //sets the object at index to be o, returns former object
    void set(size_t index, int element) {
      checkOutOfBounds(index);
      //int* former = get(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      if (value_idx < values_[array_idx]->length()) {
        values_[array_idx]->set(value_idx, element);
      } else {
        values_[array_idx]->pushBack(element);
      }
      // return former;
    } 

    int pushBack(int element) {
      if (this->length() < capacity_) {
        this->set(this->length(), element);
      } else {
          int subarrays_count = capacity_ / 100;
          intArray** new_data = new intArray*[subarrays_count * 2];
          for (int i = 0; i < subarrays_count; i++) {
              new_data[i] = values_[i];
          }
          for (int i = subarrays_count; i < subarrays_count * 2; i++) {
              new_data[i] = new intArray(100);
          }
          values_ = new_data;
          capacity_ = capacity_ * 2;
          set(size_, element);
      }
      size_ += 1;
    }
};

class FloatArrayArray: public Array {
  public: 
    floatArray** values_;

    // Creates an array of size length, where each object is data_length long
    FloatArrayArray(): Array(100) {
      values_ = new floatArray*[1];
      values_[0] = new floatArray(100);
    }

    ~FloatArrayArray() {
      size_t num_array_pointers = this->length() / 100 > 0 ? this->length() / 100 : 1;
      for (int i = 0; i < num_array_pointers; ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the object at index
    float get(size_t index) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      return values_[array_idx]->get(value_idx);
    }

    //sets the object at index to be o, returns former object
    void set(size_t index, float element) {
      checkOutOfBounds(index);
      //float* former = get(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      if (value_idx < values_[array_idx]->length()) {
        values_[array_idx]->set(value_idx, element);
      } else {
        values_[array_idx]->pushBack(element);
      }
      // return former;
    } 

    float pushBack(float element) {
      if (this->length() < capacity_) {
        this->set(this->length(), element);
      } else {
          int subarrays_count = capacity_ / 100;
          floatArray** new_data = new floatArray*[subarrays_count * 2];
          for (int i = 0; i < subarrays_count; i++) {
              new_data[i] = values_[i];
          }
          for (int i = subarrays_count; i < subarrays_count * 2; i++) {
              new_data[i] = new floatArray(100);
          }
          values_ = new_data;
          capacity_ = capacity_ * 2;
          set(size_, element);
      }
      size_ += 1;
    }
};

class DoubleArrayArray: public Array {
  public: 
    DoubleArray** values_;

    // Creates an array of size length, where each object is data_length long
    DoubleArrayArray(): Array(100) {
      values_ = new DoubleArray*[1];
      values_[0] = new DoubleArray(100);
    }

    ~DoubleArrayArray() {
      size_t num_array_pointers = this->length() / 100 > 0 ? this->length() / 100 : 1;
      for (int i = 0; i < num_array_pointers; ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the object at index
    double get(size_t index) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      return values_[array_idx]->get(value_idx);
    }

    //sets the object at index to be o, returns former object
    void set(size_t index, double element) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      if (value_idx < values_[array_idx]->length()) {
        values_[array_idx]->set(value_idx, element);
      } else {
        values_[array_idx]->pushBack(element);
      }
    } 

    double pushBack(double element) {
      if (this->length() < capacity_) {
        this->set(this->length(), element);
      } else {
          int subarrays_count = capacity_ / 100;
          DoubleArray** new_data = new DoubleArray*[subarrays_count * 2];
          for (int i = 0; i < subarrays_count; i++) {
              new_data[i] = values_[i];
          }
          for (int i = subarrays_count; i < subarrays_count * 2; i++) {
              new_data[i] = new DoubleArray(100);
          }
          values_ = new_data;
          capacity_ = capacity_ * 2;
          set(size_, element);
      }
      size_ += 1;
    }
};

class BoolArrayArray: public Array {
  public: 
    boolArray** values_;


    // Creates an array of size length, where each object is data_length long
    BoolArrayArray(): Array(100) {
      values_ = new boolArray*[1];
      values_[0] = new boolArray(100);
    }

    ~BoolArrayArray() {
      size_t num_array_pointers = this->length() / 100 > 0 ? this->length() / 100 : 1;
      for (int i = 0; i < num_array_pointers; ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the object at index
    bool get(size_t index) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      return values_[array_idx]->get(value_idx);
    }

    //sets the object at index to be o, returns former object
    void set(size_t index, bool element) {
      checkOutOfBounds(index);
      //bool* former = get(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      if (value_idx < values_[array_idx]->length()) {
        values_[array_idx]->set(value_idx, element);
      } else {
        values_[array_idx]->pushBack(element);
      }
      // return former;
    } 

    bool pushBack(bool element) {
      if (this->length() < capacity_) {
        this->set(this->length(), element);
      } else {
          int subarrays_count = capacity_ / 100;
          boolArray** new_data = new boolArray*[subarrays_count * 2];
          for (int i = 0; i < subarrays_count; i++) {
              new_data[i] = values_[i];
          }
          for (int i = subarrays_count; i < subarrays_count * 2; i++) {
              new_data[i] = new boolArray(100);
          }
          values_ = new_data;
          capacity_ = capacity_ * 2;
          set(size_, element);
      }
      size_ += 1;
    }
};

class StringArrayArray: public Array {
  public: 
    strArray** values_;

    // Creates an array of size length, where each object is data_length long
    StringArrayArray(): Array(100) {
      values_ = new strArray*[1]();
      values_[0] = new strArray(100);
    }

    ~StringArrayArray() {
      size_t num_array_pointers = this->length() / 100 > 0 ? this->length() / 100 : 1;
      for (int i = 0; i < num_array_pointers; ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    bool contains(String* s) {
      int array_idx = length() / 100;
      for (int i = 0; i < array_idx; i++) {
        if (values_[i]->contains(s)) {
          return true;
        }
      }
      return false;
    }

    //returns the object at index
    String* get(size_t index) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      return values_[array_idx]->get(value_idx);
    }

    //sets the object at index to be o, returns former object
    void set(size_t index, String* element) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      if (value_idx < values_[array_idx]->length()) {
        values_[array_idx]->set(value_idx, element);
      } else {
        values_[array_idx]->pushBack(element);
      }
      return;
    } 

    void pushBack(String* element) {
      if (this->length() < this->capacity_) {
        this->set(this->length(), element);
      } else {
          int subarrays_count = this->capacity_ / 100;
          strArray** new_data = new strArray*[subarrays_count * 2]();
          for (int i = 0; i < subarrays_count; ++i) {
            new_data[i] = values_[i];
          }
          for (int i = subarrays_count; i < subarrays_count * 2; ++i) {
            new_data[i] = new strArray(1);
          }
          
          delete[] values_;
          this->values_ = new_data;
          this->capacity_ = this->capacity_ * 2;
          set(this->size_, element);
      }
      size_ += 1;
      return;
    }

    StringArrayArray* copy() {
      StringArrayArray* s = new StringArrayArray();
      for (int i = 0; i < length(); ++i) {
        s->pushBack(get(i)->clone());
      }
      return s;
    }
};

class ObjectArrayArray: public Array {
  public: 
    objArray** values_;

    // Creates an array of size length, where each object is data_length long
    ObjectArrayArray(): Array(100) {
      values_ = new objArray*[1];
      values_[0] = new objArray(100);
    }

    ~ObjectArrayArray() {
      size_t num_array_pointers = this->length() / 100 > 0 ? this->length() / 100 : 1;
      for (int i = 0; i < num_array_pointers; ++i) {
        delete values_[i];
      }
      delete[] values_;
    };

    //returns the object at index
    Object* get(size_t index) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      return values_[array_idx]->get(value_idx);
    }

    //sets the object at index to be o, returns former object
    void set(size_t index, Object* element) {
      checkOutOfBounds(index);
      int array_idx = index / 100;
      int value_idx = index % 100;
      if (value_idx < values_[array_idx]->length()) {
        values_[array_idx]->set(value_idx, element);
      } else {
        values_[array_idx]->pushBack(element);
      }
    } 

    Object* pushBack(Object* element) {
      if (this->length() < capacity_) {
        this->set(this->length(), element);
      } else {
          int subarrays_count = capacity_ / 100;
          objArray** new_data = new objArray*[subarrays_count * 2];
          for (int i = 0; i < subarrays_count; i++) {
              new_data[i] = values_[i];
          }
          for (int i = subarrays_count; i < subarrays_count * 2; i++) {
              new_data[i] = new objArray(100);
          }
          values_ = new_data;
          capacity_ = capacity_ * 2;
          set(size_, element);
      }
      size_ += 1;
    }
};
