#ifndef TESTER
#include "render.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "material.h"
#include "surface.h"
#include "math/Matrix33.h"
#include "math/commondef.h"

#include <iostream>
#include "GL/glut.h"


using RayTracer::Render;
using namespace Math;
using namespace Graphics;
using std::vector;
using std::shared_ptr;
using std::make_shared;

Render render;
Scene scene; 
Camera camera(PI/2, 0.5, 300, 300);
Material mat = {RGB(0, 0, 0), RGB(1, 0.5, 0.5), RGB(0.1, 0.1, 0.1), 10};

void display(void) {
    render.Draw(camera, scene);
}

void init(){
    auto t = make_shared<Triangle>(mat, Point3D(-1, -1, -1), Point3D(-1, 1, -1), Point3D(-1, 0, 1));
    scene.AddSurface(t);

    DiffuseLight d(RGB(0.5, 0.4, 0.3), Vector3D(-1, sqrt(3), 0));
    SpecularLight s(RGB(1, 0.0, 0.0), Vector3D(-1, 0, 0));
    AmbientLight a(RGB(0.1, 0.0, 0.1));

    scene.SetDiffuseLight(d);
    scene.AddSpecularLight(s);
    scene.SetAmbientLight(a);
}


int main(int argc, char **argv){
    init();

    display();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutCreateWindow("dadada");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

#else
#include <gtest/gtest.h>

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    if(RUN_ALL_TESTS() != 0)
        ::std::cout<<"no!"<<::std::endl;
    return 0;
}

#endif
