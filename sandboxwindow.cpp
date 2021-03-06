#include "sandboxwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QtCore>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

/* MM: SANDBOX WINDOW STRATEGIES:
   1) Have one label per row, containing one line of text.
   This way we can ensure the length of the line of text
   doesn't go over some set width. However, this may make it
   more difficult to update the window as different coordinates
   have changing depths, since we'd have the rebuild the lines
   with string manipulation.

   2) Have one label per row,col, containing one character of text.
   This simplifies the depth changes, and we can assert a general
   limit on width based on number of columns allowed. Currently,
   though, the cells don't fit exactly to their characters, so
   the text spacing looks pretty wide. But this would also allow
   us to color parts of the display based on depth (like Andrew
   requested on 10/23). 
   //LJ  I think this will serve us better in the long run

   3) COMPROMISE: Store characters in rows and cols but display
   with one label per row. This makes the text display nicely with
   no strange spacing (though doesn't allow coloring) but is easy
   to update per-character changes upon receiving new depth maps. 
   MM: ^ currently implemented
*/


// SANDBOX WINDOW
// Initialize a SandboxWindow with text
SandboxWindow::SandboxWindow(std::vector<std::string> filenames) {
  
  // MAKE A LAYOUT
  grid = new QGridLayout;
  grid->setSpacing(0);   // set spacing on all sides

  // READ IN SOURCE TEXT(S)
  if (filenames.size() == 0)  // no source text
    readLines("");
  else {                      // one or more source texts
    for (size_t depth = 0; depth < filenames.size(); depth++)
      readLines(filenames[depth], depth);
  }

  /* ROWS AND COLS STORAGE; ROWS ONLY DISPLAY */
  // build labels from depth 0 source grid
  QFont myFont("times", 22);
  //QFontMetrics fm(myFont);
  size_t depthsToDisplay[MAXROWS][MAXCOLS] = {{0}}; // init depths to display
  for (size_t row = 0; row < MAXROWS; row++) {
    QLabel* label = new QLabel();
    label->setFont(myFont);
    grid->addWidget(label, row, 0, 1, 1);
    for (size_t col = 0; col < MAXCOLS; col++)
      depthsDisplayed[row][col] = -1;         // initialize all depthsDisplayed to -1
    //std::cout << "line width: " << fm.width(currLine) << std::endl; // MM: testing
    // MM: may want to play with fonts later, remove maximum col and measure line size
  }
  updateTextDisplay(depthsToDisplay);

  // make & config a window
  QWidget* win = new QWidget();
  win->resize(480,640);                 // set dimensions
  win->setContentsMargins(5,5,5,5);     // set margins
  win->setLayout(grid);                 // add layout
  win->setWindowTitle("Books of Sand"); // set title
  //win->show();                          // display window


  // ROTATION
  // MM: this is just a prelim addition for rotating the window to match
  //     the sandbox orientation. needs work. better way to flip display?
  //     to stop rotation, comment out this block and uncomment win->show()
  QGraphicsScene* scene = new QGraphicsScene();
  QGraphicsView* view = new QGraphicsView();
  view->setGeometry(0,0,1280,960);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  QGraphicsProxyWidget* proxy = scene->addWidget(win);
  view->setScene(scene);
  view->show();
  view->rotate(270);  // degrees clockwise
  

  // start an event handler object for Kinect
  kinectHandler = new KinectHandler(this);
  kinectHandler->exec();
}


// READ LINES
// Read input text file into grid at given depth (default 0);
// Wrap text around to next depth if needed
void SandboxWindow::readLines(std::string filename, size_t depth) {

  // INITIALIZE EMPTY SANDBOX WINDOW IF NO FILENAME
  if (filename == "") {
    notify("No input filename detected; initializing empty window.");
    for (size_t row = 0; row < MAXROWS; row++) {
      for (size_t col = 0; col < MAXCOLS; col++)
	sourceText[depth][row][col] = QChar();     // fill single depth
    }
    return;
  }

  // OPEN INFILE
  std::fstream infile(filename.c_str());
  if(!infile.good())
    error("Input file " + filename + " can't be opened."); // exits

  // BUILD SOURCE TEXT LIBRARY
  size_t row = 0;
  while(depth < MAXDEPTH && infile.good()) {

    // build one line of text
    size_t col = 0;
    while(col < MAXCOLS && infile.good()) {
      char c = infile.get();
      if(c == '\n' || c == EOF)   // move to next row for new lines
	break;
      sourceText[depth][row][col] = QChar(c);
      col++;
    }

    // update row, depth
    row++;
    if(row == MAXROWS) { // start a new depth
      row = 0;
      depth++;
    }
  }
}


// UPDATE TEXT DISPLAY
// Given updated depth map, update displayed text
void SandboxWindow::updateTextDisplay(size_t depthsToDisplay[MAXROWS][MAXCOLS]) {

  for (size_t row = 0; row < MAXROWS; row++) {

    // build line of text
    QString currLine = "";
    for (size_t col = 0; col < MAXCOLS; col++) {

      // update character to be displayed
      if (depthsDisplayed[row][col] != depthsToDisplay[row][col]) {
	size_t newDepth = depthsToDisplay[row][col];
	currLine.append(sourceText[newDepth][row][col]);  // add char for new depth
	depthsDisplayed[row][col] = newDepth;        // save depth number displayed
      }

      // add current character
      else
	currLine.append(sourceText[depthsDisplayed[row][col]][row][col]);
    }
    // set row label text
    QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, 0)->widget());
    label->setText(currLine);
  }
}


void SandboxWindow::switchToDepth(size_t depth) {     // MM: method for testing
  
  for (size_t row = 0; row < MAXROWS; row++) {
    for (size_t col = 0; col < MAXCOLS; col++) {
      QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, col)->widget());
      label->setText(sourceText[depth][row][col]);
    }
  }
}
