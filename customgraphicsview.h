#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include "constants.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QScrollBar>
#include <QOpenGLWidget>

class CustomGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	CustomGraphicsView(QWidget *parent = nullptr);
	~CustomGraphicsView();

	// QWidget interface
	void setPanEnable(bool newPanEnable);

	bool isPan() const;

	void setZoomEnbale(bool newZoomEnbale);

protected:
	void wheelEvent(QWheelEvent *event);
private:
	int panStartX, panStartY;
	bool pan = false;
	bool panEnable = false;
	bool zoomEnable = false;
	QOpenGLWidget *openGlWidget;

	// QWidget interface
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);
};

#endif // CUSTOMGRAPHICSVIEW_H
