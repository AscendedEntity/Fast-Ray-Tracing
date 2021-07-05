#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <iomanip>

#include "const_utility.h"
#include "camera.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"
#include "cuboid.h"
#include "bvh.h"

int THREAD_COUNT;

void scene1(HittableList& world, Camera& camera, int& imgWidth, int& imgHeight, double& aspectRatio)
{
	auto ground = make_shared<Lambertian>(colour(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(point(0, -1000, 0), 1000, ground));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++) 
		{
			point center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

			if ((center - point(4, 0.2, 0)).length() > 0.9) 
			{
				shared_ptr<Material> sphereMaterial;
				auto materialChooser = randomDouble();

				if (materialChooser < 0.8) 
				{
					// lambertian
					auto albedo = colour::random() * colour::random();
					sphereMaterial = make_shared<Lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (materialChooser < 0.95) 
				{
					// metal
					auto albedo = colour::random(0.5, 1);
					auto fuzz = randomDouble(0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else 
				{
					// glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	auto matte = make_shared<Lambertian>(colour(0.4, 0.2, 0.1));
	world.add(make_shared<Sphere>(point(-4, 1, 0), 1.0, matte));

	auto glass = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(point(0, 1, 0), 1.0, glass));

	auto metal = make_shared<Metal>(colour(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared < Sphere > (point(4, 1, 0), 1.0, metal));

	aspectRatio = 16.0 / 9.0;
	imgHeight = 1080;
	imgWidth = static_cast<int>(imgHeight * aspectRatio);
	auto vfov = 20;
	auto aperture = 0.1;
	auto fDis = 10.0;

	Camera cam(point(13, 2, 3), point(0, 0, 0), aspectRatio, vfov, aperture, fDis);
	camera = cam;
}

void scene2(HittableList& world, Camera& camera, int& imgWidth, int& imgHeight, double& aspectRatio)
{
	auto ground = make_shared<Lambertian>(colour(0.48, 0.83, 0.53));
	auto light = make_shared<Light>(colour(9.0, 9.0, 9.0));
	auto white = make_shared<Lambertian>(colour(.73, .73, .73));
	auto metalGrey = make_shared<Metal>(colour(0.8, 0.8, 0.9), 0.0);
	auto metalBlue = make_shared<Metal>(colour(0.2, 0.4, 0.9), 0.2);
	auto metalYellow = make_shared<Metal>(colour(1, 1, 0), 0.0);
	auto matteRed = make_shared<Lambertian>(colour(0.8, 0.0, 0.0));
	auto matteGrey = make_shared<Lambertian>(colour(0.25, 0.25, 0.25));

	world.add(make_shared<xzPlane>(123, 423, 147, 412, 554, light));

	const int boxes = 20;
	for (int i = 0; i < boxes; i++)
	{
		for (int j = 0; j < boxes; j++)
		{
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto z1 = z0 + w;
			auto y1 = randomDouble(1, 101);

			world.add(make_shared<Cuboid>(point(x0, y0, z0), point(x1, y1, z1), ground));
		}
	}

	world.add(make_shared<Sphere>(point(230, 150, 45), 50, make_shared<Dielectric>(1.5)));
	world.add(make_shared<Sphere>(point(0, 200, 145), 50, metalGrey));
	world.add(make_shared<Sphere>(point(360, 150, 145), 70,metalBlue));
	world.add(make_shared<Sphere>(point(400, 200, 400), 100, matteRed));
	world.add(make_shared<Sphere>(point(220, 330, 300), 80, metalYellow));
	
	for (int j = 0; j < 1000; j++) {
		world.add(make_shared<Sphere>(point::random(0, 165) + point(0, 300, 0), 10, white));
	}
	
	world.add(make_shared<xyPlane>(-600, 600, 0, 1000, 600, matteGrey));
	world.add(make_shared<xyPlane>(-600, 600, 0, 1000, -601, matteGrey));

	world.add(make_shared<yzPlane>(0, 1000, -601, 600, 600, matteGrey));
	world.add(make_shared<yzPlane>(0, 1000, -601, 600, -600, matteGrey));
	
	world.add(make_shared<xzPlane>(-600, 600, -601, 600, 555, matteGrey));

	aspectRatio = 1.0;
	imgHeight = 800;
	imgWidth = static_cast<int>(imgHeight * aspectRatio);
	auto vfov = 40;
	auto aperture = 0.0;
	auto fDis = 10.0;

	Camera cam(point(478, 278, -600), point(278, 278, 0.0), aspectRatio, vfov, aperture, fDis);
	camera = cam;
}

void scene3(HittableList& world, Camera& camera, int& imgWidth, int& imgHeight, double& aspectRatio)
{
	auto red = make_shared<Lambertian>(colour(.65, .05, .05));
	auto green = make_shared<Lambertian>(colour(.12, .45, .15));
	auto blue = make_shared<Lambertian>(colour(0.2, 0.4, 0.9));
	auto yellow = make_shared<Lambertian>(colour(1, 1, 0));
	auto lightGrey = make_shared<Lambertian>(colour(.73, .73, .73));

	world.add(make_shared<xyPlane>(0, 555, 0, 555, 555, lightGrey));
	world.add(make_shared<yzPlane>(0, 555, -100, 555, 555, green));
	world.add(make_shared<yzPlane>(0, 555, -100, 555, 0, red));
	world.add(make_shared<xzPlane>(0, 555, -100, 555, 0, lightGrey));
	world.add(make_shared<xzPlane>(0, 555, -100, 555, 555, lightGrey));

	world.add(make_shared<Cuboid>(point(130, 0, 65), point(295, 165, 230), lightGrey));
	world.add(make_shared<Cuboid>(point(265, 0, 295), point(430, 330, 460), lightGrey));

	aspectRatio = 1.0;
	imgHeight = 2160;
	imgWidth = static_cast<int>(imgHeight * aspectRatio);
	auto vfov = 40;
	auto aperture = 0.0;
	auto fDis = 10.0;

	Camera cam(point(278, 278, -800), point(278, 278, 0), aspectRatio, vfov, aperture, fDis);
	camera = cam;
}

void scene4(HittableList& world, Camera& camera, int& imgWidth, int& imgHeight, double& aspectRatio)
{
	auto red = make_shared<Lambertian>(colour(.65, .05, .05));
	auto green = make_shared<Lambertian>(colour(.12, .45, .15));
	auto blue = make_shared<Lambertian>(colour(0.2, 0.4, 0.9));
	auto yellow = make_shared<Lambertian>(colour(1, 1, 0));
	auto lightGrey = make_shared<Lambertian>(colour(.73, .73, .73));
	auto light = make_shared<Light>(colour(15, 15, 15));

	world.add(make_shared<xyPlane>(0, 555, 0, 555, 555, lightGrey));
	world.add(make_shared<yzPlane>(0, 555, -100, 555, 555, green));
	world.add(make_shared<yzPlane>(0, 555, -100, 555, 0, red));
	world.add(make_shared<xzPlane>(0, 555, -100, 555, 0, lightGrey));
	world.add(make_shared<xzPlane>(0, 555, -100, 555, 555, lightGrey));
	world.add(make_shared<xzPlane>(213, 343, 227, 332, 554, light));

	world.add(make_shared<Cuboid>(point(130, 0, 65), point(295, 165, 230), lightGrey));
	world.add(make_shared<Cuboid>(point(265, 0, 295), point(430, 330, 460), lightGrey));

	aspectRatio = 1.0;
	imgHeight = 2160;
	imgWidth = static_cast<int>(imgHeight * aspectRatio);
	auto vfov = 40;
	auto aperture = 0.0;
	auto fDis = 10.0;

	Camera cam(point(278, 278, -800), point(278, 278, 0), aspectRatio, vfov, aperture, fDis);
	camera = cam;
}

void scene5(HittableList& world, Camera& camera, int& imgWidth, int& imgHeight, double& aspectRatio)
{
	auto red = make_shared<Lambertian>(colour(0.8, 0.0, 0.0));
	auto light = make_shared<Light>(colour(15.0, 15.0, 15.0));
	auto black = make_shared<Lambertian>(colour(0.25, 0.25, 0.25));

	world.add(make_shared<Sphere>(point(0, -1000, 0), 1000, black));
	world.add(make_shared<Sphere>(point(0, 2, 0), 2,red));

	auto difflight = make_shared<Light>(colour(4, 4, 4));
	world.add(make_shared<xyPlane>(3, 7, 1, 5, -2, difflight));

	aspectRatio = 16.0 / 9.0;
	imgHeight = 2160;
	imgWidth = static_cast<int>(imgHeight * aspectRatio);
	auto vfov = 20;
	auto aperture = 0.0;
	auto fDis = 10.0;

	Camera cam(point(26, 3, 6), point(0, 2, 0), aspectRatio, vfov, aperture, fDis);
	camera = cam;
}

void scene6(HittableList& world, Camera& camera, int& imgWidth, int& imgHeight, double& aspectRatio)
{
	auto sun = make_shared<Light>(colour(2.0, 2.0, 2.0));
	auto ground = make_shared<Lambertian>(colour(0.25, 0.25, 0.25));

	world.add(make_shared<Sphere>(point(0.0, -20000.0, 0.0), 20000.0, ground));
	world.add(make_shared<Sphere>(point(1250.0, 1250.0, -0.0), 1000.0, sun));

	for (int a = -6; a < 6; a++)
	{
		for (int b = -4; b < 3; b++)
		{
			point center(a * 5 + randomDouble(randomDouble(-1500, 1000), randomDouble(-1500, 1000)), 50, 5 * b + randomDouble(randomDouble(-1500, 1250), randomDouble(-1500, 1250)));
			
			shared_ptr<Material> sphereMaterial;
			auto materialChooser = randomDouble();

			if (materialChooser < 0.4)
			{
				// metal
				auto albedo = colour::random(0.5, 1);
				auto fuzz = randomDouble(0, 0.25);
				sphereMaterial = make_shared<Metal>(albedo, fuzz);
				world.add(make_shared<Sphere>(center, 50, sphereMaterial));
			}
			else if (materialChooser < 0.73)
			{
				// diffuse
				auto albedo = colour::random() * colour::random();
				sphereMaterial = make_shared<Lambertian>(albedo);
				world.add(make_shared<Sphere>(center, 50, sphereMaterial));
			}
			else
			{
				// dielectric
				sphereMaterial = make_shared<Dielectric>(randomDouble(1.25, 2.0));
				world.add(make_shared<Sphere>(center, 50, sphereMaterial));
			}

				
		}
	}

	aspectRatio = 16.0 / 9.0;
	imgHeight = 2160;
	imgWidth = static_cast<int>(imgHeight * aspectRatio);
	auto vfov = 20;
	auto aperture = 0.0;
	auto fDis = 10.0;

	Camera cam(point(-3000.0, 1700, 4500) + 2500 * unitVector(point(3000, -1700, -4500)), point(0, 0, 0), aspectRatio, vfov, aperture, fDis);
	camera = cam;
}

colour getColour(const Ray& r, const colour& background, BVH_Node root, int depth, bool lightOff)
{
	hitRecord record;

	if (depth <= 0)
		return colour(0.0, 0.0, 0.0);

	if (root.hit(r, 0.001, infinity, record))
	{
		colour objColour;
		Ray reflected;
		colour emitted = record.mat_ptr->emitted();
		
		if (record.mat_ptr->scatter(r, record, objColour, reflected))
			return emitted + objColour * getColour(reflected, background, root, depth - 1, lightOff);
		return emitted;
	}
	else
	{
		if(lightOff)
			return background;
		else
		{
			vec3 unitDir = unitVector(r.direction);
			auto root = 0.5 * (unitDir.y() + 1.0);
			return (1.0 - root) * colour(1.0, 1.0, 1.0) + root * colour(0.5, 0.7, 1.0);
		}
	}
}

void split(std::string filename, const int& imgHeight, const int& imgWidth, int start, int end, bool first, const int& samples, const int& maxDepth, const Camera& camera, BVH_Node root, const colour& background, bool lightOff)
{
	std::ofstream file(filename + ".ppm");
	if(first)
		file << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

	for (int i = start - 1; i >= end; i--)
	{
		if(first)
			std::cerr << "\r" << THREAD_COUNT * (i - end) << ' ' << std::flush;

		for (int j = 0; j < imgWidth; j++)
		{
			colour pixelColour(0.0, 0.0, 0.0);
			for (int k = 0; k < samples; k++)
			{
				auto u = (j + randomDouble()) / (double(imgWidth) - 1);
				auto v = (i + randomDouble()) / (double(imgHeight) - 1);
				Ray ray = camera.getRay(u, v);
				pixelColour += getColour(ray, background, root, maxDepth, lightOff);
			}

			auto r = sqrt(pixelColour.x() / samples);
			auto g = sqrt(pixelColour.y() / samples);
			auto b = sqrt(pixelColour.z() / samples);

			file << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
		}
	}
	file.close();
}

void combine(int n)
{
	std::ofstream image;
	std::remove("image.ppm");
	image.open("image.ppm");
	std::string line;
	int i;

	for (i = n - 1; i >= 0; i--)
	{
		std::string filename = "part" + std::to_string(i) + ".ppm";
		std::ifstream file(filename);

		while (std::getline(file, line))
		{
			image << line << '\n';
		}
		file.close();
		std::remove(const_cast<char*>(filename.c_str()));
	}
	image.close();
}

int main()
{
	std::cout << "Enter the number of threads: ";
	std::cin >> THREAD_COUNT;
	
	const int samples = 5;
	const int maxDepth = 5;
	const bool lightOff = false;

	const colour background = colour(0.0, 0.0, 0.0);

	HittableList world; Camera camera; double aspectRatio; int imgWidth; int imgHeight;
	scene3(world, camera, imgWidth, imgHeight, aspectRatio);

	BVH_Node root(world.objects, 0, world.objects.size());

	auto start = clock();

	std::thread* threads = new std::thread[THREAD_COUNT];

	for (int i = THREAD_COUNT - 1; i >= 0; i--)
	{
		threads[i] = std::thread(split, "part" + std::to_string(i), imgHeight, imgWidth, (i + 1) * imgHeight / THREAD_COUNT, i * imgHeight / THREAD_COUNT, (i == THREAD_COUNT - 1), samples, maxDepth, camera, root, background, lightOff);
	}
	
	for (int i = 0; i < THREAD_COUNT; i++)
		threads[i].join();

	combine(THREAD_COUNT);

	auto stop = clock();

	double time_taken = double(stop - start) / CLOCKS_PER_SEC;
	std::cerr << "\nTime taken by program : " << time_taken  << " secs\n";
	std::cerr << "Rendered.\n";
}