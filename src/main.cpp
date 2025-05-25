/**
 * @file main.cpp
 * @brief Entry point for the Coda text editor application.
 *        Initializes and starts the Qt event loop.
 * @author Dario Romandini
 */

#include <QApplication>
#include "ResourceLoader.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ResourceLoader::ensureResources();
    MainWindow window;
    window.show();
    return app.exec();
}
