#pragma once

#include "math/Vector.h"

namespace Graphics{
    template<typename TPoint, typename TVector>
    class RayT{
        public:
            RayT() = default;
            ~RayT() = default;

            RayT(const RayT &r)
                :m_pointStart(r.m_pointStart), m_dir(r.m_dir),
                m_ratioStart(r.m_ratioStart), m_ratioEnd(r.m_ratioEnd)
            {
            }

            RayT(const TPoint &startPoint, const TVector &dirVector, float startRatio, float endRatio)
                :m_pointStart(startPoint), m_dir(dirVector),
                m_ratioStart(startRatio), m_ratioEnd(endRatio)
            { 
                assert(startRatio >= 0);
                assert(endRatio > startRatio);

                m_dir.Normalize();
            }

            inline TPoint GetPoint(float r) const
            {
                TPoint p = m_pointStart + m_dir * r;
                return p;
            }

            inline const TVector &GetDir() const
            {
                return m_dir;
            }

            inline float GetStartRatio() const
            {
                return m_ratioStart;
            }

            inline float GetEndRatio() const
            {
                return m_ratioEnd;
            }

            inline bool IsPointOnRay(const TPoint &p) const
            {
                TVector ps = p - m_pointStart;
                ps.Normalize();

                if(ps == m_dir)
                    return ps.Norm() >= m_ratioStart && ps.Norm() <= m_ratioEnd;

                return false;
            }

            void operator = (const RayT &r)
            {
                m_pointStart = r.m_pointStart;
                m_ratioStart = r.m_ratioStart;
                m_ratioEnd = r.m_ratioEnd;
                m_dir = r.m_dir;
            }


        private:
            TPoint m_pointStart;
            TVector m_dir;
            float m_ratioStart, m_ratioEnd;
    };

    typedef RayT<Math::Point2D, Math::Vector2D> Ray2D;
    typedef RayT<Math::Point3D, Math::Vector3D> Ray;
    typedef RayT<Math::Point4D, Math::Vector4D> RayHomogeneous;

    inline float Get2DLineValue(
            float pStartX, float pStartY, float pEndX, float pEndY,
            float x, float y)
    {
        return (pStartY - pEndY) * x + (pEndX - pStartX) * y 
            - pEndX * pStartY + pStartX * pEndY;
    }

}
