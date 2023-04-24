#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

float Etat::xrot = 0.0;
bool Etat::enmouvement = true;
bool Etat::afficheAxes = true;
bool Etat::avecMurs = true;
bool Etat::avecCube = true;
bool Etat::avecClip = true;
bool Etat::avecStencil = true;
bool Etat::avecLimite = true;
float Etat::miroirX = -2.0;
