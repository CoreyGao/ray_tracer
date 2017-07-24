#pragma once

#include "math/Matrix33.h"
#include "math/Point.h"

namespace Graphics
{
    class Transform{
        public:
            Transform():m_basis(1, 0, 0, 0, 1, 0, 0, 0, 1), m_pos()
            {
            }

            Transform(const Math::Vector3D &x, const Math::Vector3D &y, const Math::Vector3D &z)
                :m_basis(x, y, z), m_pos()
            {
            }

            virtual ~Transform()
            {
            }

            //rotation direction:view along rotation axis direction, clock-wise
            void Rotate(const Math::Vector3D &d, double theta);
            void RotateRoundX(double theta);
            void RotateRoundY(double theta);
            void RotateRoundZ(double theta);

            inline void Move(const Math::Vector3D &v)
            {
                m_pos += v;
            }

            inline const Math::Point3D & GetPos() const
            {
                return m_pos;
            }

            inline const Math::Matrix33 & GetBasis() const
            {
                return m_basis;
            }


        protected:
            Math::Matrix33 m_basis;
            Math::Point3D m_pos;
    };
}
