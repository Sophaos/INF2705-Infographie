#ifndef __PIPELINE_H__
#define __PIPELINE_H__

GLuint prog;
GLint locVertex;
GLint locColor;
GLint locmatrModel;
GLint locmatrVisu;
GLint locmatrProj;

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
