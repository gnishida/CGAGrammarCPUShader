/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Sun Oct 25 00:06:31 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionOpenCGAGrammar;
    QAction *actionExit;
    QAction *actionGenerateImages;
    QAction *actionHoge;
    QAction *actionGenerateBuildingImages;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTest;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(600, 400);
        actionOpenCGAGrammar = new QAction(MainWindowClass);
        actionOpenCGAGrammar->setObjectName(QString::fromUtf8("actionOpenCGAGrammar"));
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionGenerateImages = new QAction(MainWindowClass);
        actionGenerateImages->setObjectName(QString::fromUtf8("actionGenerateImages"));
        actionHoge = new QAction(MainWindowClass);
        actionHoge->setObjectName(QString::fromUtf8("actionHoge"));
        actionGenerateBuildingImages = new QAction(MainWindowClass);
        actionGenerateBuildingImages->setObjectName(QString::fromUtf8("actionGenerateBuildingImages"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTest = new QMenu(menuBar);
        menuTest->setObjectName(QString::fromUtf8("menuTest"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTest->menuAction());
        menuFile->addAction(actionOpenCGAGrammar);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTest->addAction(actionGenerateImages);
        menuTest->addAction(actionGenerateBuildingImages);
        menuTest->addAction(actionHoge);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpenCGAGrammar->setText(QApplication::translate("MainWindowClass", "Open CGA Grammar", 0, QApplication::UnicodeUTF8));
        actionOpenCGAGrammar->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionGenerateImages->setText(QApplication::translate("MainWindowClass", "Generate Images", 0, QApplication::UnicodeUTF8));
        actionHoge->setText(QApplication::translate("MainWindowClass", "Hoge", 0, QApplication::UnicodeUTF8));
        actionGenerateBuildingImages->setText(QApplication::translate("MainWindowClass", "Generate Building Images", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menuTest->setTitle(QApplication::translate("MainWindowClass", "Test", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
