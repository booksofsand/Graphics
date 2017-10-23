#ifndef SANDBOXWINDOW_H
#define SANDBOXWINDOW_H

#include <string>
#include <QGridLayout>

class SandboxWindow
{
public:
    SandboxWindow();
    void addWord(QString word, int row, int col, int depth);
    void removeWord(int row, int col, int depth);

private:
    QGridLayout* grid;  // grid that actually displays
    //QString words[];    // array of words at different rows, cols, depths
    // MM: ^ not sure what format we want
};

#endif // SANDBOXWINDOW_H
