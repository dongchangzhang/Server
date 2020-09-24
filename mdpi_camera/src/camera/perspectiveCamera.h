#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "../utils/cray.h"
#include "../value/const_values.h"


class perspectiveCamera{
public:
    perspectiveCamera() = default;
    ~perspectiveCamera() = default;
    perspectiveCamera(const GVector3& _eye,const GVector3& _front,const GVector3& _refUp,
            const GVector3 &_right, double _fov_hor, double _fov_ver, double _f);
    void rotateByEuler(double roll, double pitch, double yaw);
    CRay generateRay(float x,float y);

    double get_scale_h() { return fovScale_hor; }
    double get_scale_v() { return fovScale_ver; }

private:
    GVector3 eye, front, refUp;
    GVector3 right, up;
    double fov_hor, fov_ver, fovScale_hor, fovScale_ver, f;
};
#endif