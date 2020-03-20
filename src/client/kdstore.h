#pragma once
#include "../store/kvstore.h"
#include "../datastructures/dataframe.h"

class KDStore {

    public:
        KVStore kv_;

        KDStore(KVStore& kv) {
            kv_ = kv;
        }

        ~KDStore() {}

        DataFrame* get(Key* key) {
            return dynamic_cast<DataFrame*>(kv_.get(key)->payload_);
        }
        
        void put(Key* key, DataFrame* df) {
            kv_.put(key, new Value(df));
        }

        DataFrame* wait_and_get(Key* key) {
            return dynamic_cast<DataFrame*>(kv_.wait_and_get(key)->payload_);
        }
};

