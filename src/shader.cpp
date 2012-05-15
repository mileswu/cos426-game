#include "shader.h"
#include "gl.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Shader::Shader(const char *name) {
  stringstream ss_f, ss_v;
  ss_f << "shaders/" << name << ".frag";
  ss_v << "shaders/" << name << ".vert";
  
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  vertex = glCreateShader(GL_VERTEX_SHADER);
  
  ifstream fragment_file (ss_f.str().c_str(), ios::in | ios::binary | ios::ate);
  int fragment_file_size = fragment_file.tellg();
  fragment_file.seekg(0, ios::beg);
  char *fragment_source = (char *)malloc(fragment_file_size);
  fragment_file.read(fragment_source, fragment_file_size);
  fragment_file.close();
  
  glShaderSource(fragment, 1, (const GLchar**)&fragment_source, &fragment_file_size);
  glCompileShader(fragment);
  
  ifstream fragment_file2 (ss_v.str().c_str(), ios::in | ios::binary | ios::ate);
  int fragment_file_size2 = fragment_file2.tellg();
  fragment_file2.seekg(0, ios::beg);
  char *fragment_source2 = (char *)malloc(fragment_file_size2);
  fragment_file2.read(fragment_source2, fragment_file_size2);
  fragment_file2.close();
  
  glShaderSource(vertex, 1, (const GLchar**)&fragment_source2, &fragment_file_size2);
  glCompileShader(vertex);
  
  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  
  int retval, retval2;
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &retval);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &retval2);
  if(retval != GL_TRUE) {
    cout << "Shader did not compile correctly" << endl;
    GLint blen = 0;	
    GLsizei slen = 0;

    glGetShaderiv(fragment, GL_INFO_LOG_LENGTH , &blen);       
    if (blen > 1)
    {
     GLchar* compiler_log = (GLchar*)malloc(blen);
     glGetInfoLogARB(fragment, blen, &slen, compiler_log);
     cout << "compiler_log:\n" << compiler_log << endl;
     free (compiler_log);
    }
    exit(1);
  }
  if(retval2 != GL_TRUE) {
    cout << "Shader did not compile correctly" << endl;
    GLint blen = 0;	
    GLsizei slen = 0;

    glGetShaderiv(vertex, GL_INFO_LOG_LENGTH , &blen);       
    if (blen > 1)
    {
     GLchar* compiler_log = (GLchar*)malloc(blen);
     glGetInfoLogARB(vertex, blen, &slen, compiler_log);
     cout << "compiler_log:\n" << compiler_log << endl;
     free (compiler_log);
    }
    exit(1);
  }
}
