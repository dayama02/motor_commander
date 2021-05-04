#include <iostream>
#include "chat_server.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main()
{
    tcp_sample::ChatServer server;

    server.run();

    while(server.getState() == tcp_sample::ThreadState::Running){
        //wait for the worker thread to stop
    }

    server.stop();
}