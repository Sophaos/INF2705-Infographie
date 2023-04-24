#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;
GLint locVertex;
GLint locColor;
GLint locmatrModel;
GLint locmatrVisu;
GLint locmatrProj;
GLint locplanCoupe;

GLuint vao[2];
GLuint vbo[3];

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
