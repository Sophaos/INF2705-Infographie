#version 410

uniform mat4 matrProj;
uniform int texnumero;

layout(points) in;
//layout(points, max_vertices = 1) out;
layout(triangle_strip, max_vertices = 4) out;


in Attribs {
    vec4 couleur;
    float tempsDeVieRestant;
    float sens; // du vol
} AttribsIn[];

out Attribs {
    vec4 couleur;
	vec2 textCoord;
} AttribsOut;

void main()
{
    // assigner la position du point
    //gl_Position = gl_in[0].gl_Position;

    // assigner la taille des points (en pixels)
    gl_PointSize = gl_in[0].gl_PointSize;

    // assigner la couleur courante
    AttribsOut.couleur = AttribsIn[0].couleur;


	
	vec2 textCoord[4];
	textCoord[0] = vec2(-0.5, -0.5); // gauche bas
	textCoord[1] = vec2(0.5, -0.5);  // droite bas
	textCoord[3] = vec2(0.5, 0.5);   // droite haut
	textCoord[2] = vec2(-0.5, 0.5);  // gauche haut

	
	for (int i = 0; i < 4; i++) {
		
		float facteur = gl_PointSize / 8.0;
		facteur /= (matrProj * gl_in[0].gl_Position).z;
		gl_Position = matrProj * vec4(gl_in[0].gl_Position.xy + facteur * textCoord[i], gl_in[0].gl_Position.z, gl_in[0].gl_Position.w);

		// FLOCON ETINCELLE
		if (texnumero == 1)
		{

			float vitesseRot = 6.0;
			float angleRot = AttribsIn[0].sens * vitesseRot * AttribsIn[0].tempsDeVieRestant;

			AttribsOut.textCoord = mat2(cos(angleRot), sin(angleRot), -sin(angleRot), cos(angleRot)) * textCoord[i] + vec2(0.5,0.5);
		}	
		// LUTIN PENGUIN & OISEAU
		else if (texnumero == 2 || texnumero == 3)
		{
			

			float NB_SPRITES = 16.0;
			float spriteCourant =  int(18.0 * AttribsIn[0].tempsDeVieRestant) / NB_SPRITES;

			AttribsOut.textCoord = textCoord[i] + vec2(0.5,0.5);
			AttribsOut.textCoord.x /= NB_SPRITES;
			AttribsOut.textCoord.x += spriteCourant;
			AttribsOut.textCoord.x *= AttribsIn[0].sens;
		}
		else 
		{
		
			AttribsOut.textCoord = textCoord[i] + vec2(0.5,0.5);
		}
		
		EmitVertex();

	}


		


    //EmitVertex();
}
