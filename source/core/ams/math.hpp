#pragma once
#include <cml/cml.h>

namespace ams
{

using vec2f = cml::vector2f;
using vec2i = cml::vector2i;

inline vec2f cross_product(const vec2f& v)
{
    return {-v[1], v[0]};
}

}
