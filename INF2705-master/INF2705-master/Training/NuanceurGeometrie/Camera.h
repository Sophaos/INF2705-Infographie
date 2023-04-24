#ifndef __CAMERA_H__
#define __CAMERA_H__

// variables pour définir le point de vue
class Camera
{
public:
    Camera() { theta = phi = 0.0; dist = 10.0; }
    double theta;         // angle de rotation de la caméra (coord. sphériques)
    double phi;           // angle de rotation de la caméra (coord. sphériques)
    double dist;          // distance (coord. sphériques)
} camera;

#endif
