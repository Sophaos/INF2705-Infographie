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

} AttribsOut[];


layout(vertices = 3) out;

uniform float TessLevelInner;
uniform float TessLevelOuter;

void main(void)
{
	

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if ( gl_InvocationID == 0 )
    {
        gl_TessLevelInner[0] = TessLevelInner;
        gl_TessLevelOuter[0] = TessLevelOuter;
        gl_TessLevelOuter[1] = TessLevelOuter;
        gl_TessLevelOuter[2] = TessLevelOuter;
    }

	AttribsOut[gl_InvocationID].couleur = AttribsIn[gl_InvocationID].couleur;
	AttribsOut[gl_InvocationID].normale = AttribsIn[gl_InvocationID].normale;
	AttribsOut[gl_InvocationID].lum1Dir = AttribsIn[gl_InvocationID].lum1Dir;
	AttribsOut[gl_InvocationID].lum2Dir = AttribsIn[gl_InvocationID].lum2Dir;
	AttribsOut[gl_InvocationID].lum3Dir = AttribsIn[gl_InvocationID].lum3Dir;
	AttribsOut[gl_InvocationID].obsDir = AttribsIn[gl_InvocationID].obsDir;
	AttribsOut[gl_InvocationID].textCoord = AttribsIn[gl_InvocationID].textCoord;

}
