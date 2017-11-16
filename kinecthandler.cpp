#include "kinecthandler.h"


// Misc include files from Sandbox.cpp
#include <Misc/SizedTypes.h>
#include <Misc/SelfDestructPointer.h>
#include <Misc/FixedArray.h>
#include <Misc/FunctionCalls.h>
#include <Misc/FileNameExtensions.h>
#include <Misc/StandardValueCoders.h>
#include <Misc/ArrayValueCoders.h>
#include <Misc/ConfigurationFile.h>

// MM: added in attempt to counter seg fault:
#include <IO/Directory.h>
#include <IO/StandardDirectory.h>
//#include <Cluster/OpenFile.h>

// Kinect include files from Sandbox.cpp
#include <Kinect/FileFrameSource.h>
#include <Kinect/DirectFrameSource.h>
#include <Kinect/OpenDirectFrameSource.h>

KinectHandler::KinectHandler(SandboxWindow* theBox) : QEventLoop(0) { // MM: 0 = parent
  box = theBox;
  startTimer(3000);
  currDepth = 1; // MM: testing only
  
  std::cout << "In KinectHandler::KinectHandler." << std::endl; // MM: testing
  
  // Set the current directory of the IO sub-library
  //IO::Directory::setCurrent(Cluster::openDirectory(0, "."));  // MM: added in attempt to counter seg fault
  IO::Directory::setCurrent(new IO::StandardDirectory("."));  // MM: added in attempt to counter seg fault
  
  // Read the sandbox's default configuration parameters
  std::string sandboxConfigFileName = CONFIG_CONFIGDIR;
  sandboxConfigFileName.push_back('/');
  sandboxConfigFileName.append(CONFIG_DEFAULTCONFIGFILENAME);
  Misc::ConfigurationFile sandboxConfigFile(sandboxConfigFileName.c_str());
  Misc::ConfigurationFileSection cfg = sandboxConfigFile.getSection("/BookOfSands");
  unsigned int cameraIndex = cfg.retrieveValue<int>("./cameraIndex", 0);
  std::string cameraConfiguration = cfg.retrieveString("./cameraConfiguration", "Camera");
  double scale = cfg.retrieveValue<double>("./scaleFactor", 100.0);
  
  std::cout << "Done reading in config file." << std::endl; // MM: testing
  
  std::string sandboxLayoutFileName = CONFIG_CONFIGDIR;
  sandboxLayoutFileName.push_back('/');
  sandboxLayoutFileName.append(CONFIG_DEFAULTBOXLAYOUTFILENAME);
  sandboxLayoutFileName = cfg.retrieveString("./sandboxLayoutFileName", sandboxLayoutFileName);
  
  std::cout << "Done reading in layout file." << std::endl; // MM: testing

  /* MM: think we can delete this whole section; will keep for now
  Math::Interval<double> elevationRange = cfg.retrieveValue<Math::Interval<double> >("./elevationRange",Math::Interval<double>(-1000.0,1000.0));
  bool haveHeightMapPlane=cfg.hasTag("./heightMapPlane");
  Plane heightMapPlane;
  if(haveHeightMapPlane)
  heightMapPlane=cfg.retrieveValue<Plane>("./heightMapPlane");
  unsigned int numAveragingSlots=cfg.retrieveValue<unsigned int>("./numAveragingSlots",30);
  unsigned int minNumSamples=cfg.retrieveValue<unsigned int>("./minNumSamples",10);
  unsigned int maxVariance=cfg.retrieveValue<unsigned int>("./maxVariance",2);
  float hysteresis=cfg.retrieveValue<float>("./hysteresis",0.1f); 
  Misc::FixedArray<unsigned int,2> wtSize;
  wtSize[0]=640;
  wtSize[1]=480;
  wtSize=cfg.retrieveValue<Misc::FixedArray<unsigned int,2> >("./waterTableSize",wtSize);
  waterSpeed=cfg.retrieveValue<double>("./waterSpeed",1.0);
  waterMaxSteps=cfg.retrieveValue<unsigned int>("./waterMaxSteps",30U);
  Math::Interval<double> rainElevationRange=cfg.retrieveValue<Math::Interval<double> >("./rainElevationRange",Math::Interval<double>(-1000.0,1000.0));
  rainStrength=cfg.retrieveValue<GLfloat>("./rainStrength",0.25f);
  double evaporationRate=cfg.retrieveValue<double>("./evaporationRate",0.0);
  float demDistScale=cfg.retrieveValue<float>("./demDistScale",1.0f);
  std::string controlPipeName=cfg.retrieveString("./controlPipeName","");
  */

  // Open the 3D camera device of the selected index
  Kinect::DirectFrameSource* realCamera = Kinect::openDirectFrameSource(cameraIndex);
  Misc::ConfigurationFileSection cameraConfigurationSection = cfg.getSection(cameraConfiguration.c_str());
  realCamera->configure(cameraConfigurationSection);
  camera = realCamera;
  
  std::cout << "Done opening camera." << std::endl; // MM: testing

  for(int i = 0 ; i < 2; ++i)
    frameSize[i] = camera->getActualFrameSize(Kinect::FrameSource::DEPTH)[i]; // 640 by 480
  std::cout << "Done updating frame size." << std::endl; // MM: testing
  
  // MM: the below line causes a seg fault. camera isn't NULL. modified Camera.cpp's method for quick fix
  // Get the camera's per-pixel depth correction parameters and evaluate it on the depth frame's pixel grid
  Kinect::FrameSource::DepthCorrection* depthCorrection = camera->getDepthCorrectionParameters();
  
  std::cout << "Done getting depth correction parameters." << std::endl; // MM: testing
  
  if(depthCorrection != 0) {
	pixelDepthCorrection = depthCorrection->getPixelCorrection(frameSize);
	delete depthCorrection;
  }
  else {
    // Create dummy per-pixel depth correction parameters
    pixelDepthCorrection = new PixelDepthCorrection[frameSize[1]*frameSize[0]];
    PixelDepthCorrection* pdcPtr=pixelDepthCorrection;
    for(unsigned int y = 0; y<frameSize[1]; ++y)
      for(unsigned int x = 0; x<frameSize[0]; ++x,++pdcPtr) {
	pdcPtr->scale = 1.0f;
	pdcPtr->offset = 0.0f; 
      }
  }
  std::cout << "Done fixing depth correction." << std::endl; // MM: testing
  
  
  // MM: the below line causes a seg fault. camera isn't NULL. 
  // Get the camera's intrinsic parameters
  cameraIps = camera->getIntrinsicParameters();

  std::cout << "Done getting camera's intrinsic parameters." << std::endl; // MM: testing
  
  // Start streaming depth frames
  camera->startStreaming(0, Misc::createFunctionCall(this, &KinectHandler::rawDepthFrameDispatcher));

  /* MM: we may not end up using / needing this
  // Create the frame filter object
  frameFilter = new FrameFilter(frameSize, numAveragingSlots, pixelDepthCorrection, cameraIps.depthProjection, basePlane);
  frameFilter->setValidElevationInterval(cameraIps.depthProjection, basePlane, elevationRange.getMin(), elevationRange.getMax());
  frameFilter->setStableParameters(minNumSamples, maxVariance);
  frameFilter->setHysteresis(hysteresis);
  frameFilter->setSpatialFilter(true);
  frameFilter->setOutputFrameFunction(Misc::createFunctionCall(this, &Sandbox::receiveFilteredFrame));
  */
}


