#define _CRT_SECURE_NO_WARNINGS
#include"aliase.h"
#include"Renderer.h"
#include"Camera.h"
#include"hittable.h"
#include"Material.h"
#include"Scene.h"

int main() {
	const int SCR_WIDTH = 600;
	const int SCR_HEIGHT = 600;
	Renderer renderer;
	auto scene = std::make_shared<Scene>();
	{
		auto camera = std::make_shared<Camera>(
			vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f,
			SCR_WIDTH, SCR_HEIGHT);
		scene->camera = camera;
	}
	{
		auto white = std::make_shared<Material>(MatType::DIFFUSE,color(1.0f),1.0f);
		auto red = std::make_shared<Material>(MatType::DIFFUSE, color(1.0f, 0.0f, 0.0f), 1.0f);
		auto green = std::make_shared<Material>(MatType::DIFFUSE, color(1.0f, 0.0f, 0.0f), 1.0f);
		auto glass = std::make_shared<Material>(MatType::GLASS, color(1.0f, 1.0f, 1.0f), 1.5f);
		auto light =  std::make_shared<Material>(MatType::LIGHT, color(1.0f, 1.0f, 1.0f), 1.5f);

		auto s1 = std::make_shared<Sphere>(vec3(0.0f,-500.0f,0.0f), 490.0f, white);
		scene->add(s1);
		auto s2 = std::make_shared<Sphere>(vec3(0.0f),0.25f, red);
		scene->add(s2);
		auto s3 = std::make_shared<Sphere>(vec3(0.0f, 1.0f, 0.0f), 1.0f, light);
		scene->add(s3);
	}
	renderer.render(scene);
	renderer.writePic("./out.ppm", scene);
}