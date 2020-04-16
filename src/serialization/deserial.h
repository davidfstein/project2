
#pragma once
#include <cstring>
#include "../objects/string.h"
#include "../datastructures/array.h"
#include "msgkind.h"
#include "message.h"

class Deserial {

    public:
        StrBuff buf;
        size_t offset;

        Deserial(StrBuff given) {
            buf = given;
            offset = 0;
        }

        void* make_object(int code) {
            switch (code) {
                case 15: return make_double();
                case 1: return make_int();
                case 2: return make_size_t();
                case 3: return make_char();
                case 4: return make_String();
                case 5: return make_StringArray();
                case 6: return make_DoubleArray();
                case 7: return make_MsgKind();
                case 8: return make_Message();
                case 9: return make_Ack();
                case 10: return make_Status();
                //case 11: return make_in_addr();
                //case 12: return make_sockaddr_in();
                case 13: return make_Register();
                case 14: return make_Directory();
                default: exit(1);
            }
        }

        char* get_char_bytes(int b) {
            char* char_result = new char[b];
            char* raw = buf.val_;
            strncpy(char_result, raw + offset, b);
            offset += b;
            return char_result;
        }

        int* make_int() {
            char* code = get_char_bytes(sizeof(int));
            return reinterpret_cast<int*>(code);
        }

        size_t* make_size_t() {
            char* code = get_char_bytes(sizeof(size_t));
            return reinterpret_cast<size_t*>(code);
        }

        double* make_double() {
            char* code = get_char_bytes(sizeof(double));
            return reinterpret_cast<double*>(code);
        }

        char* make_char() {
            char* code = get_char_bytes(2);
            return reinterpret_cast<char*>(code);
        }

        // String is external
        String* make_String() {
            size_t* size = (size_t*)this->read();
            StrBuff* buf = new StrBuff();
            for (int i = 0; i < *size; i++) {
                buf->c((char*)this->read());
            }
            String* result = buf->get();
            return result;
        }

        strArray* make_StringArray() {
            size_t* size = (size_t*)this->read();
            strArray* result = new strArray(*size);
            for (int i = 0; i < *size; i++) {
                result->pushBack((String*)this->read());
            }
            return result;
        }

        DoubleArray* make_DoubleArray() {
            size_t* size = (size_t*)this->read();
            size_t* cap = (size_t*)this->read();
            DoubleArray* result = new DoubleArray(*cap);
            for (int i = 0; i < *size; i++) {
                double* element = (double*)this->read();
                result->pushBack(*element);
            }
            return result;
        }

        MsgKind* make_MsgKind() {
            char* raw = get_char_bytes(sizeof(MsgKind));
            MsgKind* result = reinterpret_cast<MsgKind*>(raw);
            return result;
        }

        Message* make_Message() {
            MsgKind* kind = (MsgKind*)this->read();
            size_t* send = (size_t*)this->read();
            size_t* tar = (size_t*)this->read();
            size_t* id = (size_t*)this->read();
            return new Message(*kind, *send, *tar, *id);
        }

        Ack* make_Ack() {
            Message* m = (Ack*)this->read();
            Ack* ack = new Ack(m);
            delete m;
            return ack;
        }


        Status* make_Status() {
            String* msg = (String*)this->read();
            Status* result = (Status*)this->read();
            result->msg_ = msg;
            return result;
        }

        Register* make_Register() {
            sockaddr_in sock;
            size_t* fam = (size_t*)this->read();
            int* port = (int*)this->read();
            int* s_address = (int*)this->read();
            in_addr iaddress;
            iaddress.s_addr= (in_addr_t)*s_address;
            sock.sin_family = *fam;
            sock.sin_port = *port;
            sock.sin_addr = iaddress;

            Register* result = (Register*)this->read();
            result->client = sock;
            result->port = *port;
            return result;
        }

        Directory* make_Directory() {
            size_t* client = (size_t*)this->read();
            size_t* port_size = (size_t*)this->read();
            size_t* ports = new size_t[*port_size];
            for (int i = 0; i < *port_size; i++) {
                size_t* e = (size_t*)this->read();
                ports[i] = *e;
            }
            size_t* addr_size = (size_t*)this->read();
            String** addrs = new String*[*addr_size];
            for (int i = 0; i < *addr_size; i++) {
                String* e = (String*)this->read();
                addrs[i] = e;
            }
            Directory* m = (Directory*)this->read();
            m->client = *client;
            m->port_size = *port_size;
            m->ports = ports;
            m->addresses_size = *addr_size;
            m->addresses = addrs;
            return m;
        }

        void* read() {
            char* code = get_char_bytes(sizeof(int));
            int* code_ptr = reinterpret_cast<int*>(code);
            //printf("code: %d\n", *code_ptr);
            return make_object(*code_ptr);
        }
};
