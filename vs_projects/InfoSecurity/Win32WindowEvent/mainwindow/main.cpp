#include "mainwindow.h"
#include <QApplication>
#include <qt_windows.h>
#include "MyGlobalShortCut.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	//ע���ݼ�
	MyGlobalShortCut *shortcut = new MyGlobalShortCut("Ctrl+F9", &w);
	//����Ӧ�ȼ���slot
	QObject::connect(shortcut, SIGNAL(activated()), &w, SLOT(activated()));
	
	w.show();
	return a.exec();
}
