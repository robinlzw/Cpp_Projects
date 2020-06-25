#ifndef LAYERRENDERER_H
#define LAYERRENDERER_H
#include <QQuickPaintedItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTransform>

class LayerRenderer : public QQuickPaintedItem
{
    Q_OBJECT
public:
    LayerRenderer(QQuickItem *parent = Q_NULLPTR);
    virtual void paint(QPainter *painter) override;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
private:
    void drawMesh(QPainter* painter);
private:
    QPoint m_transOrigin;
    QTransform m_transform;
    QPoint m_prevPoint;
    qreal m_scale = 1.0;
};

#endif // LAYERRENDERER_H
