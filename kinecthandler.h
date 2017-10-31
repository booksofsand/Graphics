#ifndef KINECTHANDLER_H
#define KINECTHANDLER_H
#define MAXROWS 30
#define MAXCOLS 60
#include "sandboxwindow.h"
#include <QEventLoop>

class SandboxWindow;

class KinectHandler : public QEventLoop
{
    Q_OBJECT
public:
    KinectHandler(SandboxWindow* box);

private:
    SandboxWindow* box;
    size_t currDepth; // MM: testing only
    
    void timerEvent(QTimerEvent* event);
    void calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]);
    //    size_t** calcDepthsToDisplay();
    //void KinectHandler::rawDepthFrameDispatcher(const Kinect::FrameBuffer& frameBuffer);
    //void KinectHandler::receiveFilteredFrame(const Kinect::FrameBuffer& frameBuffer);

    /* MM: the following copied from Sandbox.h (requires Vrui and Kinect): 
    Kinect::FrameSource* camera; // The Kinect camera device
    unsigned int frameSize[2]; // Width and height of the camera's depth frames
    PixelDepthCorrection* pixelDepthCorrection; // Buffer of per-pixel depth correction coefficients // ???
    Kinect::FrameSource::IntrinsicParameters cameraIps; // Intrinsic parameters of the Kinect camera
    FrameFilter* frameFilter; // Processing object to filter raw depth frames from the Kinect camera
    bool pauseUpdates; // Pauses updates of the topography
    Threads::TripleBuffer<Kinect::FrameBuffer> filteredFrames; // Triple buffer for incoming filtered depth frames
    //     ^ See Vrui/Threads/TripleBuffer.h
    */
};

#endif // KINECTHANDLER_H
