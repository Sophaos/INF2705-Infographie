#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 16) out;

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
    // émettre les sommets
    for ( int i = 0 ; i < gl_in.length() ; ++i )
    {
        gl_ViewportIndex = 0;
        gl_Position = gl_in[i].gl_Position;
        AttribsOut.couleurAvant = AttribsIn[i].couleurAvant;
        AttribsOut.couleurArriere = AttribsIn[i].couleurArriere;
        EmitVertex();
    }
    EndPrimitive();

    for ( int i = 0 ; i < gl_in.length() ; ++i )
    {
        gl_ViewportIndex = 1;
        gl_Position = gl_in[i].gl_Position;
        AttribsOut.couleurAvant = AttribsIn[i].couleurArriere;
        AttribsOut.couleurArriere = AttribsIn[i].couleurAvant;
        EmitVertex();
    }
    EndPrimitive();

    for ( int i = 0 ; i < gl_in.length() ; ++i )
    {
        gl_ViewportIndex = 2;
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x = -gl_Position.x;
        AttribsOut.couleurAvant = AttribsIn[i].couleurAvant;
        AttribsOut.couleurArriere = AttribsIn[i].couleurArriere;
        EmitVertex();
    }
    EndPrimitive();

    for ( int i = 0 ; i < gl_in.length() ; ++i )
    {
        gl_ViewportIndex = 3;
        gl_Position = gl_in[i].gl_Position;
        gl_Position.xy = -gl_Position.xy;
        AttribsOut.couleurAvant = AttribsIn[i].couleurAvant;
        AttribsOut.couleurArriere = AttribsIn[i].couleurArriere;
        EmitVertex();
    }
    EndPrimitive();
}
