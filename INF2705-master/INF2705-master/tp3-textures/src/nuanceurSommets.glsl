#version 410

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position[3];      // dans le repère du monde
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource;

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

layout (std140) uniform varsUnif
{
    // partie 1: illumination
    int typeIllumination;     // 0:Gouraud, 1:Phong
    bool utiliseBlinn;        // indique si on veut utiliser modèle spéculaire de Blinn ou Phong
    bool afficheNormales;     // indique si on utilise les normales comme couleurs (utile pour le débogage)
    // partie 2: texture
    int numTexCoul;           // numéro de la texture de couleurs appliquée
    int numTexNorm;           // numéro de la texture de normales appliquée
    bool utiliseCouleur;      // doit-on utiliser la couleur de base de l'objet en plus de celle de la texture?
    int afficheTexelFonce;    // un texel foncé doit-il être affiché 0:normalement, 1:mi-coloré, 2:transparent?
	bool utiliseTess;
};

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;
uniform mat3 matrNormale;
 
/////////////////////////////////////////////////////////////////

layout(location=0) in vec4 Vertex;
layout(location=2) in vec3 Normal;
layout(location=3) in vec4 Color;
layout(location=8) in vec4 TexCoord;

out Attribs {
    vec4 couleur;
	vec3 normale;

	vec3 lum1Dir;
	vec3 lum2Dir;
	vec3 lum3Dir;

	vec3 obsDir;

	vec2 textCoord;

} AttribsOut;

vec4 calculerReflexion( in vec3 L1, in vec3 L2, in vec3 L3, in vec3 N, in vec3 O )
{

	// Couleur au sommet apres illumination
	vec4 couleur;

	// calcul de l'éclairage seulement si le produit scalaire est positif
	float NdotL1 = max( 0.0, dot( N, L1 ) );
	float NdotL2 = max( 0.0, dot( N, L2 ) );
	float NdotL3 = max( 0.0, dot( N, L3 ) );

		
	// calcul de la composante ambiante
	couleur = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;
	couleur += FrontMaterial.ambient * LightSource.ambient;

	// Calcul de la composante diffuse et speculaire pour la première souce de lumière (L1)
	if ( NdotL1 > 0.0 )
	{
		// calcul de la composante diffuse
		couleur += ((utiliseCouleur) ? vec4(0.7,0.7,0.7,1.0) : FrontMaterial.diffuse ) * LightSource.diffuse * NdotL1;
		//couleur += AttribsIn.couleur * LightSource.diffuse * NdotL1; //(ici, on utilise plutôt la couleur de l'objet)

		// calcul de la composante spéculaire (Blinn ou Phong)
		float NdotHV1 = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L1 + O ), N ) : dot( reflect( -L1, N ), O ) );
		couleur += FrontMaterial.specular * LightSource.specular * ( ( NdotHV1 == 0.0 ) ? 0.0 : pow( NdotHV1, FrontMaterial.shininess ) );
	}

	// Calcul de la composante diffuse et speculaire pour la deuxième souce de lumière (L2)
	if ( NdotL2 > 0.0 )
	{
		// calcul de la composante diffuse
		couleur += ((utiliseCouleur) ? vec4(0.7,0.7,0.7,1.0) : FrontMaterial.diffuse ) * LightSource.diffuse * NdotL2;
		//couleur += AttribsIn.couleur * LightSource.diffuse * NdotL2; //(ici, on utilise plutôt la couleur de l'objet)

		// calcul de la composante spéculaire (Blinn ou Phong)
		float NdotHV2 = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L2 + O ), N ) : dot( reflect( -L2, N ), O ) );
		couleur += FrontMaterial.specular * LightSource.specular * ( ( NdotHV2 == 0.0 ) ? 0.0 : pow( NdotHV2, FrontMaterial.shininess ) );
	}

	// Calcul de la composante diffuse et speculaire pour la troisième souce de lumière (L3)
	if ( NdotL3 > 0.0 )
	{
		// calcul de la composante diffuse
		couleur += ((utiliseCouleur) ? vec4(0.7,0.7,0.7,1.0) : FrontMaterial.diffuse ) * LightSource.diffuse * NdotL3;
		//coul += AttribsIn.couleur * LightSource.diffuse * NdotL3; //(ici, on utilise plutôt la couleur de l'objet)

		// calcul de la composante spéculaire (Blinn ou Phong)
		float NdotHV3 = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L3 + O ), N ) : dot( reflect( -L3, N ), O ) );
		couleur += FrontMaterial.specular * LightSource.specular * ( ( NdotHV3 == 0.0 ) ? 0.0 : pow( NdotHV3, FrontMaterial.shininess ) );
	}

	return couleur;


    //vec4 grisUniforme = vec4(0.7,0.7,0.7,1.0);
    //return( grisUniforme );
	//return (vec4(1,1,1,1));
}


void main(void)
{

	AttribsOut.textCoord = TexCoord.st;

    // transformation standard du sommet (P * V * M * sommet)
    gl_Position = (utiliseTess) ? gl_Position = matrModel * Vertex : matrProj * matrVisu * matrModel * Vertex;
	

    // calculer la normale (N) qui sera interpolée pour le nuanceur de fragment
    AttribsOut.normale = matrNormale * Normal;





    // calculer la position (P) du sommet (dans le repère de la caméra)
    vec3 pos = vec3( matrVisu * matrModel * Vertex );

    // vecteur de la direction (L) de la première lumière (dans le repère de la caméra) vers le sommet
    AttribsOut.lum1Dir = (LightSource.position[0].w != 0.0) ? 
    ( matrVisu * LightSource.position[0] / LightSource.position[0].w ).xyz - pos : (matrVisu * LightSource.position[0]).xyz;

	// vecteur de la direction (L) de la deuxième lumière (dans le repère de la caméra) vers le sommet
	AttribsOut.lum2Dir = (LightSource.position[1].w != 0.0) ? 
    ( matrVisu * LightSource.position[1] / LightSource.position[1].w ).xyz - pos : (matrVisu * LightSource.position[1]).xyz;

	// vecteur de la direction (L) de la troisième lumière (dans le repère de la caméra) vers le sommet
	AttribsOut.lum3Dir = (LightSource.position[2].w != 0.0) ? 
    ( matrVisu * LightSource.position[2] / LightSource.position[2].w ).xyz - pos : (matrVisu * LightSource.position[2]).xyz;

    // vecteur de la direction (O) vers l'observateur (dans le repère de la caméra)
    AttribsOut.obsDir = ( LightModel.localViewer ?
                          normalize(-pos) :        // =(0-pos) un vecteur qui pointe vers le (0,0,0), c'est-à-dire vers la caméra
                          vec3( 0.0, 0.0, 1.0 ) ); // on considère que l'observateur (la caméra) est à l'infini dans la direction (0,0,1)

	// TYPE ILLUMINATION GOURAUD
	if (!utiliseTess && typeIllumination == 0)
	{

		vec3 L1 = normalize( AttribsOut.lum1Dir ); // vecteur vers la première source lumineuse
		vec3 L2 = normalize( AttribsOut.lum2Dir ); // vecteur vers la deuxième source lumineuse
		vec3 L3 = normalize( AttribsOut.lum3Dir ); // vecteur vers la troisième source lumineuse
		// Le vecteur de la normale en dépendance de gl_FrontFacing
		vec3 N = normalize( AttribsOut.normale );
		vec3 O = normalize( AttribsOut.obsDir );  // position de l'observateur

		AttribsOut.couleur = calculerReflexion(L1, L2, L3, N, O);

	}
}
