#ifndef __POISSON_H__
#define __POISSON_H__

//
// un poisson
//
class Poisson
{
    static FormeCylindre *cylindre; // un cylindre centré dans l'axe des Z, de rayon 1, entre (0,0,0) et (0,0,1)
public:
    Poisson( glm::vec3 pos = glm::vec3(3.0,1.0,0.0), glm::vec3 vit = glm::vec3(1.0,0.0,0.0), float tai = 0.5 )
        : position(pos), vitesse(vit), taille(tai), estSelectionne(false)
    {
		couleurSelection = { 0.0, 0.0, 0.0, 0.0 };
        // créer un poisson graphique
        initialiserGraphique();
    }
    ~Poisson()
    {
        conclureGraphique();
    }
    void initialiserGraphique()
    {
        // créer quelques formes
        if ( cylindre == NULL ) cylindre = new FormeCylindre( 1.0, 1.0, 1.0, 12, 1, true );
    }
    void conclureGraphique()
    {
        delete cylindre;
    }

    void afficher()
    {
        matrModel.PushMatrix();{ // sauvegarder la tranformation courante

            // amener le repère à la position courante
            matrModel.Translate( position.x, position.y, position.z );

            // partie 2: modifs ici ...
            // donner la couleur de sélection
			if (Etat::modeSelection == true) {
				//glDisable(GL_BLEND); // Mettre en commentaire si on veut voir l'effet alpha de chaque poisson ;3
				glDisable(GL_BLEND);
                glPolygonMode(GL_FRONT, GL_LINE);
				//...
				glVertexAttrib4fv(locColor, glm::value_ptr(couleurSelection));
			}

            // afficher le corps
            // (en utilisant le cylindre centré dans l'axe des Z, de rayon 1, entre (0,0,0) et (0,0,1))
			if (Etat::modeSelection == false) {
				glm::vec3 coulCorps( 0.0, 1.0, 0.0 ); // vert
				glVertexAttrib3fv( locColor, glm::value_ptr(coulCorps) );
			}
            matrModel.PushMatrix();{
                matrModel.Scale( 5.0*taille, taille, taille );
                matrModel.Rotate( 90.0, 0.0, 1.0, 0.0 );
                if ( vitesse.x > 0.0 ) matrModel.Rotate( 180.0, 0.0, 1.0, 0.0 );
                matrModel.Translate( 0.0, 0.0, -0.5 );
                glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation

                cylindre->afficher();
            }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

            // afficher les yeux
            // (en utilisant le cylindre centré dans l'axe des Z, de rayon 1, entre (0,0,0) et (0,0,1))
			if (Etat::modeSelection == false) {
				glm::vec3 coulYeux(1.0, 1.0, 0.0); // jaune
				glVertexAttrib3fv(locColor, glm::value_ptr(coulYeux));
			}
            matrModel.PushMatrix();{
                matrModel.Rotate( 90.0, 1.0, 0.0, 0.0 );
                matrModel.Scale( 0.5*taille, 0.5*taille, 2.0*taille );
                matrModel.Translate( vitesse.x > 0.0 ? 4.0 : -4.0, 0.2, -0.5 );
                glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
                cylindre->afficher();
                matrModel.Translate( 0.0, 0.0, 2.0 );
            }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void avancerPhysique()
    {
		if (!estSelectionne) {
			const float dt = 0.5; // intervalle entre chaque affichage (en secondes)
			position += dt * vitesse;
			// test rapide pour empêcher que le poisson sorte de l'aquarium
			if (abs(position.x) > 0.9*Etat::bDim.x) vitesse = -vitesse;
		}
    }

    // les variables du poisson
    glm::vec3 position;   // en unités
    glm::vec3 vitesse;    // en unités/seconde
    float taille;         // en unités
	bool estSelectionne;
	glm::vec4 couleurSelection;
};

FormeCylindre* Poisson::cylindre = NULL;

#endif
