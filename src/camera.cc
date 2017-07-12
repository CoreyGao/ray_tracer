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
        :m_fov(fov), m_focalLen(focalLen), m_horizontalPixNum(horizontalPixNum), m_verticalPixNum(verticalPixNum),
        m_canvasFixed(false), m_u(0.0, 1.0, 0.0), m_v(0.0, 0.0, 1.0), m_w(1.0, 0.0, 0.0), m_pos(), m_rays(m_verticalPixNum){
        CalRays();
    }



    void Camera::CalRays(){
        m_rays.clear();
        m_points.clear();

        double canvasHalfV =  m_focalLen * tan(m_fov / 2);
        double canvasHalfU = m_horizontalPixNum / m_verticalPixNum * canvasHalfV;

        Vector3D pLeftDownInUVW(-m_focalLen, -canvasHalfU, -canvasHalfV);

        Matrix33 transfer(m_w, m_u, m_v);
        transfer = transfer.Inverse();

        m_rays.resize(m_verticalPixNum);
        m_points.resize(m_verticalPixNum);

        for(int j = 0 ; j < m_verticalPixNum ; j++){
            for(int i = 0 ; i < m_horizontalPixNum ; i++){
                double posU = pLeftDownInUVW.y + canvasHalfU * (i + 0.5) / (m_horizontalPixNum / 2);
                double posV = pLeftDownInUVW.z + canvasHalfV * (j + 0.5) / (m_verticalPixNum / 2);

                Vector3D p(pLeftDownInUVW.x, posU, posV);
                m_points[j].push_back(Point3D(p.x, p.y, p.z));
                m_rays[j].push_back(Ray(m_pos, p, 0, 1000));
            }
        }
    }
};
