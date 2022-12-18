#ifndef COMPONENTSLIST_H
#define COMPONENTSLIST_H

#include <QListWidget>

class ComponentsList : public QListWidget {
    Q_OBJECT
public:
    explicit ComponentsList(int componentSize, QWidget *parent = nullptr);
    void addComponent(const QPixmap &pixmap, const QPoint &location);

    static QString engineMimeType() { return QStringLiteral("image/x-puzzle-piece" );}

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;

    int m_ComponentSize;
};

#endif // COMPONENTSLIST_H
