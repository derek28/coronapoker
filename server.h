#pragma once
#include <iostream>
#include <netinet/in.h> 
#include "game_state.h"

class StreamServer {
public:
    StreamServer();
    ~StreamServer();

    void Initialize();
    void Send(GameStateNoVector);
    void Send(LegalActionsSimplify);
    Action ReadAction();
    void WaitForConnection();

private:
    int server_fd, new_socket, valread; 
    struct sockaddr_in address;
    int opt = 1; 
    socklen_t addrlen = sizeof(address); 
    char buffer[1024] = {0};
    int port = 8088;
};
