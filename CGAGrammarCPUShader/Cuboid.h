#pragma once

#include <boost/shared_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "Shape.h"

class RenderManager;

namespace cga {

class Cuboid : public Shape {
public:
	Cuboid() {}
	Cuboid(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float depth, float height, const glm::vec3& color);
	boost::shared_ptr<Shape> clone(const std::string& name) const;
	void comp(const std::map<std::string, std::string>& name_map, std::vector<boost::shared_ptr<Shape> >& shapes);
	void setupProjection(float texWidth, float texHeight);
	void size(float xSize, float ySize, float zSize);
	void split(int splitAxis, const std::vector<float>& sizes, const std::vector<std::string>& names, std::vector<boost::shared_ptr<Shape> >& objects);
	void generateGeometry(float opacity, std::vector<std::vector<Vertex> >& vertices) const;
};

}
