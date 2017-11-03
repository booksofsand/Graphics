#include "kinecthandler.h"
#include "sandboxwindow.h"
#include <iostream>
#include <QTimerEvent>

KinectHandler::KinectHandler(SandboxWindow* theBox) : QEventLoop(0) { // MM: 0 = parent
  box = theBox;
  startTimer(3000);   // 3-second timer

  currDepth = 0; // MM: testing only  

  /* MM: following from Sandbox.cpp. requires Vrui and Kinect

  // MM: we'll want to keep some of this init code
  // Read the sandbox's default configuration parameters
  std::string sandboxConfigFileName=CONFIG_CONFIGDIR;
  sandboxConfigFileName.push_back('/');
  sandboxConfigFileName.append(CONFIG_DEFAULTCONFIGFILENAME);
  Misc::ConfigurationFile sandboxConfigFile(sandboxConfigFileName.c_str());
  Misc::ConfigurationFileSection cfg=sandboxConfigFile.getSection("/SARndbox");
  unsigned int cameraIndex=cfg.retrieveValue<int>("./cameraIndex",0);
  std::string cameraConfiguration=cfg.retrieveString("./cameraConfiguration","Camera");
  double scale=cfg.retrieveValue<double>("./scaleFactor",100.0);
  std::string sandboxLayoutFileName=CONFIG_CONFIGDIR;
  sandboxLayoutFileName.push_back('/');
  sandboxLayoutFileName.append(CONFIG_DEFAULTBOXLAYOUTFILENAME);
  sandboxLayoutFileName=cfg.retrieveString("./sandboxLayoutFileName",sandboxLayoutFileName);
  Math::Interval<double> elevationRange=cfg.retrieveValue<Math::Interval<double> >("./elevationRange",Math::Interval<double>(-1000.0,1000.0));
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

// Start reading in input

  // Start streaming depth frames
  camera->startStreaming(0, Misc::createFunctionCall(this, &KinectHandler::rawDepthFrameDispatcher));

  // Create the frame filter object (MM: we may not end up using / needing this)
  frameFilter=new FrameFilter(frameSize, numAveragingSlots, pixelDepthCorrection, cameraIps.depthProjection, basePlane);
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

  std::cout << event << std::endl;
  size_t depthsToDisplay[MAXROWS][MAXCOLS]; // initialize new array
  calcDepthsToDisplay(depthsToDisplay);     // calculate depth levels to display
  box->updateTextDisplay(depthsToDisplay);  // send Sandbox Window depth levels
    
  // TODO: pass frame through filtering
  // TODO: pass frame to SandboxWindow to find new depths & update window
}


// CALC DEPTHS TO DISPLAY
// Calculates which depth levels to display at each row,col
void KinectHandler::calcDepthsToDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]) {
  for (size_t row = 0; row < MAXROWS; row++) {
    for (size_t col = 0; col < MAXCOLS; col++) {
      depthsToDisplay[row][col] = col < MAXCOLS / 3 ? 0 : 1;  // MM: testing
    }
  }

}

//void KinectHandler::rawDepthFrameDispatcher(const Kinect::FrameBuffer& frameBuffer) {

  // MM: if we don't use frameFilter, here we can just analyze the framebuffer

  

  /* MM: following is original Sandbox.cpp code
  // Pass the received frame to the frame filter and the hand extractor
  if(frameFilter != 0 && !pauseUpdates)
    frameFilter->receiveRawFrame(frameBuffer);
  if(handExtractor != 0)
    handExtractor->receiveRawFrame(frameBuffer);
  */
//}


//void KinectHandler::receiveFilteredFrame(const Kinect::FrameBuffer& frameBuffer) {

  /* MM: following is original Sandbox.cpp code
  // Put the new frame into the frame input buffer
  filteredFrames.postNewValue(frameBuffer);  
  // Wake up the foreground thread
  Vrui::requestUpdate();
  */
//}
