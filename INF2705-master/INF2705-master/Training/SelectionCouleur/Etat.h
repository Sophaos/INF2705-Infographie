#ifndef __ETAT_H__
#define __ETAT_H__

#include <glm/glm.hpp>
#include "Singleton.h"

//
// variables d'état
//
class Etat : public Singleton<Etat>
{
    SINGLETON_DECLARATION_CLASSE(Etat);
public:
    static int modeSelection; // on est en mode sélection?
    static glm::ivec2 sourisPosPrec;
};

#endif
