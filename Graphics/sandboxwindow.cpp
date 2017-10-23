#include "sandboxwindow.h"
#include <QGridLayout>
#include <QLabel>
//#include <cstdlib>
#include <string>

SandboxWindow::SandboxWindow() {
    // make a layout with blank labels
    grid = new QGridLayout;
    //grid->setVerticalSpacing(5);

    /* MM: Maybe instead of row,col cells, just have one label per row
           with multiple words on it? That might be easier than handling
           the stretch of some words being too long to fit in a cell.
           But that may also make it more difficult to update the window
           as different coordinates have changing depths... */

    for (int row = 0; row < 32; row++) {     // MM: how to decide number of cells?
        for (int col = 0; col < 10; col++) { // window can't fit many words at once
            QLabel* label = new QLabel();
            label->setText("sandbox");    // sample text for now
            grid->addWidget(label, row, col, 1, 1);
        }
    }

    // make & config a window
    QWidget* win = new QWidget();
    win->resize(480,640);                 // set dimensions
    win->setContentsMargins(5,5,5,5);     // set margins
    win->setLayout(grid);                 // add layout
    win->setWindowTitle("Books of Sand"); // set title
    win->show();                          // display window

    addWord("NEW", 15, 5, 0);             // set a word (test)
    removeWord(5,5,0);                    // remove a word (test)
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
