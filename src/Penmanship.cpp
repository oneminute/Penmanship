// Penmanship.cpp : Defines the entry point for the application.
//

#include "Penmanship.h"
#include <QDebug>
#include <QApplication>
#include <QFontDatabase>
#include "MainWindow.h"

using namespace std;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	int fontId = QFontDatabase::addApplicationFont("fonts/tyzk.ttf");
	QFontDatabase database;
	qDebug() << "fontId:" << fontId;
	//qDebug() << database.families();

	MainWindow window;
	window.showNormal();
	int result = app.exec();
	return result;
}
