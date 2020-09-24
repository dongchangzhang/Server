
#include"perspectiveCamera.h"
#include "../tools/utils.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>


perspectiveCamera::perspectiveCamera() {

}

perspectiveCamera::~perspectiveCamera() {
}

perspectiveCamera::perspectiveCamera(const GVector3 &_eye, const GVector3 &_front, const GVector3 &_refUp,
                                     float _fov_hor, float _fov_ver) {
    eye = _eye;
    front = _front;
    refUp = _refUp;
    fov_hor = _fov_hor;
    fov_ver = _fov_ver;
    right = front.crossMul(refUp);
    up = right.crossMul(front);
    fovScale_hor = tan(fov_hor * (PI * 0.5f / 180)) * 2;
    fovScale_ver = tan(fov_ver * (PI * 0.5f / 180)) * 2;
}

perspectiveCamera::perspectiveCamera(const GVector3 &_eye, const GVector3 &_front, const GVector3 &_refUp,
                                     const GVector3 &_right, float _fov_hor, float _fov_ver, float _f) {

    f = _f;
    eye = _eye;
    front = _front;
    up = _refUp;
    right = _right;
    fov_hor = _fov_hor;
    fov_ver = _fov_ver;

    fovScale_hor =  f * tan(fov_hor * (PI * 0.5f / 180)) * 2;
    fovScale_ver =  f * tan(fov_ver * (PI * 0.5f / 180)) * 2;

}

CRay perspectiveCamera::generateRay(float x, float y) {
    GVector3 r = right * ((x - fovScale_hor / 2) / f); // / min(fovScale_hor, fovScale_ver));
    GVector3 u = up * ((y - fovScale_ver / 2) / f); //  min(fovScale_hor, fovScale_ver));
    GVector3 tmp = front + r + u;
    tmp.normalize();
    return CRay(eye, tmp);
}

void perspectiveCamera::rotateByEuler(double roll, double pitch, double yaw) {
    roll = degrees2radians(roll);
    pitch = degrees2radians(pitch);
    yaw = degrees2radians(yaw);

    // init euler: Z-Y-X -> RPY
    Eigen::Vector3d ea(roll, pitch, yaw);

    // euler to rotation matrix
    Eigen::Matrix3d rotation_matrix3, vectors_in, vectors_out;

    rotation_matrix3 = Eigen::AngleAxisd(ea[0], Eigen::Vector3d(front.x, front.y, front.z)) *
                       Eigen::AngleAxisd(ea[1], Eigen::Vector3d(right.x, right.y, right.z)) *
                       Eigen::AngleAxisd(ea[2], Eigen::Vector3d(up.x, up.y, up.z));

    up.normalize();
    right.normalize();
    front.normalize();

    vectors_in << front.x, front.y, front.z, right.x, right.y, right.z, up.x, up.y, up.z;
    vectors_out = vectors_in * rotation_matrix3;
    front = GVector3(vectors_out(0, 0), vectors_out(0, 1), vectors_out(0, 2));
    right = GVector3(vectors_out(1, 0), vectors_out(1, 1), vectors_out(1, 2));
    up = GVector3(vectors_out(2, 0), vectors_out(2, 1), vectors_out(2, 2));

    up.normalize();
    right.normalize();
    front.normalize();
}

