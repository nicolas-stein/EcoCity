#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include "Constants.h"
#include <QGraphicsView>
#include <QWheelEvent>
#include <QRectF>
#include <QScrollBar>

class CustomGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	CustomGraphicsView(QWidget *parent = nullptr);

	// QWidget interface
	void setPanEnable(bool newPanEnable);

	bool isPan() const;

	void setZoomEnbale(bool newZoomEnbale);

	void setResizeEnable(bool newResizeEnable);

protected:
	void wheelEvent(QWheelEvent *event);
private:
	int panStartX, panStartY;
	bool pan = false;
	bool panEnable = false;
	bool zoomEnable = false;
	bool resizeEnable = false;

	// QWidget interface
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
};

#endif // CUSTOMGRAPHICSVIEW_H
