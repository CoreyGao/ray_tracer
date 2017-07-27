#include "camera.h"
#include "math/commondef.h"
#include "math/Matrix.h"
#include "surface.h"
#include "color.h"
#include <cmath>

using std::vector;
using std::shared_ptr;
using namespace Math;

namespace Graphics{
    Camera::Camera(double fov, double focalLen, int horizontalPixNum, int verticalPixNum)
        :m_fov(fov), m_focalLen(focalLen), m_horizontalPixNum(horizontalPixNum),
        m_verticalPixNum(verticalPixNum)
    {
        m_verticalPixNum = verticalPixNum;
    }
};
