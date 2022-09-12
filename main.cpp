#include <iostream>
#include <memory>
#include <limits>
#include <math.h>
#include <algorithm>

#include "include/vector3.hpp"
#include "include/hittables.hpp"
#include "include/ray.hpp"
#include "include/camera.hpp"
#include "include/materials.hpp"
#include "include/light.hpp"

colour image(Ray& ray, Hittables world, Lights light_world, int depth=5)
{
    hit_record record;
    if (depth <= 0)
        // return Gradient(colour(1,1,1), colour(0.5, 0.7, 1.0)).get_colour(ray);
        return colour(0,0,0);

    if (world.intersect(ray, std::numeric_limits<double>::infinity(), record))
    {
        Ray scattered = record.material->scatter(ray, record);
        return image(scattered, world, light_world, depth-1) * record.col;
    }
    // return Gradient(colour(1,1,1), colour(0.5, 0.7, 1.0)).get_colour(ray);
    return light_world.getLight(ray);
}

void generate_image()
{
    //image dimensions
    const double aspect_ratio = 6.0/13.0;
    const int width = 375; // HD
    const int height = width / aspect_ratio;

    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    //camera -- at (0,0,0)
    Camera camera(point3(0,0,0), 2.0, aspect_ratio, 1.0);

    //lights - LIGHT CONSTRUCTION
    //set scaling factor to max radius of intensity (all beyond is black)
    Lights light_world;
    // light_world.add(std::make_shared<Light>(camera.getUpperLeftCorner()+vector3(2,1,0), 10, colour(1.0,1.0,1.0)));
    for (int i=0; i<10; i++)
    {
        light_world.add(std::make_shared<Light>(camera.getUpperLeftCorner()+vector3(0.4*i,0,0), 0.4, randColour()));
    }

    // WORLD CONSTRUCTION
    std::shared_ptr<Diffuse> ground = std::make_shared<Diffuse>(colour(0.8, 0.8, 0.0), "/home/meshva/cpp_projects/RayTracer/textures/im_2.jpg");
    std::shared_ptr<Metal> central = std::make_shared<Metal>(colour(0.7, 0.3, 0.3));
    std::shared_ptr<Metal> solid = std::make_shared<Metal>(colour(0.2,0.2,0.2));
    std::shared_ptr<Glass> glass = std::make_shared<Glass>(2.0);

    Hittables world;
    world.add(std::make_shared<Sphere>(100,0,-100.5,-1, ground));
    // world.add(std::make_shared<Sphere>(0.5,-1,0,-1, central, 0, 2*PI, 0, PI));
    // world.add(std::make_shared<Sphere>(0.5,0,0,-1, solid));
    // world.add(std::make_shared<Sphere>(0.5,1,0,-1, central));

    // world.add(std::make_shared<Rectangle>(point3(-1,0.5,-1), point3(-0.5,-0.5,-1.5), ground, true));
    // world.add(std::make_shared<Rectangle>(point3(-0.9,0.4,-1.2), point3(-0.4,-0.4,-1.4), ground, true));
    // world.add(std::make_shared<TriPlane>(point3(0,0,-1), point3(0,1,-1), point3(1,0,-2), central, false));

    //make many spheres on ground
    double r = 102;

    for (int i=0; i<100; i++)
    {
        double z = random_bounds(-3,0.2);
        double x = random_bounds(-2,2);
        //solve y in bounds
        double y = sqrt(r*r - pow(z+1,2) - x*x) - 102.5 + 0.1;

        std::cerr << x << " , " << y << " , " << z << '\n' << std::flush;
        world.add(std::make_shared<Sphere>(0.1,x,y,z, central));
    }

    //constants
    const int sample_size = 40;

    // RENDER IMAGE
    for (int r=height-1; r >=0; r--)
    {
        if (r % 10 == 0)
            std::cerr << "line: " << r << '\n' << std::flush;

        for(int c=0; c < width; c++)
        {
            // SMOOTH EDGES -- use multiple u/v to average vectors nearby
            colour pixel_col(0,0,0);
            for (int i=0; i<sample_size; i++)
            {
                double u = (double(c) + random_bounds(-1,1)) / (width-1);
                double v = (double(r) + random_bounds(-1,1)) / (height-1);

                //ray starting from origin (loc of camera) in direction u,v
                Ray ray = camera.rayFromCamera(u, v);
                pixel_col += image(ray, world, light_world);
            }

            pixel_col /= sample_size;
            pixel_col.print();
        }
    }
    std::cerr << "COMPLETE" << std::flush;
}

int main ()
{
    generate_image();
    return 0;
}