// MM: we may not actually need a timerEvent. if we have our
//     Kinect::FrameSource* camera item, it's set up to call
//     rawDepthFrameDispatcher. frameFilter is set up to call receiveFilteredFrame
void KinectHandler::timerEvent(QTimerEvent *event) {
  std::cout << "In KinectHandler::timerEvent." << std::endl; // MM: testing
  //  std::cout << "Timer ID:" << event->timerId() << std::endl; // MM: testing

  float* f = frameBuffer.getData<GLfloat>();
  if (!frameBuffer.isValid())
    std::cout << "NOT VALID" << std::endl;
  for (int row = 0; row < 480; row = row+50) {    
    for (int col = 0; col < 640; col+=50) {

      //if ((row*640)+col < frameBuffer.getData<GLfloat>().size())
	std::cout << f[(row*640)+col] << "\t";
    }
    std::cout << std::endl;
  }
  //exit(0);
  
  
  //std::cout << std::endl << "DEPTH IMAGE" << std::endl << frameBuffer.getData<GLfloat>()[2] << std::endl << std::endl;
  // MM: frameBuffer.getData<GLfloat>() is a pointer to a list of 307200 floats (the frame is 480 by 640 in dimension)
/*  
  std::cout << event << std::endl;
  size_t depthsToDisplay[MAXROWS][MAXCOLS]; // initialize new array
  calcDepthsToDisplay(depthsToDisplay);     // calculate depth levels to display
  box->updateTextDisplay(depthsToDisplay);  // send Sandbox Window depth levels
  */
}


// CALC DEPTHS TO DISPLAY
// Calculates which depth levels to display at each row,col
void KinectHandler::calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]) {
  for (size_t row = 0; row < MAXROWS; row++) {
    for (size_t col = 0; col < MAXCOLS; col++) {
      //depthsToDisplay[row][col] = col < MAXCOLS / 3 ? 0 : 1;  // MM: testing
      depthsToDisplay[row][col] = currDepth;  // MM: testing
    }
  }
  currDepth++;
}

void KinectHandler::rawDepthFrameDispatcher(const Kinect::FrameBuffer& newFrameBuffer) {

  // MM: if we don't use frameFilter, here we can just analyze the framebuffer
  
  frameBuffer = newFrameBuffer;

  //std::cout << "In KinectHandler::rawDepthFrameDispatcher." << std::endl; // MM: testing
  //int i = 0;
  //i++;
  
  /* MM: following is original Sandbox.cpp code
  // Pass the received frame to the frame filter and the hand extractor
  if(frameFilter != 0 && !pauseUpdates)
    frameFilter->receiveRawFrame(frameBuffer);
  if(handExtractor != 0)
    handExtractor->receiveRawFrame(frameBuffer);
  */
}


//void KinectHandler::receiveFilteredFrame(const Kinect::FrameBuffer& frameBuffer) {

  /* MM: following is original Sandbox.cpp code
  // Put the new frame into the frame input buffer
  filteredFrames.postNewValue(frameBuffer);  
  // Wake up the foreground thread
  Vrui::requestUpdate();
  */
//}
