/********************************************************************
File: graphics.cc
Name: Lia Jundt, Maya Montgomery, Eseosa Asiruwa
Date: 10/19/17
Description: Make a graphics window that displays words
********************************************************************/
#include <iostream>
#include <stdlib.h>
#include <QWindow>

using namespace std;

bool display(QWindow win, string word, int xcoord, int ycoord, bool OF)
{
  if(OF){
    //display word at x,y on window
    //if no error return true, false if error
  }
  return true;
}

int main()
{
  QWindow win = new QWindow();
  string w = "hakuna matata";
  display(win, w, 50, 50);
}
