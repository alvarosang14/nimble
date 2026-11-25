#include <QPainter>
#include "SmoothPathWidget.h"

SmoothPathWidget::SmoothPathWidget()
{
  qreal  dpr = window()->devicePixelRatio();
  setMinimumSize(800/dpr,600/dpr);
  setStyleSheet("background-color: rgba(0,0,0,0)");
}

SmoothPathWidget::~SmoothPathWidget()
{
}

void SmoothPathWidget::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  //Translate painter coordinate origin to match normalized point orgin
  painter.translate(this->width()/2, this->height()/2);

  //Set paint color and width
  painter.setPen(QPen(Qt::magenta, 2));

  //Convert tracePoint normalized coordinates to QPoints
  std::vector<QPointF> points;
  for(int i = 0; i < path.size(); i++)
  {
    QPointF p;

    p.setX(path[i].x() * this->width() / 2.0);
    p.setY(-path[i].y() * this->height() / 2.0);

    points.push_back(p);
  }

  //Draw a polyline consisting of lines between the points
  painter.drawPolyline(points.data(), points.size());
}


void SmoothPathWidget::addNormPathPoint(QPointF normP)
{
  path.push_back(normP);
}

void SmoothPathWidget::clearPath()
{
  path.clear();
}
