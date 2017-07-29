#ifndef TESTER
#include "raytracer.h"
#include "camera.h"
#include "light.h"
#include "scene.h"
#include "material.h"
#include "surface.h"
#include "math/Matrix.h"
#include "math/commondef.h"

#include <iostream>
#include "GL/glut.h"

using namespace Graphics;

using std::vector;
using std::shared_ptr;
using std::make_shared;

using Math::PI;
using Math::Point3D;
using Math::Vector3D;
using Math::Matrix33;


const int HORIZONTAL_PIX_NUM = 300;
const int VERTICAL_PIX_NUM = 300;

RayTracer render;
Scene scene; 
Camera camera(PI/2, 0.2f, 10.0f, HORIZONTAL_PIX_NUM, VERTICAL_PIX_NUM);
Material mat = {RGB(0, 0, 0), RGB(1, 0.5, 0.5), RGB(0.1, 0.1, 0.1), 10};

void display(void) {
    render.Draw(scene);
}

void process_key(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 119: //w
            camera.Move(Vector3D(-0.1, 0, 0));
            display();
            break;

        case 97: //a
            camera.Move(Vector3D(0, 0.1, 0));
            display();
            break;

        case 115: //s
            camera.Move(Vector3D(0.1, 0, 0));
            display();
            break;

        case 100: //d
            camera.Move(Vector3D(0, -0.1, 0));
            display();
            break;

        case 101: //e
            camera.RotateRoundZ(PI / 30);
            display();
            break;

        case 113: //q
            camera.RotateRoundZ(-PI / 30);
            display();
            break;
    }
}


void init(){
    render.SetCamera(&camera);
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
    glutInitWindowSize(HORIZONTAL_PIX_NUM, VERTICAL_PIX_NUM);
    glutInitWindowPosition(100, 100);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glutCreateWindow("dadada");
    glutDisplayFunc(display);

    glutKeyboardFunc(process_key);
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
