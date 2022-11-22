#pragma once
#include"aliase.h"
class Ray {
public:
	Ray() = default;
	~Ray() = default;
	
	Ray(vec3 origin, vec3 direction): orig(origin),dir(direction) {}

	vec3 at(float t)const {
		return orig + t * dir;
	}

public:
	vec3 orig;
	vec3 dir;
};

