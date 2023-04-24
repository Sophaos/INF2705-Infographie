#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "inf2705-matrice.h"
#include "inf2705-nuanceur.h"
#include "inf2705-fenetre.h"
#include "Etat.h"
#include "Pipeline.h"
#include "Camera.h"

void calculerPhysique( )
{
    if ( Etat::enmouvement )
    {
        Etat::rtri += 0.2 * 10;
        Etat::rquad -= 0.15 * 10;
    }
}

void chargerNuanceurs()
{
    // créer le programme
    prog = glCreateProgram();

    // attacher le nuanceur de sommets
    const GLchar *chainesSommets = ProgNuanceur::lireNuanceur( "nuanceurSommets.glsl" );
    if ( chainesSommets != NULL )
    {
        GLuint nuanceurObj = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( nuanceurObj, 1, &chainesSommets, NULL );
        glCompileShader( nuanceurObj );
        glAttachShader( prog, nuanceurObj );
        ProgNuanceur::afficherLogCompile( nuanceurObj );
        delete [] chainesSommets;
    }
    // attacher le nuanceur de fragments
    const GLchar *chainesFragments = ProgNuanceur::lireNuanceur( "nuanceurFragments.glsl" );
    if ( chainesFragments != NULL )
    {
        GLuint nuanceurObj = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( nuanceurObj, 1, &chainesFragments, NULL );
        glCompileShader( nuanceurObj );
        glAttachShader( prog, nuanceurObj );
        ProgNuanceur::afficherLogCompile( nuanceurObj );
        delete [] chainesFragments;
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

static GLint Nconnect = 3, Nconnecq = 6;

void FenetreTP::initialiser()
{
    // couleur de l'arrière-plan
    glClearColor( 0.2, 0.2, 0.2, 1.0 );

    glEnable( GL_DEPTH_TEST );

    // charger les nuanceurs
    chargerNuanceurs();

    FenetreTP::VerifierErreurGL("debut initialiser");

    // Charger le modèle
    GLfloat coot[] = { 0.0,  1.0, 0.0,    // sommet 1 triangle
                      -1.0, -1.0, 0.0,    // sommet 2 triangle
                       1.0, -1.0, 0.0 };  // sommet 3 triangle
    GLfloat cooq[] = {-1.0, -1.0, 0.0,    // sommet 1 carré
                       1.0, -1.0, 0.0,    // sommet 2 carré
                       1.0,  1.0, 0.0,    // sommet 3 carré
                      -1.0,  1.0, 0.0 };  // sommet 4 carré

    GLfloat coulTriangle[] = { 1.0, 0.0, 0.0,    // couleurs du triangle
                               0.0, 1.0, 0.0,
                               0.0, 0.0, 1.0 };
    const GLuint connect[] = { 0, 1, 2 };
    const GLuint connecq[] = { 0, 1, 2, 2, 3, 0 };

    // allouer les objets OpenGL
    glGenVertexArrays( 2, vao );
    glGenBuffers( 5, vbo );

    // initialiser le premier VAO
    glBindVertexArray( vao[0] );
    // charger le VBO pour les sommets
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(coot), coot, GL_STATIC_DRAW );
    glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(locVertex);
    // charger le VBO pour les couleurs
    glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(coulTriangle), coulTriangle, GL_STATIC_DRAW );
    glVertexAttribPointer( locColor, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(locColor);
    // charger le VBO pour la connectivité
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[2] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(connect), connect, GL_STATIC_DRAW );
    glBindVertexArray(0);

    // initialiser le second VAO
    glBindVertexArray( vao[1] );
    // charger le VBO pour les sommets
    glBindBuffer( GL_ARRAY_BUFFER, vbo[3] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(cooq), cooq, GL_STATIC_DRAW );
    glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(locVertex);
    // pas de tableaux de couleurs ici (on donnera une seule couleur plus tard)
    //glDisableVertexAttribArray(locColor); // pas nécessaire: c'est désactivé par défaut
    // charger le VBO pour la connectivité
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[4] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(connecq), connecq, GL_STATIC_DRAW );
    glBindVertexArray(0);

    // Défaire tous les liens
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    FenetreTP::VerifierErreurGL("fin initialiser");
}

void FenetreTP::conclure()
{
    glUseProgram( 0 );
    glDeleteVertexArrays( 2, vao );
    glDeleteBuffers( 5, vbo );
}

