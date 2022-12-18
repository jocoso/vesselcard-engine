#include <QApplication>
#include "window.h"


int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(engine);
    QApplication app(argc, argv);
    Window window;
    window.loadImage(QStringLiteral(":/images/example.jpg"));
    window.show();

    return app.exec();

}
