#ifndef RUNNABLE_INTERFACE_H
#define RUNNABLE_INTERFACE_H

#include <thread>
namespace tcp_sample
{
    class IRunnable
    {
        public:
            virtual ~IRunnable(){}
            virtual void run() = 0;
            virtual void stop() = 0;
    };
}// end of namespace tcp_sample
#endif