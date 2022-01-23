#include "customgraphicspixmapitem.h"

CustomGraphicsPixmapItem::CustomGraphicsPixmapItem()
{

}

void CustomGraphicsPixmapItem::setPos(qreal x, qreal y)
{
	lastX = x;
	lastY = y;
	emit requestPosChange(x, y);
}

void CustomGraphicsPixmapItem::setPosMainThread(qreal x, qreal y)
{
	QGraphicsPixmapItem::setPos(x, y);
}

void CustomGraphicsPixmapItem::setPixmap(const QPixmap &pixmap)
{
	lastPixmap = pixmap;
	emit requestPixmapChange(pixmap);
}

void CustomGraphicsPixmapItem::setPixmapMainThread(const QPixmap &pixmap)
{
	QGraphicsPixmapItem::setPixmap(pixmap);
}

void CustomGraphicsPixmapItem::mainThreadConnected()
{
	setPosMainThread(lastX, lastY);
	setPixmapMainThread(lastPixmap);
}

const QPixmap &CustomGraphicsPixmapItem::getLastPixmap() const
{
	return lastPixmap;
}
