#ifndef CUSTOMGRAPHICSPIXMAPITEM_H
#define CUSTOMGRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>

//Classe représentant un QGraphicsPixmapItem personnalisé
class CustomGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	CustomGraphicsPixmapItem();
	void setPos(qreal x, qreal y);				//On reféfini la fonction setPos car elle est appelée ailleurs dans le code par différents thread
	void setPosMainThread(qreal x, qreal y);	//Cette fonction sera appelée par le thread graphique (principal)

	void setPixmap(const QPixmap &pixmap);			//On reféfini la fonction setPixmap car elle est appelée ailleurs dans le code par différents thread
	void setPixmapMainThread(const QPixmap &pixmap);//Cette fonction sera appelée par le thread graphique (principal)

	void mainThreadConnected();					//On appelle cette fonction pour dire que le thread graphique est bien connecté à ce pixmap item

	const QPixmap &getLastPixmap() const;		//Récupération du pixmap en attente d'être mis à jour

private:
	qreal lastX, lastY;		//Coordonnées qui seront utilisées par le thread principal pour mettre à jour la position du pixmapItem
	QPixmap lastPixmap;		//Texture qui sera utilisé par le thread principal pour mettre à jour la texture du pixmapItem

signals:
	void requestPosChange(qreal x, qreal y);		//Signal indiquant au thread graphique qu'il faut mettre à jour la position de ce pixmapItem
	void requestPixmapChange(const QPixmap &pixmap);//Signal indiquant au thread graphique qu'il faut mettre à jour la texture de ce pixmapItem
};

#endif // CUSTOMGRAPHICSPIXMAPITEM_H
