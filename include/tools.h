#pragma once

#include "math/Matrix33.h"
#include "math/Vector3D.h"
#include "math/Point.h"

namespace Graphics
{
    Math::Vector3D NonInlinearV(const Math::Vector3D &v);
    //in new basis, t is x
    Math::Matrix33 BuildBasis(const Math::Vector3D &t);
    Math::Matrix33 BuildRotateMatix(const Math::Vector3D &d, double theta);
}
