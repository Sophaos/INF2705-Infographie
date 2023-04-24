#ifndef __CAMERA_H__
#define __CAMERA_H__

// variables pour définir le point de vue
class Camera
{
public:
    Camera() { theta = phi = 0.0; dist = 30.0; }
    void verifierAngles()
    {
        const GLdouble MINPHI = -90.0, MAXPHI = 90.0;
        if ( phi > MAXPHI ) phi = MAXPHI; else if ( phi < MINPHI ) phi = MINPHI;
        if ( theta > 360.0 ) theta -= 360.0; else if ( theta < 0.0 ) theta += 360.0;
    }
    double theta;         // angle de rotation de la caméra (coord. sphériques)
    double phi;           // angle de rotation de la caméra (coord. sphériques)
    double dist;          // distance (coord. sphériques)
} camera;

#endif
