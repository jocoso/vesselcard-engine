#include "componentslist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

ComponentsList::ComponentsList(int componentSize, QWidget *parent)
    : QListWidget(parent), m_ComponentSize(componentSize) {

    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(m_ComponentSize, m_ComponentSize));
    setSpacing(10);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

void ComponentsList::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat(ComponentsList::engineMimeType()))
        event->accept();
    else
        event->ignore();
}

void ComponentsList::dragMoveEvent(QDragMoveEvent *event) {
    if(event->mimeData()->hasFormat(ComponentsList::engineMimeType())) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void ComponentsList::dropEvent(QDropEvent *event) {
    if(event->mimeData()->hasFormat(ComponentsList::engineMimeType())) {
        QByteArray componentData = event->mimeData()->data(ComponentsList::engineMimeType());
        QDataStream dataStream(&componentData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        addComponent(pixmap, location);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void ComponentsList::addComponent(const QPixmap &pixmap, const QPoint &location) {
    QListWidgetItem *componentItem = new QListWidgetItem(this);
    componentItem->setIcon(QIcon(pixmap));
    componentItem->setData(Qt::UserRole, QVariant(pixmap));
    componentItem->setData(Qt::UserRole+1, location);
    componentItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

void ComponentsList::startDrag(Qt::DropActions /*supportedActions*/) {
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QPoint location = item->data(Qt::UserRole+1).toPoint();

    dataStream << pixmap << location;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(ComponentsList::engineMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

    if(drag->exec(Qt::MoveAction) == Qt::MoveAction)
        delete takeItem(row(item));
}
