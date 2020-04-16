#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "./src/network/server.h"
#include "./src/network/client.h"

int main(int argc, char const *argv[]) {
	String* clientOrServer = new String(argv[0]);
	if (clientOrServer->equals(new String("./client"))){
		Client* c = new Client(new String(argv[2]), new String(argv[4]), IPPORT_USERRESERVED + 1);
		c->register_self();
        
	} else if (clientOrServer->equals(new String("./client1"))) {
        Client* c = new Client(new String("127.0.0.3"), new String("127.0.0.1"), IPPORT_USERRESERVED + 1);
        std::thread t4(&Client::register_self, c);
        c->send_message(new String("127.0.0.2, Hi from client 1."));
        // sleep(5);
        // for (int i = 0; i < c->neighbors_->length(); ++i) {
        //     printf("%s\n", c->neighbors_->get(i)->c_str());
        // }
        while(1);
    }
}