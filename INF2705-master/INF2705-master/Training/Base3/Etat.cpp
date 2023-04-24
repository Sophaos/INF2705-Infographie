#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

bool Etat::enPerspective = true;
bool Etat::enmouvement = true;
GLdouble Etat::rtri = 0.;
GLdouble Etat::rquad = 0.;
int Etat::solution = 1;
