#ifndef COLOR_H
#define COLOR_H
//==============================================================================================
// Originally written in 2020 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "vec3.h"

#include <iostream>

struct COL
{
    char r, g, b;
};

COL get_color(color pixel_color, int samples_per_pixel) 
{
    COL a;
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);


    a.r = static_cast<char>(256 * clamp(r, 0.0, 0.999));
    a.g = static_cast<char>(256 * clamp(g, 0.0, 0.999));
    a.b = static_cast<char>(256 * clamp(b, 0.0, 0.999));

    return a;
}

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    COL col = get_color(pixel_color, samples_per_pixel);

    char a[3] = {
        col.r,
        col.g,
        col.b
    };

    out.write(a, 3);
}






#endif
