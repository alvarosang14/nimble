#ifndef SMOOTHPATHWIDGET_H
#define SMOOTHPATHWIDGET_H

#include <QWidget>
#include <QPointF>
#include <vector>

class SmoothPathWidget : public QWidget
{
    Q_OBJECT

    public:
        SmoothPathWidget();
        virtual ~SmoothPathWidget();

        void addNormPathPoint(QPointF normP);
        void clearPath();

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        std::vector<QPointF> path;
};

#endif // SMOOTHPATHWIDGET_H
