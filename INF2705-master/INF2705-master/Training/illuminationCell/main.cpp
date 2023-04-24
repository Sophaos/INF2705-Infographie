#include <stdlib.h>
#include <iostream>
#include "inf2705-matrice.h"
#include "inf2705-nuanceur.h"
#include "inf2705-fenetre.h"
#include "inf2705-forme.h"
#include "Pipeline.h"
#include "Camera.h"

// les formes
FormeCube *cube = NULL;
FormeSphere *sphere = NULL;
FormeTore *tore = NULL;
FormeTheiere *theiere = NULL;
FormeIcosaedre *icosaedre = NULL;
FormeDodecaedre *dodecaedre = NULL;

////////////////////////////////////////
// déclaration des variables globales //
////////////////////////////////////////

int modele = 1; // le modèle à afficher

// position de la lumière dans le repère de modélisation
glm::vec4 lumiPosition( 4., 1., 15., 1.0 );

void chargerNuanceurs()
{
    // créer le programme
    prog = glCreateProgram();
    // attacher le nuanceur de sommets
    const GLchar *chainesSommets[2] = { "#version 410\n#define NUANCEUR_SOMMETS\n", ProgNuanceur::lireNuanceur( "nuanceurs.glsl" ) };
    if ( chainesSommets[1] != NULL )
    {
        GLuint nuanceurObj = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( nuanceurObj, 2, chainesSommets, NULL );
        glCompileShader( nuanceurObj );
        glAttachShader( prog, nuanceurObj );
        ProgNuanceur::afficherLogCompile( nuanceurObj );
        delete [] chainesSommets[1];
    }
    // attacher le nuanceur de fragments
    const GLchar *chainesFragments[2] = { "#version 410\n#define NUANCEUR_FRAGMENTS\n", ProgNuanceur::lireNuanceur( "nuanceurs.glsl" ) };
    if ( chainesFragments[1] != NULL )
    {
        GLuint nuanceurObj = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( nuanceurObj, 2, chainesFragments, NULL );
        glCompileShader( nuanceurObj );
        glAttachShader( prog, nuanceurObj );
        ProgNuanceur::afficherLogCompile( nuanceurObj );
        delete [] chainesFragments[1];
    }
    // faire l'édition des liens du programme
    glLinkProgram( prog );
    ProgNuanceur::afficherLogLink( prog );

    // demander la "Location" des variables
    if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
    if ( ( locmatrModel = glGetUniformLocation( prog, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
    if ( ( locmatrVisu = glGetUniformLocation( prog, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
    if ( ( locmatrProj = glGetUniformLocation( prog, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
    if ( ( locmatrNormale = glGetUniformLocation( prog, "matrNormale" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrNormale" << std::endl;
    if ( ( loclumiPosition = glGetUniformLocation( prog, "lumiPosition" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de lumiPosition" << std::endl;
}

// initialisation d'openGL
void FenetreTP::initialiser()
{
    // couleur de l'arrière-plan
    glClearColor( 0.9, 0.9, 0.9, 1.0 );

    // activer les Etat:: openGL
    glEnable( GL_DEPTH_TEST );

    // charger les nuanceurs
    chargerNuanceurs();

    // créer quelques autres formes
    glUseProgram( prog );
    cube = new FormeCube( 2.0 );
    sphere = new FormeSphere( 0.6, 32, 32 );
    tore = new FormeTore( 0.3, 0.6, 32, 32 );
    theiere = new FormeTheiere( );
    dodecaedre = new FormeDodecaedre( );
    icosaedre = new FormeIcosaedre( );
}

void FenetreTP::conclure()
{
    delete cube;
    delete sphere;
    delete tore;
    delete dodecaedre;
    delete icosaedre;
}

void afficherModele()
{
    // Dessiner le modèle
    matrModel.PushMatrix(); {

        // tourner le modèle
        matrModel.Rotate( camera.phi, -1.0, 0.0, 0.0 );
        matrModel.Rotate( camera.theta, 0.0, -1.0, 0.0 );

        // mise à l'échelle
        matrModel.Scale( 5.0, 5.0, 5.0 );

        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix3fv( locmatrNormale, 1, GL_TRUE, glm::value_ptr( glm::inverse( glm::mat3( matrVisu.getMatr() * matrModel.getMatr() ) ) ) );

        switch ( modele )
        {
        default:
        case 1:
            tore->afficher( );
            break;
        case 2:
            sphere->afficher( );
            break;
        case 3:
            matrModel.Scale( 0.5, 0.5, 0.5 );
            matrModel.Rotate( -90.0, 1.0, 0.0, 0.0 );
            matrModel.Translate( 0.0, 0.0, -1.0 );
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            glUniformMatrix3fv( locmatrNormale, 1, GL_TRUE, glm::value_ptr( glm::inverse( glm::mat3( matrVisu.getMatr() * matrModel.getMatr() ) ) ) );
            theiere->afficher( );
            break;
        case 4:
            matrModel.Scale( .7, .7, .7 );
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            glUniformMatrix3fv( locmatrNormale, 1, GL_TRUE, glm::value_ptr( glm::inverse( glm::mat3( matrVisu.getMatr() * matrModel.getMatr() ) ) ) );
            dodecaedre->afficher( );
            break;
        case 5:
            matrModel.Scale( 1.2, 1.2, 1.2 );
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            glUniformMatrix3fv( locmatrNormale, 1, GL_TRUE, glm::value_ptr( glm::inverse( glm::mat3( matrVisu.getMatr() * matrModel.getMatr() ) ) ) );
            icosaedre->afficher( );
            break;
        case 6:
            cube->afficher( );
            break;
        }
    } matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
}

// fonction d'affichage
void FenetreTP::afficherScene()
{
    // effacer l'écran et le tampon de profondeur
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram( prog );

    // définir le pipeline graphique
    matrProj.Perspective( 35.0, (GLdouble)largeur_ / (GLdouble)hauteur_, 0.1, 60.0 );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );

    matrVisu.LookAt( 0.0, 0.0, camera.dist,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0 );
    glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );

    matrModel.LoadIdentity();
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    glUniform4fv( loclumiPosition, 1, glm::value_ptr( lumiPosition ) );

    // dessiner la scène
    afficherModele();

    // permuter plans avant et arrière
    swap();
}

// fonction de redimensionnement de la fenêtre graphique
void FenetreTP::redimensionner( GLsizei w, GLsizei h )
{
    glViewport( 0, 0, w, h );
}

// fonction de gestion du clavier
void FenetreTP::clavier( TP_touche touche )
{
    switch ( touche )
    {
    case TP_ECHAP:
    case TP_q: // Quitter l'application
        quit();
        break;

    case TP_v: // Recharger les fichiers des nuanceurs et recréer le programme
        chargerNuanceurs();
        std::cout << "// Recharger nuanceurs" << std::endl;
        break;

    case TP_m: // Choisir le modèle affiché: cube, théière, tore, sphère, dodécaèdre, icosahèdre
        if ( ++modele > 6 ) modele = 1;
        std::cout << " modele=" << modele << std::endl;
        break;

    case TP_0:
        camera.theta = 0.0; camera.phi = 0.0; camera.dist = 30.0; // placer les choses afin d'avoir une belle vue
        break;

    default:
        std::cout << " touche inconnue : " << (char) touche << std::endl;
        imprimerTouches();
        break;
    }

}

// fonction callback pour un clic de souris
glm::ivec2 sourisPosPrec(0,0);
static enum { deplaceCam, deplaceLumi } deplace = deplaceCam;
static bool presse = false;
void FenetreTP::sourisClic( int button, int state, int x, int y )
{
    presse = ( state == TP_PRESSE );
    if ( presse )
    {
        sourisPosPrec.x = x;
        sourisPosPrec.y = y;
        switch ( button )
        {
        case TP_BOUTON_GAUCHE:
            deplace = deplaceCam;
            break;
        case TP_BOUTON_MILIEU:
        case TP_BOUTON_DROIT:
            deplace = deplaceLumi;
            break;
        }
    }
}

void FenetreTP::sourisMolette( int x, int y )
{
}

// fonction de mouvement de la souris
void FenetreTP::sourisMouvement( int x, int y )
{
    if ( presse )
    {
        int dx = x - sourisPosPrec.x;
        int dy = y - sourisPosPrec.y;
        switch ( deplace )
        {
        case deplaceCam:
            camera.theta -= dx / 3.0;
            camera.phi   -= dy / 3.0;
            break;
        case deplaceLumi:
            lumiPosition.x += 0.03 * dx;
            lumiPosition.y -= 0.03 * dy;
            break;
        }

        sourisPosPrec.x = x;
        sourisPosPrec.y = y;

        camera.verifierAngles();
    }
}

int main( int argc, char *argv[] )
{
    // créer une fenêtre
    FenetreTP fenetre( "cell" );

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
