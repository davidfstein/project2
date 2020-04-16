#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "./src/network/server.h"
#include "./src/network/client.h"

int main(int argc, char const *argv[]) {
    Server* s = new Server(new String(argv[2]), IPPORT_USERRESERVED + 1);
    s->serve();
}