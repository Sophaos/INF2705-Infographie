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
    static bool modeSelection;       // on est en mode sélection?
    static bool enmouvement;         // le modèle est en mouvement/rotation automatique ou non
    static bool afficheAxes;         // indique si on affiche les axes
    static bool attenuation;      // indique si on veut atténuer selon l'éloignement
    static glm::vec4 bDim;           // les dimensions de l'aquarium: une boite [-x,+x][-y,+y][-z,+z]
    static glm::ivec2 sourisPosPrec;
    // partie 1: utiliser des plans de coupe:  Ax + By + Cz + D = 0  <=>  Ax + By + Cz = -D
    static glm::vec4 planRayonsX;    // équation du plan de rayonsX (partie 1)
    static glm::vec4 planDragage;    // équation du plan de dragage (partie 1)
    static GLfloat angleDragage;     // angle (degrés) du plan de dragage autour de y (partie 1)
};

#endif
