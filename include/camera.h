#include "math/Vector3D.h"
#include "ray.h"
#include <vector>
#include <memory>
#include "transform.h"

namespace Graphics{

    class Surface;
    class IntersectInfo;
    class RGB;

    class Camera : public Transform
    {
        public:
            Camera(double fov, double focalLen, int horizontalPixNum, int verticalPixNum);

            inline int GetHorizontolPixNum() const{
                return m_horizontalPixNum;
            }

            inline int GetVerticalPixNum() const{
                return m_verticalPixNum;
            }

            inline double GetFocalLen() const{
                return m_focalLen;
            }

            inline double GetFov() const{
                return m_fov;
            }

            inline const Math::Vector3D &GetU() const{
                return m_basis[1];
            }

            inline const Math::Vector3D &GetV() const{
                return m_basis[2];
            }

            inline const Math::Vector3D &GetW() const{
                return m_basis[0];
            }

            inline const Math::Point3D &GetPos() const{
                return m_pos;
            }

        private:
            double m_fov;
            double m_focalLen;
            int m_horizontalPixNum, m_verticalPixNum;
    };


};
