#include "render.h"
#include "surface.h"
#include "camera.h"
#include "color.h"
#include "scene.h"
#include "math/Vector3D.h"
#include "math/Matrix33.h"
#include "shading.h"
#include <iostream>


using namespace Graphics;
using std::vector;
using std::shared_ptr;
using namespace Math;

GLuint Framebuffer = 0;

Material defaultMat;
float *texture = nullptr;
static const GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
void RayTracer::Draw(const Scene &scene)
{
    vector<vector<Graphics::Ray>> rays;
    CalRays(rays);
    auto intersectInfoList = GetIntersectInfo(scene.GetSurfaces(), rays); //TODO:if camera and surfaces not change, then shouldn't do this

    vector<vector<RGB>> colors(m_camera->GetVerticalPixNum());
    for(int i = 0 ; i < m_camera->GetVerticalPixNum() ; i++){
        colors[i].resize(m_camera->GetHorizontolPixNum());
    }

    int rowIndex = 0;
    for(auto intersectInfoRow : intersectInfoList)
    {
        int colIndex = 0;
        for(auto intersectIno : intersectInfoRow)
        {   
            CalculateColor(intersectIno, colors[rowIndex][colIndex], scene);
            colIndex++;
        }
        rowIndex++;
    }


    if(texture)
    {
        free(texture);
    }
    texture = new float[m_camera->GetHorizontolPixNum() * m_camera->GetVerticalPixNum() * 3];
    int textureIndex = 0;
    for(auto colorRow : colors)
    {
        for(auto color : colorRow)
        {
            texture[textureIndex++]  = color.r;
            texture[textureIndex++]  = color.g;
            texture[textureIndex++]  = color.b;
        }
    }


    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glDrawPixels(m_camera->GetHorizontolPixNum(), m_camera->GetVerticalPixNum(), GL_RGB, GL_FLOAT, texture);
    glFlush();
}

vector<vector<IntersectInfo>> RayTracer::GetIntersectInfo(const vector<shared_ptr<Surface>> &surfaces, const vector<vector<Ray> > &rays) const{
    vector<vector<IntersectInfo>> infoList(rays.size());

    int infoListRow = 0;
    for(auto rows : rays)
    {
        for(auto ray : rows)
        {
            IntersectInfo *min = nullptr;

            for(auto p_surface : surfaces)
            {
                IntersectInfo info(p_surface->GetMaterial(), ray);
                if(p_surface->IntersectWithRay(ray, info)){
                    if(!min)
                        min = &info;
                    else if(min->ratio > info.ratio)
                        min = &info;
                }
            }
            infoList[infoListRow].push_back(min?*min:IntersectInfo(defaultMat));
        }
        infoListRow++;
    }

    return infoList;
}

void RayTracer::CalculateColor(const IntersectInfo &info, RGB &rgb, const Graphics::Scene &scene) const{
    if(info.IsEmpty()){
        rgb.r = 0;
        rgb.g = 0;
        rgb.b = 0;
        return;
    }

    rgb += Shading::DiffuseColor(info.mat.k_diffuse, scene.GetDiffuseLight().intensity, scene.GetDiffuseLight().dir, info.surfaceNormal);
    const vector<SpecularLight> &spec = scene.GetSpecularLightList();
    for(auto s:spec)
    {
        rgb += Shading::SpecularColor(info.mat.k_specular, s.intensity, s.dir,
                info.surfaceNormal, info.r.GetDir(), info.mat.specular_factor);
    }

    rgb += Shading::AmbientColor(info.mat.k_ambient, scene.GetAmbientLight().intensity);
}


void RayTracer::CalRays(std::vector<std::vector<Graphics::Ray>> &rays){
    rays.clear();

    double canvasHalfV =  m_camera->GetFocalLen() * tan(m_camera->GetFov() / 2);
    double canvasHalfU = m_camera->GetHorizontolPixNum() / m_camera->GetVerticalPixNum()* canvasHalfV;

    Vector3D pLeftDownInUVW(-m_camera->GetFocalLen(), -canvasHalfU, -canvasHalfV);

    Matrix33 transfer(m_camera->GetW(), m_camera->GetU(), m_camera->GetV());

    rays.resize(m_camera->GetVerticalPixNum());

    for(int j = 0 ; j < m_camera->GetVerticalPixNum(); j++){
        for(int i = 0 ; i < m_camera->GetHorizontolPixNum(); i++){
            double posU = pLeftDownInUVW.y + canvasHalfU * (i + 0.5) / (m_camera->GetHorizontolPixNum()/ 2);
            double posV = pLeftDownInUVW.z + canvasHalfV * (j + 0.5) / (m_camera->GetVerticalPixNum()/ 2);

            Vector3D p(pLeftDownInUVW.x, posU, posV);
            p = transfer * p;
            rays[j].push_back(Ray(m_camera->GetPos(), p, 0, 1000));
        }
    }
}

