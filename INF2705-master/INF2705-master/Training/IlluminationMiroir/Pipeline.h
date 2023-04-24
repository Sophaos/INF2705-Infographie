#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;
GLint locVertex;
GLint locNormal;
GLint locColor;
GLint locmatrModel;
GLint locmatrVisu;
GLint locmatrProj;
GLint locmatrNormale;
GLint locplanCoupe;
GLuint indLightSource;
GLuint indFrontMaterial;
GLuint indLightModel;
GLuint progBase;  // le programme de nuanceurs de base
GLint locVertexBase = -1;
GLint locColorBase = -1;
GLint locmatrModelBase = -1;
GLint locmatrVisuBase = -1;
GLint locmatrProjBase = -1;

GLuint vao[2];
GLuint vbo[4];
GLuint ubo[3];

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
