#include "rasterizer.h"
#include "scene.h"
#include "camera.h"
#include "shading.h"
#include "light.h"

#include "math/Matrix.h"
#include "math/Vector.h"

#include <cmath>
#include <memory>
#include <GL/glut.h>

using std::vector;

using Math::Point2D;
using Math::Point3D;
using Math::Point4D;

using Math::Vector3D;
using Math::Vector4D;

namespace Graphics
{
    static const int OFF_SCREEN_X = -1;
    static const int OFF_SCREEN_Y = -1;
    const UBYTE_RGB BLUE(0, 0, 255);

    std::ostream &operator<<(std::ostream &os, const Fragment &f)
    {
        os<<"(" <<(unsigned int)f.r << ", " <<(unsigned int)f.g<<", "<<(unsigned int)f.b <<", " <<(unsigned int)f.z<<") ";
        return os;
    }

    void Rasterizer::SetCamera(const Camera *camera)
    {
        Render::SetCamera(camera);

        m_fragments.clear();

        m_fragments.resize(m_camera->GetVerticalPixNum());
        for(auto &rows : m_fragments)
        {
            for(unsigned int j = 0 ; j < m_camera->GetHorizontolPixNum(); j++)
                rows.push_back(Fragment());
        }

        OnCameraChange();
    }

    static int ViewZToZBuffer(float viewZ)
    {
        assert(viewZ >= -1 && viewZ <= 1);
        return std::floor((viewZ + 1) / 2 * UCHAR_MAX);
    }

