#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
    public:
        double aspect_ratio = 16.0 / 9.0;
        int image_width = 400;
        int samples_per_pixel = 10; // Count of random samples for each pixel
        int max_depth = 10; //Maximum number of ray bounces into scene

        void render(const hittable& world) {
            initialise();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j=0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i=0; i < image_width; i++) {
                colour pixel_colour(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_colour += ray_colour(r, max_depth, world);
                }
                write_colour(std::cout, pixel_samples_scale * pixel_colour);
            }
        }
        
        std::clog << "\rDone.           \n";

        }

    private:
        int image_height;
        double pixel_samples_scale; // Colour scale factor for a sum of pixel samples
        point3 center; // Camera center
        point3 pixel100_loc; // Location of pixel 0, 0
        vec3 pixel_delta_u; // Distance to pixel to the right
        vec3 pixel_delta_v; // Distance to pixel below
        
        void initialise() {
            // Caclualte image height and make sure it's at least 1
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1)? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = point3(0, 0, 0);

            // Determine viewport dimensions
            auto focal_lengrh = 1;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges
            // Setting based on right handed coordianate system
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - vec3(0, 0, focal_lengrh) - viewport_u/2 - viewport_v/2;
            pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            // Create a camera ray originitating from the origin and directed at a randomly sampled point around pixel i, j
            
            auto offset = sample_square();
            auto pixel_sample = pixel100_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            // Returns the vector to a random point in the [-0.5, -0.5]-[+0.5, +0.5] unit square
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        colour ray_colour(const ray& r, int depth, const hittable& world) const {
            // If we've exceedeed ray bounce limit, more more light is gathered.
            if (depth <= 0) {
                return colour(0,0,0);
            }
            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                colour attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)){
                    //return colour(0,0,0);
                    return attenuation * ray_colour(scattered, depth-1, world);
                    std::clog << "depht";
                }
                return colour(0,0,0);
            }
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a) * colour(1.0, 1.0, 1.0) + a*colour(0.5, 0.7, 1.0);
        }
};

#endif
