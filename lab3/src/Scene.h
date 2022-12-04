#pragma once
#include<memory>
#include<vector>
#include"hittable.h"

class hittable;
class Ray;
class hit_record;
class Camera;

using std::shared_ptr;
using std::vector;

class Scene:public hittable{
public:
	Scene();
	~Scene();

	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
	void add(const shared_ptr<hittable>& object);
	void addLight(const shared_ptr<hittable>& light);

public:
	vector<shared_ptr<hittable>> objects;
	vector<shared_ptr<hittable>> lights;
	shared_ptr<Camera> camera; 
};
