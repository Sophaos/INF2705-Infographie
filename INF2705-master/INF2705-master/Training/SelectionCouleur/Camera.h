#ifndef __CAMERA_H__
#define __CAMERA_H__

// variables pour définir le point de vue
class Camera
{
public:
    Camera() { theta = -90.0; phi = 80.0; dist = 2.5; }
    double theta;         // angle de rotation de la caméra (coord. sphériques)
    double phi;           // angle de rotation de la caméra (coord. sphériques)
    double dist;          // distance (coord. sphériques)
} camera;

#endif
