#include "Utils.h"
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>

namespace utils {

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                                   Point_2;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>                   Pwh_list_2;

bool union_polygons(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2, std::vector<glm::vec3>& union_polygon) {
	union_polygon.clear();

	if (polygon1.size() < 3 || polygon2.size() < 3) return false;


	glm::vec3 x_axis = glm::normalize(polygon1[1] - polygon1[0]);
	glm::vec3 y_axis = glm::normalize(polygon1[2] - polygon1[1]);
	glm::vec3 z_axis = glm::cross(x_axis, y_axis);
	y_axis = glm::cross(z_axis, x_axis);

	glm::mat4 mat;
	mat[0] = glm::vec4(x_axis, 0);
	mat[1] = glm::vec4(y_axis, 0);
	mat[2] = glm::vec4(z_axis, 0);
	mat[3] = glm::vec4(0, 0, 0, 1);

	glm::mat4 inv = glm::inverse(mat);

	Polygon_2 pol1;
	float z;
	for (int i = 0; i < polygon1.size(); ++i) {
		glm::vec3 p = glm::vec3(inv * glm::vec4(polygon1[i], 1));
		z = p.z;
		pol1.push_back(Point_2(round1(p.x), round1(p.y)));
	}
	if (pol1.is_clockwise_oriented()) {
		pol1.reverse_orientation();
	}

	Polygon_2 pol2;
	for (int i = 0; i < polygon2.size(); ++i) {
		glm::vec3 p = glm::vec3(inv * glm::vec4(polygon2[i], 1));
		pol2.push_back(Point_2(round1(p.x), round1(p.y)));
	}
	if (pol2.is_clockwise_oriented()) {
		pol2.reverse_orientation();
	}

	Polygon_with_holes_2 unionPol;
	if (CGAL::join (pol1, pol2, unionPol)) {
		for (int k = 0; k < unionPol.outer_boundary().size(); ++k) {
			glm::vec3 p(CGAL::to_double(unionPol.outer_boundary()[k].x()), CGAL::to_double(unionPol.outer_boundary()[k].y()), z);
			glm::vec3 p2 = glm::vec3(mat * glm::vec4(p, 1));
			if (k > 0 && glm::length(union_polygon.back() - p2) < 0.1) continue;

			if (k > 1 && glm::dot(glm::normalize(union_polygon.back() - union_polygon[union_polygon.size() - 2]), glm::normalize(p2 - union_polygon.back())) > 0.99) {
				union_polygon[union_polygon.size() - 1] = p2;
			} else {
				union_polygon.push_back(p2);
			}
		}

		if (union_polygon.size() > 3 && glm::dot(glm::normalize(union_polygon.back() - union_polygon[union_polygon.size() - 2]), glm::normalize(union_polygon[0] - union_polygon.back())) > 0.99) {
			union_polygon.erase(union_polygon.begin() + union_polygon.size() - 1);
		}

		return true;
	} else {
		return false;
	}
}

float round1(float val) {
	if (val >= 0.0f) {
		return (float)((int)(val * 10 + 0.5)) * 0.1f;
	} else {
		return (float)((int)(val * 10 - 0.5)) * 0.1f;
	}
}

float round2(float val) {
	if (val >= 0.0f) {
		return (float)((int)(val * 100 + 0.5)) * 0.01f;
	} else {
		return (float)((int)(val * 100 - 0.5)) * 0.01f;
	}
}

glm::vec3 round1(const glm::vec3& v) {
	glm::vec3 ret;
	ret.x = round1(v.x);
	ret.y = round1(v.y);
	ret.z = round1(v.z);

	return ret;
}

glm::vec3 round2(const glm::vec3& v) {
	glm::vec3 ret;
	ret.x = round2(v.x);
	ret.y = round2(v.y);
	ret.z = round2(v.z);

	return ret;
}

}