#include <iostream>
#include "GLWidget3D.h"
#include "MainWindow.h"
#include <GL/GLU.h>
#include "GLUtils.h"
#include "GrammarParser.h"
#include "Rectangle.h"

#define SQR(x)	((x) * (x))

GLWidget3D::GLWidget3D() {
}

/**
 * This event handler is called when the mouse press events occur.
 */
void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	camera.mousePress(e->x(), e->y());
}

/**
 * This event handler is called when the mouse release events occur.
 */
void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
}

/**
 * This event handler is called when the mouse move events occur.
 */
void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (e->buttons() & Qt::LeftButton) { // Rotate
		camera.rotate(e->x(), e->y());
	} else if (e->buttons() & Qt::MidButton) { // Move
		camera.move(e->x(), e->y());
	} else if (e->buttons() & Qt::RightButton) { // Zoom
		camera.zoom(e->x(), e->y());
	}

	updateGL();
}

/**
 * This function is called once before the first call to paintGL() or resizeGL().
 */
void GLWidget3D::initializeGL() {
	fb = new FrameBuffer(width(), height());
	fb->loadStrokes("..\\strokes", "..\\strokes_mini\\");
}

/**
 * This function is called whenever the widget has been resized.
 */
void GLWidget3D::resizeGL(int width, int height) {
	camera.updatePMatrix(width, height);

	fb->resize(width, height);
}

/**
 * This function is called whenever the widget needs to be painted.
 */
void GLWidget3D::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	fb->setClearColor(glm::vec3(1, 1, 1));
	fb->clear();

	//glutils::drawBox(1, 1, 1, glm::vec4(1, 1, 1, 1), glm::mat4(), vertices);
	fb->rasterize(&camera, vertices);
	fb->draw();
}

void GLWidget3D::loadCGA(const std::string& filename) {
	vertices.clear();

	float object_width = 2.0f;
	float object_height = 1.0f;

	{
		cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
		system.stack.push_back(boost::shared_ptr<cga::Shape>(start));
	}

	try {
		cga::Grammar grammar;
		cga::parseGrammar(filename.c_str(), grammar);
		system.randomParamValues(grammar);
		system.derive(grammar);
		system.generateGeometry(vertices);
	} catch (const std::string& ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	} catch (const char* ex) {
		std::cout << "ERROR:" << std::endl << ex << std::endl;
	}

	normalizeObjectSize(vertices);
	
	updateGL();
}

void GLWidget3D::normalizeObjectSize(std::vector<std::vector<Vertex> >& vertices) {
	glm::vec3 minPt((std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)());
	glm::vec3 maxPt = -minPt;

	// もとのサイズを計算
	for (int i = 0; i < vertices.size(); ++i) {
		for (int j = 0; j < vertices[i].size(); ++j) {
			minPt.x = min(minPt.x, vertices[i][j].position.x);
			minPt.y = min(minPt.y, vertices[i][j].position.y);
			minPt.z = min(minPt.z, vertices[i][j].position.z);
			maxPt.x = max(maxPt.x, vertices[i][j].position.x);
			maxPt.y = max(maxPt.y, vertices[i][j].position.y);
			maxPt.z = max(maxPt.z, vertices[i][j].position.z);
		}
	}

	glm::vec3 center = (maxPt + minPt) * 0.5f;

	float size = max(maxPt.x - minPt.x, max(maxPt.y - minPt.y, maxPt.z - minPt.z));
	float scale = 1.0f / size;

	// 単位立方体に入るよう、縮尺・移動
	for (int i = 0; i < vertices.size(); ++i) {
		for (int j = 0; j < vertices[i].size(); ++j) {
			vertices[i][j].position = (vertices[i][j].position - center) * scale;
		}
	}
}
