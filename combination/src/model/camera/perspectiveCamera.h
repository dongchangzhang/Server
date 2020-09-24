#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H
#include "../data/gvector3.h"
#include "../tools/cray.h"


class perspectiveCamera{
public:
    perspectiveCamera();
    ~perspectiveCamera();
    perspectiveCamera(const GVector3& _eye,const GVector3& _front,const GVector3& _refUp,float _fov_hor, float _fov_ver);
    perspectiveCamera(const GVector3& _eye,const GVector3& _front,const GVector3& _refUp,
            const GVector3 &_right, float _fov_hor, float _fov_ver, float _f);
    void rotateByEuler(double roll, double pitch, double yaw);
    CRay generateRay(float x,float y);

    float get_scale_h() { return fovScale_hor; }
    float get_scale_v() { return fovScale_ver; }

private:
    GVector3 eye, front, refUp;
    GVector3 right, up;
    float fov_hor, fov_ver, fovScale_hor, fovScale_ver, f;
};
#endif
