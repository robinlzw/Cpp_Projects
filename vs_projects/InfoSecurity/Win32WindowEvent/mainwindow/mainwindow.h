#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "keymonitor.h"
#include "capturer.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	//������Ӧ�ȼ��Ĳۺ���
public slots:
	void activated();
};

#endif // MAINWINDOW_H
