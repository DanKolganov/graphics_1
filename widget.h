#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget> 
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QSplitter>
#include <QScreen>
#include <QGuiApplication>
#include <QCloseEvent>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QAction>
#include <QContextMenuEvent>
#include <QProgressBar>

#include <iostream>
#include <thread>
#include <chrono>

class Widget : public QWidget
{
    Q_OBJECT
    QLabel *image;
    QPixmap *pixmap;
    QProgressBar *progressBar;

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void OpenFile();
    void ColorToGrey();
    void Inverse();
    void GreyWorld();
    void LinearCorrection();
    void GaussianFilter();
    void ShapeFilter();
    void TrenFilter();
    void MedianFilter();
    void Dilation();
    void TopHat();
    void PancilDrow();
    void Diag();
    void Save();
};
#endif // WIDGET_H