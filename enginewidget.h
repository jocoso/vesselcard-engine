#ifndef ENGINEWIDGET_H
#define ENGINEWIDGET_H

#include <QPoint>
#include <QPixmap>
#include <QVector>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class EngineWidget : public QWidget {
    Q_OBJECT
public:
    explicit EngineWidget(int imageSize, QWidget *parent = nullptr);
    void clear();

    int componentSize() const;
    int imageSize() const;

signals:
    void engineCompleted();


protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    struct Component {
        QPixmap pixmap;
        QRect rect;
        QPoint location;
    };

    int findComponent(const QRect &componentRect) const;
    const QRect targetSquare(const QPoint &position) const;

    QVector<Component> components;
    QRect highlightedRect;
    int inPlace;
    int m_ImageSize;
};

#endif // ENGINEWIDGET_H
