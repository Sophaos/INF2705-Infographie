#ifndef __ETAT_H__
#define __ETAT_H__

#include "Singleton.h"

//
// variables d'état
//
class Etat : public Singleton<Etat>
{
    SINGLETON_DECLARATION_CLASSE(Etat);
public:
    static float xrot;
    static bool enmouvement;  // le modèle est en mouvement/rotation automatique ou non
    static bool afficheAxes; // indique si on affiche les axes
    static bool avecMurs;
    static bool avecCube;
    static bool avecClip;
    static bool avecStencil;
    static bool avecLimite;
    static float miroirX;
};

#endif
