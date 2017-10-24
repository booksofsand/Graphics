#include "sandboxwindow.h"
#include <QGridLayout>
#include <QLabel>
//#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

SandboxWindow::SandboxWindow(std::string filename) {
    std::cout << "In SandboxWindow::SandboxWindow!" << std::endl;  // MM: testing

    // make a layout with blank labels
    grid = new QGridLayout;
    grid->setSpacing(0);
    grid->setMargin(0);
    grid->setContentsMargins(0,0,0,0);

    /* MM: OPTIONS:
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

    // read in source text
    readLines(filename);

    // build labels from depth 0 source grid
    for (int row = 0; row < MAXROWS; row++) {
        for (int col = 0; col < MAXCOLS; col++) {
            QLabel* label = new QLabel();
            //label->setMargin(0);                // MM: doesn't seem to do anything
            //label->setContentsMargins(0,0,0,0); // MM: doesn't seem to do anything
            label->setText(lines[0][row][col]);
            //label->setStyleSheet("border: 1px solid red"); // MM: for test viewing
            grid->addWidget(label, row, col, 1, 1);
        }
    }
    /* MM: commenting out to try cols again
    // build labels from depth 0 source grid
    for (int row = 0; row < 32; row++) {
        QLabel* label = new QLabel();
        label->setText(lines[0][row]);
        grid->addWidget(label, row, 0, 1, 1);
    } */

    // make & config a window
    QWidget* win = new QWidget();
    win->resize(480,640);                 // set dimensions
    win->setContentsMargins(5,5,5,5);     // set margins
    win->setLayout(grid);                 // add layout
    win->setWindowTitle("Books of Sand"); // set title
    win->show();                          // display window

    //addWord("NEW", 15, 5, 0);             // set a word (test)
    //removeWord(5,5,0);                    // remove a word (test)
}

void SandboxWindow::readLines(std::string filename) {
    std::cout << "In SandboxWindow::readLines!" << std::endl;   // MM: testing

    // open infile
    std::fstream infile(filename.c_str());    // convert input to c-string
    if(!infile.good()) {
      std::cout << "Input file " << filename << " can't be opened. :(" << std::endl;
      exit(1);
    }

    // build source text grid line by line
    size_t row = 0;
    size_t depth = 0;
    size_t MAXLENGTH = 64; // figure out how to calc; no hardcoding
    while(infile.good()) {

        // build one line of text
        size_t col = 0;
        //QString line = "";
        //while(line.size() < MAXLENGTH && infile.good()) {
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

void SandboxWindow::switchToDepth(int depth) { // MM: good for testing, at least
    //std::cout << "In SandboxWindow::switchToDepth!" << std::endl;  // MM: testing

    for (size_t row = 0; row < MAXROWS; row++) {
        for (size_t col = 0; col < MAXCOLS; col++) {
            QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, col)->widget());
            //label->setText(lines[depth][row]);
            label->setText(lines[depth][row][col]);
        }
    }
}

void SandboxWindow::addWord(QString word, int row, int col, int depth) {
    depth++; // stop giving me an unused parameter warning; implement later

    QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, col)->widget());
    label->setText(word);
}

void SandboxWindow::removeWord(int row, int col, int depth) {
    depth++; // stop giving me an unused parameter warning; implement later

    QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, col)->widget());
    label->setText("");
}
