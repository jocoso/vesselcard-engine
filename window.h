#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QPixmap>

class ComponentsList;
class EngineWidget;
QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
    void loadImage(const QString &path);

public slots:
    void openImage();
    void setupEngine();

private slots:
    void setCompleted();

private:
    void setupMenus();
    void setupWidgets();

    QPixmap engineImage;
    ComponentsList *componentsList;
    EngineWidget *engineWidget;

};

#endif // WINDOW_H
