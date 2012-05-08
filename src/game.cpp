#include <iostream>
#include "game.h"
#include "gl.h"

using namespace std;

int main(int argc, char **argv) {
  cout << "Hi" << endl;
  
  CreateWindow();
  glutMainLoop();
  
  return 0;
}