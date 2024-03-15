#include <math.h>
#include <stdio.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

// fonction qui calcule l'angle à prendre pour aller d'un point A(X,Y) à un point B(x,y)

float GetTravelAngle(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{

    float dx = end_x - start_x;
    float dy = end_y - start_y;

    float angle_rad = atan2(-dy, dx);

    angle_rad -= (M_PI / 2);

    // Conversion de radians en degrés
    float angle_deg = angle_rad * (180.0 / M_PI);

    // Si l'angle est en négatif, on le règle pour qu'il soit entre 0 et 359
    if (angle_deg < 0)
        angle_deg += 360.0;

    else if (angle_deg >= 360)
        angle_deg -= 360;

    return angle_deg;
}

// fonction qui calcule quel collecteur doit viser quelle planète
