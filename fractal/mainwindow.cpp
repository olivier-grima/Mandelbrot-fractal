#include "mainwindow.h"
#include "fractalimage.h"
#include "mandelbrotimage.h"
#include "juliaimage.h"
#include <iostream>
#include <string>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  //
  // construct image_widget

  image_widget_ = new QLabel;
  image_widget_->setBackgroundRole(QPalette::Base);
  image_widget_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  image_widget_->setScaledContents(true);
  image_widget_->setMinimumSize(300, 300);

  create_actions();
  create_menus();

  setCentralWidget(image_widget_);
  resize(QGuiApplication::primaryScreen()->availableSize() * 2 / 5);

}


void MainWindow::create_actions() {

  action_fractal_image_ = new QAction(tr("Fractal Image"), this);
  connect(action_fractal_image_, SIGNAL(triggered()), this, SLOT(slot_load_fractal_image()));

  action_exit_ = new QAction(tr("E&xit"), this);
  action_exit_->setShortcut(tr("Ctrl+Q"));
  connect(action_exit_, SIGNAL(triggered()), this, SLOT(slot_exit()));

  action_about_ = new QAction(tr("&About"), this);
  connect(action_about_, SIGNAL(triggered()), this, SLOT(slot_about()));
}

void MainWindow::slot_exit() {
  close();
}

void MainWindow::slot_about() {
    QMessageBox::about(this, tr("About Fractal Viewer"),
            tr("<p>use the mouse wheel to zoom in/out</p><p>use the mouse wheel + ctrl to move left/right</p><p>use the mouse wheel + shift to switch fractal set</p>"));
}


void MainWindow::create_menus() {
  menu_open_ = new QMenu(tr("&Open"), this);

  menu_open_->addAction(action_fractal_image_);
  menu_open_->addAction(action_exit_);

  menu_help_ = new QMenu(tr("&Help"), this);
  menu_help_->addAction(action_about_);

  menuBar()->addMenu(menu_open_);
  menuBar()->addMenu(menu_help_);
}

void MainWindow::wheelEvent(QWheelEvent *event) {

    if(event->modifiers().testFlag(Qt::ControlModifier))
    {
        if (event->angleDelta().y() > 0) {
          xc+=-0.1*d;
        }
        else if(event->angleDelta().y() <0){
          xc-=-0.1*d;
        }
    }
    else if(event->modifiers().testFlag(Qt::ShiftModifier))
    {
        xc=-0.5;d=1;
        if (event->angleDelta().y() > 0) {
          std::cout << "Switch to Mandelbrot generation" << std::endl;
          fractalType=true;
        }
        else if(event->angleDelta().y() <0){
          std::cout << "Switch to Julia generation" << std::endl;
          fractalType=false;
        }
    }
    else
    {
        if (event->angleDelta().y() > 0) {
        d/=1.25;
        }
        else if(event->angleDelta().y() <0){
        d*=1.25;
        }
    }
    slot_load_fractal_image();
}

void MainWindow::slot_load_fractal_image() {

    FractalImage *fractal;

    if(fractalType){
        fractal = new MandelbrotImage(600,400,xc,0.,d);
    }
    else{
        fractal = new JuliaImage(600,400,xc,0.,d);
    }

    /* Only for computation time test
    fractal->process_simple();
    for(int i=2;i<=16;i++)
    {
        fractal->process_threaded(i);
    }
    */

    fractal->process_threaded(8);

    image_widget_->setPixmap(QPixmap::fromImage(*fractal));
    image_widget_->setFixedSize(600, 400);
    adjustSize();

    delete fractal;

}
