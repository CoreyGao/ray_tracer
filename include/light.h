#pragma once

#include "math/Vector3D.h"
#include "color.h"

namespace Graphics{
    class Light{
        public:
            RGB intensity;
            Light(const RGB &c):intensity(c) {}
            Light(){}
            ~Light() {}

    };
    class DiffuseLight : public Light{
        public:
            DiffuseLight(const DiffuseLight &s)
                :Light(s), dir(s.dir){
                dir.Normalize();
            }
            DiffuseLight(){}
            DiffuseLight(const RGB &c, const Math::Vector3D &d)
                :Light(c), dir(d)
            {
                dir.Normalize();
            }
            ~DiffuseLight(){}

            Math::Vector3D dir;
    };

    class SpecularLight : public Light{
        public:
            SpecularLight(const SpecularLight &s)
                :Light(s), dir(s.dir){
                dir.Normalize();
            }
            SpecularLight(const RGB &c, const Math::Vector3D &d)
                :Light(c), dir(d)
            {
                dir.Normalize();
            }
            SpecularLight() {}
            ~SpecularLight() {}

            Math::Vector3D dir;
    };

    class AmbientLight : public Light{
        public:
            AmbientLight(){}
            AmbientLight(const RGB &c) :Light(c){}
            ~AmbientLight(){}
    };
}
