// LANGUAGE: CwC

#pragma once
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <thread>
#include "../utils/network_utils.h"
#include "../objects/object.h"
#include "../objects/string.h"
#include "../datastructures/array.h"

// A CwC implementation of a client that can bind to an ip and connect to a server. This implementation
// supports sending messages to the server and other connected clients and recieving messages as well.
class Client : public Object {

    // Maximum number of connections to accept.
    int MAX_CONNECTIONS = 256;
    // Largest supported message length.
    int MAX_MSG = 512;

    public:
        // The ip to be bound by this client.
        String* ip_;
        // The ip of the server.
        String* server_ip_;
        // The ips of other clients connected to the server.
        strArray* neighbors_;
        // The file descriptor of the socket established by the client.
        int sock_fd_;
        // The servers socket.
        int server_socket_;
        // Active file descriptions.
        fd_set active_fds_;
        // The port to listen on.
        uint16_t port_ = IPPORT_USERRESERVED + 1;

        // Creates a client with an ip. Should not be used
        // outside of this class for the time being.
        Client(String* ip) {
            ip_ = ip;
            sock_fd_ = Utils::make_socket_and_bind(ip_, port_);
            neighbors_ = new strArray(10);
        }

        // Creates a client with an ip and the ip of a 
        // server to connect to.
        Client(String* ip, String* server_ip, uint16_t port) : Client(ip) {
            server_ip_ = server_ip;
            port_ = port;
        }

        // Closes the socket and exits the threads.
        // Deletes owned memory.
        ~Client() {
            close(sock_fd_);
            pthread_exit(NULL);
            delete[] neighbors_;
            delete server_ip_;
            delete ip_;
        }

        // Set the list of neighbors.
        // neighbors: the list of neighbors to set.
        void set_neighbors(strArray* neighbors) {
            neighbors_->clear();
            for (int i = 0; i < neighbors->length(); ++i) {
                neighbors_->pushBack(neighbors->get(i));
            }
        }

        // Send a message to a connected node.
        // fd: the socket over which to send the message
        // message: the message to send
        void send_message(String* message) {
            strArray* split_strs = Utils::split(message->c_str(), ",");
            struct sockaddr_in name;
            Utils::init_sockaddr(&name, split_strs->get(0)->c_str(), port_);

            int connect_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (connect_sock < 0) {
                printf("Could not create socket.");
                exit(1);
            }

            int connection = connect(connect_sock, (struct sockaddr *) &name, sizeof (name));
            if (connection < 0) {
                printf("Could not connect to server.");
                exit(1);
            }
            send_message(connect_sock, split_strs->get(1));
        }

        // Send a message to a connected node.
        // fd: the socket over which to send the message
        // message: the message to send
        void send_message(int fd, String* message) {
            int mes_bytes = strlen(message->c_str());
            if (mes_bytes > MAX_MSG) {
                int num_messages = mes_bytes / MAX_MSG;
                int overflow = mes_bytes % MAX_MSG == 0 ? 0 : 1;
                int total_sends = num_messages + overflow;
                for (int i = 0; i < total_sends; ++i) {
                    // TODO: Bad for memory. Fix.
                    char buf[MAX_MSG];
                    strncpy(buf, &message->c_str()[MAX_MSG * i], MAX_MSG);
                    write(fd, buf, strlen(buf));
                    memset(buf, 0, MAX_MSG);
                }
                return;
            }
            int nbytes = write(fd, message->c_str(), strlen(message->c_str()));
            if (nbytes < 0) {
                printf("Could not write.");
                exit(1);
            }
        }

        // Spawn a thread and being listening for incoming messages.
        void* listen_up(int sock) {
            while(1) {
                int i = Client::read_server_message(sock);
                if (i < 0) {
                    teardown();
                    exit(0);
                }
            }
        }

        void teardown() {
            printf("Server stopped.\n");
            exit(0);
        }

        // Reads an incoming message from a node.
        // fd: the socket of the node to read from
        int read_message(int fd) {
            char buffer[MAX_MSG];
            int nbytes = read(fd, buffer, MAX_MSG);

            if (nbytes < 0) {
                printf("Could not read.");
                exit(1);
            } else if (nbytes == 0) {
                return -1;
            } else {
                printf("%s\n", buffer);
                memset(buffer, 0, MAX_MSG);
                return 0;
            }
        }

        int read_server_message(int fd) {
            char buffer[MAX_MSG];
            int nbytes = read(fd, buffer, MAX_MSG);
            
            if (nbytes < 0) {
                printf("Could not read.");
                exit(1);
            } else if (nbytes == 0) {
                return -1;
            } else {
                strArray* s = Utils::split(buffer, ",");
                set_neighbors(s);
                return 0;
            }
        }

        // Connect to a servre.
        void connect_to_server() {
            struct sockaddr_in servername;
            Utils::init_sockaddr(&servername, server_ip_->c_str(), port_);
            int server_connect_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (server_connect_sock < 0) {
                printf("Could not create socket.");
                exit(1);
            }

            int connection = connect(server_connect_sock, (struct sockaddr *) &servername, sizeof (servername));
            if (connection < 0) {
                printf("Could not connect to server.");
                exit(1);
            }
            send_message(server_connect_sock, ip_);
            server_socket_ = server_connect_sock;
        }

        void register_self() {
            connect_to_server(); 
            Utils::listen_for_connections(sock_fd_, MAX_CONNECTIONS);
            std::thread t1(&Client::listen_up, this, server_socket_);
            
            struct sockaddr_in nodename;

            fd_set active_fd_set, read_fd_set;
            active_fds_ = active_fd_set;
            FD_ZERO(&active_fd_set);
            FD_SET(sock_fd_, &active_fd_set);

            while (1) {
                read_fd_set = active_fd_set;
                // This blocks until a connection comes in!!!!
                int selecting = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
                if (selecting < 0) {
                    printf("Could not block for connections.");
                    exit(1);
                }

                for (int i = 0; i < FD_SETSIZE; ++i) {
                    if (FD_ISSET(i, &read_fd_set)) {
                        if (i == sock_fd_) {
                            /* Connection request on original socket. */
                            size_t size = sizeof(nodename);
                            int incoming_connection = accept(sock_fd_, (struct sockaddr *) &nodename, (socklen_t*) &size);
                            if (incoming_connection < 0) {
                                printf("Failed to accept incoming connection.");
                                exit(1);
                            }
                            FD_SET(incoming_connection, &active_fd_set);
                        } else {
                            read_message(i);
                        }
                    }
                }
            }
        }
};