#include "EditGraphicsViewer.h"

#include <QFontDatabase>
#include <QDebug>
#include <QBoxLayout>
#include <QGraphicsProxyWidget>
#include <QComboBox>
#include <QPainterPath>
#include <QPalette>
#include <QtMath>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPainterPath>
#include <QLabel> 
#include <QPrinter>
#include <QPrintDialog>
#include "EditGraphicsScene.h"

EditGraphicsViewer::EditGraphicsViewer(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new EditGraphicsScene(this))
    , m_rubberBandActive(false)
    , m_mousePressed(false)
	, m_isKeyDelPress(false)
	, m_isKeyShiftPressed(false)
	, m_lastTime(0)
	, m_curTime(0)
	, m_rullerWidth(15)
	, m_rows(12)
	, m_cols(8)
{
    setScene(m_scene.data());
    init();
}

EditGraphicsViewer::~EditGraphicsViewer()
{
	m_scene->deleteLater();
}

void EditGraphicsViewer::paintEvent(QPaintEvent * event)
{
	QPainter painter(viewport());

	QGraphicsView::paintEvent(event);
}

void EditGraphicsViewer::wheelEvent(QWheelEvent * event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        zoomBy(qPow(1.2, event->delta() / 240.0));
    }
    else
        QGraphicsView::wheelEvent(event);
}

void EditGraphicsViewer::zoomBy(qreal factor)
{
    const qreal currentZoom = zoomFactor();
    if ((factor < 1 && currentZoom < 0.01) || (factor > 1 && currentZoom > 10))
        return;
    scale(factor, factor);
    emit zoomChanged();
}

void EditGraphicsViewer::mousePressEvent(QMouseEvent * event)
{
    QGraphicsView::mousePressEvent(event);
    if (event->isAccepted())
        return;
}

void EditGraphicsViewer::mouseMoveEvent(QMouseEvent * event)
{
    QPoint point = event->pos();
	//shift keyboard
	if (event->modifiers() & Qt::ShiftModifier) {
		m_isKeyShiftPressed = true;
	}
	else {
		m_isKeyShiftPressed = false;
	}
    QGraphicsView::mouseMoveEvent(event);
    QPointF pos = mapToScene(point);
    emit mouseMoved(pos);
}

void EditGraphicsViewer::mouseReleaseEvent(QMouseEvent * event)
{
	
    QGraphicsView::mouseReleaseEvent(event);

    m_mousePressed = false;
	m_isKeyShiftPressed = false;
}

void EditGraphicsViewer::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
		case Qt::Key_Shift:
			m_isKeyShiftPressed = true;
			break;
		case Qt::Key_Delete:
			m_isKeyDelPress = true;
			break;
	}
	QGraphicsView::keyPressEvent(event);
}

void EditGraphicsViewer::keyReleaseEvent(QKeyEvent * event)
{
	switch (event->key())
	{
		case Qt::Key_Shift:
			m_isKeyShiftPressed = false;
		case Qt::Key_Delete:
			m_isKeyDelPress = false;
			break;
			break;
	}
	QGraphicsView::keyReleaseEvent(event);
}

qreal EditGraphicsViewer::zoomFactor() const
{
    return transform().m11();
}

int EditGraphicsViewer::mm2PixelsX(float mm) const
{
	return mm / 25.4f * logicalDpiX();
}

int EditGraphicsViewer::mm2PixelsY(float mm) const
{
	return mm / 25.4f * logicalDpiY();
}

float EditGraphicsViewer::pixels2mmX(int pixels) const
{
	return pixels / logicalDpiX() * 25.4f;
}

float EditGraphicsViewer::pixels2mmY(int pixels) const
{
	return pixels / logicalDpiY() * 25.4f;
}

float EditGraphicsViewer::widthMM() const 
{ 
	return pixels2mmX(m_width); 
}

