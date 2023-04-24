#version 410

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;

layout(location=0) in vec4 Vertex;
layout(location=3) in vec4 Color;

// out gl_PerVertex // <-- déclaration implicite
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// };

out Attribs {
    vec4 couleurAvant;
    vec4 couleurArriere;
} AttribsOut;

void main(void)
{
    // transformation standard du sommet (P * V * M * sommet)
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // la couleur du sommet
    AttribsOut.couleurAvant = Color;
    AttribsOut.couleurArriere = vec4( 1.0 - Color.rgb, Color.a );
}
