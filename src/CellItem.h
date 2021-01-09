#pragma once

#include <QGraphicsItem>

class CellItemPrivate;
class CellItem : public QGraphicsItem
{
public:
    enum class State {
        Normal,
        Editing,
        Selected
    };

    explicit CellItem(QGraphicsItem* parent = nullptr);
    explicit CellItem(const QRectF& rect, QGraphicsItem* parent = nullptr);
    explicit CellItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr);
    ~CellItem();

    QPen pen() const;
    void setPen(const QPen& pen);
    QBrush brush() const;
    void setBrush(const QBrush& brush);

    QRectF rect() const;
    void setRect(const QRectF& rect);
    QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    State state() const;

public slots:
    void init();
    void beginEditing();
    void endEditing();

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

protected:
    QScopedPointer<CellItemPrivate> d_ptr;

private:
    Q_DISABLE_COPY(CellItem)
    Q_DECLARE_PRIVATE(CellItem)
};