void EditGraphicsViewer::setWidthMM(float widthMM)
{ 
	m_width = mm2PixelsX(widthMM); 
	setSceneRect(0, 0, m_width, m_height);
}

float EditGraphicsViewer::heightMM() const 
{
	return pixels2mmY(m_height); 
}

void EditGraphicsViewer::setHeightMM(float heightMM) 
{ 
	m_height = mm2PixelsY(heightMM); 
	setSceneRect(0, 0, m_width, m_height);
}

int EditGraphicsViewer::rows() const
{
	return m_rows;
}

void EditGraphicsViewer::setRows(int rows)
{
	m_rows = rows;
}

int EditGraphicsViewer::cols() const
{
	return m_cols;
}

void EditGraphicsViewer::setCols(int cols)
{
	m_cols = cols;
}

float EditGraphicsViewer::gridWidthMM() const
{
	return pixels2mmX(m_gridWidth);
}

void EditGraphicsViewer::setGridWidthMM(float widthMM)
{
	m_gridWidth = mm2PixelsX(widthMM);
}

float EditGraphicsViewer::gridHeightMM() const
{
	return pixels2mmY(m_gridHeight);
}

void EditGraphicsViewer::setGridHeightMM(float heightMM)
{
	m_gridHeight = mm2PixelsY(heightMM);
}

void EditGraphicsViewer::init()
{
    setRubberBandSelectionMode(Qt::ItemSelectionMode::IntersectsItemBoundingRect);
    setInteractive(true);
    setMouseTracking(true);

	setWidthMM(210);
	setHeightMM(297);
	setGridWidthMM(22);
	setGridHeightMM(22);

	qDebug() << QFontDatabase::applicationFontFamilies(0)[0];
	m_font = QFont(QFontDatabase::applicationFontFamilies(0)[0]);
	qDebug() << m_font.family();
}

void EditGraphicsViewer::creatTextEdit()
{
	m_textEdit = new QTextEdit();
	QPoint startPoint = QPoint(m_textInputPoint.x(), m_textInputPoint.y());
	QGraphicsProxyWidget *proxy = m_scene->addWidget(m_textEdit);
	proxy->setPos(startPoint);
	m_textEdit->setFocus();
	m_textEdit->resize(24, 42);
	m_textEdit->setStyleSheet("background-color : rgb(0,255,0,0%);");
	m_textEdit->setFrameShape(QFrame::NoFrame);
	m_textEdit->setWordWrapMode(QTextOption::WrapMode::NoWrap);
	m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(m_textEdit->document(), SIGNAL(contentsChanged()), SLOT(textAreaChanged()));
}

void EditGraphicsViewer::releaseTextEdit()
{
	QPoint end(m_textInputPoint.x() + m_textEdit->document()->size().width(), m_textInputPoint.y() + m_textEdit->document()->size().height());
	QRect rect(m_textInputPoint, end);
	m_textEdit->deleteLater();
}

void EditGraphicsViewer::zoomIn()
{
    zoomBy(2);
}

void EditGraphicsViewer::zoomOut()
{
    zoomBy(0.5);
}

void EditGraphicsViewer::resetZoom()
{
    if (!qFuzzyCompare(zoomFactor(), qreal(1))) {
        resetTransform();
        emit zoomChanged();
    }
}

void EditGraphicsViewer::textAreaChanged()
{
	int newwidth = m_textEdit->document()->size().width();
	int newheight = m_textEdit->document()->size().height();
	m_textEdit->resize(newwidth, newheight);
}

void EditGraphicsViewer::drawGrids()
{
}

void EditGraphicsViewer::drawGrid()
{
}

void EditGraphicsViewer::print()
{
	QPrinter printer;
	QPrintDialog dlg(&printer, this);
	if (dlg.exec() == QDialog::Accepted)
	{
		QPainter painter(&printer);
		m_scene->render(&painter, m_scene->sceneRect());
		painter.end();
	}
}
