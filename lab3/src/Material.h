#pragma once
#include"aliase.h"

class SolidColor;
class Ray;
class hittable;
class hitRecord;

class Material {
public:
	virtual bool scatter(
		const Ray& r_in, const hitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
	virtual bool emit()const { return false; };
};

class Lambertian :public Material {
public:
	Lambertian(const vec3& a);

	virtual bool scatter(
		const Ray& r, const hitRecord& rec,
		vec3& attenuation,
		Ray& scattered
	)const override;

public:
	color albedo;
};

class Metal : public Material {
public:
	Metal(const color& a, double f);
	virtual bool scatter(
		const Ray& r, const hitRecord& rec,
		vec3& attenuation,
		Ray& scattered
	) const override;
public:
	color albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float index_of_refraction);

	virtual bool scatter(
		const Ray& r, const hitRecord& rec, vec3& attenuation, Ray& scattered
	)const override;
public:
	float ir;
private:
	static double reflectance(double cosine, double ref_idx);
};

class DiffuseLight : public Material {
public:
	DiffuseLight(color c);

	virtual bool scatter(const Ray& r_in, const hitRecord& rec, vec3& attenuation, Ray& scattered)const override;

	virtual vec3 emitted(float u, float v, const vec3& p)const override;
public:
	color emit;
};
