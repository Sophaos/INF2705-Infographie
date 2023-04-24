#version 410

uniform sampler2D leLutin;
uniform int texnumero;

in Attribs {
    vec4 couleur;
	vec2 textCoord;
} AttribsIn;

out vec4 FragColor;

void main( void )
{

    FragColor = AttribsIn.couleur;

	if ( texnumero != 0 )
    {
		vec4 couleurTex = texture( leLutin, AttribsIn.textCoord );
		FragColor.rgb = mix( FragColor.rgb, couleurTex.rgb, 0.6);
		FragColor.a = couleurTex.a;
		if (FragColor.a < 0.1) 
			discard;
    }

}
