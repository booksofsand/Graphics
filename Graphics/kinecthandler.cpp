#include "kinecthandler.h"
#include <iostream>
#include <QTimerEvent>

KinectHandler::KinectHandler(QEventLoop *parent) : QEventLoop(parent)
{
    startTimer(1000);   // 1-second timer
}

void KinectHandler::timerEvent(QTimerEvent *event)
{
    std::cout << "Timer ID:" << event->timerId() << std::endl; // MM: testing
    // TODO: call kinect frame function to get frame
    // TODO: pass frame through filtering
    // TODO: pass frame to SandboxWindow to find new depths & update window
}
