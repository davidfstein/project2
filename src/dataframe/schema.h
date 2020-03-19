//lang::CwC

#pragma once
#include <stdio.h>
#include "../objects/object.h"
#include "../objects/string.h"
#include "../datastructures/array.h"

/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
    public:

        StringArrayArray* types_;
        StringArrayArray* names_;
        StringArrayArray* row_names_;
 
        /** Copying constructor */
        Schema(const Schema& from) {
            types_ = from.types_->copy();
            names_ = from.names_->copy();
            row_names_ = new StringArrayArray();
        }

        Schema(const Schema& from, bool keepRowNames) : Schema((const Schema&) from) {
            if (keepRowNames) {
                row_names_ = from.row_names_->copy();
            }
        }
        
        /** Create an empty schema **/
        Schema() {
            types_ = new StringArrayArray();
            names_ = new StringArrayArray();
            row_names_ = new StringArrayArray();
        }
        
        /** Create a schema from a string of types. A string that contains
            * characters other than those identifying the four type results in
            * undefined behavior. The argument is external, a nullptr argument is
            * undefined. **/
        Schema(const char* types) {
            size_t num_types = strlen(types);
            types_ = new StringArrayArray();
            names_ = new StringArrayArray();
            row_names_ = new StringArrayArray();
            for (int i = 0; i < num_types; i++) {
                char type = types[i];
                if (type != 'S' && type != 'B' && type != 73 && type != 'F') {
                    printf("Unsupported type\n");
                    exit(1);
                }
                types_->pushBack(new String(&type));
            }
        }

        ~Schema() {
            delete types_;
            delete names_;
            delete row_names_;
        }
        
        /** Add a column of the given type and name (can be nullptr), name
            * is external. Names are expectd to be unique, duplicates result
            * in undefined behavior. */
        void add_column(char* typ, String* name) { 
            types_->pushBack(new String(typ));
            if (name == nullptr) {
                names_->pushBack(nullptr);
            } else {
                names_->pushBack(name->clone());
            }
        }
        
        /** Add a row with a name (possibly nullptr), name is external.  Names are
         *  expectd to be unique, duplicates result in undefined behavior. */
        void add_row(String* name) {
            row_names_->pushBack(name->clone());
        }

        void set_row_name(size_t i, String* name) {
            if (i >= row_names_->length()) {
                printf("Row index out of bounds\n");
                exit(1);
            }
            if (name == nullptr) {
                printf("Row name cannot be set to null\n");
                exit(1);
            }
            String* old = row_name(i);
            row_names_->set(i, name->clone());
            delete old;
        }
        
        /** Return name of row at idx; nullptr indicates no name. An idx >= width
            * is undefined. */
        String* row_name(size_t idx) {
            return row_names_->get(idx);
        }
        
        /** Return name of column at idx; nullptr indicates no name given.
            *  An idx >= width is undefined.*/
        String* col_name(size_t idx) {
            return names_->get(idx);
        }
        
        /** Return type of column at idx. An idx >= width is undefined. */
        char col_type(size_t idx) {
            return types_->get(idx)->at(0);
        }
        
        /** Given a column name return its index, or -1. */
        int col_idx(const char* name) {
            for (int i = 0; i < names_->length(); i++) {
                if (names_->get(i)->equals(new String(name))) {
                    return i;
                }
            }
            return -1;
        }
        
        /** Given a row name return its index, or -1. */
        int row_idx(const char* name) {
            for (int i = 0; i < row_names_->length(); i++) {
                if (row_names_->get(i)->equals(new String(name))) {
                    return i;
                }
            }
            return -1;
        }
        
        /** The number of columns */
        size_t width() {
            return types_->length();
        }
        
        /** The number of rows */
        size_t length() {
            return row_names_->length();
        }
};
