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

in Attribs {
    vec4 couleur;
	vec3 normale;

	vec3 lum1Dir;
	vec3 lum2Dir;
	vec3 lum3Dir;

	vec3 obsDir;

	vec2 textCoord;

} AttribsIn[];

out Attribs {
    vec4 couleur;
	vec3 normale;

	vec3 lum1Dir;
	vec3 lum2Dir;
	vec3 lum3Dir;

	vec3 obsDir;

	vec2 textCoord;

} AttribsOut;

layout(triangles) in;



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

float interpole( float v0, float v1, float v2 )
{
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec2 interpole( vec2 v0, vec2 v1, vec2 v2 )
{
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec3 interpole( vec3 v0, vec3 v1, vec3 v2 )
{
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec4 interpole( vec4 v0, vec4 v1, vec4 v2 )
{
    return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}

void main(void)
{
	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;

	gl_Position = interpole( p0, p1, p2 ); 

	vec4 c0 =  AttribsIn[0].couleur;
	vec4 c1 =  AttribsIn[1].couleur;
	vec4 c2 =  AttribsIn[2].couleur;

    AttribsOut.couleur = interpole( c0, c1, c2 );
	
    AttribsOut.normale = interpole( AttribsIn[0].normale, AttribsIn[1].normale, AttribsIn[2].normale );

    AttribsOut.lum1Dir = interpole( AttribsIn[0].lum1Dir, AttribsIn[1].lum1Dir, AttribsIn[2].lum1Dir );
    AttribsOut.lum2Dir = interpole( AttribsIn[0].lum2Dir, AttribsIn[1].lum2Dir, AttribsIn[2].lum2Dir );
    AttribsOut.lum3Dir = interpole( AttribsIn[0].lum3Dir, AttribsIn[1].lum3Dir, AttribsIn[2].lum3Dir );

    AttribsOut.obsDir = interpole( AttribsIn[0].obsDir, AttribsIn[1].obsDir, AttribsIn[2].obsDir );
    AttribsOut.textCoord = interpole( AttribsIn[0].textCoord, AttribsIn[1].textCoord, AttribsIn[2].textCoord );		


	vec3 L1 = normalize( AttribsOut.lum1Dir ); // vecteur vers la première source lumineuse
	vec3 L2 = normalize( AttribsOut.lum2Dir ); // vecteur vers la deuxième source lumineuse
	vec3 L3 = normalize( AttribsOut.lum3Dir ); // vecteur vers la troisième source lumineuse
	vec3 N = normalize( AttribsOut.normale );
	vec3 O = normalize( AttribsOut.obsDir );  // position de l'observateur

	AttribsOut.couleur = calculerReflexion(L1, L2, L3, N, O);

	gl_Position = matrProj * matrVisu * gl_Position;

}
