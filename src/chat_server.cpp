#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "tcp_sample/chat_server.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

#define BUFFSIZE 1024

namespace tcp_sample
{
    ChatServer::ChatServer()
    {
        
    }
    ChatServer::~ChatServer()
    {
        cout << "destructor" << endl;
        worker_thread_.join();
        cout << "server is stopped" << endl;
    };
    
    void ChatServer::run()
    {
        state_ = ThreadState::Running;
        worker_thread_ = std::thread([this](){this->workerTask();});
        // worker_thread_.join();            
    }
    void ChatServer::stop()
    {
        cout << "worker thread stop request" << endl;
        state_ = ThreadState::Stop;   
    }

    void ChatServer::workerTask()
    {
        cout << "worker thread start" << endl;
        /* メッセージキューの初期化 */
        typedef struct  {
            long int type;
            double   speed;
            double   angle;
            char data[BUFFSIZE];
        } MessageData;

        int message_queue_id;
        int counter = 0;
        MessageData data;
        data.type = 1111;

        /* メッセージキューの作成,取得 */
        if((message_queue_id = msgget((key_t)1111, 0666 | IPC_CREAT)) == -1){
            perror("msgget failure");
        }


        /* ソケットの作成 */
        sock0 = socket(AF_INET, SOCK_STREAM, 0);

        /* ソケットの設定 */
        addr.sin_family = AF_INET;
        addr.sin_port = htons(TCP_PORT_NUMBER);
        addr.sin_addr.s_addr = INADDR_ANY;
        bind(sock0, (struct sockaddr *)&addr, sizeof(addr));

        /* TCPクライアントからの接続要求を待てる状態にする */
        listen(sock0, 5);

        /* TCPクライアントからの接続要求を受け付ける */
        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client, (socklen_t*)&len);
        cout << "connected" << endl;
        while(state_ == ThreadState::Running){
            /* サーバからデータを受信 */
            memset(buf, 0, sizeof(buf));
            n = recv(sock, buf, sizeof(buf), 0);

            if(n < 0){
                /* エラー */
                cout << "error !!" << endl;
                break;
            }
            if(n == 0){
                cout << "connection closed !!" << endl;
                break;
            }
            cout << "> " << n << "," << buf << endl;

            try
            {
                typedef struct {
                    double speed;
                    double angle;
                } ReceiveData;
                ReceiveData* receive_data;
                receive_data = (ReceiveData*)&buf;
                cout << "speed:" << (*receive_data).speed << "angle:" << (*receive_data).angle << endl;
            }
            catch(std::exception e)
            {
                //pass
            }
            data.speed = (double)counter * 0.1;
            data.angle = (double)counter++ * 0.2;
            strncpy(data.data, "", BUFFSIZE);
            /* メッセージを送信 */
            if(msgsnd(message_queue_id, &data, BUFFSIZE, 0) == -1){
                perror("msgsnd failure");
            }
        }
        strncpy(data.data, "exit", BUFFSIZE);
        /* メッセージを送信 */
        if(msgsnd(message_queue_id, &data, BUFFSIZE, 0) == -1){
            perror("msgsnd failure");
        }        
        /* TCPセッションの終了 */
        close(sock);

        /* listen するsocketの終了 */
        close(sock0);

        state_ = ThreadState::Stop;
        cout << "worker thread end" << endl;
    }

    ThreadState ChatServer::getState()
    {
        return state_;
    }

}// end of namespace tcp_sample