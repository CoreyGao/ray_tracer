#include "../include/camera.h"
#include "../include/math/commondef.h"
#include "../include/math/Matrix33.h"
#include "../include/surface.h"
#include "../include/color.h"
#include <cmath>

using std::vector;
using std::shared_ptr;
using namespace Math;

namespace Graphics{
    Camera::Camera(double fov, double focalLen, int horizontalPixNum, int verticalPixNum)
        :m_fov(fov), m_focalLen(focalLen), m_horizontalPixNum(horizontalPixNum),
        m_verticalPixNum(verticalPixNum),
        m_u(0.0, 1.0, 0.0), m_v(0.0, 0.0, 1.0), m_w(1.0, 0.0, 0.0), m_pos()
    {

    }
};
