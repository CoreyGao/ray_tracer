#include "color.h"

namespace Math{
    class Vector3D;
}

namespace Graphics{
    class Shading{
        public:
            static Graphics::RGB DiffuseColor(const Graphics::RGB &k , const Graphics::RGB &i, 
                    const Math::Vector3D &l, const Math::Vector3D &n);
            static Graphics::RGB SpecularColor(const Graphics::RGB &k, const Graphics::RGB &i, 
                    const Math::Vector3D &l, const Math::Vector3D &n, const Math::Vector3D &v, double factor);
            static Graphics::RGB AmbientColor(const Graphics::RGB &k, const Graphics::RGB &i) ;
    };
}

