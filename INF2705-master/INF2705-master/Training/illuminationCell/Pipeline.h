#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// variables pour l'utilisation des nuanceurs
GLuint prog;      // votre programme de nuanceurs
GLint locVertex;
GLint locmatrModel;
GLint locmatrVisu;
GLint locmatrProj;
GLint locmatrNormale;
GLint loclumiPosition;

// matrices du pipeline graphique
MatricePipeline matrModel, matrVisu, matrProj;

#endif
