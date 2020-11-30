#include "stefanmainwindow.h"
#include "ui_mainwindow.h"

StefanMainWindow::StefanMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StefanMainWindow)
{
    ui->setupUi(this);
}

StefanMainWindow::~StefanMainWindow()
{
    delete ui;
}
