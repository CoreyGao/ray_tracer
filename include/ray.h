#pragma once

#include "math/Point.h"
#include "math/Vector.h"

namespace Graphics{
    class Ray{
        public:
            Ray() = default;
            Ray(const Math::Point3D &ps, const Math::Point3D &pt, double rs, double re)
                :m_pointStart(ps), m_pointT(pt), m_ratioStart(rs), m_ratioEnd(re),
                m_dir(pt.x - ps.x, pt.y - ps.y, pt.z - ps.z)
            {
                    m_dir.Normalize();
            }

            Ray(const Ray &r)
                :m_pointStart(r.m_pointStart), m_pointT(r.m_pointT), 
                m_ratioStart(r.m_ratioStart), m_ratioEnd(r.m_ratioEnd),
                m_dir(r.m_dir){ }

            Ray(const Math::Point3D &ps, const Math::Vector3D &dir, double rs, double re)
                :m_pointStart(ps), 
                m_pointT(m_pointStart.x + dir.x, m_pointStart.y + dir.y, m_pointStart.z + dir.z),
                m_ratioStart(rs), m_ratioEnd(re), m_dir(dir)
            { 
                m_dir.Normalize();
            }

            inline Math::Point3D GetPoint(float r) const
            {
                Math::Point3D p = m_pointStart + (m_pointT - m_pointStart) * r;
                return p;
            }

            inline const Math::Vector3D& GetDir() const
            {
                return m_dir;
            }

            inline double GetStartRatio() const{return m_ratioStart;}
            inline double GetEndRatio() const{return m_ratioEnd;}
            inline bool IsPointOnRay(const Math::Point3D &p) const
            {
                Math::Vector3D ps = p - m_pointStart;

                if(ps.Unit() == m_dir){
                    return ps.Norm() >= m_ratioStart && ps.Norm() <= m_ratioEnd;
                }
                return false;
            }

            void operator = (const Ray &r)
            {
                m_pointStart = r.m_pointStart;
                m_pointT = r.m_pointT;
                m_ratioStart = r.m_ratioStart;
                m_ratioEnd = r.m_ratioEnd;
                m_dir = r.m_dir;
            }


        private:
            Math::Point3D m_pointStart, m_pointT;
            double m_ratioStart, m_ratioEnd;
            Math::Vector3D m_dir;
    };
}
