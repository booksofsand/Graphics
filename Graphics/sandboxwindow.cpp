#include "sandboxwindow.h"
#include "helpers.h"
#include "kinecthandler.h"
#include <QGridLayout>
#include <QLabel>
#include <QtCore>
//#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/* MM: SANDBOX WINDOW OPTIONS:
    1) Have one label per row, containing one line of text.
       This way we could ensure the length of the line of text
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
       requested on 10/23). */


SandboxWindow::SandboxWindow(std::vector<std::string> filenames) {
    std::cout << "In SandboxWindow::SandboxWindow!" << std::endl;  // MM: testing

    // make a layout with blank labels
    grid = new QGridLayout;
    grid->setSpacing(0);
    grid->setMargin(0);
    grid->setContentsMargins(0,0,0,0);

    // read in source text
    if (filenames.size() == 0)  // no source text
        readLines("");
    else {                      // one or more source texts
        for (size_t depth = 0; depth < filenames.size(); depth++)
            readLines(filenames[depth], depth);
    }

    // build labels from depth 0 source grid
    for (size_t row = 0; row < MAXROWS; row++) {
        for (size_t col = 0; col < MAXCOLS; col++) {

            QLabel* label = new QLabel();
            label->setText(lines[0][row][col]);

            label->setMargin(0);                // MM: doesn't seem to do anything
            label->setContentsMargins(0,0,0,0); // MM: doesn't seem to do anything
            label->adjustSize();                // MM: doesn't seem to do anything
            label->setIndent(0);                // MM: doesn't seem to do anything
            label->setLineWidth(0);             // MM: doesn't seem to do anything
            label->setAlignment(Qt::AlignCenter);
            //label->setStyleSheet("border: 0.5px solid red"); // MM: for test viewing

            depthsDisplayed[row][col] = 0;        // save depth number displayed
            grid->addWidget(label, row, col, 1, 1);
        }
    }

    /*
    // build labels from depth 0 source grid
    for (size_t row = 0; row < MAXROWS; row++) {
        QLabel* label = new QLabel();
        label->setText(lines[0][row]);
        grid->addWidget(label, row, 0, 1, 1);
    }
    */

    // make & config a window
    QWidget* win = new QWidget();
    win->resize(480,640);                 // set dimensions
    win->setContentsMargins(5,5,5,5);     // set margins
    win->setLayout(grid);                 // add layout
    win->setWindowTitle("Books of Sand"); // set title
    win->show();                          // display window

    // start an event handler object for Kinect
    //kinectHandler = new KinectHandler();
    kinectHandler = new KinectHandler(this);
    kinectHandler->exec();
}


// READLINES
// Read input text file into grid at given depth (default 0);
// Wrap text around to next depth if needed
void SandboxWindow::readLines(std::string filename, size_t depth) {
    std::cout << "In SandboxWindow::readLines!" << std::endl;   // MM: testing

    // initialize empty SandboxWindow if no filename
    if (filename == "") {
        notify("No input filename detected; initializing empty window.");
        for (size_t row = 0; row < MAXROWS; row++) {
            for (size_t col = 0; col < MAXCOLS; col++)
                lines[depth][row][col] = QChar();     // fill single depth
	      //lines[depth][row] = QChar();     // fill single depth
        }
        return;
    }

    // open infile
    std::fstream infile(filename.c_str());
    if(!infile.good())
        error("Input file " + filename + " can't be opened."); // exits

    // build source text grid line by line
    size_t row = 0;
    size_t MAXLENGTH = 64; // figure out how to calc; no hardcoding?
    while(infile.good()) {

        // build one line of text
        //QString line = "";
        //while(line.size() < MAXLENGTH && infile.good()) {
        size_t col = 0;
	while(col < MAXLENGTH && infile.good()) {
            char c = infile.get();
            if(c == '\n' || c == EOF)   // move to next row for new lines
                break;
            //line.append(QChar(c));
            lines[depth][row][col] = QChar(c);
            col++;
            //std::cout << c;                                // MM: testing
            //std::cout << line.toStdString() << std::endl;  // MM: testing
        }
        //lines[depth][row] = line;
        //std::cout << std::endl;    // MM: testing

        // update row, depth
        row++;
        if(row == MAXROWS) { // start a new depth
            row = 0;
            depth++;
            std::cout << "Increasing depth; now " << depth << std::endl; // MM: testing
        }
    }

    //for(size_t row = 0; row < MAXROWS; row++)                   // MM: testing
    //    std::cout << lines[0][row].toStdString() << std::endl;  // MM: can't cout a QString
}


// SETTEXT
// Set text of label at given row and col
void SandboxWindow::setText(QString text, size_t row, size_t col) {
  //  std::cout << "In SandboxWindow::setText!" << std::endl;  // MM: testing

  QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, col)->widget());
  label->setText(text);
}


// UPDATEDEPTHDISPLAY
// Given updated depth map, update displayed text
void SandboxWindow::updateDepthDisplay(size_t** depthsToDisplay) {
  std::cout << "In SandboxWindow::updateDepthDisplay!" << std::endl;  // MM: testing

  for (size_t row = 0; row < MAXROWS; row++) {
    for (size_t col = 0; col < MAXCOLS; col++) {
      if (depthsDisplayed[row][col] != depthsToDisplay[row][col]) {
	size_t newDepth = depthsToDisplay[row][col];
	setText(lines[newDepth][row][col], row, col);
	depthsDisplayed[row][col] = newDepth;
      }
    }
  }
}


void SandboxWindow::switchToDepth(size_t depth) { // MM: good for testing, at least
    std::cout << "In SandboxWindow::switchToDepth!" << std::endl;  // MM: testing

    for (size_t row = 0; row < MAXROWS; row++) {
        for (size_t col = 0; col < MAXCOLS; col++) {
            QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, col)->widget());
            //label->setText(lines[depth][row]);
            label->setText(lines[depth][row][col]);
        }
    }
}
