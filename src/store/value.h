#pragma once
#include "../objects/object.h"

class Value : public Object {
    
    public:
        Object* payload_;

        // Value owns payload.
        Value(Object* payload) {
            payload_ = payload;
        }

        ~Value() {
            delete payload_;
        }

};