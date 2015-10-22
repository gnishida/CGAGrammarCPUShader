#pragma once

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Camera.h"
#include <QVector2D>
#include <vector>
#include "FrameBuffer.h"
#include "CGA.h"

using namespace std;

class MainWindow;

class GLWidget3D : public QGLWidget {
private:
	Camera camera;
	QPoint lastPos;
	FrameBuffer* fb;
	cga::CGA system;
	std::vector<std::vector<Vertex> > vertices;

public:
	GLWidget3D();

	void loadCGA(const std::string& filename);
	void normalizeObjectSize(std::vector<std::vector<Vertex> >& vertices);

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();    
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
};

