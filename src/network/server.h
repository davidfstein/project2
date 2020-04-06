// LANGUAGE: CwC

#pragma once
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../objects/object.h"
#include "../objects/string.h"
#include "../datastructures/array.h"
#include "../utils/network_utils.h"

// A CwC server that allows up to 256 connections from clients and facillitates
// communication between clients. Supports messages up to MAX_MSG length.
// Development was guided by and borrows from the GNU software manual 
// Socket description: https://www.gnu.org/software/libc/manual/html_node/Sockets.html.
class Server : public Object {
    
    // Maximum number of connections to accept.
    int MAX_CONNECTIONS = 256;
    // Maximum message length to accept.
    int MAX_MSG = 512;

    public:
        // This servers ip.
        String* ip_;
        // The ips of the connected clients.
        strArray* clients_;
        // This servers socket.
        int sock_fd_;
        // The port for the server to listen on.
        uint16_t port_ = IPPORT_USERRESERVED + 1;

        // Construct a new server object.
        // ip: this servers ip 
        Server(String* ip, uint16_t port) {
            ip_ = ip;
            port_ = port;
            clients_ = new strArray(MAX_CONNECTIONS);
        }

        // Stop transmitting and recieiving.
        ~Server() {
            shutdown(sock_fd_, 2);
        }

        // Add a client ip to the list of client ips.
        // client_ip: the ip of the client to add
        void register_client(String* client_ip) {
            if (client_ip == nullptr) {
                printf("Client ip cannot be null.");
                exit(1);
            } else if (clients_->contains(client_ip)) {
                printf("Duplicate ip address detected.");
                exit(1);
            }
            clients_->pushBack(client_ip);
        }

        // Construct a string representation of connected ips
        // to broadcast to the connected clients.
        String* prepare_broadcast() {
            StrBuff s;
            for (int i = 0; i < clients_->length()-1; ++i) {
                s.c(*clients_->get(i));
                s.c(",");
            }
            String* st = clients_->get(clients_->length()-1);
            s.c(*clients_->get(clients_->length()-1));
            return s.get();
        }

        // Send a string representation of connected ips to connected clients.
        // ips: the string representing connected ips
        void broadcast_ips(String* ips, fd_set active_fds) {
            if (ips == NULL) {
                return;
            }
            for (int i = 0; i < FD_SETSIZE; ++i) {
                if (FD_ISSET(i, &active_fds)) {
                    if (i == sock_fd_) {
                        continue;
                    } 
                    int nbytes = write(i, ips->c_str(), strlen(ips->c_str()));
                    if (nbytes < 0) {
                        printf("Could not write to fd %d.", i);
                        exit(1);
                    }
                }
            }
        }

        void teardown() {
            printf("A client connection was lost. Terminating.\n");
            exit(0);
        }

        // Reads an incoming message from a client and sends a response 
        // to a recipient. Note: recipient can be this server.
        // fd: the socket of the client to read from
        // active_fds: the connected sockets
        String* read_from_client(int fd) {
            char buffer[MAX_MSG];
            int nbytes = read(fd, buffer, MAX_MSG);

            if (nbytes < 0) {
                printf("Could not read.");
                exit(1);
            } else if (nbytes == 0) {
                teardown();
            } else {
                return new String(buffer);
            }
        }

        // Begins serving. Fields incoming connections and messages from clients.
        void serve() {
            struct sockaddr_in clientname;
            sock_fd_ = Utils::make_socket_and_bind(ip_, port_);
            Utils::listen_for_connections(sock_fd_, MAX_CONNECTIONS);

            fd_set active_fd_set, read_fd_set;
            FD_ZERO(&active_fd_set);
            FD_SET(sock_fd_, &active_fd_set);

            while (1) {
                read_fd_set = active_fd_set;
                int selecting = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
                if (selecting < 0) {
                    printf("Could not block for connections.");
                    exit(1);
                }

                for (int i = 0; i < FD_SETSIZE; ++i) {
                    if (FD_ISSET(i, &read_fd_set)) {
                        if (i == sock_fd_) {
                            /* Connection request on original socket. */
                            size_t size = sizeof(clientname);
                            int incoming_connection = accept(sock_fd_, (struct sockaddr *) &clientname, (socklen_t*) &size);
                            if (incoming_connection < 0) {
                                printf("Failed to accept incoming connection.");
                                exit(1);
                            }
                            FD_SET(incoming_connection, &active_fd_set);
                        } else {
                            String* ip = read_from_client(i);
                            if (ip->size() > 0) {
                                register_client(ip);
                                printf("Server: connect from host %s, port %d.\n", ip->c_str(), ntohs(clientname.sin_port));
                                String* ips = prepare_broadcast();
                                broadcast_ips(ips, active_fd_set);
                            }
                        }
                    }
                }
            }
        }
};