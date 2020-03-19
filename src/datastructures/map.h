//lang::CwC
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../objects/string.h"
#include "../objects/object.h"

class MapPair : public Object {
    public:

    Object* key;
    Object* val;
    bool used;
    bool tomb;

    MapPair() {
        this->key = 0;
        this->val = 0;
        this->used = 0;
        this->tomb = 0;
    }

    MapPair(Object* k, Object* v) {
        this->key = k;
        this->val = v;
        this->used = 0;
        this->tomb = 0;
    }
};

class Map : public Object {
    public:

    int length;
    int capacity;
    MapPair** data;

    Map() {
        this->length = 0;
        this->capacity = 4;
        this->data = new MapPair*[4];
        for (int i = 0; i < this->capacity; i++) {
            this->data[i] = new MapPair();
        }
    }

    ~Map() {
    }

    int hash_idx(Object* o) {
        return o->hash() & (this->capacity - 1);
    }

    Object* get(Object* key) {
        if (key) {
            int i = hash_idx(key);
            MapPair* p = this->data[i];
            if (p->key) {
                if (p->key->equals(key) && !p->tomb) {
                    return p->val;
                }
                else {
                    return linear_probe(key, i);
                }
            }
        }
        return nullptr;
    } 

    Object* linear_probe(Object* key, int i) {
        MapPair* pair = this->data[i];
        while(pair->used) {
            if (pair->key->equals(key)) {
                if (!pair->tomb) {
                    return pair->val;
                }
                else {
                    return nullptr;
                }
            }
            else if (i < this->capacity) {
                pair = this->data[i++];
            }
            else {
                pair = this->data[0];
            }
        }
        return nullptr;
    }
    
    void put(Object* key, Object* value) {
        if (!key || !value) {
            return;
        }
        if (this->length >= this->capacity / 2) {
            this->expand_hashmap();
        }
        this->linear_put(key, value);
    }

    void expand_hashmap() {
        MapPair** old_data = this->data;
        long old_cap = this->capacity;
        this->length = 0;
        this->data = new MapPair*[old_cap * 2];
        this->capacity = old_cap * 2;
        for(int i = 0; i < this->capacity; i++) {
            this->data[i] = new MapPair();
        }
        for (int i = 0; i < old_cap; i++) {
            if (old_data[i]->used) {
                this->put(old_data[i]->key, old_data[i]->val);
            }
        }
   } 

    void linear_put(Object* key, Object* value) {
        int i = this->hash_idx(key);
        while(1) {
            if (i < this->capacity) {
                if (!this->data[i]->used) {
                    this->data[i]->key = key;
                    this->data[i]->val = value;
                    this->data[i]->used = 1;
                    this->data[i]->tomb = 0;
                    this->length += 1;
                    break;
                }
                else if (this->data[i]->key->equals(key)) {
                    this->data[i]->val = value;
                    this->data[i]->used = 1;
                    this->data[i]->tomb = 0;
                    this->length += 1;
                    break;
                }
                else {
                    i++;
                }
            }
            else {
                i = 0;
            }
        }
    }

    bool has_key(Object* key) {
        return this->get(key) != nullptr;
    }

    size_t size() {
        return this->length;
    }

    Object* remove(Object* key) {
        if (!this->get(key)) {
            return nullptr;
        }
        else {
            int i = hash_idx(key);
            MapPair* pair = this->data[i];
            while(pair->used) {
                if (pair->key->equals(key)) {
                    this->data[i]->tomb = 1;
                    this->length -= 1;
                    return this->data[i]->val;
                }
                else if (i < this->capacity) {
                    i++;
                    pair = this->data[i];
                }
                else {
                    i = 0;
                    pair = this->data[i];
                }
            }
        }
    }

};