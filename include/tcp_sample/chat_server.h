#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include <thread>
#include <netinet/in.h>
#include "runnable_interface.h"

namespace tcp_sample
{
    enum class ThreadState : int 
    {
        Stop = 0,
        Running
    };

    class ChatServer : IRunnable
    {
        public:
            ChatServer();
            ~ChatServer();

            void run();
            void stop();
            ThreadState getState();

        protected:
            const char* TCP_ADDRESS     = "127.0.0.1";
            const int   TCP_PORT_NUMBER = 12345;
            int sock0;
            struct sockaddr_in addr;
            struct sockaddr_in client;
            int len;
            int sock;
            char buf[32];
            int n;
            ThreadState state_;
            std::thread worker_thread_;          
            void workerTask();
    };
}// end of namespace tcp_sample
#endif