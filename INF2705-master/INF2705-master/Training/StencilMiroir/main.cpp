#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "inf2705-matrice.h"
#include "inf2705-nuanceur.h"
#include "inf2705-fenetre.h"
#include "inf2705-forme.h"
#include "Etat.h"
#include "Pipeline.h"

// les formes
FormeCube *cube = NULL;
FormeCylindre *cylindre = NULL;

void calculerPhysique( )
{
    if ( Etat::enmouvement )
    {
        Etat::xrot += 1;
        if ( Etat::avecLimite )
            if ( Etat::xrot > 360 ) Etat::xrot -= 360;
    }
}

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
    if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;
    if ( ( locmatrModel = glGetUniformLocation( prog, "matrModel" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrModel" << std::endl;
    if ( ( locmatrVisu = glGetUniformLocation( prog, "matrVisu" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrVisu" << std::endl;
    if ( ( locmatrProj = glGetUniformLocation( prog, "matrProj" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de matrProj" << std::endl;
    if ( ( locplanCoupe = glGetUniformLocation( prog, "planCoupe" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de planCoupe" << std::endl;
}

static GLint Nconnecq = 6;

void FenetreTP::initialiser()
{
    glEnable( GL_DEPTH_TEST );

    // charger les nuanceurs
    chargerNuanceurs();
    glUseProgram( prog );

    FenetreTP::VerifierErreurGL("debut initialiser");

    // allouer les objets OpenGL
    glGenVertexArrays( 2, vao );
    glGenBuffers( 3, vbo );

    // initialiser le VAO pour tracer le miroir
    GLfloat cooq[] = { Etat::miroirX, -1.0, -2.0,    // sommet 1 carré
                       Etat::miroirX,  1.0, -2.0,    // sommet 2 carré
                       Etat::miroirX,  1.0,  2.0,    // sommet 3 carré
                       Etat::miroirX, -1.0,  2.0 };  // sommet 4 carré

    const GLuint connecq[] = { 0, 1, 2, 2, 3, 0 };

    glBindVertexArray( vao[0] );
    // charger le VBO pour les sommets et faire le lien avec le nuanceur de sommets
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(cooq), cooq, GL_STATIC_DRAW );
    glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(locVertex);
    // charger le VBO pour la connectivité
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(connecq), connecq, GL_STATIC_DRAW );
    glBindVertexArray(0);

    // initialiser le VAO pour tracer les murs
#define p0 -3., -3., -3.
#define p1  3., -3., -3.
#define p2  3.,  3., -3.
#define p3 -3.,  3., -3.
#define p4 -3., -3.,  3.
#define p5  3., -3.,  3.
#define p6  3.,  3.,  3.
#define p7 -3.,  3.,  3.
    GLfloat sommetsMurs[] = { p0, p4,  p1, p5,  p2, p6,  p3, p7,
                              p1, p2,  p0, p3,  p4, p7,  p5, p6 };
#undef p0
#undef p1
#undef p2
#undef p3
#undef p4
#undef p5
#undef p6
#undef p7

    glBindVertexArray( vao[1] );
    // charger le VBO pour les sommets et faire le lien avec le nuanceur de sommets
    glBindBuffer( GL_ARRAY_BUFFER, vbo[2] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(sommetsMurs), sommetsMurs, GL_STATIC_DRAW );
    glVertexAttribPointer( locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(locVertex);

    glBindVertexArray(0);

    // créer quelques autres formes
    glUseProgram( prog );
    cube = new FormeCube( 1.0 );
    cylindre = new FormeCylindre( .25, .25, 4.0, 18, 8 );

    FenetreTP::VerifierErreurGL("fin initialiser");
}

void FenetreTP::conclure()
{
    glUseProgram( 0 );
    glDeleteVertexArrays( 1, vao );
    glDeleteBuffers( 3, vbo );
    delete cube;
    delete cylindre;
}

// Cette fonction affiche la scène et ne fait rien de spécial pour le miroir
static void dessinerScene( GLenum ordre )
{
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    if ( Etat::avecMurs )
    {
        // les quatre murs
        // ne pas afficher les murs dont on verrait la face arrière
        glEnable( GL_CULL_FACE );
        glCullFace( GL_BACK );
        // (on donne ici l'ordre dans lequel les faces seront affichées)
        glFrontFace( ordre );

        // affiche un cube d'arête 1
        glBindVertexArray( vao[1] );
        // les 3 murs
        glVertexAttrib3f( locColor, 1, 1, .5 );
        glDrawArrays( GL_TRIANGLE_STRIP, 2, 4 );
        glDrawArrays( GL_TRIANGLE_STRIP, 8, 8 );
        // le sol
        glVertexAttrib3f( locColor, .5, .5, .5 );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ); // plancher
        glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 ); // plafond
        glBindVertexArray(0);

        glDisable( GL_CULL_FACE );
    }

    // un cylindre bleu au milieu de la pièce
    glVertexAttrib3f( locColor, .5, .5, 1 );
    matrModel.PushMatrix();{
        matrModel.Rotate( 5*Etat::xrot, 1, 0, 0 );
        matrModel.Translate( 1, 0, -2 );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        cylindre->afficher();
    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    // un autre cylindre rouge entre le miroir et le mur
    glVertexAttrib3f( locColor, 1, .5, .5 );
    matrModel.PushMatrix();{
        matrModel.Rotate( -2*Etat::xrot, 1, 0, 0 );
        matrModel.Translate( -2.5, 0, -2 );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        cylindre->afficher();
    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    if ( Etat::avecCube )
    {
        // le cube
        glVertexAttrib3f( locColor, .8, .75, .7 );
        matrModel.PushMatrix();{
            matrModel.Translate( -.5, -1, -1 );
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            cube->afficher( );
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    }
}

// Cette fonction affiche le miroir
static void dessinerMiroir( void )
{
    GLint modePlein[2];
    glGetIntegerv( GL_POLYGON_MODE, modePlein );
    if ( modePlein[0] == GL_LINE )
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // pour afficher quand même tous les fragments du miroir

    matrModel.PushMatrix();{
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glBindVertexArray( vao[0] );
        glDrawElements( GL_TRIANGLES, Nconnecq, GL_UNSIGNED_INT, 0 );
        glBindVertexArray(0);
    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    if ( modePlein[0] == GL_LINE )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

// Cette fonction affiche le miroir
static void dessinerSceneReflechie( void )
{
    matrModel.PushMatrix();{

        if ( Etat::avecClip )
        {
            // Mettre un plan de clipping à la position du miroir afin de garder
            // ce qui est en avant du miroir et d'éliminer les réflexions des
            // objets qui sont à l'arrière du miroir.
            // Etat::miroirX est la position du plan en X et la normale du plan est vers l'arrière
            const glm::vec4 planCoupe( -1, 0, 0, Etat::miroirX ); // équation du plan de coupe
            glUniform4fv( locplanCoupe, 1, glm::value_ptr(planCoupe) );
            glEnable( GL_CLIP_PLANE0 );
        }

        // faire une translation pour amener le plan de réflexion à la position du miroir
        matrModel.Translate( Etat::miroirX, 0, 0 );
        // le miroir est dans le plan YZ; faire une réflexion par -1 en X
        matrModel.Scale( -1, 1, 1 );
        // faire la translation inverse
        matrModel.Translate( -Etat::miroirX, 0, 0 );

        // tracer la scène
        dessinerScene( GL_CW );
        glDisable( GL_CLIP_PLANE0 );

    }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
}

void FenetreTP::afficherScene()
{
    glUseProgram( prog );

    // Donner la projection
    GLdouble aspect = (GLdouble) largeur_ / (GLdouble) hauteur_;
    matrProj.Perspective( 50.0, aspect, 1.0, 20.0 );
    glUniformMatrix4fv( locmatrProj, 1, GL_FALSE, matrProj );

    // Donner la position de la caméra
    matrVisu.LookAt( 0, 1, 7,  -1, 0, 0,  0, 1, 0 );
    matrVisu.Rotate( -.4*Etat::xrot, 0, 1, 0 );
    glUniformMatrix4fv( locmatrVisu, 1, GL_FALSE, matrVisu );

    // et faire une rotation: tourner la scène par rapport à la caméra
    matrModel.LoadIdentity();
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

    if ( Etat::avecStencil )
    {
        // 0) Effacer l'écran: les couleurs, les valeurs de Z et le stencil
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

        // 1) Remplir le stencil avec le miroir (on veut des 1 pour tous les
        // pixels du miroir, 0 partout ailleurs), mais on ne veut rien tracer à
        // l'écran pour le moment.  On s'organise donc pour que le test du
        // stencil ne passe jamais tout en remplissant le stencil de 1.
        glEnable( GL_STENCIL_TEST );
        glStencilFunc( GL_NEVER, 1, 1 ); // void glStencilFunc( GLenum func, GLint ref, GLuint mask );
        glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
        dessinerMiroir( );

        // 2) Maintenant que le stencil est rempli de 1 à la position du miroir,
        // on trace la scène réfléchie.  On fait en sorte que seulement la région
        // du stencil avec des 1 soit tracée (c'est-à-dire seulement la région du miroir)
        glStencilFunc( GL_EQUAL, 1, 1 );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
        dessinerSceneReflechie( );
        glDisable( GL_STENCIL_TEST );
    }
    else
    {
        // 0) Effacer l'écran: les couleurs et les valeurs de Z
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // 1) Tracer d'abord la scène réfléchie
        dessinerSceneReflechie( );

        // 2) Mettre tous les z de la scène réfléchie à la profondeur maximale (=1)
        glClear( GL_DEPTH_BUFFER_BIT );
    }

    // 3) Tracer maintenant le miroir comme une vitre presque complètement
    // transparente. Ça mettra les bonnes valeurs de z pour le miroir.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glVertexAttrib4f( locColor, 0, 0, 0, .4 );
    dessinerMiroir( );
    glDisable( GL_BLEND );

    // 4) Tracer la scène normalement
    dessinerScene( GL_CCW );

    // tracer les décorations
    // afficher les axes
    if ( Etat::afficheAxes ) FenetreTP::afficherAxes( 1.0 );

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

    case TP_x: // Activer/désactiver l'affichage des axes
        Etat::afficheAxes = !Etat::afficheAxes;
        std::cout << "// Affichage des axes ? " << ( Etat::afficheAxes ? "OUI" : "NON" ) << std::endl;
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

    case TP_ESPACE: // Mettre en pause ou reprendre l'animation
        Etat::enmouvement = !Etat::enmouvement;
        break;

    case TP_m: // Afficher ou non les murs
        Etat::avecMurs = !Etat::avecMurs;
        break;

    case TP_t: // Afficher ou non le cube
        Etat::avecCube = !Etat::avecCube;
        break;

    case TP_c: // Utiliser ou non un plan de coupe
        Etat::avecClip = !Etat::avecClip;
        break;

    case TP_s: // Utiliser ou non le tampon de stencil
        Etat::avecStencil = !Etat::avecStencil;
        break;

    case TP_l: // Limiter ou non la rotation de la caméra
        Etat::avecLimite = !Etat::avecLimite;
        break;

    case TP_0: // Remise de l'angle de la caméra à zéro
        Etat::xrot = 0;
        break;
    case TP_DROITE: // Incrémenter l'angle de la caméra
        Etat::xrot += 1;
        break;
    case TP_GAUCHE: // Décrementer l'angle de la caméra
        Etat::xrot -= 1;
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
    FenetreTP fenetre( "miroir", 600, 500 );

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