    void Rasterizer::OnCameraChange()
    {
        int horizontalPixNum = m_camera->GetHorizontolPixNum();
        int verticalPixNum = m_camera->GetVerticalPixNum();

        float fov = m_camera->GetFov();

        float near = m_camera->GetNearPlane();
        float far = m_camera->GetFarPlane();
        float top = - near * std::tan(fov / 2);
        float bottom = -top;
        float left = -top * horizontalPixNum / verticalPixNum;
        float right = - left;

        for(auto &row:m_fragments)
        {
            for(auto &f:row)
                f.Clear();
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


        m_camT.Clean();
        m_camT[0] = m_camera->GetBasis()[0];
        m_camT[1] = m_camera->GetBasis()[1];
        m_camT[2] = m_camera->GetBasis()[2];
        m_camT[0][3] = m_camera->GetPos()[0];
        m_camT[1][3] = m_camera->GetPos()[1];
        m_camT[2][3] = m_camera->GetPos()[2];
        m_camT[3][3] = 1;

        m_allT = m_vpT * m_orthoT * m_perspectiveT * m_camT;
        m_camInverseT = m_camT.Inverse();
    }

    void Rasterizer::DrawFragments(int x, int y, const UBYTE_RGB &urgb, int z)
    {
        Fragment &targetFragment = m_fragments[y][x];
        if(z < targetFragment.z){
            targetFragment.r = urgb.r;
            targetFragment.g = urgb.g;
            targetFragment.b = urgb.b;
        }
    }

    static inline int ViewPosToFragmentPos(float pos)
    {
        return std::floor(pos + 0.5);
    }

    void Rasterizer::DrawLine(const Ray2D &line)
    {

        const Point2D &pStart = line.GetPoint(line.GetStartRatio());
        const Point2D &pEnd = line.GetPoint(line.GetEndRatio());

        int pStartX = ViewPosToFragmentPos(pStart.x);
        int pStartY = ViewPosToFragmentPos(pStart.y);
        int pEndX = ViewPosToFragmentPos(pEnd.x);
        int pEndY = ViewPosToFragmentPos(pEnd.y);

        pStartX = pStartX >= 0 ? pStartX : 0;
        pStartY = pStartY >= 0 ? pStartY : 0;
        pEndX = pEndX < (int)(m_camera->GetHorizontolPixNum()) ? pEndX : m_camera->GetHorizontolPixNum();
        pEndY = pEndY < (int)(m_camera->GetVerticalPixNum()) ? pEndY : m_camera->GetVerticalPixNum();

        if(Math::almost_equal(pEnd.x - pStart.x, 0.0f, 5))
        {
            for(int y = pStartY ; y <= pEndY; y++)
                DrawFragments(pStartX, y, BLUE, 0);
        }
        else{
            float slope = (pEnd.y- pStart.y) / (pEnd.x - pStart.x);
            float yIncr = pEndX - pStartX;
            float xIncr = pStartY - pEndY;
            if(slope >= 0 && slope <= 1)
            {
                int y = pStartY;
                int d = Get2DLineValue(pStartX, pStartY, pEndX
                        , pEndY, pStartX + 1, pStartY + 0.5);
                for(int x = pStartX ; x <= pEndX ; x++)
                {
                    DrawFragments(x, y, BLUE, 0);
                    if(d < 0)
                    {
                        y = y + 1;
                        d += xIncr + yIncr;
                    }
                    else
                        d += xIncr;
                }
            }
            else if(slope > 1)
            {
                int x = pStartX;
                int d = Get2DLineValue(pStartX, pStartY, pEndX, pEndY, 
                        pStartX + 0.5, pStartY + 1);
                for(int y = pStartY ; y <= pEndY ; y++)
                {
                    DrawFragments(x, y, BLUE, 0);
                    if(d<0)
                        d += yIncr;
                    else
                    {
                        x += 1;
                        d += xIncr + yIncr;
                    }
                }
            }
            else if(slope > -1)
            {
                int y = pStartY;
                int d = Get2DLineValue(pStartX, pStartY, pEndX, pEndY,
                            pStartX - 1, pStartY + 0.5);
                for(int x = pStartX; x >= pEndX; x--)
                {
                    DrawFragments(x, y, BLUE, 0);
                    if(d < 0)
                    {
                        y = y + 1;
                        d += yIncr - xIncr;
                    }
                    else
                        d += -xIncr;
                }
            }
            else //slope < -1
            {
                int x = pStartX;
                int d = Get2DLineValue(pStartX, pStartY, pEndX, pEndY, 
                        pStartX - 0.5, pStartY + 1);
                for(int y = pStartY ; y <= pEndY ; y++)
                {
                    DrawFragments(x, y, BLUE, 0);
                    if(d<0)
                        d += yIncr;
                    else
                    {
                        x -= 1;
                        d += yIncr - xIncr;
                    }
                }
            }

        }
    }

    RGB Rasterizer::CalculateColor(const Point3D &p, const Material &mat, 
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

        return result;
    }

    void Rasterizer::DrawTriangle(const Triangle &t, const Scene &scene)
    {
        RGB colocA = CalculateColor(t.GetVertexA(), t.GetMaterial(), scene, t.GetNormal(t.GetVertexA()));
        RGB colocB = CalculateColor(t.GetVertexB(), t.GetMaterial(), scene, t.GetNormal(t.GetVertexB()));
        RGB colocC = CalculateColor(t.GetVertexC(), t.GetMaterial(), scene, t.GetNormal(t.GetVertexC()));

        Point4D vertexAH(t.GetVertexA());
        Point4D vertexBH(t.GetVertexB());
        Point4D vertexCH(t.GetVertexC());
        vertexAH.w = 1;
        vertexBH.w = 1;
        vertexCH.w = 1;

        vertexAH = m_allT * vertexAH;
        vertexBH = m_allT * vertexBH;
        vertexCH = m_allT * vertexCH;

        vertexAH.Dehomogeneous();
        vertexBH.Dehomogeneous();
        vertexCH.Dehomogeneous();

        int minX = ViewPosToFragmentPos(
                std::min({vertexAH.x, vertexBH.x, vertexCH.x}));
        int maxX = ViewPosToFragmentPos(
                std::max({vertexAH.x, vertexBH.x, vertexCH.x}));
        int minY = ViewPosToFragmentPos(
                std::min({vertexAH.y, vertexBH.y, vertexCH.y}));
        int maxY = ViewPosToFragmentPos(
                std::max({vertexAH.y, vertexBH.y, vertexCH.y}));

        float fA = Get2DLineValue(vertexBH.x, vertexBH.y, 
                vertexCH.x, vertexCH.y,  vertexAH.x, vertexAH.y);
        float fB = Get2DLineValue(vertexCH.x, vertexCH.y, 
                vertexAH.x, vertexAH.y,  vertexBH.x, vertexBH.y);
        float fC = Get2DLineValue(vertexAH.x, vertexAH.y, 
                vertexBH.x, vertexBH.y,  vertexCH.x, vertexCH.y);

        for(int x = minX; x <= maxX ; x++)
        {
            for(int y = minY ; y <= maxY; y++)
            {
                float alpha = Get2DLineValue(vertexBH.x, vertexBH.y,
                        vertexCH.x, vertexCH.y, x, y) / fA;
                float beta = Get2DLineValue(vertexCH.x, vertexCH.y, 
                vertexAH.x, vertexAH.y,  x, y) / fB;
                float gamma = Get2DLineValue(vertexAH.x, vertexAH.y, 
                vertexBH.x, vertexBH.y,  x, y) / fC;
                if(alpha >= 0 && beta >= 0 && gamma >= 0)
                {
                    if((alpha > 0 || fA * Get2DLineValue(vertexBH.x, vertexBH.y,
                        vertexCH.x, vertexCH.y, OFF_SCREEN_X, OFF_SCREEN_Y) > 0)
                        && (beta > 0 || fB * Get2DLineValue(vertexCH.x, vertexCH.y, 
                vertexAH.x, vertexAH.y,  OFF_SCREEN_X, OFF_SCREEN_Y)  > 0)
                        &&(gamma > 0 || fC * Get2DLineValue(vertexAH.x, vertexAH.y, 
                vertexBH.x, vertexBH.y,  OFF_SCREEN_X, OFF_SCREEN_Y) > 0))
                    {
                        const UBYTE_RGB &c = RGB_Convert_to_UBYTE_RGB(colocA * alpha + colocB * beta + colocC * gamma);
                        float zbuffer = alpha * vertexAH.z + beta * vertexBH.z + gamma * vertexCH.z;
                        DrawFragments(x, y, c, ViewZToZBuffer(zbuffer));
                    }
                }
            }
        }
    }

    static ubyte *texture = nullptr;
    static const GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
    void Rasterizer::Draw(const Scene &s)
    {
        OnCameraChange();

        const std::vector<std::shared_ptr<Surface>> &surfaces = s.GetSurfaces();
        for(auto pSurface : surfaces)
        {
            std::shared_ptr<Triangle> t = std::dynamic_pointer_cast<Triangle>(pSurface);
            DrawTriangle(*t, s);
        }


        if(texture)
        {
            free(texture);
        }
        texture = new ubyte[m_camera->GetHorizontolPixNum() * m_camera->GetVerticalPixNum() * 3];
        int textureIndex = 0;
        for(auto fRow: m_fragments)
        {
            for(auto f: fRow)
            {
                texture[textureIndex++]  = f.r;
                texture[textureIndex++]  = f.g;
                texture[textureIndex++]  = f.b;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glDrawPixels(m_camera->GetHorizontolPixNum(), m_camera->GetVerticalPixNum(), GL_RGB, GL_UNSIGNED_BYTE, texture);
        glFlush();
    }
}
