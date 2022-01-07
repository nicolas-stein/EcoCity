#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent): QGraphicsView(parent)
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QOpenGLWidget *gl = new QOpenGLWidget();
	QSurfaceFormat format;
	format.setSamples(256);
	format.setSwapInterval(1);
	gl->setFormat(format);
	setViewport(gl);
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
	if(!zoomEnable){
		QGraphicsView::wheelEvent(event);
		event->ignore();
		return ;
	}

	QRectF poly = mapToScene(viewport()->geometry()).boundingRect();
	if(event->angleDelta().y() < 0){
		if(poly.width() < TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE*3/4 && poly.height() < TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE*3/4){
			scale(0.9, 0.9);
			event->accept();
			return;
		}
	}
	else{
		if(poly.width() > TERRAIN_SQUARE_SIZE*6 && poly.height() > TERRAIN_SQUARE_SIZE*6){
			scale(1.1, 1.1);
			event->accept();
			return;
		}
	}

	event->ignore();
}

void CustomGraphicsView::setResizeEnable(bool newResizeEnable)
{
	resizeEnable = newResizeEnable;
}

void CustomGraphicsView::setZoomEnbale(bool newZoomEnbale)
{
	zoomEnable = newZoomEnbale;
}

bool CustomGraphicsView::isPan() const
{
	return pan;
}

void CustomGraphicsView::setPanEnable(bool newPanEnable)
{
	panEnable = newPanEnable;
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton){
		pan = true;
		panStartX = event->position().x();
		panStartY = event->position().y();
		viewport()->setCursor(Qt::ClosedHandCursor);
		event->accept();
		return;
	}
	event->ignore();
	QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton){
		pan = false;
		viewport()->setCursor(Qt::ArrowCursor);
		event->accept();
		return;
	}
	event->ignore();
	QGraphicsView::mouseReleaseEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if(pan){
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->position().x() - panStartX));
		verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->position().y() - panStartY));
		panStartX = event->position().x();
		panStartY = event->position().y();
		event->accept();
		return;
	}
	event->ignore();
	QGraphicsView::mouseMoveEvent(event);
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
	if(!resizeEnable){
		QGraphicsView::resizeEvent(event);
		return;
	}
	QRectF poly = mapToScene(viewport()->geometry()).boundingRect();
	if((poly.width() < TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE*3/4 && poly.height() < TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE*3/4) || (poly.width() > TERRAIN_SQUARE_SIZE*6 && poly.height() > TERRAIN_SQUARE_SIZE*6)){
		fitInView(scene()->width()/4, scene()->height()/4, scene()->width()/2, scene()->height()/2, Qt::KeepAspectRatioByExpanding);
	}
}
