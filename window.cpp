#include "window.h"
#include "componentlist.h"
#include "enginewidget.h"

#include <QtWidgets>
#include <stdlib.h>

Window::Window(QWidget *parent)
    : QMainWindow{parent}
{
    setupMenus();
    setupWidgets();
    
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("Component"));
}


void Window::openImage() {
    const QString directory =
            QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).value(0, QDir::homePath());
    QFileDialog dialog(this, tr("open Image"), directory);
    dialog.setAcceptMode(QFileDialog::ExistingFile)
}
