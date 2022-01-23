#ifndef CUSTOMGRAPHICSPIXMAPITEM_H
#define CUSTOMGRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>

class CustomGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	CustomGraphicsPixmapItem();
	void setPos(qreal x, qreal y);
	void setPosMainThread(qreal x, qreal y);

	void setPixmap(const QPixmap &pixmap);
	void setPixmapMainThread(const QPixmap &pixmap);

	void mainThreadConnected();

	const QPixmap &getLastPixmap() const;

private:
	qreal lastX, lastY;
	QPixmap lastPixmap;

signals:
	void requestPosChange(qreal x, qreal y);
	void requestPixmapChange(const QPixmap &pixmap);
};

#endif // CUSTOMGRAPHICSPIXMAPITEM_H
