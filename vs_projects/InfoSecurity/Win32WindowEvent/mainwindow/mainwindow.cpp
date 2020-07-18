#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(KeyCapturer::instance(), &KeyCapturer::getKey, [=](int key) {
		qDebug() << QString::number(key);
	});
	startHook();
}

MainWindow::~MainWindow()
{
	delete ui;
}
//������Ӧ�ȼ��Ĳۺ���
void MainWindow::activated()
{
	QMessageBox::information(NULL, "title", "hotKey pressd");
}
