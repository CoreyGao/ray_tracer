#pragma once

#include "math/Vector.h"
#include "ray.h"
#include <vector>
#include <memory>
#include "transform.h"

namespace Graphics{

    class Surface;
    class IntersectInfo;

    class Camera : public Transform
    {
        public:
            Camera(float fov, float near, float far, int horizontalPixNum, int verticalPixNum)
                :m_fov(fov), m_near(near), m_far(far), m_horizontalPixNum(horizontalPixNum),
                m_verticalPixNum(verticalPixNum)
            {
            }

            inline unsigned int GetHorizontolPixNum() const
            {
                return m_horizontalPixNum;
            }

            inline unsigned int GetVerticalPixNum() const
            {
                return m_verticalPixNum;
            }

            inline float GetNearPlane() const
            {
                return m_near;
            }

            inline float GetFarPlane() const
            {
                return m_near;
            }

            inline float GetFov() const
            {
                return m_fov;
            }

        private:
            float m_fov;
            float m_near, m_far;
            unsigned int m_horizontalPixNum, m_verticalPixNum;
    };


};
