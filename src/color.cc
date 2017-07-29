
#include "color.h"

namespace Graphics
{
    UBYTE_RGB RGB_Convert_to_UBYTE_RGB(const RGB &f)
    {
        assert(f.r >= 0.0f && f.r <= 1.0f);
        assert(f.g >= 0.0f && f.g <= 1.0f);
        assert(f.b >= 0.0f && f.b <= 1.0f);

        UBYTE_RGB ub;

        ub.r = (int)(std::numeric_limits<unsigned char>::max() * f.r);
        ub.g = (int)(std::numeric_limits<unsigned char>::max() * f.g);
        ub.b = (int)(std::numeric_limits<unsigned char>::max() * f.b);

        return ub;
    }
}
