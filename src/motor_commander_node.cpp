#include "motor_commander/motor_commander.h"
#include "tcp_sample/chat_server.h"
#include <iostream>

int main(int argc, char**argv)
{
    tcp_sample::ChatServer server;
    server.run();
 
    ros::init(argc, argv, "MotorCommander");
    motor_commander::MotorCommander commander;
    return 0;
}