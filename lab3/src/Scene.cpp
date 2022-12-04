#include"Ray.h"
#include"Scene.h"
#include"hittable.h"
Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::add(const shared_ptr<hittable>& object) {
	objects.emplace_back(object);
}

void Scene::addLight(const shared_ptr<hittable>& light) {
	objects.emplace_back(light);
	lights.emplace_back(light);
}


bool Scene::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest = t_max;
	for (auto object : objects) {
		if (object->hit(r, t_min, closest, temp_rec)) {
			hit_anything = true;
			closest = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}