#include"hittable.h"
#include"Ray.h"
#include"Material.h"


void hit_record::set_face_normal(const Ray& r, const vec3& outward_normal) {
	this->front_face = dot(r.dir, outward_normal) < 0;
	this->normal = front_face ? outward_normal : -outward_normal;
}

Sphere::~Sphere() {

}

Sphere::Sphere(const vec3& center, double radius, std::shared_ptr<Material> material) :
	center(center), radius(radius), mat_ptr(material) {
}

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec)const {
	float t = -1.0f;
	vec3 ooc = r.orig - center;
	float a = dot(r.dir, r.dir);
	float half_b = dot(ooc, r.dir);
	float c = dot(ooc, ooc) - radius * radius;

	float delta = half_b * half_b - a * c;
	if (delta < 0) {
		return false;
	}
	float sqrtdelta = sqrtf(delta);
	float root = (-half_b - sqrtdelta) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtdelta) / a;
		if (root < t_min || t_max < root) {
			return false;
		}
	}
	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;
	rec.object = shared_from_this();
	//rec.T = normalize(vec3(-rec.p.y, rec.p.x, 0));
	//rec.B = normalize(cross(rec.normal, rec.T));

	return true;
}

void Sphere::addMaterial(const shared_ptr<Material>& mat) {
	this->mat_ptr = mat;
}
