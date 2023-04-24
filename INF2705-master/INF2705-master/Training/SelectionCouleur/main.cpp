#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "inf2705-matrice.h"
#include "inf2705-nuanceur.h"
#include "inf2705-fenetre.h"
#include "inf2705-forme.h"
#include "Etat.h"
#include "Pipeline.h"
#include "Camera.h"

// les formes
FormeSphere *sphere = NULL;
FormeCube *cube = NULL;

void chargerNuanceurs()
{
    // créer le programme
    prog = glCreateProgram();

    // attacher le nuanceur de sommets
    {
        GLuint nuanceurObj = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( nuanceurObj, 1, &ProgNuanceur::chainesSommetsMinimal, NULL );
        glCompileShader( nuanceurObj );
        glAttachShader( prog, nuanceurObj );
        ProgNuanceur::afficherLogCompile( nuanceurObj );
    }
    // attacher le nuanceur de fragments
    {
        GLuint nuanceurObj = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( nuanceurObj, 1, &ProgNuanceur::chainesFragmentsMinimal, NULL );
        glCompileShader( nuanceurObj );
        glAttachShader( prog, nuanceurObj );
        ProgNuanceur::afficherLogCompile( nuanceurObj );
    }
    // faire l'édition des liens du programme
    glLinkProgram( prog );
    ProgNuanceur::afficherLogLink( prog );

    // demander la "Location" des variables
    if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
    if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
    if ( ( locmatrModel = glGetUniformLocation( prog, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
    if ( ( locmatrVisu = glGetUniformLocation( prog, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
    if ( ( locmatrProj = glGetUniformLocation( prog, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
}

// initialisation d'openGL
void FenetreTP::initialiser()
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); // fond noir

    // activer le tampon de profondeur
    glEnable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    chargerNuanceurs();

    // créer quelques autres formes
    glUseProgram( prog );
    sphere = new FormeSphere( 4.0, 24, 20 );
    cube = new FormeCube( 6.0 );
}

void FenetreTP::conclure()
{
    delete sphere;
    delete cube;
}

void selectionner()
{
    // s'assurer que toutes les opérations sont terminées
    glFinish();

    // obtenir la clôture et calculer la position demandée
    GLint cloture[4]; glGetIntegerv( GL_VIEWPORT, cloture );
    GLint posX = Etat::sourisPosPrec.x, posY = cloture[3]-Etat::sourisPosPrec.y;

    // dire de lire le tampon arrière où l'on vient tout juste de dessiner
    glReadBuffer( GL_BACK );

    // obtenir la couleur
    GLubyte couleur[3];
    glReadPixels( posX, posY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, couleur );
    std::cout << "couleur = " << (int) couleur[0] << " " << (int) couleur[1] << " " << (int) couleur[2] << std::endl;

    // obtenir la profondeur (accessoirement)
    GLfloat profondeur;
    glReadPixels( posX, posY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &profondeur );
    std::cout << "profondeur = " << profondeur << std::endl;

    // la couleur lu indique l'objet sélectionné
    if ( couleur[1] != 0 )
        std::cout << "\tobjet = CUBE " << couleur[1] / 50 << std::endl;
    else if ( couleur[0] != 0 )
        std::cout << "\tobjet = SPHERE " << couleur[0] / 50 << std::endl;
}

// fonction d'affichage
void FenetreTP::afficherScene()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // définir la projection
    matrProj.Ortho( -8.0, 8.0, -8.0, 8.0, -15.0, 15.0 );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );

    matrVisu.LookAt( camera.dist*cos(glm::radians(camera.theta))*sin(glm::radians(camera.phi)),
                     camera.dist*sin(glm::radians(camera.theta))*sin(glm::radians(camera.phi)),
                     camera.dist*cos(glm::radians(camera.phi)),
                     0, 0, 0, 0, 0, 1 );
    glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );

    matrModel.LoadIdentity();
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    // afficher la sphère
    glVertexAttrib3f( locColor, 1, 0, 1 ); // du magenta
    matrModel.Translate( -2.0, 0.0, -2.0 );
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    sphere->afficher();

    // afficher le cube
    if ( !Etat::modeSelection ) { glEnable( GL_BLEND ); glDepthMask( GL_FALSE ); }

    glVertexAttrib4f( locColor, 0, 1, 0, 0.3 ); // du vert
    matrModel.Rotate( -45.0, 1.0, 1.0, 0.0 );
    matrModel.Translate( 5.0, -4.0, 0.0 );
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    cube->afficher();

    if ( !Etat::modeSelection ) { glDisable( GL_BLEND ); glDepthMask( GL_TRUE ); }

    // sélectionner ?
    if ( Etat::modeSelection == 1 )
    {
        selectionner();

        // la sélection a été faite
        Etat::modeSelection = 0;

        // pas d'appel à swap(): on ne veut pas montrer ce qu'on vient de tracer pour la sélection
    }
    else
        // permuter plans avant et arrière
        swap();
}

void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
    glViewport( 0, 0, w, h );
}

void FenetreTP::clavier( TP_touche touche )
{
    switch ( touche )
    {
    case TP_ECHAP:
    case TP_q: // Quitter l'application
        quit();
        break;
    case TP_v: // Recharger les nuanceurs
        chargerNuanceurs();
        std::cout << "// Recharger nuanceurs" << std::endl;
        break;
    case TP_s: // Afficher les couleurs de sélection temporairement
        Etat::modeSelection = 2;
        std::cout << " Etat::modeSelection=" << Etat::modeSelection << std::endl;
        break;
    case TP_g: // Permuter l'affichage en fil de fer ou plein
        {
            GLint modePlein[2];
            glGetIntegerv( GL_POLYGON_MODE, modePlein );
            glPolygonMode( GL_FRONT_AND_BACK, ( modePlein[0] == GL_LINE ) ? GL_FILL : GL_LINE );
        }
        break;
    default:
        std::cout << " touche inconnue : " << (char) touche << std::endl;
        imprimerTouches();
        break;
    }
}

static bool presse = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
    presse = ( state == TP_PRESSE );
    if ( presse )
    {
        Etat::modeSelection = ( button != TP_BOUTON_GAUCHE ) ? 1 : 0;
        Etat::sourisPosPrec.x = x;
        Etat::sourisPosPrec.y = y;
    }
    else
    {
        Etat::modeSelection = 0;
    }
}
void FenetreTP::sourisMolette( int x, int y )
{
}
void FenetreTP::sourisMouvement( int x, int y )
{
    if ( presse )
    {
        if ( !Etat::modeSelection )
        {
            int dx = x - Etat::sourisPosPrec.x;
            int dy = y - Etat::sourisPosPrec.y;
            camera.theta -= dx / 3.0;
            camera.phi   -= dy / 3.0;

            if ( camera.theta > 360.0 )
                camera.theta -= 360.0;
            else if ( camera.theta < 0.0 )
                camera.theta += 360.0;

            const GLdouble MINPHI = 0.01, MAXPHI = 180.0 - 0.01;
            if ( camera.phi > MAXPHI )
                camera.phi = MAXPHI;
            else if ( camera.phi < MINPHI )
                camera.phi = MINPHI;
        }

        Etat::sourisPosPrec.x = x;
        Etat::sourisPosPrec.y = y;
    }
}

int main( int argc, char *argv[] )
{
    // créer une fenêtre
    FenetreTP fenetre( "SelectionCouleur", 800, 800 );

    // allouer des ressources et définir le contexte OpenGL
    fenetre.initialiser();

    bool boucler = true;
    while ( boucler )
    {
        // affichage
        fenetre.afficherScene();

        // récupérer les événements et appeler la fonction de rappel
        boucler = fenetre.gererEvenement();
    }

    // détruire les ressources OpenGL allouées
    fenetre.conclure();

    return 0;
}
