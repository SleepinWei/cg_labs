#pragma once
#include<memory>
#include<vector>
#include"hittable.h"

class hittable;
class Ray;
class hit_record;
using std::shared_ptr;
using std::vector;

class Scene:public hittable{
public:
	Scene();
	~Scene();

	virtual void hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;
	void add(const shared_ptr<hittable>& object);

public:
	vector<shared_ptr<hittable>> world;
	vector<shared_ptr<hittable>> lights;
};
