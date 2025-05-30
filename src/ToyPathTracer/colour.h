#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using colour = vec3;

void write_colour(std::ostream& out, const colour& pixel_colour) {
    auto r = pixel_colour.x();
    auto g = pixel_colour.y();
    auto b = pixel_colour.z();

    // Convert from [0, 1] to byte range [0, 255]
    int rbyte = int(255.9 * r);
    int gbyte = int(255.9 * g);
    int bbyte = int(255.9 * b);

    // Write out the colour components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif