#include "window.h"
#include "componentslist.h"
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
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    QStringList mimeTypeFilters;
    for(const QByteArray &mimeTypeName : QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if(dialog.exec() == QDialog::Accepted)
        loadImage(dialog.selectedFiles().constFirst());
}

void Window::loadImage(const QString &fileName) {
    QPixmap newImage;
    if(!newImage.load(fileName)) {
        QMessageBox::warning(
                    this,
                    tr("Open Image"),
                    tr("The image file could not be loaded."),
                    QMessageBox::Close
                    );
        return;
    }
    engineImage = newImage;
    setupEngine();
}

void Window::setCompleted() {
    QMessageBox::information(
                this,
                tr("Engine running"),
                tr("Congratulation, shit works!"),
                QMessageBox::Ok
                );
    setupEngine();
}

void Window::setupEngine() {
    int size = qMin(engineImage.width(), engineImage.height());
    engineImage = engineImage.copy((engineImage.width() - size) / 2,
                                   (engineImage.height() - size) / 2, size, size).scaled(engineWidget->width(),
                                    engineWidget->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    componentsList->clear();
    
    for(int y = 0; y < 5; ++y) {
        for(int x = 0; x < 5; ++x) {
            int componentSize = engineWidget->componentSize();
            QPixmap componentImage = engineImage.copy(x * componentSize, y * componentSize, componentSize, componentSize);
            componentsList->addComponent(componentImage, QPoint(x, y));
        }
    }
    
    for(int i = 0; i < componentsList->count(); ++i) {
        if(QRandomGenerator::global()->bounded(2) == 1) {
            QListWidgetItem *item = componentsList->takeItem(i);
            componentsList->insertItem(0, item);
        }
    }

    engineWidget->clear();
}

void Window::setupMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAction = fileMenu->addAction(tr("&Open..."), this, &Window::openImage);
    openAction->setShortcuts(QKeySequence::Open);

    QAction *exitAction = fileMenu->addAction(tr("E&xit"), qApp, &QCoreApplication::quit);
    exitAction->setShortcut(QKeySequence::Quit);

    QMenu *engineMenu = menuBar()->addMenu(tr("&Engine"));

    engineMenu->addAction(tr("&Restart"), this, &Window::setupEngine);
}

void Window::setupWidgets() {
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    engineWidget = new EngineWidget(400);

    componentsList = new ComponentsList(engineWidget->componentSize(), this);

    connect(
                engineWidget,
                &EngineWidget::engineCompleted,
                this,
                &Window::setCompleted,
                Qt::QueuedConnection);

    frameLayout->addWidget(componentsList);
    frameLayout->addWidget(engineWidget);
    setCentralWidget(frame);
}
