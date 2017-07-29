#include "transform.h"
#include "tools.h"
#include <cmath>

using Math::Vector3D;
using Math::Matrix33;

namespace Graphics
{
    void Transform::Rotate(const Vector3D &d, double theta)
    {
        Matrix33 r = BuildRotateMatix(d, theta);
        m_basis[0] = r * m_basis[0];
        m_basis[1] = r * m_basis[1];
        m_basis[2] = r * m_basis[2];
    }

    void Transform::RotateRoundX(double theta)
    {
        Rotate(m_basis[0], theta);
    }

    void Transform::RotateRoundY(double theta)
    {
        Rotate(m_basis[1], theta);
    }

    void Transform::RotateRoundZ(double theta)
    {
        Rotate(m_basis[2], theta);
    }
}
