#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

namespace utils {

bool union_polygons(const std::vector<glm::vec3>& polygon1, const std::vector<glm::vec3>& polygon2, std::vector<glm::vec3>& union_polygon);
float round1(float val);
float round2(float val);
glm::vec3 round1(const glm::vec3& v);
glm::vec3 round2(const glm::vec3& v);

}

