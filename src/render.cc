#include "../include/render.h"
#include "../include/surface.h"
#include "../include/camera.h"
#include "../include/color.h"
#include "../include/scene.h"
#include "../include/math/Vector3D.h"
#include <iostream>


using namespace Graphics;
using std::vector;
using std::shared_ptr;
using namespace Math;

GLuint Framebuffer = 0;

Material defaultMat;
float *texture = nullptr;
static const GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
namespace RayTracer{
    void Render::Draw(const Camera &camera, const Scene &scene)
    {
        auto intersectInfoList = GetIntersectInfo(scene.GetSurfaces(), camera.GetRays()); //TODO:if camera and surfaces not change, then shouldn't do this
        
        vector<vector<RGB>> colors(camera.GetVerticalPixNum());
        for(int i = 0 ; i < camera.GetVerticalPixNum() ; i++){
            colors[i].resize(camera.GetHorizontolPixNum());
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
        texture = new float[camera.GetHorizontolPixNum() * camera.GetVerticalPixNum() * 3];
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
        glDrawPixels(camera.GetHorizontolPixNum(), camera.GetVerticalPixNum(), GL_RGB, GL_FLOAT, texture);
        glFlush();
    }

    vector<vector<IntersectInfo>> Render::GetIntersectInfo(const vector<shared_ptr<Surface>> &surfaces, const vector<vector<Ray> > &rays) const{
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
    
    void Render::CalculateColor(const IntersectInfo &info, RGB &rgb, const Graphics::Scene &scene) const{
        if(info.IsEmpty()){
            rgb.r = 0;
            rgb.g = 0;
            rgb.b = 0;
            return;
        }

        rgb += DiffuseColor(info.mat.k_diffuse, scene.GetDiffuseLight().intensity, scene.GetDiffuseLight().dir, info.surfaceNormal);
        const vector<SpecularLight> &spec = scene.GetSpecularLightList();
        for(auto s:spec)
        {
            rgb += SpecularColor(info.mat.k_specular, s.intensity, s.dir,
                        info.surfaceNormal, info.r.GetDir(), info.mat.specular_factor);
        }

        rgb += AmbientColor(info.mat.k_ambient, scene.GetAmbientLight().intensity);
    }

    RGB Render::DiffuseColor(const RGB &k, const RGB &i, const Vector3D &l, const Vector3D &n) const{
        double costheta = Dot(n, -l);
        costheta = (0 > costheta)?0:costheta;
        return k * i * costheta;
    }

    RGB Render::SpecularColor(const RGB &k, const RGB &i, const Vector3D &l, const Vector3D &n, const Vector3D &v, double factor) const{
        Vector3D uV = v.Unit();
        Vector3D uL = l.Unit();
        Vector3D h = -uV + -uL;
        h.Normalize();
        double costheta = Dot(n, h);
        costheta = (0 > costheta)?0:costheta;
        return (k * i) * pow(costheta, factor);
    }

    RGB Render::AmbientColor(const RGB &k, const RGB &i) const{
        return k * i;
    }

}
