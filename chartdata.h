#ifndef CHARTDATA_H
#define CHARTDATA_H

#include "chartlayeritem.h"

#include <QPen>

class PlainChart;
class ChartAxis;
class ChartText;
class ChartSplineData;


enum DataType { polygs, trajects, spline, points };


class ChartDataItem
{
public:
    ChartDataItem() { bounds.resize(4); }
    virtual ~ChartDataItem() {}

    virtual void paint(QPainter* painter) = 0;
    virtual void setData(const QVector<QPointF>& data) = 0;
    virtual void setPen(const QPen& pen) { mainPen = pen; }
    virtual void setBrush(const QBrush& br) { mainBrush = br; }
    virtual void setColor(Qt::GlobalColor color) { mainPen.setColor(color); mainBrush.setColor(color); }
    virtual void clearData() = 0;
    virtual bool isEmpty() const = 0;
    virtual const QVector<qreal> range() const { return bounds; }

    QPen pen() const { return mainPen; }
    QBrush brush() const { return mainBrush; }

protected:
    QVector<qreal> bounds;
    QPen mainPen;
    QBrush mainBrush;
};


class ChartData : public ChartLayerItem
{
public:
    explicit ChartData(PlainChart* chart = NULL);
    virtual ~ChartData() { clearData(); }

    virtual void paint(QPainter* painter);

    ChartDataItem* createItem(DataType type);
    void addDataItem(ChartDataItem* item);
    ChartDataItem* itemAt(int index);
    void setHeightItem(ChartDataItem* item);
    ChartSplineData* heightItem() const { return hghtItem; }

    void clearData();
    bool isEmpty() const;
    const QVector<qreal> range() const;

private:
    void initPainter(QPainter* painter);

    PlainChart* chart;
    ChartSplineData* hghtItem;
    QVector<ChartDataItem*> mData;

    friend class PlainChart;
    friend class ChartText;
    friend class ChartAxis;
};


class ChartPolygonData : public ChartDataItem
{
public:
    ChartPolygonData();
    virtual ~ChartPolygonData() { clearData(); }

    virtual void paint(QPainter* painter);
    virtual void setData(const QVector<QPointF>& pols);
    virtual void clearData();
    virtual bool isEmpty() const { return polygs.isEmpty(); }

private:
    void setPolys(const QPolygonF& pols);

    QPolygonF polygs;
    QVector<QPair<QPen, QBrush> > stylePairs;
};


class ChartTrajectoryData : public ChartDataItem
{
public:
    ChartTrajectoryData();
    virtual ~ChartTrajectoryData() { clearData(); }

    virtual void paint(QPainter* painter);
    virtual void setData(const QVector<QPointF>& data);
    virtual void clearData();
    virtual bool isEmpty() const { return traj.isEmpty(); }

    void setColor(Qt::GlobalColor trajectoryColor);

private:
    void setTraj(const QVector<QPointF>& newTraj);

    QVector<QPointF> traj;
};


class ChartSplineData : public ChartDataItem
{
public:
    ChartSplineData();
    virtual ~ChartSplineData() { clearData(); }

    virtual void paint(QPainter* painter);
    virtual void setData(const QVector<QPointF>& prof);
    virtual void clearData();
    virtual bool isEmpty() const { return spline.isEmpty(); }

    qreal heightValue(qreal x_value) const;

private:
    void setSpline(const QVector<QPointF>& prof);

    QVector<QPointF> spline;
};


class ChartPointData : public ChartDataItem
{
public:
    ChartPointData();
    virtual ~ChartPointData() { clearData(); }

    virtual void paint(QPainter* painter);
    virtual void setData(const QVector<QPointF>& points);
    virtual void clearData();
    virtual bool isEmpty() const { return points.isEmpty(); }

    void setZeroPointBrush(QBrush newBrush) { zeroPointBr = newBrush; }
    void setZeroPointPen(QPen newPen) { zeroPointPen = newPen; }

    QPen zeroPen() const { return zeroPointPen; }

private:
    void setPoints(const QVector<QPointF>& pts);

    QVector<QPointF> points;
    QPen zeroPointPen;
    QBrush zeroPointBr;
};

#endif // CHARTDATA_H
