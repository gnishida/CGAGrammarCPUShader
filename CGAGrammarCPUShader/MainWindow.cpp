#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) { 
	ui.setupUi(this);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGAGrammar, SIGNAL(triggered()), this, SLOT(onOpenCGAGrammar()));
	connect(ui.actionGenerateImages, SIGNAL(triggered()), this, SLOT(onGenerateImages()));
	connect(ui.actionGenerateBuildingImages, SIGNAL(triggered()), this, SLOT(onGenerateBuildingImages()));
	connect(ui.actionHoge, SIGNAL(triggered()), this, SLOT(onHoge()));

	glWidget = new GLWidget3D();
	setCentralWidget(glWidget);
}

MainWindow::~MainWindow() {
}

void MainWindow::onOpenCGAGrammar() {
	QString new_filename = QFileDialog::getOpenFileName(this, tr("Open CGA file..."), "", tr("CGA Files (*.xml)"));
	if (new_filename.isEmpty()) return;

	fileLoaded = true;
	filename = new_filename;
	glWidget->loadCGA(filename.toUtf8().data());
	this->setWindowTitle("CGA Shape Grammar - " + new_filename);
}

void MainWindow::onGenerateImages() {
	glWidget->generateImages(256, 256, false, false);
}

void MainWindow::onGenerateBuildingImages() {
	glWidget->generateBuildingImages(256, 256, false, false);
}

void MainWindow::onHoge() {
	glWidget->hoge();
}