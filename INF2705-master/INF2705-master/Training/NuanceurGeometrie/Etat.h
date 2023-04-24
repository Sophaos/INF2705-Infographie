#ifndef __ETAT_H__
#define __ETAT_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Singleton.h"

//
// variables d'état
//
class Etat : public Singleton<Etat>
{
    SINGLETON_DECLARATION_CLASSE(Etat);
public:
    static bool enmouvement;     // le modèle est en mouvement/rotation automatique ou non
    static GLdouble rtri;
    static GLdouble rquad;
    static int nvues;            // nombre de viewports (1, 2, 4)
};

#endif
