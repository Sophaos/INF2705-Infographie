// "#version 4xx\n#define NUANCEUR_...\n" doit être ajouté en préambule au chargement de chaque nuanceur
////////////////////////////////////////////////////////////////////////////////
#if defined(NUANCEUR_SOMMETS)

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;

uniform vec4 planCoupe; // équation du plan de coupe

layout(location=0) in vec4 Vertex;
layout(location=3) in vec4 Color;

// out gl_PerVertex // <-- déclaration implicite
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// };

out Attribs {
    vec4 couleur;
} AttribsOut;

void main(void)
{
    // transformation standard du sommet (P * V * M * sommet)
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // Initialiser la variable gl_ClipDistance[] pour que le découpage soit fait par OpenGL
    {
        vec4 pos = matrModel * Vertex;
        gl_ClipDistance[0] = dot( planCoupe, pos );
    }

    // la couleur du sommet
    AttribsOut.couleur = Color;
}

////////////////////////////////////////////////////////////////////////////////
#elif defined(NUANCEUR_FRAGMENTS)

in Attribs {
    vec4 couleur;
} AttribsIn;

out vec4 FragColor;

void main(void)
{
    // la couleur du fragment est la couleur interpolée
    FragColor = AttribsIn.couleur;
}

#endif
