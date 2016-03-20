#include <vector>
#include <fstream>
#include <math.h>

#include "Sphere.h"
#ifdef _WIN32
	#define M_PI 3.141592653589793
	#define INFINITY 1e8
#endif
#define MAX_DEPTH 5

float mix(const float a, const float b, const float mixRatio);
Vector3 trace(const Vector3& rayStart, const Vector3& rayDir, const std::vector<Sphere>& spheres, const int depth);
void render(const std::vector<Sphere>& spheres);

int main(int argc, char* args[])
{
	srand48(13);
	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vector3( 0.0, -10004, -20), 10000, Vector3(0.20, 0.20, 0.20), 0, 0.0));
	spheres.push_back(Sphere(Vector3( 0.0, 0, -20), 4, Vector3(1.00, 0.32, 0.36), 1, 0.5));
	spheres.push_back(Sphere(Vector3( 5.0, -1, -15), 2, Vector3(0.90, 0.76, 0.46), 1, 0.0));
	spheres.push_back(Sphere(Vector3( 5.0, 0, -25), 3, Vector3(0.65, 0.77, 0.97), 1, 0.0));
	spheres.push_back(Sphere(Vector3(-5.5, 0, -15), 3, Vector3(0.90, 0.90, 0.90), 1, 0.0));
	spheres.push_back(Sphere(Vector3( 0.0, 20, -30), 3, Vector3(0.00, 0.00, 0.00), 0, 0.0, Vector3(3)));
	render(spheres);
	return 0;
}

float mix(const float a, const float b, const float mixRatio)
{
	return a * mixRatio + b * (1 - mixRatio);
}

Vector3 trace(const Vector3& rayStart, const Vector3& rayDir, const std::vector<Sphere>& spheres, const int depth)
{
	float near = INFINITY;
	const Sphere* sphere = nullptr;

	for(unsigned int i = 0; i < spheres.size(); i++)
	{
		float l1, l2;
		l1 = l2 = INFINITY;

		if(spheres[i].intersect(rayStart, rayDir, l1, l2))
		{
			if(l1 < 0)
				l1 = l2;

			if(l1 < near)
			{
				near = l1;
				sphere = &spheres[i];
			}
		}
	}

	if(!sphere)
		return Vector3(2);

	Vector3 surfaceColor;

	Vector3 hitPoint = rayStart + rayDir * near;
	Vector3 normal = hitPoint - sphere -> center;
	normal.normalize();

	bool inside = false;
	if(rayDir.dot(normal) > 0)
	{
		normal = -normal;
		inside = true;
	}

	float bias = 1e-4;

	if((sphere -> transparency > 0 || sphere -> reflection > 0) && depth < MAX_DEPTH)
	{
		float facingRatio = -rayDir.dot(normal);
		float fresnelEffect = mix(pow(1 - facingRatio, 3), 1, 0.1);

		Vector3 reflectionDir = rayDir - normal * 2 * rayDir.dot(normal);
		reflectionDir.normalize();

		Vector3 reflection = trace(hitPoint + normal * bias, reflectionDir, spheres, depth + 1);
		Vector3 refraction = 0;

		if(sphere -> transparency > 0)
		{
			float ior = 1.1, eta;
			if(inside)
				eta = ior;
			else
				eta = 1/ior;

			float cosi = -normal.dot(rayDir);
			float k = 1 - eta * eta * (1 - cosi * cosi);

			Vector3 refractionDir = rayDir * eta + normal * (eta * cosi - sqrt(k));
			refractionDir.normalize();

			refraction = trace(hitPoint + normal * bias, reflectionDir, spheres, depth + 1);
		}

		surfaceColor = (reflection * fresnelEffect + refraction * sphere -> transparency * (1 - fresnelEffect)) * sphere -> surfaceColor;
	}
	else
	{
		for(unsigned int i = 0; i < spheres.size(); i++)
		{
			if(spheres[i].emissionColor.x > 0)
			{
				Vector3 transmission = 1;
				Vector3 lightDirection = spheres[i].center - hitPoint;
				lightDirection.normalize();

				for(unsigned int j = 0; j < spheres.size(); j++)
					if(j != i)
					{
						float l1, l2;
						if(spheres[j].intersect(hitPoint + normal * bias, lightDirection, l1, l2))
						{
							transmission = 0;
							break;
						}
					}

				surfaceColor += sphere -> surfaceColor * transmission * std::max(float(0), normal.dot(lightDirection)) * spheres[i].emissionColor;
			}
		}
	}
	return surfaceColor + sphere -> emissionColor;
}

void render(const std::vector<Sphere>& spheres)
{
	const float width = 640, height = 480;
	Vector3 *image = new Vector3[int(width)*int(height)], *pixel = image;

	const float FOV = 30, aspectRatio = width/height;
	const float angle = tan(M_PI * FOV/360);

	for(unsigned int y = 0; y < height; y++)
		for(unsigned int x = 0; x < width; x++, pixel++)
		{
			float xx = (2 * ((x + 0.5)/width) - 1) * angle * aspectRatio;
			float yy = (1 - 2 * ((y + 0.5)/height)) * angle;

			Vector3 rayDir(xx, yy, -1);
			rayDir.normalize();
			*pixel = trace(Vector3(0, 0, 0), rayDir, spheres, 0);
		}

	std::ofstream file("./untitled.ppm");
	file << "P6\n" << width << " " << height << "\n255\n";
	for(unsigned int i = 0; i < width * height; i++)
		file << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
				(unsigned char)(std::min(float(1), image[i].y) * 255) <<
				(unsigned char)(std::min(float(1), image[i].z) * 255);
	file.close();

	delete[] image;
}
