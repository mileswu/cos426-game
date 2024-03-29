#include <iostream>
#include "game.h"
#include "gl.h"
#include <stdlib.h>
#include <stdio.h>
#define SFML_STATIC
#include <SFML/Audio.hpp>
#include <time.h>

using namespace std;



int main(int argc, char **argv) {
  CreateGameWindow(argc, argv);
  
  
  
  sf::Music music;

  // http://offog.org/music/songs.html
  if (!music.openFromFile("audio/hello-world.ogg")) {
    printf("failed to find music\n");
    return EXIT_FAILURE;
  }

  int milisec = 20; // length of time to sleep, in miliseconds
  struct timespec ts = {0};
  ts.tv_sec = 0;
  ts.tv_nsec = milisec * 1000000L;

  // Play the music
  music.setLoop(true);
  music.play();
  for (float i = 0; i < 101; i++) {
    music.setVolume(i);
    nanosleep(&ts, (struct timespec *)NULL);
  }
  

  glutMainLoop();
  
  return 0;
}
