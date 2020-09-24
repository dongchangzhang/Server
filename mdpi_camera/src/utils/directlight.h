#ifndef DIRECTLIGHT_H
#define DIRECTLIGHT_H
#include "color.h"
#include "gvector3.h"
#include "csphere.h"

class DirectLight
{
public:
    DirectLight();
    DirectLight(Color _color,GVector3 _direction,bool _isShadow);
    Color intersect(CSphere &sphereModel, IntersectResult &result, unsigned char* rgb);

private:
    bool isShadow;

    Color color;
    GVector3 direction;
};

#endif // DIRECTLIGHT_H
