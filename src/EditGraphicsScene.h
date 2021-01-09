#ifndef EDITGRAPHICSSCENE_H
#define EDITGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>

class QPaintEvent;
class QPushButton;
class EditGraphicsViewer;

class EditGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit EditGraphicsScene(EditGraphicsViewer* viewer, QObject* parent = nullptr);
    ~EditGraphicsScene();

	QGraphicsRectItem* backgroundItem() { return m_background; }

    int rows() const;
    int cols() const;

public slots:
    void updateLayout();
    void addCellItem(int row, int col);
    void generateCells();

private:
    QGraphicsRectItem* m_background;
    qreal m_topIndentation;
    qreal m_bottomIndentation;
    qreal m_leftIndentation;
    qreal m_rightIndentation;
    Qt::Alignment m_alignment;
    QPointF m_pageTopLeft;
    QPointF m_pageBottomRight;
    QRectF m_pageRect;
    int m_rows;
    int m_cols;
    QSizeF m_cellSize;
    QRectF m_actualRect;

    EditGraphicsViewer* m_viewer;
};

#endif // EDITGRAPHICSSCENE_H