//GazeTraceWidget.h
#ifndef GAZETRACEWIDGET_H
#define GAZETRACEWIDGET_H

//Include standard files
#include <QWidget>
#include <QPoint>
#include <vector>
#include "GazeTraceData.h"

class GazeTraceWidget : public QWidget
{
    Q_OBJECT

    public:
        GazeTraceWidget();
        virtual ~GazeTraceWidget();

        bool hasTracePoints();
        void addTracePoint(GazeTraceEvent event);
        void clearTracePoints();

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        std::vector<GazeTraceEvent> tracePoints;
};

#endif // GAZETRACEWIDGET_H
