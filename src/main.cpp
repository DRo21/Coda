/**
 * @file main.cpp
 * @brief Entry point for the Coda text editor application.
 *        Initializes and starts the Qt event loop.
 * @author Dario
 */

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
