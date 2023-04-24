// "#version 4xx\n#define NUANCEUR_...\n" doit être ajouté en préambule au chargement de chaque nuanceur
////////////////////////////////////////////////////////////////////////////////
#if defined(NUANCEUR_SOMMETS)

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;
uniform vec4 lumiPosition;

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;

out Attribs {
    vec3 normale, lumiDir;
} AttribsOut;

void main( void )
{
    // transformation standard du sommet
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // calculer la normale qui sera interpolée pour le nuanceur de fragment
    AttribsOut.normale = matrNormale * Normal;

    // calculer la position du sommet dans le repère de la caméra
    vec3 pos = vec3( matrVisu * matrModel * Vertex );

    // vecteur de la direction de la lumière (lumière positionnelle)
    AttribsOut.lumiDir = ( matrVisu * lumiPosition ).xyz - pos;
}

////////////////////////////////////////////////////////////////////////////////
#elif defined(NUANCEUR_FRAGMENTS)

in Attribs {
    vec3 normale, lumiDir;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    vec3 N = normalize( AttribsIn.normale ); // vecteur normal
    vec3 L = normalize( AttribsIn.lumiDir ); // vecteur vers la source lumineuse

    float albedo = max( 0.0, dot( N, L ) );

    vec4 couleur;
    if ( albedo > 0.95 )      couleur = vec4( 1.0, 0.5, 0.5, 1.0 );
    else if ( albedo > 0.5 )  couleur = vec4( 0.6, 0.3, 0.3, 1.0 );
    else if ( albedo > 0.25 ) couleur = vec4( 0.4, 0.2, 0.2, 1.0 );
    else                      couleur = vec4( 0.2, 0.1, 0.1, 1.0 );

    FragColor = clamp( couleur, 0.0, 1.0 );
}
#endif
