#include "raytracer.h"
#include "surface.h"
#include "camera.h"
#include "color.h"
#include "scene.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "shading.h"
#include <GL/glut.h>
#include <iostream>


using namespace Graphics;
using namespace Math;

using std::make_shared;
using std::shared_ptr;
using std::dynamic_pointer_cast;
using std::vector;


bool RayTracerTriangle::IntersectWithRay(const Ray &r, IntersectInfo &info) const
{
    Point3D e = r.GetPoint(0);
    Vector3D d = r.GetPoint(1) - e;

    Point3D a = GetVertexA();
    Point3D b = GetVertexB();
    Point3D c = GetVertexC();
    Vector3D ab = a - b;
    Vector3D ac = a - c;
    Vector3D ae = a - e;

    double detA = Matrix33(ab.x, ac.x, d.x, ab.y, ac.y, d.y, ab.z, ac.z, d.z).Det();

    double detBeta = Matrix33(ae.x, ac.x, d.x, ae.y, ac.y, d.y, ae.z, ac.z, d.z).Det();
    double beta = detBeta / detA ;
    double detGame = Matrix33(ab.x, ae.x, d.x, ab.y, ae.y, d.y, ab.z, ae.z, d.z).Det();
    double gama = detGame / detA;
    double detT = Matrix33(ab.x, ac.x, ae.x, ab.y, ac.y, ae.y, ab.z, ac.z, ae.z).Det();
    double t = detT / detA;

    if(t < r.GetStartRatio() || t> r.GetEndRatio())
        return false;
    if(beta < 0 || beta > 1)
        return false;
    if(gama < 0 || gama > (1 - beta))
        return false;

    info.interPoint = r.GetPoint(t);
    info.surfaceNormal = GetNormal(info.interPoint);
    info.ratio = t;
    info.mat = &m_mat;
    info.r = r;

    return true;
}

const Material &RayTracerTriangle::GetMaterial() const
{
    return Triangle::GetMaterial();
}


bool RayTracerSphere::IntersectWithRay(const Ray &r, IntersectInfo &info) const{
    Point3D e = r.GetPoint(0);
    const Vector3D& d = r.GetDir();
    Point3D center = GetCenter();
    double radius = GetRadius();

    Vector3D ec(e.x - center.x, e.y - center.y, e.z - center.z);
    double A = Dot(d, d);
    double B = 2 * Dot(d, ec);
    double C = Dot(ec, ec) - std::pow(radius, 2); 

    double root = std::pow(B, 2) - 4 * A * C;
    if(root < 0 )
        return false;

    double t1 = (-B + std::sqrt(root)) / (2 * A);
    double t2 = (-B - std::sqrt(root)) / (2 * A);

    double t = 0;
    if(t2 >= r.GetStartRatio() && t2 <= r.GetEndRatio())
        t = t2;
    else if(t1 >= r.GetStartRatio() && t1 <= r.GetEndRatio())
        t = t1;
    else
        return false;

    info.interPoint = r.GetPoint(t);
    info.surfaceNormal = GetNormal(info.interPoint);
    return true;
}

const Material &RayTracerSphere::GetMaterial() const
{
    return Sphere::GetMaterial();
}


GLuint Framebuffer = 0;

float *texture = nullptr;
static const GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
void RayTracer::Draw(const Scene &scene)
{
    vector<vector<Graphics::Ray>> rays;
    CalRays(rays);

    vector<shared_ptr<Surface>> surfaces = scene.GetSurfaces();
    vector<shared_ptr<RaytracerSurfaceInterface>> interfaces;
    for(auto s : surfaces)
    {
        shared_ptr<Triangle> ri = dynamic_pointer_cast<Triangle>(s);
        if(ri)
            interfaces.push_back(make_shared<RayTracerTriangle>(*ri));

        shared_ptr<Sphere> si = dynamic_pointer_cast<Sphere>(s);
        if(si)
            interfaces.push_back(make_shared<RayTracerSphere>(*si));
    }

    auto intersectInfoList = GetIntersectInfo(interfaces, rays); //TODO:if camera and surfaces not change, then shouldn't do this

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

vector<vector<IntersectInfo>> RayTracer::GetIntersectInfo(
        const vector<shared_ptr<RaytracerSurfaceInterface>> &surfaces, 
        const vector<vector<Ray> > &rays) const{

    vector<vector<IntersectInfo>> infoList(rays.size());

    int infoListRow = 0;
    for(auto rows : rays)
    {
        for(auto ray : rows)
        {
            IntersectInfo minInfo;

            IntersectInfo info;
            for(auto p_surface : surfaces)
            {
                if(p_surface->IntersectWithRay(ray, info)){
                    if(minInfo.IsEmpty() || minInfo.ratio > info.ratio)
                        minInfo = info;
                }
            }
            infoList[infoListRow].push_back(minInfo);
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

    rgb += Shading::DiffuseColor(info.mat->k_diffuse, scene.GetDiffuseLight().intensity, scene.GetDiffuseLight().dir, info.surfaceNormal);
    const vector<SpecularLight> &spec = scene.GetSpecularLightList();
    for(auto s:spec)
    {
        rgb += Shading::SpecularColor(info.mat->k_specular, s.intensity, s.dir,
                info.surfaceNormal, info.r.GetDir(), info.mat->specular_factor);
    }

    rgb += Shading::AmbientColor(info.mat->k_ambient, scene.GetAmbientLight().intensity);
}


void RayTracer::CalRays(std::vector<std::vector<Graphics::Ray>> &rays){
    rays.clear();

    double canvasHalfV =  m_camera->GetFocalLen() * tan(m_camera->GetFov() / 2);
    double canvasHalfU = m_camera->GetHorizontolPixNum() / m_camera->GetVerticalPixNum()* canvasHalfV;

    Vector3D pLeftDownInUVW(-m_camera->GetFocalLen(), -canvasHalfU, -canvasHalfV);

    const Matrix33 &transfer = m_camera->GetBasis();

    rays.resize(m_camera->GetVerticalPixNum());

    for(int j = 0 ; j < m_camera->GetVerticalPixNum(); j++){
        for(int i = 0 ; i < m_camera->GetHorizontolPixNum(); i++){
            double posU = pLeftDownInUVW.y + canvasHalfU * (i + 0.5) / (m_camera->GetHorizontolPixNum()/ 2);
            double posV = pLeftDownInUVW.z + canvasHalfV * (j + 0.5) / (m_camera->GetVerticalPixNum()/ 2);

            Point3D p(pLeftDownInUVW.x, posU, posV);
            p =  transfer * p;
            rays[j].push_back(Ray(m_camera->GetPos(), p, 0, 1000));
        }
    }
}


