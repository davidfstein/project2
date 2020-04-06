
#pragma once
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include "array.h"

// Right now a grab bag of utility functions. Should be split up more intentionally later.
class Utils {

    public:
        static strArray* split(char* str, char* delimiter) {
            strArray* split_strs = new strArray(2);
            char* token = strtok(str, delimiter);
            split_strs->pushBack(new String(token));
            while (token != NULL) { 
                token = strtok(NULL, delimiter);
                if (token != NULL) {
                    split_strs->pushBack(new String(token));
                }
            }
            return split_strs;
        }

        // Makes a socket and binds to an
        // ip address. Exit if socket creation
        // or binding fails.
        // port: the port on which to bind
        static int make_socket_and_bind(String* ip, uint16_t port) {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in name;

            if (sock < 0) {
                printf("Socket initialization failure.");
                exit(1);
            }
            
            name.sin_family = AF_INET;
            name.sin_port = htons(port);
            name.sin_addr.s_addr = inet_addr(ip->c_str());
            if (bind(sock, (struct sockaddr *) &name, sizeof (name)) < 0) {
                printf("Unable to bind socket.");
                exit(1);
            }

            return sock;
        }

        // Begin listening for incoming connections.
        // Exit if listening fails.
        static void listen_for_connections(int sock, int max_connections) {
            int listening = listen(sock, max_connections);
            if (listening < 0) {
                printf("Listening failed.");
                exit(1);
            }
        }

        // Initialize a sockaddr_in struct.
        // name: a sockaddr_in object
        // hostname: the hostname for the struct
        // port: the port for the struct
        static void init_sockaddr(struct sockaddr_in *name, const char *hostname, uint16_t port) {
            name->sin_family = AF_INET;
            name->sin_port = htons(port);
            struct hostent *hostinfo = gethostbyname(hostname);
            if (hostinfo == NULL) {
                printf("Unknown host %s.\n", hostname);
                exit(1);
            }
            name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
        }


};
