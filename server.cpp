#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <string.h> 
#include "game_state.h"
#include "server.h"


StreamServer::StreamServer(){
    Initialize();
    WaitForConnection();
}

StreamServer::~StreamServer(){
    std::cout << "port " << port << "closed" << std::endl;
}


void StreamServer::Initialize(){
    // Creating socket file descriptor 
    // AF_INET = IPV4
    // SOCK_STREAM = TCP
    // 0 = Protocol value, 0 is for IP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Attach socket to PORT
    // AF_INET = IPV4
    // INADDR_ANY = 
    /* INADDR_ANY is used when you don't need to bind a socket to a specific IP. When you use this value as the address when calling bind() , the socket accepts connections to all the IPs of the machine */
    // HTONS converts the unsigned short integer hostshort from host byte order to network byte order
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
       
    // Bind  socket to the port 
    if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    std::cout << "server initialized. Port: " << port << std::endl;
}

void StreamServer::WaitForConnection(){
   if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    std::cout << "listening for connections" << std::endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    std::cout<< "connection accepted, new socket:" << new_socket << std::endl;
}

Action StreamServer::ReadAction(){
    memset(buffer,0,sizeof(buffer));
    valread = read( new_socket , buffer, 1024); 
    std::cout << "read status: " << valread << std::endl;
    if (valread == 0 || valread == -1) {
        std::cout << "error reading from socket! closing the socket with this client" << std::endl;
        close(new_socket);
        WaitForConnection();
    }
    Action* ret;
    ret = (Action*) buffer;
    return *ret;
} 

void StreamServer::Send(GameStateNoVector gs){
    char buffer[2048] = {0};
    int size = sizeof(gs) + 4 + 1;
    int type = 1;
    memcpy(buffer,&size,sizeof(int));
    memcpy(buffer+4,&type,1);    
    memcpy(buffer+5,&gs,sizeof(gs));
    int i = send(new_socket , buffer , sizeof(gs)+5 , 0 ); 
    std::cout << "[SERVER]GameState sent. Sent status:" << i << std::endl; 
}

void StreamServer::Send(LegalActionsSimplify la){
    char buffer[2048] = {0};
    int size = sizeof(la) + 4 + 1;
    int type = 2;
    memcpy(buffer,&size,sizeof(int));
    memcpy(buffer+4,&type,1);   
    memcpy(buffer+5,&la,sizeof(la));
    int i = send(new_socket , buffer , sizeof(la)+5 , 0 ); 
    std::cout << "[SERVER]LegalAction sent. Sent status:" << i << std::endl; 
}


