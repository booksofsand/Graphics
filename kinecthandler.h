#ifndef KINECTHANDLER_H
#define KINECTHANDLER_H

#include "config.h"
#include "sandboxwindow.h"
#include <iostream>
#include <QEventLoop>
#include <QTimerEvent>

// Declarations for QT
class SandboxWindow;

class KinectHandler : public QEventLoop
{
    Q_OBJECT
public:
    KinectHandler(SandboxWindow* box);

private:
	
    SandboxWindow* box;
    size_t boxMaxDepth;
    size_t startlate;
    
    void timerEvent(QTimerEvent* event);
    void calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]);

};

#endif // KINECTHANDLER_H
