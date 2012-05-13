#include "fbo.h"
#include <iostream>

Framebuffer::Framebuffer(int w, int h) {
  glGenFramebuffers(1, &framebuffer);
  glGenTextures(1, &texture);
  glGenRenderbuffers(1, &renderbuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, w, h, 0, GL_RGBA, GL_HALF_FLOAT_ARB, NULL);  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, 0); 
   
}

