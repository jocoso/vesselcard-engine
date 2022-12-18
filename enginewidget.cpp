#include "enginewidget.h"
#include "componentslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

EngineWidget::EngineWidget(int imageSize, QWidget *parent)
    : QWidget(parent), m_ImageSize(imageSize) {

    setAcceptDrops(true);
    setMinimumSize(m_ImageSize, m_ImageSize);
    setMaximumSize(m_ImageSize, m_ImageSize);
}

void EngineWidget::clear() {
    components.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void EngineWidget::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat(ComponentsList::engineMimeType()))
        event->accept();
    else
        event->ignore();
}

void EngineWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void EngineWidget::dragMoveEvent(QDragMoveEvent *event) {
    QRect updateRect = highlightedRect.united(targetSquare(event->position().toPoint()));

    if(event->mimeData()->hasFormat(ComponentsList::engineMimeType())
            && findComponent(targetSquare(event->position().toPoint())) == -1) {
        QByteArray componentData = event->mimeData()->data(ComponentsList::engineMimeType());
        QDataStream dataStream(&componentData, QIODevice::ReadOnly);
        Component component;
        component.rect = targetSquare(event->position().toPoint());
        dataStream >> component.pixmap >> component.location;

        components.append(component);

        highlightedRect = QRect();
        update(component.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if(component.location == component.rect.topLeft() / componentSize()) {
            inPlace++;
            if(inPlace == 25)
                emit engineCompleted();
        }
    } else {
        highlightedRect = QRect();
        event->ignore();
    }

    update(updateRect);

}

void EngineWidget::dropEvent(QDropEvent *event) {
    if(event->mimeData()->hasFormat(ComponentsList::engineMimeType())
            && findComponent(targetSquare(event->position().toPoint())) == -1) {

        QByteArray componentData = event->mimeData()->data(ComponentsList::engineMimeType());
        QDataStream dataStream(&componentData, QIODevice::ReadOnly);
        Component component;
        component.rect = targetSquare(event->position().toPoint());
        dataStream >> component.pixmap >> component.location;

        components.append(component);

        highlightedRect = QRect();
        update(component.rect);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        if(component.location == component.rect.topLeft() / componentSize() ) {
            inPlace++;
            if (inPlace == 25)
                emit engineCompleted();
        }
    } else {
        highlightedRect = QRect();
        event->ignore();
    }
}

int EngineWidget::findComponent(const QRect &componentRect) const {
    for(int i = 0, size = components.size(); i < size; ++i) {
        if(components.at(i).rect == componentRect)
            return i;
    }
    return -1;
}

void EngineWidget::mousePressEvent(QMouseEvent *event) {
    QRect square = targetSquare(event->pos());
    const int found = findComponent(square);

    if(found == -1)
        return;

    Component component = components.takeAt(found);

    if(component.location == square.topLeft() / componentSize())
        inPlace--;

    update(square);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << component.pixmap << component.location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(ComponentsList::engineMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos() - square.topLeft());
    drag->setPixmap(component.pixmap);

    if(drag->exec(Qt::MoveAction) != Qt::MoveAction) {
        components.insert(found, component);
        update(targetSquare(event->pos()));

        if(component.location == square.topLeft() / componentSize())
            inPlace++;
    }

}


void EngineWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::white);

    if(highlightedRect.isValid()) {
        painter.setBrush(QColor("#ffcccc"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
    }

    for(const Component &component: components)
        painter.drawPixmap(component.rect, component.pixmap);
}

const QRect EngineWidget::targetSquare(const QPoint &position) const {
    return QRect(position / componentSize() * componentSize(),
                 QSize(componentSize(), componentSize()));
}

int EngineWidget::componentSize() const {
    return m_ImageSize / 5;
}

int EngineWidget::imageSize() const {
    return m_ImageSize;
}


































































