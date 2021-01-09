#ifndef EDITGRAPHICSVIEWER_H
#define EDITGRAPHICSVIEWER_H

#include <QObject>
#include <QGraphicsView>
#include <QTime>
#include <QTextEdit>

class EditGraphicsScene;

class EditGraphicsViewer : public QGraphicsView
{
    Q_OBJECT
public:
    EditGraphicsViewer(QWidget* parent = nullptr);
    ~EditGraphicsViewer();

    qreal zoomFactor() const;
    int mm2PixelsX(float mm) const;
    int mm2PixelsY(float mm) const;
    float pixels2mmX(int pixels) const;
    float pixels2mmY(int pixels) const;

    float widthMM() const;
    void setWidthMM(float widthMM);
    float heightMM() const;
    void setHeightMM(float heightMM);

    int rows() const;
    void setRows(int rows);
    int cols() const;
    void setCols(int cols);

    float gridWidthMM() const;
    void setGridWidthMM(float widthMM);
    float gridHeightMM() const;
    void setGridHeightMM(float heightMM);

private:
    void init();
	void creatTextEdit();
	void releaseTextEdit();

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();
	void textAreaChanged();
    void drawGrids();
    void drawGrid();
    void print();

signals:
    void zoomChanged();
    void beginSelecting();
    void endSelecting();
    void cancelSelecting();
    void mouseMoved(const QPointF& pos);
	void creatingText();
	void readyText();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    void zoomBy(qreal factor);
	//mouse
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
	//key
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void keyReleaseEvent(QKeyEvent *event) override;


private:
    QScopedPointer<EditGraphicsScene> m_scene;
    //LaserScene* m_scene;
    bool m_rubberBandActive;
    QPoint m_rubberBandOrigin;
    bool m_mousePressed;
    QPoint m_lastDragPos;

    QPointF m_selectionStartPoint;
    QPointF m_selectionEndPoint;


	//Text
	QPoint m_textInputPoint;
	QTime m_time;
	int m_curTime;
	int m_lastTime;
	QTextEdit *m_textEdit;

	bool m_isKeyShiftPressed;
	bool m_isKeyDelPress;
	//Ruller
	qreal m_rullerWidth;

    float m_width;
    float m_height;
    int m_rows;
    int m_cols;
    float m_gridWidth;
    float m_gridHeight;

    QFont m_font;
};

#endif // EDITGRAPHICSVIEWER_H