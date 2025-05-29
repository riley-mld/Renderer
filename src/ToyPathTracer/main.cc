#include "colour.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

colour ray_colour(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a) * colour(1.0, 1.0, 1.0) + a*colour(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Caclualte image height and make sure it's at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1)? 1 : image_height;

    // Camera
    auto focal_lengrh = 1;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    // Setting based on right handed coordianate system
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_lengrh) - viewport_u/2 - viewport_v/2;
    auto pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Renderer
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j=0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i=0; i < image_width; i++) {
            auto pixel_center = pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            colour pixel_colour = ray_colour(r);
            write_colour(std::cout, pixel_colour);
        }
    }
    
    std::clog << "\rDone.           \n";
}