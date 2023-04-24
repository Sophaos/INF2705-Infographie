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
    vec4 pos; // position dans le repère de la caméra
} AttribsOut;

void main(void)
{
    // transformation standard du sommet (P * V * M * sommet)
    gl_Position = matrProj * matrVisu * matrModel * Vertex;

    // on pourrait aussi faire comme ceci:
    // mat4 VM = matrVisu * matrModel;
    // mat4 PVM = matrProj * VM;
    // gl_Position = PVM * Vertex;

    // la couleur du sommet
    AttribsOut.couleurAvant = Color;
    AttribsOut.couleurArriere = vec4( 1.0 - Color.rgb, Color.a );

    vec4 pos = matrVisu * matrModel * Vertex;
    AttribsOut.pos = pos;
    // AttribsOut.couleurArriere = vec4( abs(pos.x), 0.0, 0.0, 1.0 );
    // AttribsOut.couleurArriere = vec4( 0.0, abs(pos.y), 0.0, 1.0 );
    // AttribsOut.couleurArriere = vec4( abs(pos.x), abs(pos.y), 0.0, 1.0 );
    // AttribsOut.couleurArriere = AttribsOut.couleurAvant.brga;
}
