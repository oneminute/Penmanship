#include "CellItem.h"

#include <QBrush>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QPen>
#include <QTextEdit>

class CellItemPrivate
{
    Q_DECLARE_PUBLIC(CellItem)
public:
    CellItemPrivate(CellItem* ptr)
        : q_ptr(ptr)
    {}
    QPen pen;
    QBrush brush;
    QChar character;
    CellItem::State state;
    QFont font;

    QRectF rect;
    mutable QRectF boundingRect;

    QGraphicsProxyWidget* editingWidget;

    CellItem* q_ptr;
};

CellItem::CellItem(QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , d_ptr(new CellItemPrivate(this))
{
}

CellItem::CellItem(const QRectF& rect, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , d_ptr(new CellItemPrivate(this))
{
    init();
    setRect(rect);
}

CellItem::CellItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , d_ptr(new CellItemPrivate(this))
{
    init();
    setRect(QRectF(x, y, w, h));
}

CellItem::~CellItem()
{
}

QPen CellItem::pen() const
{
    Q_D(const CellItem);
    return d->pen;
}

void CellItem::setPen(const QPen& pen)
{
    Q_D(CellItem);
    if (d->pen == pen)
        return;
    d->pen = pen;
    d->boundingRect = QRectF();
    update();
}

QBrush CellItem::brush() const
{
    Q_D(const CellItem);
    return d->brush;
}

void CellItem::setBrush(const QBrush& brush)
{
    Q_D(CellItem);
    if (d->brush == brush)
        return;
    d->brush = brush;
    update();
}

QRectF CellItem::rect() const
{
    return QRectF();
}

void CellItem::setRect(const QRectF& rect)
{
    Q_D(CellItem);
    if (d->rect == rect)
        return;

    d->rect = rect;

}

QRectF CellItem::boundingRect() const
{
    Q_D(const CellItem);
    if (d->boundingRect.isNull())
    {
        qreal halfpw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF() / 2;
        d->boundingRect = d->rect;
        if (halfpw > 0.0)
            d->boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return d->boundingRect;
}

void CellItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_D(CellItem);
    Q_UNUSED(widget);
    painter->setPen(d->pen);
    painter->setBrush(d->brush);
    painter->drawRect(d->rect);

    painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
    painter->drawLine(d->rect.topLeft(), d->rect.bottomRight());
    painter->drawLine(d->rect.topRight(), d->rect.bottomLeft());
    painter->drawLine(QPointF(d->rect.left(), d->rect.center().y()), QPointF(d->rect.right(), d->rect.center().y()));
    painter->drawLine(QPointF(d->rect.center().x(), d->rect.top()), QPointF(d->rect.center().x(), d->rect.bottom()));

    if (!d->character.isNull())
    {
        painter->setPen(QPen(Qt::black, 2));
        painter->setFont(d->font);
        painter->drawText(d->rect, Qt::AlignCenter, QChar(d->character));
    }
}

CellItem::State CellItem::state() const
{
    Q_D(const CellItem);
    return d->state;
}

void CellItem::beginEditing()
{
    Q_D(CellItem);
    d->state = State::Editing;
    QTextEdit* edit = new QTextEdit();
    d->editingWidget = scene()->addWidget(edit);
    d->editingWidget->setPos(d->rect.topLeft());
    edit->resize(d->rect.width(), d->rect.height());
   	edit->setStyleSheet("background-color : rgb(0,255,0,0%);");
	edit->setFrameShape(QFrame::NoFrame);
	edit->setWordWrapMode(QTextOption::WrapMode::NoWrap);
	edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	edit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    edit->setFocus();
    edit->setFont(d->font);
    edit->setAlignment(Qt::AlignCenter);
    if (!d->character.isNull())
    {
        edit->setText(d->character);
        edit->selectAll();
    }
}

void CellItem::endEditing()
{
}

void CellItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (state() != State::Editing)
        beginEditing();
}

void CellItem::init()
{
    Q_D(CellItem);
    d->pen = QPen(Qt::red, 2);
    d->brush = QBrush(Qt::NoBrush);
    d->font = QFont(QFontDatabase::applicationFontFamilies(0)[0]);
    d->font.setPixelSize(60);
    d->character = QChar(0x4e00);
}
