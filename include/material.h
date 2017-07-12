#pragma once

#include "color.h"

namespace Graphics{
    class Material{
        public:
            RGB k_diffuse, k_specular, k_ambient;
            int specular_factor;

    };
}
