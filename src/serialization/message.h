//lang::CwC
#pragma once
#include <netinet/in.h>

class Message : public Object {
    public:
        MsgKind kind_;
        size_t sender_;
        size_t target_;
        size_t id_;

        Message() { }

        Message(MsgKind k, size_t s, size_t t, size_t id) {
            kind_ = k;
            sender_ = s;
            target_ = t;
            id_ = id;
        }
};

class Ack : public Message {
    public:
        Ack() { }

        // message is external
        Ack(Message* m) : 
            Message(m->kind_, m->sender_, m->target_, m->id_) {
            }

};

class Status : public Message {
    public:
        String* msg_;

        Status(String* given, Message* m) : 
            Message(m->kind_, m->sender_, m->target_, m->id_) {
                msg_ = given->clone();
            }

        ~Status() {
            delete msg_;
        }
};

class Register : public Message {
    public:
        sockaddr_in client;
        size_t port;

        Register() {
        }
};

class Directory : public Message {
    public:
        size_t client;
        size_t* ports;
        int port_size;
        String** addresses;
        int addresses_size;

        Directory() {

        }

        Directory(int port, int addr) {
            port_size = port;
            ports = new size_t[port_size];
            addresses_size = addr;
            addresses = new String*[addresses_size];
        }

        ~Directory() {
            for (int i = 0; i < port_size; i++) {
                delete &ports[i];
            }
            for (int i = 0; i < addresses_size; i++) {
                delete addresses[i];
            }
        }
};
