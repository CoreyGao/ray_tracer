#include "rasterizer.h"
#include "scene.h"
#include "camera.h"
#include "shading.h"
#include "light.h"

#include "math/Matrix.h"
#include "math/Vector.h"

#include <cmath>

using std::vector;

using Math::Point2D;
using Math::Point3D;
using Math::Point4D;

using Math::Vector3D;
using Math::Vector4D;

namespace Graphics
{

    void Rasterizer::OnCameraChange()
    {
        int horizontalPixNum = m_camera->GetHorizontolPixNum();
        int verticalPixNum = m_camera->GetVerticalPixNum();

        float fov = m_camera->GetFov();

        float near = -(m_camera->GetNearPlane());
        float far = -(m_camera->GetFarPlane());
        float top = near * std::tan(fov / 2);
        float bottom = -top;
        float left = -top * horizontalPixNum / verticalPixNum;
        float right = - left;

        m_fragments.clear();

        m_fragments.resize(verticalPixNum);
        for(auto rows : m_fragments)
        {
            for(int j = 0 ; j < horizontalPixNum ; j++)
                rows.push_back(Fragment());
        }

        m_vpT.Clean();
        m_vpT[0][0] = ((float)horizontalPixNum) / 2;
        m_vpT[0][3] = ((float)horizontalPixNum - 1.0f) / 2;
        m_vpT[1][1] = ((float)verticalPixNum) / 2;
        m_vpT[1][3] = ((float)verticalPixNum - 1.0f) / 2;
        m_vpT[2][2] = 1;
        m_vpT[3][3] = 1;

        m_orthoT.Clean();
        m_orthoT[0][0] = 2 / (right - left);
        m_orthoT[0][3] = -(right + left) / (right - left);
        m_orthoT[1][1] = 2 / (top - bottom);
        m_orthoT[1][3] = -(top + bottom) / (top - bottom);
        m_orthoT[2][2] = 2 / (near - far);
        m_orthoT[2][3] = -(near + far) / (near - far);
        m_orthoT[3][3] = 1;

        m_perspectiveT.Clean();
        m_perspectiveT[0][0] = near;
        m_perspectiveT[1][1] = near;
        m_perspectiveT[2][2] = near + far;
        m_perspectiveT[2][3] = - near * far;
        m_perspectiveT[3][2] = 1;

        m_allT = m_vpT * m_orthoT * m_perspectiveT;

        m_camT.Clean();
        m_camT[0] = m_camera->GetBasis()[0];
        m_camT[1] = m_camera->GetBasis()[1];
        m_camT[2] = m_camera->GetBasis()[2];
        m_camT[0][3] = m_camera->GetPos()[0];
        m_camT[1][3] = m_camera->GetPos()[1];
        m_camT[2][3] = m_camera->GetPos()[2];
        m_camT[3][3] = 1;

        m_camInverseT = m_camT.Inverse();
    }

    void Rasterizer::DrawFragments(int x, int y, const UBYTE_RGB &urgb, int z)
    {
        Fragment &targetFragment = m_fragments[x][y];
        if(z < targetFragment.z){
            targetFragment.r = urgb.r;
            targetFragment.g = urgb.g;
            targetFragment.b = urgb.b;
        }
    }

    static inline int FragmentPosToFragmentIndex(float pos)
    {
        return std::floor(pos + 0.5);
    }

    void Rasterizer::DrawLine(const Ray2D &line)
    {
        const UBYTE_RGB BLUE(0, 0, 255);

        const Point2D &pStart = line.GetPoint(line.GetStartRatio());
        const Point2D &pEnd = line.GetPoint(line.GetEndRatio());

        int pStartX = FragmentPosToFragmentIndex(pStart.x);
        int pStartY = FragmentPosToFragmentIndex(pStart.y);
        int pEndX = FragmentPosToFragmentIndex(pEnd.x);
        int pEndY = FragmentPosToFragmentIndex(pEnd.y);

        float slope = (pEnd.y- pStart.y) / (pEnd.x - pStart.x);
        if(slope >= 0 && slope <= 1)
        {
            int y = pStartY;
            int d = GetPointLineValue(pStart, pEnd, pStartX + 1, pStartY + 0.5);
            for(int x = pStartX ; x <= pEndX ; x++)
            {
                DrawFragments(x, y, BLUE, 0);
                if(d < 0)
                {
                    y = y + 1;
                    d += pStartY - pEndY + pEndX - pStartX;
                }
                else
                    d += pStartY - pEndY;
            }
        }
    }

    UBYTE_RGB Rasterizer::CalculateColor(const Point3D &p, const Material &mat, 
            const Scene &scene, const Vector3D &n)
    {
        Point4D pH(p);
        pH.w = 1;
        pH = m_camInverseT * pH;

        Vector3D viewDir(pH.x, pH.y, pH.z);

        RGB result;
        result  = result + Shading::DiffuseColor(mat.k_diffuse, 
                scene.GetDiffuseLight().intensity, 
                scene.GetDiffuseLight().dir, n);

        for(auto spec : scene.GetSpecularLightList())
        {
            result = result +Shading::SpecularColor(
                    mat.k_specular, spec.intensity, 
                    spec.dir, n, viewDir, mat.specular_factor);
        }

        result = result + Shading::AmbientColor(
                mat.k_ambient, scene.GetAmbientLight().intensity);

        return RGB_Convert_to_UBYTE_RGB(result);
    }

    void Rasterizer::DrawTriangle(const Triangle &t, vector<Fragment> &fragments)
    {
        
    }

    void Rasterizer::Draw(const Scene &s)
    {

    }
}
