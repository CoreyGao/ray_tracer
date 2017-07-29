#pragma once

#include <limits.h>
#include <type_traits>
#include <algorithm>
#include <cassert>

namespace Graphics{
    template<typename T>
    class Color{
        public:
            T r,g,b;
            Color(const Color &r):r(r.r), g(r.g), b(r.b){}
            Color():r(0), g(0), b(0){}
            Color(const T &r, const T &g, const T &b): r(r), g(g), b(b){}

            Color operator +(const Color &r) const{
                Color result = *this;
                result += r;
                return result;
            }

            void operator +=(const Color &rgb){
                r += rgb.r;
                g += rgb.g;
                b += rgb.b;

                r = r > 1.0f ? 1.0f : r;
                g = r > 1.0f ? 1.0f : g;
                b = r > 1.0f ? 1.0f : b;
            }

            Color operator * (T k){
                Color result = *this;
                result.r = r * k;
                result.g = g * k;
                result.b = b * k;
                return result;
            }

            Color operator * (const Color &rgb) const{
                Color result = *this;
                result.r = r * rgb.r;
                result.g = g * rgb.g;
                result.b = b * rgb.b;
                return result;
            }
    };

    typedef Color<float> RGB;
    typedef Color<unsigned char> UBYTE_RGB;

    //convert float-format RGB value f to unsigned-char format value ub
    UBYTE_RGB RGB_Convert_to_UBYTE_RGB(const RGB &f);
}
