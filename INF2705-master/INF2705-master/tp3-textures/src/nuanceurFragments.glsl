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

uniform sampler2D laTextureCoul;
uniform sampler2D laTextureNorm;


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

} AttribsIn;

out vec4 FragColor;

vec4 calculerReflexion(in vec4 coul, in vec3 L1, in vec3 L2, in vec3 L3, in vec3 N, in vec3 O )
{
	// calcul de l'éclairage seulement si le produit scalaire est positif
	float NdotL1 = max( 0.0, dot( N, L1 ) );
	float NdotL2 = max( 0.0, dot( N, L2 ) );
	float NdotL3 = max( 0.0, dot( N, L3 ) );

		
	// calcul de la composante ambiante
	coul += FrontMaterial.ambient * LightModel.ambient;
	coul += FrontMaterial.ambient * LightSource.ambient;

	// Calcul de la composante diffuse et speculaire pour la première souce de lumière (L1)
	if ( NdotL1 > 0.0 )
	{
		// calcul de la composante diffuse
		coul += ((utiliseCouleur) ? vec4(0.7,0.7,0.7,1.0) : FrontMaterial.diffuse ) * LightSource.diffuse * NdotL1;
		//coul += AttribsIn.couleur * LightSource.diffuse * NdotL1; //(ici, on utilise plutôt la couleur de l'objet)

		// calcul de la composante spéculaire (Blinn ou Phong)
		float NdotHV1 = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L1 + O ), N ) : dot( reflect( -L1, N ), O ) );
		coul += FrontMaterial.specular * LightSource.specular * ( ( NdotHV1 == 0.0 ) ? 0.0 : pow( NdotHV1, FrontMaterial.shininess ) );
	}

	// Calcul de la composante diffuse et speculaire pour la deuxième souce de lumière (L2)
	if ( NdotL2 > 0.0 )
	{
		// calcul de la composante diffuse
		coul += ((utiliseCouleur) ? vec4(0.7,0.7,0.7,1.0) : FrontMaterial.diffuse ) * LightSource.diffuse * NdotL2;
		//coul += AttribsIn.couleur * LightSource.diffuse * NdotL2; //(ici, on utilise plutôt la couleur de l'objet)

		// calcul de la composante spéculaire (Blinn ou Phong)
		float NdotHV2 = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L2 + O ), N ) : dot( reflect( -L2, N ), O ) );
		coul += FrontMaterial.specular * LightSource.specular * ( ( NdotHV2 == 0.0 ) ? 0.0 : pow( NdotHV2, FrontMaterial.shininess ) );
	}

	// Calcul de la composante diffuse et speculaire pour la troisième souce de lumière (L3)
	if ( NdotL3 > 0.0 )
	{
		// calcul de la composante diffuse
		coul += ((utiliseCouleur) ? vec4(0.7,0.7,0.7,1.0) : FrontMaterial.diffuse ) * LightSource.diffuse * NdotL3;
		//coul += AttribsIn.couleur * LightSource.diffuse * NdotL3; //(ici, on utilise plutôt la couleur de l'objet)

		// calcul de la composante spéculaire (Blinn ou Phong)
		float NdotHV3 = max( 0.0, ( utiliseBlinn ) ? dot( normalize( L3 + O ), N ) : dot( reflect( -L3, N ), O ) );
		coul += FrontMaterial.specular * LightSource.specular * ( ( NdotHV3 == 0.0 ) ? 0.0 : pow( NdotHV3, FrontMaterial.shininess ) );
	}
			
	return coul;
    //vec4 grisUniforme = vec4(0.7,0.7,0.7,1.0);
    //return( grisUniforme );
}

void main(void)
{

	vec3 L1 = normalize( AttribsIn.lum1Dir ); // vecteur vers la première source lumineuse
	vec3 L2 = normalize( AttribsIn.lum2Dir ); // vecteur vers la deuxième source lumineuse
	vec3 L3 = normalize( AttribsIn.lum3Dir ); // vecteur vers la troisième source lumineuse
	// Le vecteur de la normale en dépendance de gl_FrontFacing

	
	vec3 N;
	N = AttribsIn.normale;
	if (numTexNorm != 0) 
	{
		vec3 couleurTexNorm = texture( laTextureNorm, AttribsIn.textCoord ).rgb;
		vec3 dN = normalize( (couleurTexNorm - 0.5) * 2.0);
		N = normalize( N + dN);
	}
	else 
	{
		// Cette ligne de code sera bien mais ne marche pas bien avec la theiere
		//N = normalize( gl_FrontFacing ? N : -N );
		N = normalize( N );
	}

	vec3 O = normalize( AttribsIn.obsDir );  // position de l'observateur

	// TYPE ILLUMINATION : PHONG
	if (typeIllumination == 1)
	{
		FragColor = calculerReflexion(vec4(0.0,0.0,0.0,0.0), L1, L2, L3, N, O);
	}
	// TYPE ILLUMINATION : GOURAUD
	else
	{
		// Sinon c'est Gouraud
		// Rien a faire pour Gourant
		FragColor = AttribsIn.couleur;
	}

			
	if (numTexCoul > 0) 
	{
		vec4 coulTexture = texture( laTextureCoul, AttribsIn.textCoord );

		if (afficheTexelFonce == 1) // Colore
		{
			coulTexture = calculerReflexion(coulTexture, L1, L2, L3, N, O);
			FragColor = 0.5 * (coulTexture * FragColor);
		}
		else if (afficheTexelFonce == 2) // Transparent
		{
			if (coulTexture.r < 0.5 && coulTexture.g < 0.5 && coulTexture.b < 0.5)
				discard;
		}
		else
		{
				FragColor = coulTexture * FragColor;
		}
	}

	if ( afficheNormales ) {
		FragColor = vec4(N, 1.0);
	}

	FragColor = clamp( FragColor, 0.0, 1.0 );

}