void afficherModele()
{
    // afficher le modèle
    matrModel.LoadIdentity();

    // afficher le triangle
    matrModel.Translate( -1.5, 0.0, 0.0 );
    matrModel.PushMatrix();{
        matrModel.Rotate( Etat::rtri, 0.0, 1.0, 0.0 );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glBindVertexArray( vao[0] );         // sélectionner le premier VAO
        // utiliser glDrawElements ou glDrawArrays; le résultat est identique
        glDrawElements( GL_TRIANGLES, Nconnect, GL_UNSIGNED_INT, 0 );
        //glDrawArrays( GL_TRIANGLES, 0, 3 );
        glBindVertexArray( 0 );              // désélectionner le VAO
    }matrModel.PopMatrix();

    // mettre une valeur fixe pour la couleur du rectangle
    // GLfloat coulRectangle[] = { 0.5, 0.5, 1.0 };
    // glVertexAttrib3fv( locColor, coulRectangle );
    glVertexAttrib3f( locColor, 0.5, 0.5, 1.0 ); // semblable au glColor() de OpenGL 2.x

    // afficher le quad
    matrModel.PushMatrix();{
        matrModel.Translate( 3.0, 0.0, 0.0 );
        matrModel.Rotate( Etat::rquad, 1.0, 0.0, 0.0 );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glBindVertexArray( vao[1] );         // sélectionner le second VAO
        glDrawElements( GL_TRIANGLES, Nconnecq, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );              // désélectionner le VAO
    }matrModel.PopMatrix();
}

void FenetreTP::afficherScene()
{
    // effacer l'ecran et le tampon de profondeur
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram( prog );

    // mettre à jour la projection
    GLdouble aspect = (GLdouble) largeur_ / (GLdouble) hauteur_;
    if ( Etat::enPerspective )
        matrProj.Perspective( 25.0, aspect, 1.0, 30.0 );
    else
        matrProj.Ortho( -3.0, 3.0, -2.0, 2.0, 1.0, 30.0 );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );

    // mettre à jour la caméra
    matrVisu.LookAt( camera.dist*cos(glm::radians(camera.phi))*sin(glm::radians(camera.theta)),
                     camera.dist*sin(glm::radians(camera.phi))*sin(glm::radians(camera.theta)),
                     camera.dist*cos(glm::radians(camera.theta)),
                     0.0, 0.0, 0.0,
                     0.0, 1.0, 0.0 );
    glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );

    // dessiner la scène
    afficherModele();

    FenetreTP::VerifierErreurGL("fin afficherScene");

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

    case TP_g: // Permuter l'affichage en fil de fer ou plein
        {
            GLint modePlein[2];
            glGetIntegerv( GL_POLYGON_MODE, modePlein );
            glPolygonMode( GL_FRONT_AND_BACK, ( modePlein[0] == GL_LINE ) ? GL_FILL : GL_LINE );
        }
        break;

    case TP_p: // Permuter la projection: perspective ou orthogonale
        Etat::enPerspective = !Etat::enPerspective;
        break;

    case TP_ESPACE: // Mettre en pause ou reprendre l'animation
        Etat::enmouvement = !Etat::enmouvement;
        break;

    case TP_SOULIGNE:
    case TP_MOINS: // Incrémenter la distance de la caméra
        camera.dist += 0.5;
        break;

    case TP_PLUS: // Décrémenter la distance de la caméra
    case TP_EGAL:
        camera.dist -= 0.5;
        break;

    case TP_0: // Remettre les angles de la caméra à 0
        camera.theta = camera.phi = 0;
        break;

    case TP_GAUCHE: // Augmenter theta
        camera.theta += 0.5;
        break;

    case TP_DROITE: // Décrémenter theta
        camera.theta -= 0.5;
        break;

    case TP_HAUT: // Augmenter phi
        camera.phi += 0.5;
        break;

    case TP_BAS:  // Décrémenter phi
        camera.phi -= 0.5;
        break;

    case TP_r: // réinitiliaser la caméra
        Etat::rtri = Etat::rquad = 0.0;
        camera.theta = camera.phi = 0.0;
        camera.dist = 10;
        break;

    default:
        std::cout << " touche inconnue : " << (char) touche << std::endl;
        imprimerTouches();
        break;
    }
}

void FenetreTP::sourisClic( int button, int state, int x, int y )
{
}

void FenetreTP::sourisMolette( int x, int y )
{
}

void FenetreTP::sourisMouvement( int x, int y )
{
}

int main( int argc, char *argv[] )
{
    // créer une fenêtre
    FenetreTP fenetre( "base" );

    // allouer des ressources et définir le contexte OpenGL
    fenetre.initialiser();

    bool boucler = true;
    while ( boucler )
    {
        // mettre à jour la physique
        calculerPhysique( );

        // affichage
        fenetre.afficherScene();

        // récupérer les événements et appeler la fonction de rappel
        boucler = fenetre.gererEvenement();
    }

    // détruire les ressources OpenGL allouées
    fenetre.conclure();

    return 0;
}
