#include "sandboxwindow.h"
#include <QGridLayout>
#include <QLabel>
//#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

SandboxWindow::SandboxWindow(std::string filename) {
    std::cout << "In SandboxWindow::SandboxWindow!" << std::endl;

    // make a layout with blank labels
    grid = new QGridLayout;
    grid->setVerticalSpacing(5);

    /* MM: Trying instead of row,col cells, just have one label per row
           with multiple words on it. Might be easier than handling
           the stretch of some words being too long to fit in a cell.
           May also make it more difficult to update the window
           as different coordinates have changing depths. */

    // read in source text
    readLines(filename);

    // build labels from depth 0 source grid
    for (int row = 0; row < 32; row++) {
        QLabel* label = new QLabel();
        label->setText(lines[0][row]);
        grid->addWidget(label, row, 0, 1, 1);
    }

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
    std::cout << "In SandboxWindow::readLines!" << std::endl;

    // open infile
    std::fstream infile(filename.c_str());    // convert input to c-string
    if(!infile.good()) {
      std::cout << "Input file " << filename << " can't be opened. :(" << std::endl;
      exit(1);
    }

    // build source text grid line by line
    size_t row = 0;
    size_t depth = 0;
    int MAXLENGTH = 64; // figure out how to calc; no hardcoding
    while(infile.good()) {
        //std::cout << "Building a new line: ";

        // build one line of text
        QString line = "";
        while(line.size() < MAXLENGTH && infile.good()) {
            char c = infile.get();
            if(c == '\n')          // move to next row for new lines
                break;
            line.append(QChar(c));
            //std::cout << c;
            //std::cout << line.toStdString() << std::endl;
        }
        lines[depth][row] = line;
        //std::cout << std::endl;

        // update row, depth
        row++;
        if(row == MAXROWS) { // start a new depth
            row = 0;
            depth++;
            std::cout << "Increasing depth; now " << depth << std::endl;
        }
    }

    //for(size_t row = 0; row < MAXROWS; row++)
    //    std::cout << lines[0][row].toStdString() << std::endl;  // MM: can't cout a QString
}

void SandboxWindow::switchToDepth(int depth) { // MM: good for testing, at least
    //std::cout << "In SandboxWindow::switchToDepth!" << std::endl;

    for (int row = 0; row < 32; row++) {
        QLabel* label = dynamic_cast<QLabel*>(grid->itemAtPosition(row, 0)->widget());
        label->setText(lines[depth][row]);
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
