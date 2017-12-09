#ifndef KINECTHANDLER_H
#define KINECTHANDLER_H

#include <iostream>
#include <QEventLoop>
#include <QTimerEvent>
#include "config.h"
#include "sandboxwindow.h"

// Includes from Sandbox.h
/*
#include <Threads/TripleBuffer.h>
#include <Geometry/Box.h>
#include <Geometry/OrthonormalTransformation.h>
#include <Geometry/ProjectiveTransformation.h>
#include <GL/gl.h>
#include <GL/GLColorMap.h>
#include <GL/GLMaterial.h>
#include <GL/GLObject.h>
#include <GL/GLGeometryVertex.h>
#include <GLMotif/ToggleButton.h>
#include <GLMotif/TextFieldSlider.h>
*/
#include <Vrui/Tool.h>
#include <Vrui/GenericToolFactory.h>
#include <Vrui/TransparentObject.h>
#include <Vrui/Application.h>
#include <Kinect/FrameBuffer.h>
#include <Kinect/FrameSource.h>

// Forward declarations from Sandbox.h
namespace Misc {
template <class ParameterParam>
class FunctionCall;
}

namespace Kinect {
class Camera;
}

// Declarations for QT
class SandboxWindow;

class KinectHandler : public QEventLoop
{
    Q_OBJECT
public:
    KinectHandler(SandboxWindow* box);

private:
  // Type defs from Sandbox.h
  //typedef Geometry::Box<Scalar,3> Box; // Type for bounding boxes
  //typedef Geometry::OrthonormalTransformation<Scalar,3> ONTransform; // Type for rigid body transformations
  typedef Kinect::FrameSource::DepthCorrection::PixelCorrection PixelDepthCorrection; // Type for per-pixel depth correction factors
	
    SandboxWindow* box;
    size_t currDepth; // MM: testing only
    //const Kinect::FrameBuffer frameBuffer;
    Kinect::FrameBuffer frameBuffer;
    
    void timerEvent(QTimerEvent* event);
    void calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]);
    void rawDepthFrameDispatcher(const Kinect::FrameBuffer& frameBuffer);
    //void receiveFilteredFrame(const Kinect::FrameBuffer& frameBuffer);

    // MM: the following taken from Sandbox.h
    Kinect::FrameSource* camera; // The Kinect camera device
    unsigned int frameSize[2]; // Width and height of the camera's depth frames
    PixelDepthCorrection* pixelDepthCorrection; // Buffer of per-pixel depth correction coefficients // ???
    Kinect::FrameSource::IntrinsicParameters cameraIps; // Intrinsic parameters of the Kinect camera
    //FrameFilter* frameFilter; // Processing object to filter raw depth frames from the Kinect camera
    bool pauseUpdates; // Pauses updates of the topography
    //Threads::TripleBuffer<Kinect::FrameBuffer> filteredFrames; // Triple buffer for incoming filtered depth frames
};

#endif // KINECTHANDLER_H
