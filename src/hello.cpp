#include <QApplication>
#include <QMainWindow>
#include <gwindow.h>
#include <init.h>
#include <QWidget>

int main(int argc, char** argv){
    stanfordcpplib::initializeLibrary(argc, argv);
    QApplication qapp(argc, argv);
    QMainWindow window;

    QWidget* widget = new QWidget(&window);
    window.setCentralWidget(widget);
    window.show();

    return qapp.exec();
}
