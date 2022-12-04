#pragma once
#include"aliase.h"

class Ray;
class hittable;
class hit_record;

enum class MatType{
	GLASS,
	DIFFUSE,
	LIGHT
};
class Material {
public:
	Material(MatType type_,color albedo_,float ratio)
		:type(type_),albedo(albedo_),refraction_ratio(ratio)
	{

	}
	MatType type;
	color albedo; 
	float refraction_ratio;
};
