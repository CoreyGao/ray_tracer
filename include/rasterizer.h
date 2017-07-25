#pragma once

#include "render.h"

namespace Graphics
{

    class Rasterizer : public Render
    {
        Rasterizer() = default;
        ~Rasterizer() = default;

        virtual void Draw(const Scene &scene);
    };
}
