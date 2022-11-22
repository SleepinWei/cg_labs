#pragma once
#include<memory>
#include"aliase.h"

using std::shared_ptr;

class Material;
class Ray;

class hit_record{
public:
	hit_record() {
		mat_ptr = nullptr;
	}
public:
	vec3 p;
	vec3 normal;
	float t;
	float u;
	float v;
	bool front_face;
	std::shared_ptr<Material> mat_ptr; // pointer of material
	//vec3 T;
	//vec3 B;

	inline void set_face_normal(const Ray& r, const vec3& outward_normal);
};

class hittable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec)const = 0;
};

class Sphere : public hittable {
public:
	Sphere(const vec3& center, double radius, std::shared_ptr<Material> m);
	~Sphere();

	virtual bool hit(const Ray& r,float t_min, float t_max, hit_record& rec) const override;

public:
	vec3 center;
	float radius;
	std::shared_ptr<Material> mat_ptr;
};
