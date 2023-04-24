#version 410

// in vec4 gl_FragCoord;   // <-- déclaration implicite
// in bool gl_FrontFacing; // <-- déclaration implicite
// in vec2 gl_PointCoord;  // <-- déclaration implicite

in Attribs {
    vec4 couleurAvant;
    vec4 couleurArriere;
} AttribsIn;

out vec4 FragColor;

void main(void)
{
    // la couleur du fragment est la couleur interpolée
    vec4 coul = gl_FrontFacing ? AttribsIn.couleurAvant : AttribsIn.couleurArriere;
    FragColor = coul;
}
