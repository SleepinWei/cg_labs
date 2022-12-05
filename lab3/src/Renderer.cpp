#include"Ray.h"
#include"Scene.h"
#include"Renderer.h"
#include"hittable.h"
#include"Material.h"
#include"Camera.h"
#include<limits>
#include<fstream>

Renderer::Renderer()
	:width(600), height(600),image(nullptr)
{

}

Renderer::~Renderer() {
	if (image)
		delete image;
}

vec3 refract(const vec3& ray_in, const vec3& normal, float ratio) {
	float cos_theta = fmin(glm::dot(-ray_in, normal), 1.0);
	vec3 r_out_perp = ratio * (ray_in + cos_theta * normal);
	vec3 r_out_parallel = -sqrtf(fabs(1.0 - glm::dot(r_out_perp,r_out_perp))) * normal;
	return r_out_perp + r_out_parallel;
}

color Renderer::trace(const Ray& ray, const shared_ptr<Scene>& scene,int depth) {
	const float reflect_atten = 0.7f;
	const float refract_atten = 0.3f;
	const float ka = 0.05f;
	if (depth >= 5) {
		return color(0.0f);
	}

	hit_record rec;
	constexpr float infinity = std::numeric_limits<float>::infinity();

	bool isHit = scene->hit(ray, 0.001f, infinity, rec);
	if (!isHit) {
		return background;
	}

	vec3 finalColor = vec3(0.0f);
	//TODO: implement ray tracing and shading algorithm for 3 type of material.

	//end of TODO
	return finalColor;
}

#include<iostream>
void Renderer::render(const shared_ptr<Scene>& scene) {
	auto& camera = scene->camera;
	int h = camera->height;
	int w = camera->width;

	image = new int[3 * w * h];

	for (int j = h-1; j >= 0; j--) {
		if (j % 20 == 0) {
			std::cout << (h-1-j) * 1.0f / (h - 1) << "%\n";
		}
		for (int i = 0; i < w; i++) {
			vec3 color(0, 0, 0);
			auto u = float(i) / (w - 1);
			auto v = float(j) / (h - 1);
			Ray r = camera->get_ray(u, v);
			//std::cout << r.dir.x << ' ' << r.dir.y << ' ' << r.dir.z << '\n';
			color = trace(r,scene,0);

			int pos = j * w + i;
			//write_color(color, pos);
			color = glm::clamp(glm::sqrt(color),0.0f,1.0f);
			image[3 * pos] = 255 * color.x;
			image[3 * pos + 1] = 255 * color.y;
			image[3 * pos + 2] = 255 * color.z;
		}
	}
}

void Renderer::writePic(const string& filename, const shared_ptr<Scene>&scene) {
	std::cout << "Writing to file: " << filename << '\n';
	auto& camera = scene->camera;

	std::ofstream f(filename);
	int h = camera->height;
	int w = camera->width;
	f << "P3\n" << camera->width << ' ' << camera->height << "\n255\n";

	for (int j = h - 1; j >= 0; j--) {
		for (int i = 0; i < w; i++) {
			int index = j * w + i;
			f << image[3 * index] << ' ' << image[3 * index + 1]
				<< ' ' << image[3 * index + 2] << '\n';
		}
	}
}
