#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent): QGraphicsView(parent)
{
	//On configure la vue pour avoir le meilleur rapport qualité/performance
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	openGlWidget = new QOpenGLWidget(this);
	QSurfaceFormat format;
	format.setSamples(256);
	format.setSwapInterval(1);
	openGlWidget->setFormat(format);
	setViewport(openGlWidget);
}

CustomGraphicsView::~CustomGraphicsView()
{
	openGlWidget->deleteLater();
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
	//Si le zoom est désactiver
	if(!zoomEnable){
		QGraphicsView::wheelEvent(event);
		event->ignore();
		return ;
	}

	//Si le zoom est activé, on regarde dans quel sens la molette a tournée

	QRectF poly = mapToScene(viewport()->geometry()).boundingRect();
	if(event->angleDelta().y() < 0){
		//On dézoom si on est pas déjà trop loin
		if(poly.width() < TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE*0.9 && poly.height() < TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE*0.9){
			scale(0.9, 0.9);
			event->accept();
			return;
		}
	}
	else if(event->angleDelta().y() > 0){
		//On zoom si on est pas déjà trop près
		if(poly.width() > TERRAIN_SQUARE_SIZE*6 && poly.height() > TERRAIN_SQUARE_SIZE*6){
			scale(1.1, 1.1);
			event->accept();
			return;
		}
	}

	event->ignore();
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
	//Si on fait un clique droit, on dit qu'on déplace la vue et on défini les coordonnées de départ du déplacement
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
	//Si on relache le clique droit, on arrête de déplacer la vue
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
	//Si la vue est en cours de déplacement, on la bouge en fonction de comment on a déplacé la souris
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
	//Si la vue	a été redimensionnée, on s'assure que la scène est toujours correctement affichée
	Q_UNUSED(event);
	QRectF poly = mapToScene(viewport()->geometry()).boundingRect();
	if(poly.width() > TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE || poly.height() > TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE){
		fitInView(0, 0, scene()->width(), scene()->height(), Qt::KeepAspectRatioByExpanding);
	}
	event->ignore();
	QGraphicsView::resizeEvent(event);
}
