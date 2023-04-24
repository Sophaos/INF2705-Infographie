#version 410

// in vec4 gl_FragCoord;   // <-- déclaration implicite
// in bool gl_FrontFacing; // <-- déclaration implicite
// in vec2 gl_PointCoord;  // <-- déclaration implicite

in Attribs {
    vec4 couleurAvant;
    vec4 couleurArriere;
    vec4 pos; // position dans le repère de la caméra
} AttribsIn;

// out float gl_FragDepth; // <-- déclaration implicite
out vec4 FragColor;

void main(void)
{
    // la couleur du fragment est la couleur interpolée
    vec4 coul = gl_FrontFacing ? AttribsIn.couleurAvant : AttribsIn.couleurArriere;
    FragColor = coul;

    // FragColor = vec4( 1.0, 0.5, 0.5, 1.0 );

    // if ( gl_FragCoord.x < 270.0 ) discard;
    // if ( gl_FragCoord.y > 300.0 ) discard;

    // float x = gl_FragCoord.x - 400.;
    // float y = gl_FragCoord.y - 300.;
    // if ( sqrt( x*x + y*y ) < 180. ) { FragColor.rgb = vec3(1.); }
    // if ( sqrt( x*x + y*y ) < 150. ) discard;

    // if ( coul.r < .4 && coul.g < .4 && coul.b < .4 ) discard;
    // if ( coul.r > .5 && coul.g > .5 && coul.b > .5 ) discard;
    // if ( coul.r < .4 && coul.g < .4 ) discard;
    // if ( coul.r * coul.g * coul.b > .25 ) discard;

    // if ( abs(AttribsIn.pos.x) > 1.7 ) discard;
}
