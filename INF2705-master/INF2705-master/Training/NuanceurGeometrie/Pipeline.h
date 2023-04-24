#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;
GLint locVertex;
GLint locColor;
GLint locmatrModel;
GLint locmatrVisu;
GLint locmatrProj;

GLuint vao[2];   // deux VAO, un pour chaque figure tracée
GLuint vbo[5];   // les VBO de cet exemple

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
