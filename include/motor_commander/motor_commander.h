#ifndef MOTOR_COMMANDER_H
#define MOTOR_COMMANDER_H
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "tcp_sample/chat_server.h"
#define BUFFSIZE 1024
namespace motor_commander
{
    class MotorCommander
    {
        public:
            MotorCommander();
            ~MotorCommander();
        protected:
            ros::NodeHandle nh_;
            ros::NodeHandle private_nh_;
            ros::Publisher cmd_vel_pub_;
            tcp_sample::ChatServer server_;
            geometry_msgs::Twist twist_msg_;
            int message_queue_id_;
            long read_type_;
            typedef struct {
            /* メッセージの先頭はかならずlong int型でなければならない */
            long int type;
            double speed;
            double angle;
            char data[BUFFSIZE];
            } MessageBuffer;
            MessageBuffer message_buffer_;

            void init();
            void run();
            void cmdVelCallback();

    };
}

#endif