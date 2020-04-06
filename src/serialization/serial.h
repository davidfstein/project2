// lang::CwC

#pragma once
#include <cstring>
#include "../objects/string.h"
#include "../datastructures/array.h"
#include "msgkind.h"
#include "message.h"

class Serial : public Object {
    public:
        StrBuff buf;

        /* Class to wrap writing serialization to a given file descriptor*/ 
        Serial() {
            StrBuff buf();
        }

        void print() {
            printf("%s\n", buf.val_);
        }

        void add_code(int i) {
            const char* code = reinterpret_cast<char*>(&i);
            this->buf = buf.c(code, sizeof(int));
        }

        void write(double db) {
            add_code(15);
            const char* str = reinterpret_cast<char*>(&db);
            this->buf = buf.c(str, sizeof(double));
        }

        void write(int i) {
            add_code(1);
            const char* str = reinterpret_cast<const char*>(&i);
            this->buf = buf.c(str, sizeof(int));
        }

        void write(size_t s) {
            add_code(2);
            const char* str = reinterpret_cast<char*>(&s);
            this->buf = buf.c(str, sizeof(size_t));
        }

        void write(char* ch) {
            add_code(3);
            this->buf = buf.c(ch);
        }

        void write(String* s) {
            add_code(4);
            write(s->size());
            char* chars = s->c_str();
            for (int i = 0; i < s->size(); i++) {
                write(chars[i]);
            }
        }

        void write(StringArray* sa) {
            add_code(5);
            this->write(sa->size());
            for (int i = 0; i < sa->size(); i++) {
                write(sa->get(i));
            }
        }

        void write(DoubleArray* d) {
            add_code(6);
            this->write(d->size());
            this->write(d->capacity_);
            for (int i = 0; i < d->size(); i++) {
                write(d->get(i));
            }
        }

        void write(MsgKind* kind) {
            add_code(7);
            const char* str = reinterpret_cast<char*>(kind);
            this->buf = buf.c(str, sizeof(MsgKind));
        }

        void write(Message* m) {
            add_code(8);
            write(&m->kind_);
            write(m->sender_);
            write(m->target_);
            write(m->id_);
        }

        void write(Ack* a) {
            add_code(9);
            write(dynamic_cast<Message*>(a));
        }

        void write(Status* s) {
            add_code(10);
            write(s->msg_);
            write(dynamic_cast<Message*>(s));
        } 

        //void write(in_addr* addr) {
            //add_code(11);
            //// TODO: i'm tired, does this work?
            //write((size_t)addr->s_addr);
        //}

        //void write(sockaddr_in*  addr) {
            //add_code(12);
            //write(addr->sin_family);
            //write(addr->sin_port);
            //write(&addr->sin_addr);
        //}

        void write(Register* r) {
            add_code(13);
            write(r->client.sin_family);
            write((int)r->client.sin_port);
            write((int)r->client.sin_addr.s_addr);
            write(reinterpret_cast<Message*>(r));
        }


        void write(Directory* d) {
            add_code(14);
            write(d->client);
            write(d->port_size);
            for (int i = 0; i < d->port_size; i++) {
                write(d->ports[i]);
            }
            write(d->addresses_size);
            for (int i = 0; i < d->addresses_size; i++) {
                write(d->addresses[i]);
            }
            write(reinterpret_cast<Message*>(d));
        }
};

