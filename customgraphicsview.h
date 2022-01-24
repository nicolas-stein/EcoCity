#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include "constants.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QScrollBar>
#include <QOpenGLWidget>

//Classe représentant une GraphicsView personnalisée
class CustomGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	CustomGraphicsView(QWidget *parent = nullptr);
	~CustomGraphicsView();

	void setPanEnable(bool newPanEnable);			//Active le déplacement de la caméra

	bool isPan() const;

	void setZoomEnbale(bool newZoomEnbale);			//Active la possiblité de zoomer

protected:
	void wheelEvent(QWheelEvent *event);			//Lorsque l'on utilise la molette de la souris (pour zoomer)
	void mousePressEvent(QMouseEvent *event);		//Lorsque l'on clique avec la souris
	void mouseReleaseEvent(QMouseEvent *event);		//Lorsque l'on lache la souris
	void mouseMoveEvent(QMouseEvent *event);		//Lorsque l'on déplace la souris
	void resizeEvent(QResizeEvent *event);			//Lorsque la vue est redimensionnée
private:
	int panStartX, panStartY;		//Coordonnées du début de déplacement de la caméra
	bool pan = false;				//Déplacement de la scène en cours ?
	bool panEnable = false;			//Activer ou non la possiblité de déplacer la caméra
	bool zoomEnable = false;		//Activer ou non la possibilité de zoomer
	QOpenGLWidget *openGlWidget;	//Widget OpenGL pour améliorer les performances

};

#endif // CUSTOMGRAPHICSVIEW_H
