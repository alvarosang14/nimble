#include <QPainter>
#include "TraceWidget.h"

TraceWidget::TraceWidget()
{
    qreal  dpr = window()->devicePixelRatio();
    setMinimumSize(800/dpr,600/dpr);
  setStyleSheet("background-color: rgba(0,0,0,0)");
}

TraceWidget::~TraceWidget()
{
}

void TraceWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  //Translate painter coordinate origin to match normalized point orgin
  painter.translate(this->width()/2, this->height()/2);

  //Set paint color and width
  painter.setPen(QPen(Qt::red, 4));

  //Convert tracePoint normalized coordinates to QPoints
  std::vector<QPoint> points;
  for(int i = 0; i < tracePoints.size(); i++)
  {
    QPoint p;

    p.setX(static_cast<int>(tracePoints[i].x * this->width() / 2.0));
    p.setY(static_cast<int>(-tracePoints[i].y * this->height() / 2.0));

    points.push_back(p);
  }

  //Draw a polyline consisting of lines between the points
  painter.drawPolyline(points.data(), points.size());
}

bool TraceWidget::hasTracePoints()
{
  return !tracePoints.empty();
}

void TraceWidget::addTracePoint(HapticTraceEvent event)
{
  double xRange = 1.0 / this->width();
  double yRange = 1.0 / this->height();

  //check for uniqueness in (x,y) coordinates before saving
  for(int i = 0; i < tracePoints.size(); i++)
  {
    //if close enough to an existing point do not add
    if(tracePoints[i].x <= (event.x + xRange) &&
       tracePoints[i].x >= (event.x - xRange) &&
       tracePoints[i].y <= (event.y + yRange) &&
       tracePoints[i].y >= (event.y - yRange) )
    {
      return;
    }
  }

  tracePoints.push_back(event);
}

void TraceWidget::clearTracePoints()
{
  tracePoints.clear();
}
