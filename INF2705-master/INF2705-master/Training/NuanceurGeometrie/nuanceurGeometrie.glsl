#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;
//layout(line_strip, max_vertices = 4) out;
//layout(points, max_vertices = 4) out;

// in gl_PerVertex // <-- déclaration implicite
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// } gl_in[];

// out gl_PerVertex // <-- déclaration implicite
// {
//   vec4 gl_Position;
//   float gl_PointSize;
//   float gl_ClipDistance[];
// };
// out int gl_Layer;
// out int gl_ViewportIndex; // si GL 4.1 ou ARB_viewport_array.

in Attribs {
    vec4 couleurAvant;
    vec4 couleurArriere;
} AttribsIn[];

out Attribs {
    vec4 couleurAvant;
    vec4 couleurArriere;
} AttribsOut;

void main()
{
    // calculer le centre
    vec4 centre = vec4(0.0);
    for ( int i = 0 ; i < gl_in.length() ; ++i )
        centre += gl_in[i].gl_Position;
    centre /= gl_in.length();

    // émettre les sommets
    for ( int i = 0 ; i < gl_in.length() ; ++i )
    {
        gl_Position = gl_in[i].gl_Position;
        // Modifier un peu la valeur qu'on vient de calculer
        //gl_Position.xy = 0.3 * gl_Position.xy;
        //gl_Position.z = 0.3 * gl_Position.z;
        //gl_Position.xyz = 0.3 * gl_Position.xyz;
        //gl_Position.xyzw = 0.3 * gl_Position.xyzw;
        //gl_Position.w = 3.0 * gl_Position.w;
        //gl_Position -= 0.1 * ( gl_in[i].gl_Position - centre );
        //gl_Position = mix( gl_in[i].gl_Position, centre, 0.1 );

        AttribsOut.couleurAvant = AttribsIn[i].couleurAvant;
        AttribsOut.couleurArriere = AttribsIn[i].couleurArriere;
        EmitVertex();
    }
    //EndPrimitive();
}


// Pourquoi l'utilisation de l'un ou l'autre de ces énoncés affichent exactement la même chose à l'écran?
//      gl_Position.xy = 0.3 * gl_Position.xy;
//      gl_Position.xyz = 0.3 * gl_Position.xyz;




// Dans le nuanceur de sommets, les sommets sont déjà multipliés par la
// matrice de projection (matrProj) et les sommets à la sortie du nuanceur
// de sommets sont ainsi dans le repère de l'écran (== après la projection).
// Après la multiplication par la matrice projection, les coordonnées des
// sommets sont donc dans l'intervalle [-1,+1].  Dans ce système de
// coordonnées (ou repère), les x et y représentent des positions à l'écran,
// tandis que le z représente la distance entre le sommet et le plan avant.
// La multiplication par 0.3 a pour effet de rapprocher les x et y du centre
// de l'écran, tandis qu'elle change le z qui est la valeur de la
// profondeur, mais ce changement de z n'a pas ici d'effet visible à
// l'écran.
