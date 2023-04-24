// "#version 4xx\n#define NUANCEUR_...\n" doit être ajouté en préambule au chargement de chaque nuanceur
////////////////////////////////////////////////////////////////////////////////

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;      // dans le repère du monde
    vec3 spotDirection; // dans le repère du monde
    float spotExponent;
    float spotAngleOuverture; // ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource[1];

// Définition des paramètres des matériaux
layout (std140) uniform MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} FrontMaterial;

// Définition des paramètres globaux du modèle de lumière
layout (std140) uniform LightModelParameters
{
    vec4 ambient;       // couleur ambiante
    bool localViewer;   // observateur local ou à l'infini?
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

////////////////////////////////////////////////////////////////////////////////
#if defined(NUANCEUR_SOMMETS)

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;

uniform vec4 planCoupe; // équation du plan de coupe

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;

// out gl_PerVertex // <-- déclaration implicite
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// };

out Attribs {
    vec3 lumiDir;
    vec3 normale, obsVec;
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

    // calculer la normale (N) qui sera interpolée pour le nuanceur de fragment
    AttribsOut.normale = matrNormale * Normal;

    // calculer la position (P) du sommet (dans le repère de la caméra)
    vec3 pos = vec3( matrVisu * matrModel * Vertex );

    // vecteur de la direction (L) de la lumière (dans le repère de la caméra)
    AttribsOut.lumiDir = ( matrVisu * LightSource[0].position ).xyz - pos;

    // vecteur de la direction (O) vers l'observateur (dans le repère de la caméra)
    AttribsOut.obsVec = ( LightModel.localViewer ?
                          normalize(-pos) :        // =(0-pos) un vecteur qui pointe vers le (0,0,0), c'est-à-dire vers la caméra
                          vec3( 0.0, 0.0, 1.0 ) ); // on considère que l'observateur (la caméra) est à l'infini dans la direction (0,0,1)

    // vecteur de la direction du spot (dans le repère de la caméra)
    //AttribsOut.spotDir = transpose(inverse(mat3(matrVisu))) * -LightSource[0].spotDirection;
}

////////////////////////////////////////////////////////////////////////////////
#elif defined(NUANCEUR_FRAGMENTS)

bool utiliseBlinn = true;

in Attribs {
    vec3 lumiDir;
    vec3 normale, obsVec;
    vec4 couleur;
} AttribsIn;

out vec4 FragColor;

void main(void)
{
    vec3 L = normalize( AttribsIn.lumiDir ); // vecteur vers la source lumineuse
    //vec3 N = normalize( AttribsIn.normale ); // vecteur normal
    vec3 N = normalize( gl_FrontFacing ? AttribsIn.normale : -AttribsIn.normale );
    vec3 O = normalize( AttribsIn.obsVec );  // position de l'observateur

    vec4 coul = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;

    // calcul de la composante ambiante
    coul += FrontMaterial.ambient * LightSource[0].ambient;

    // utiliser le .alpha de la couleur courante (essentiellement pour le miroir transparent)
    coul.a = AttribsIn.couleur.a;

    // calcul de l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calcul de la composante diffuse
        //coul += FrontMaterial.diffuse * LightSource[0].diffuse * NdotL;
        coul += AttribsIn.couleur * LightSource[0].diffuse * NdotL; //(ici, on utilise plutôt la couleur de l'objet)

        // calcul de la composante spéculaire (Blinn ou Phong)
        float NdotHV = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L + O ), N ) : dot( reflect( -L, N ), O ) );
        coul += FrontMaterial.specular * LightSource[0].specular * ( ( NdotHV == 0.0 ) ? 0.0 : pow( NdotHV, FrontMaterial.shininess ) );
    }

    // assigner la couleur finale
    FragColor = clamp( coul, 0.0, 1.0 );

    //FragColor = clamp(vec4(N,1.0),0.,1.);
}

#endif
