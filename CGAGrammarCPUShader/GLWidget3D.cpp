#include <iostream>
#include "GLWidget3D.h"
#include "MainWindow.h"
#include <GL/GLU.h>
#include "GLUtils.h"
#include "GrammarParser.h"
#include "Rectangle.h"
#include <QDir>
#include <QTextStream>
#include "Utils.h"

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
	fb->setClearColor(glm::vec3(1, 1, 1));
	fb->clear();

	fb->rasterize(&camera, vertices, 0);
	fb->draw();
}

void GLWidget3D::loadCGA(const std::string& filename) {
	vertices.clear();

	float object_width = 10.0f;
	float object_height = 8.0f;

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

	simplifyGeometry(vertices);
	normalizeObjectSize(vertices);
	
	updateGL();
}

void GLWidget3D::simplifyGeometry(std::vector<std::vector<Vertex> >& vertices) {
	// simplify the coordinates
	for (int i = 0; i < vertices.size(); ++i) {
		for (int k = 0; k < vertices[i].size(); ++k) {
			vertices[i][k].position = utils::round1(vertices[i][k].position);
		}
	}

	while (true) {
		bool merged = false;

		for (int i = 0; i < vertices.size() && !merged; ++i) {
			glm::vec3 n1 = glm::normalize(glm::cross(vertices[i][1].position - vertices[i][0].position, vertices[i][2].position - vertices[i][0].position));
			float d1 = glm::dot(n1, vertices[i][0].position);
			AABB box1;
			for (int k = 0; k < vertices[i].size(); ++k) {
				box1.AddPoint(vertices[i][k].position);
			}

			for (int j = i + 1; j < vertices.size() && !merged; ++j) {
				glm::vec3 n2 = glm::normalize(glm::cross(vertices[j][1].position - vertices[j][0].position, vertices[j][2].position - vertices[j][0].position));
				if (glm::dot(n1, n2) < 0.99) continue;

				float d2 = glm::dot(n2, vertices[j][0].position);
				if (fabs(d1 - d2) > 0.01) continue;

				AABB box2;
				for (int k = 0; k < vertices[j].size(); ++k) {
					box2.AddPoint(vertices[j][k].position);
				}

				if (!box1.intersect(box2)) continue;

				// merge two polygons
				std::vector<glm::vec3> polygon1;
				for (int k = 0; k < vertices[i].size(); ++k) {
					polygon1.push_back(vertices[i][k].position);
				}
				std::vector<glm::vec3> polygon2;
				for (int k = 0; k < vertices[j].size(); ++k) {
					polygon2.push_back(vertices[j][k].position);
				}

				std::vector<glm::vec3> union_polygon;
				if (utils::union_polygons(polygon1, polygon2, union_polygon)) {
					std::vector<Vertex> union_vertices;
					for (int k = 0; k < union_polygon.size(); ++k) {
						union_vertices.push_back(Vertex(union_polygon[k], glm::vec3(0, 0, 1), glm::vec4(0, 0, 0, 1)));
					}

					vertices.erase(vertices.begin() + j);
					vertices.erase(vertices.begin() + i);
					vertices.push_back(union_vertices);

					merged = true;
				}
			}
		}

		if (!merged) break;
	}
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

void GLWidget3D::generateImages(int image_width, int image_height, bool invertImage, bool blur) {
	QDir dir("..\\cga\\window\\");
	//QDir dir("..\\cga\\windows_low_LOD\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);

	camera.xrot = 90.0f;
	camera.yrot = 0.0f;
	camera.zrot = 0.0f;
	camera.pos = glm::vec3(0, 0, 2.5f);
	camera.updateMVPMatrix();

	int origWidth = width();
	int origHeight = height();
	resize(image_width, image_height);
	resizeGL(image_width, image_height);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;
	
		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 1.0f; object_width <= 2.6f; object_width += 0.05f) {
			for (float object_height = 1.0f; object_height <= 1.8f; object_height += 0.05f) {
				for (int k = 0; k < 2; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					std::vector<float> param_values;

					vertices.clear();

					// generate a window
					cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

					try {
						cga::Grammar grammar;
						cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
						param_values = system.randomParamValues(grammar);
						system.derive(grammar, true);
						system.generateGeometry(vertices);
					} catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					} catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					normalizeObjectSize(vertices);

					// put ratio of width/height at the begining of the param values array
					param_values.insert(param_values.begin(), object_width / object_height);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";

					// render a window
					fb->setClearColor(glm::vec3(1, 1, 1));
					fb->clear();
					fb->rasterize(&camera, vertices, count);
					fb->draw();

					QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 4, 10, QChar('0'));
					QImage image = grabFrameBuffer();

					if (invertImage) {
						image.invertPixels();
					}

					if (blur) {
						cv::Mat mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine());
						cv::GaussianBlur(mat, mat, cv::Size(7, 7), 0, 0);
					}
			
					image.save(filename);

					count++;
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::generateBuildingImages(int image_width, int image_height, bool invertImage, bool blur) {
	QDir dir("..\\cga\\building\\");
	//QDir dir("..\\cga\\windows_low_LOD\\");

	if (!QDir("results").exists()) QDir().mkdir("results");

	srand(0);

	int origWidth = width();
	int origHeight = height();
	resize(image_width, image_height);
	resizeGL(image_width, image_height);

	QStringList filters;
	filters << "*.xml";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
	for (int i = 0; i < fileInfoList.size(); ++i) {
		int count = 0;
	
		if (!QDir("results/" + fileInfoList[i].baseName()).exists()) QDir().mkdir("results/" + fileInfoList[i].baseName());

		QFile file("results/" + fileInfoList[i].baseName() + "/parameters.txt");
		if (!file.open(QIODevice::WriteOnly)) {
			std::cerr << "Cannot open file for writing: " << qPrintable(file.errorString()) << std::endl;
			return;
		}

		QTextStream out(&file);

		for (float object_width = 10.0f; object_width <= 14.0f; object_width += 0.5f) {
			for (float object_height = 10.0f; object_height <= 14.0f; object_height += 0.5f) {
				for (int k = 0; k < 16; ++k) { // 1 images (parameter values are randomly selected) for each width and height
					// change camera view direction
					camera.xrot = 35.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
					camera.yrot = -45.0f + ((float)rand() / RAND_MAX - 0.5f) * 40.0f;
					camera.zrot = 0.0f;
					camera.pos = glm::vec3(0, 0, 2.5f);
					camera.updateMVPMatrix();

					std::vector<float> param_values;

					vertices.clear();

					// generate a window
					cga::Rectangle* start = new cga::Rectangle("Start", glm::translate(glm::rotate(glm::mat4(), -3.141592f * 0.5f, glm::vec3(1, 0, 0)), glm::vec3(-object_width*0.5f, -object_height*0.5f, 0)), glm::mat4(), object_width, object_height, glm::vec3(1, 1, 1));
					system.stack.push_back(boost::shared_ptr<cga::Shape>(start));

					try {
						cga::Grammar grammar;
						cga::parseGrammar(fileInfoList[i].absoluteFilePath().toUtf8().constData(), grammar);
						param_values = system.randomParamValues(grammar);
						system.derive(grammar, true);
						system.generateGeometry(vertices);
					} catch (const std::string& ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					} catch (const char* ex) {
						std::cout << "ERROR:" << std::endl << ex << std::endl;
					}

					normalizeObjectSize(vertices);

					// put ratio of width/height at the begining of the param values array
					param_values.insert(param_values.begin(), object_width / object_height);

					// write all the param values to the file
					for (int pi = 0; pi < param_values.size(); ++pi) {
						if (pi > 0) {
							out << ",";
						}
						out << param_values[pi];
					}
					out << "\n";

					// render a window
					fb->setClearColor(glm::vec3(1, 1, 1));
					fb->clear();
					fb->rasterize(&camera, vertices, count);
					fb->draw();

					QString filename = "results/" + fileInfoList[i].baseName() + "/" + QString("image_%1.png").arg(count, 4, 10, QChar('0'));
					QImage image = grabFrameBuffer();

					if (invertImage) {
						image.invertPixels();
					}

					if (blur) {
						cv::Mat mat(image.height(), image.width(), CV_8UC4, image.bits(), image.bytesPerLine());
						cv::GaussianBlur(mat, mat, cv::Size(7, 7), 0, 0);
					}
			
					image.save(filename);

					count++;
				}
			}
		}

		file.close();
	}

	resize(origWidth, origHeight);
	resizeGL(origWidth, origHeight);
}

void GLWidget3D::hoge() {
	this->resize(256, 256);
	resizeGL(256, 256);
	updateGL();
}