#include <QApplication>
#include <QLabel>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    // make an application
    QApplication a(argc, argv);

    // make a layout & labels
    QGridLayout* grid = new QGridLayout;
    QLabel* label1 = new QLabel();
    label1->setText("books");
    QLabel* label2 = new QLabel();
    label2->setText("of");
    QLabel* label3 = new QLabel();
    label3->setText("sand");

    // add labels to layout
    grid->addWidget(label1, 0, 0, 1, 1);  // object, row, col, rowspan, colspan
    grid->addWidget(label2, 1, 1, 1, 1);
    grid->addWidget(label3, 2, 2, 1, 1);

    // make & config a window
    QWidget* win = new QWidget();
    win->resize(480,640);                 // set dimensions
    win->setContentsMargins(5,5,5,5);     // set margins
    win->setLayout(grid);                 // add layout
    win->setWindowTitle("Books of Sand"); // set title
    win->show();                          // display window

    // return the executable
    return a.exec();
}
