#pragma once
#include "../objects/object.h"
#include "../client/kdstore.h"

class Application: public Object {

    public:

        size_t index;
        KDStore* store;

        Application(size_t idx) {
            index = idx;
            store = new KDStore(*new KVStore());
        }

        virtual ~Application() {}
};