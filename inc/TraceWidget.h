#ifndef TRACEWIDGET_H
#define TRACEWIDGET_H

#include <QWidget>
#include <QPoint>
#include <vector>
#include "HapticTraceData.h"

class TraceWidget : public QWidget
{
    Q_OBJECT

    public:
        TraceWidget();
        virtual ~TraceWidget();

        bool hasTracePoints();
        void addTracePoint(HapticTraceEvent event);
        void clearTracePoints();

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        std::vector<HapticTraceEvent> tracePoints;
};

#endif // TRACEWIDGET_H
