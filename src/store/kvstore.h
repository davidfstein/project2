#pragma once
#include "../objects/object.h"
#include "../datastructures/map.h"
#include "value.h"
#include "key.h"

class KVStore : public Object {
    
    public:
        Map* pairs_;

        // Store owns its pairs?
        KVStore() {
            pairs_ = new Map();
        }

        ~KVStore() {
            delete pairs_;
        }

        Value* get(Key* key) {
            return dynamic_cast<Value*>(pairs_->get(key));
        }

        void put(Key* key, Value* value) {
            pairs_->put(key, value);
        }

        Value* wait_and_get(Key* key) {
            wait();
            return get(key);
        }

        void wait() {}
};