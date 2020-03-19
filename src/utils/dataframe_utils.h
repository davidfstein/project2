//lang::CwC

#pragma once
#include <stdarg.h>
#include <ctype.h>
#include "../objects/string.h"
#include "../dataframe/types.h"
#include "../datastructures/array.h"

class Utils {
    public:

        static Array* buildArray(types type, int n, va_list ap) {
            switch (type) { 
                case STRING: {
                    StringArrayArray* arr = new StringArrayArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, String*));
                    }
                    va_end(ap);
                    return arr; 
                }
                case INT: {
                    IntArrayArray* arr = new IntArrayArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, int));
                    }
                    va_end(ap);
                    return arr; 
                }
                case FLOAT: {
                    FloatArrayArray* arr = new FloatArrayArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, float));
                    }
                    va_end(ap);
                    return arr; 
                }
                case BOOL: {
                    BoolArrayArray* arr = new BoolArrayArray();
                    for (int i = 0; i < n; i++) {
                        arr->pushBack(va_arg(ap, bool));
                    }
                    va_end(ap);
                    return arr;
                } 
                default: printf("Unsupported type.\n");
                         exit(1);
                        break;   
            } 
            return NULL;
        }        
};
