#pragma once
#include<memory>
#include"aliase.h"

using std::shared_ptr;

class Material;
class Ray;
class hittable;

class hit_record{
public:
	hit_record() {
		mat_ptr = nullptr;
		object = nullptr;
	}
public:
	vec3 p;
	vec3 normal;
	float t;
	bool front_face;
	std::shared_ptr<Material> mat_ptr; // pointer of material
	std::shared_ptr<const hittable> object;
	//vec3 T;
	//vec3 B;

	inline void set_face_normal(const Ray& r, const vec3& outward_normal);
};

class hittable :public std::enable_shared_from_this<hittable>{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec)const = 0;
};

class Sphere : public hittable {
public:
	Sphere(const vec3& center, double radius, std::shared_ptr<Material> m);
	~Sphere();

	virtual bool hit(const Ray& r,float t_min, float t_max, hit_record& rec) const override;

	void addMaterial(const shared_ptr<Material>& mat);

public:
	vec3 center;
	float radius;
	std::shared_ptr<Material> mat_ptr;
};
