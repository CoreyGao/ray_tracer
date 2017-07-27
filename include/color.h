#pragma once

namespace Graphics{
    class RGB{
        public:
            float r,g,b;
            RGB(const RGB &r):r(r.r), g(r.g), b(r.b){}
            RGB():r(0), g(0), b(0){}
            RGB(double r, double g, double b): r(r), g(g), b(b){}

            RGB operator +(const RGB &r) const{
                RGB result = *this;
                result += r;
                return result;
            }

            void operator +=(const RGB &rgb){
                r += rgb.r;
                g += rgb.g;
                b += rgb.b;
            }

            RGB operator * (float k){
                RGB result = *this;
                result.r = r * k;
                result.g = g * k;
                result.b = b * k;
                return result;
            }

            RGB operator * (const RGB &rgb) const{
                RGB result = *this;
                result.r = r * rgb.r;
                result.g = g * rgb.g;
                result.b = b * rgb.b;
                return result;
            }
    };
}
