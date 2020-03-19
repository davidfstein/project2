#pragma once
#include "../objects/object.h"
#include "../objects/string.h"

class Key : public Object {
    
    public:
        String* key_name_;
        size_t node_index_;

        // Name is external.
        Key(String* key_name, size_t node_index) {
            this->key_name_ = key_name->clone();
            this->node_index_ = node_index;
        }

        ~Key() {
            delete key_name_;
        }
};