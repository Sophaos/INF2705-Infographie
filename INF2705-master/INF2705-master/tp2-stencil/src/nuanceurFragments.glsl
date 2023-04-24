#version 410

uniform int attenuation;

const float debAttenuation = 30.0;
const float finAttenuation = 50.0;

in Attribs {
    vec4 couleur;
	float poszCylindre;
} AttribsIn;

out vec4 FragColor;

void main( void )
{

    // la couleur du fragment est la couleur interpolée
    FragColor = AttribsIn.couleur;

    // atténuer selon la profondeur
    if ( attenuation == 1 )
    {		
		// Obtenir la distance à la caméra du sommet dans le repère de la caméra
        float dist = gl_FragCoord.z / gl_FragCoord.w;
		
        // Obtenir un facteur d'interpolation entre 0 et 1
        float factDist = smoothstep(finAttenuation, debAttenuation, dist);

		// Modifier la couleur de vert a bleu du corps(cylindre) du poisson
		if (AttribsIn.couleur.rgb == vec3(0.0, 1.0, 0.0)) 
		{
			vec3 bleu = vec3( 0.0, 1.0, 1.0 );
			vec3 vert = vec3( 0.0, 1.0, 0.0 );
			FragColor.rgb = mix(vert, bleu, AttribsIn.poszCylindre);
		}

        // Modifier la couleur du fragment en utilisant ce facteur
		FragColor.rgb *= factDist;
    }
}
