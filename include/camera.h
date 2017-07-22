#include "math/Vector3D.h"
#include "ray.h"
#include <vector>
#include <memory>

namespace Graphics{
    class Surface;
    class IntersectInfo;
    class RGB;

    class Camera{
        public:
            Camera(double fov, double focalLen, int horizontalPixNum, int verticalPixNum);

            inline const std::vector<std::vector<Graphics::Ray>> & GetRays() const{
                return m_rays;
            }

            inline const std::vector<std::vector<Math::Point3D>> & GetPoints() const{
                return m_points;
            }

            inline void SetCanvasOrthogonal(){
                m_canvasFixed = false;
            }
            inline void SetCanvasFixed(){
                m_canvasFixed = true;
            }

            inline int GetHorizontolPixNum() const{
                return m_horizontalPixNum;
            }

            inline int GetVerticalPixNum() const{
                return m_verticalPixNum;
            }

        private:
            void CalRays();

        private:
            double m_fov;
            double m_focalLen;
            int m_horizontalPixNum, m_verticalPixNum;
            bool m_canvasFixed;

            Math::Vector3D m_u, m_v, m_w;
            Math::Point3D m_pos;
            std::vector<std::vector<Graphics::Ray>> m_rays;
            std::vector<std::vector<Math::Point3D>> m_points;
    };


};
