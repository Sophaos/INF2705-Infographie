#include "Etat.h"

SINGLETON_DECLARATION_CPP(Etat);

bool         Etat::enmouvement = true;
bool         Etat::impression = false;
bool         Etat::afficheAxes = true;
GLenum       Etat::modePolygone = GL_FILL;
unsigned int Etat::nparticules = 400;
float        Etat::tempsDeVieMax = 10.0;
float        Etat::temps = 0.0;
float        Etat::dt = 1.0/60.0;
float        Etat::gravite = 0.3;
int          Etat::texnumero = 1;
GLuint       Etat::textureETINCELLE = 0;
GLuint       Etat::textureOISEAU = 0;
GLuint       Etat::textureBONHOMME = 0;
bool         Etat::boucleTexture = false;
glm::ivec2   Etat::sourisPosPrec = glm::ivec2(0);
int          Etat::modele = 1;
glm::vec3    Etat::posPuits = glm::vec3( 0.0, 0.0, 0.0 );
glm::vec3    Etat::bDim = glm::vec3( 2.2, 1.5, 2.1 );
AffStereo    Etat::affichageStereo = affMono;
float        Etat::pointsize = 5.0;
//float        Etat::pointsize = 9.0; // pour mieux voir les points au lab
