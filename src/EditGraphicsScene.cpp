#include "EditGraphicsScene.h"

#include <QDebug>
#include <QtMath>
#include "EditGraphicsViewer.h"
#include "CellItem.h"

EditGraphicsScene::EditGraphicsScene(EditGraphicsViewer* viewer, QObject* parent)
    : QGraphicsScene(parent)
	, m_background(nullptr)
    , m_viewer(viewer)
{
    m_rows = 12;
    m_cols = 8;
    m_topIndentation = m_viewer->mm2PixelsY(20);
    m_bottomIndentation = m_viewer->mm2PixelsY(15);
    m_leftIndentation = m_viewer->mm2PixelsX(10);
    m_rightIndentation = m_viewer->mm2PixelsX(10);
    m_alignment = Qt::AlignmentFlag::AlignCenter;
    m_pageRect = QRectF(0, 0, m_viewer->mm2PixelsX(210), m_viewer->mm2PixelsY(297));
    m_cellSize = QSizeF(m_viewer->mm2PixelsX(20), m_viewer->mm2PixelsY(20));

    m_background = addRect(m_pageRect);
    m_background->setPen(QPen(Qt::black, 2));
    updateLayout();
}

EditGraphicsScene::~EditGraphicsScene()
{
    clearSelection();
}

int EditGraphicsScene::rows() const
{
    return m_rows;
}

int EditGraphicsScene::cols() const
{
    return m_cols;
}

void EditGraphicsScene::addCellItem(int row, int col)
{
    QRectF cellRect(m_actualRect.topLeft(), m_cellSize);
    cellRect.translate(m_cellSize.width() * col, m_cellSize.height() * row);
    CellItem* item = new CellItem(cellRect);
    addItem(item);
    update();
}

void EditGraphicsScene::generateCells()
{
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
        {
            addCellItem(r, c);
        }
    }
}

void EditGraphicsScene::updateLayout()
{
    qreal expectedWidth = m_pageRect.width() - m_leftIndentation - m_rightIndentation;
    qreal expectedHeight = m_pageRect.height() - m_topIndentation - m_bottomIndentation;

    m_cols = qFloor(expectedWidth / m_cellSize.width());
    m_rows = qFloor(expectedHeight / m_cellSize.height());
    qreal actualWidth = m_cellSize.width() * m_cols;
    qreal actualHeight = m_cellSize.height() * m_rows;

    qDebug() << "actual rows:" << m_rows << ", cols:" << m_cols;
    qDebug() << "actual width:" << actualWidth << ", height:" << actualHeight;

    QPointF actualTL(0, 0);
    QPointF actualBR(0, 0);
    if (m_alignment.testFlag(Qt::AlignHCenter))
    {
        actualTL.setX((m_pageRect.width() - actualWidth) / 2);
        actualBR.setX(m_pageRect.width() - actualTL.x());
    }

    if (m_alignment.testFlag(Qt::AlignVCenter))
    {
        actualTL.setY(m_topIndentation + (expectedHeight - actualHeight) / 2);
        actualBR.setY(actualTL.y() + actualHeight);
    }

    qDebug() << "actual top left point:" << actualTL;
    qDebug() << "actual bottom right point:" << actualBR;

    m_actualRect = QRectF(actualTL, actualBR);
}

