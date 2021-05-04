#include "motor_commander/motor_commander.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

using std::cout;
using std::endl;

namespace motor_commander
{
    MotorCommander::MotorCommander() : 
    nh_(""), private_nh_("~"), read_type_{1111}
    {
        init();
        run();
    }

    MotorCommander::~MotorCommander()
    {

    }

    void MotorCommander::init()
    {
        /* メッセージキューの作成,取得 */
        errno = 0;
        if((message_queue_id_ = msgget((key_t)1111, 0666 | IPC_CREAT)) == -1){
            perror("msgget failure");
        }
        cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    }

    void MotorCommander::run()
    {
        ros::Rate rate(10);
        while(ros::ok()){
            /* メッセージの受信 */
            errno = 0;
            if(msgrcv(message_queue_id_, &message_buffer_, BUFFSIZE, read_type_, 0) == -1){
                perror("msgrcv failure");
                break;
            }else{
                printf("target speed: %1.1f, angle:%1.1f\n", message_buffer_.speed, message_buffer_.angle);
                //   fprintf(stdout,"received message:\t%s\n",message.data);
            }
            /* "exit"を受信すると終了する */
            if(strcmp(message_buffer_.data, "exit") == 0){
                break;  
            }
            twist_msg_.linear.x = message_buffer_.speed;
            twist_msg_.angular.z = message_buffer_.angle;
            cmd_vel_pub_.publish(twist_msg_);
            // ros::spinOnce();
            rate.sleep();
        }
        /* メッセージキューの削除 */
        errno = 0;
        if(msgctl(message_queue_id_, IPC_RMID, NULL) == -1){
            perror("msgctl failure");        
        }
    }
}// end of namespace motor_commander