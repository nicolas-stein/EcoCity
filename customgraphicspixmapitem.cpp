#include "customgraphicspixmapitem.h"

CustomGraphicsPixmapItem::CustomGraphicsPixmapItem()
{

}

//Cette fonction n'est pas exécutée par le thread graphique (mais par le thread de mise à jour de la logique)
void CustomGraphicsPixmapItem::setPos(qreal x, qreal y)
{
	//On met à jour les coordonnées et on émet le signal
	lastX = x;
	lastY = y;
	emit requestPosChange(x, y);
}

//Cette fonction est exécutée par le thread graphique
void CustomGraphicsPixmapItem::setPosMainThread(qreal x, qreal y)
{
	QGraphicsPixmapItem::setPos(x, y);	//On a des soucis si on change la position d'un pixmapItem depuis un thread qui n'est pas le thread graphique
}

//Cette fonction n'est pas exécutée par le thread graphique (mais par le thread de mise à jour de la logique)
void CustomGraphicsPixmapItem::setPixmap(const QPixmap &pixmap)
{
	lastPixmap = pixmap;
	emit requestPixmapChange(pixmap);
}

//Cette fonction est exécutée par le thread graphique
void CustomGraphicsPixmapItem::setPixmapMainThread(const QPixmap &pixmap)
{
	QGraphicsPixmapItem::setPixmap(pixmap);	//On a des soucis si on change la texture d'un pixmapItem depuis un thread qui n'est pas le thread graphique
}

//Cette fonction est exécutée par le thread graphique
void CustomGraphicsPixmapItem::mainThreadConnected()
{
	setPosMainThread(lastX, lastY);
	setPixmapMainThread(lastPixmap);
}

const QPixmap &CustomGraphicsPixmapItem::getLastPixmap() const
{
	return lastPixmap;
}
