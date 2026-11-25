#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QBasicTimer>
#include <QElapsedTimer>


#include "HapticEnvironment.h"
#include "HapticTraceData.h"
#include "IrisHiruEnvironment.h"

class MainWindow;

class GraphicsWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    public:
        GraphicsWidget(HapticEnvironment *hEnv, MainWindow *mainWindow, QWidget *parent = 0);
        virtual ~GraphicsWidget();

        //void loadPattern(QLatin1String s);
        void loadPattern(const QString &s);

        HAPI::Vec3 getMouseNormPosition();

    /*signals:
        void startAnotherPart(); // Señal para iniciar otra parte del programa*/


    protected:
        void timerEvent(QTimerEvent *e);

        void initializeGL();
        void resizeGL(int width, int height);
        void paintGL();

    private:
        void initFrameBuffers();
        void draw();
        void drawStylus();
        void drawSphere(double r, int lats, int longs);
        void qglColor(QColor color);
        void qglClearColor(QColor clearColor);
        void checkPosition(); //verificar la posición

        QBasicTimer *m_timer;
        QElapsedTimer mapTimer; // Temporizador para medir el tiempo en el área objetivo de salida
        QElapsedTimer finalTimer; // Temporizador para medir el tiempo en el área objetivo de llegada

        HAPI::Vec3 pos;

        //variable para saber en que mapa estoy
        int map;
        bool inTargetArea; // Estado para verificar si estamos en el área objetivo inicial
        bool inFinalArea; // Estado para verificar si estamos en el área objetivo final

        QOpenGLFramebufferObject *m_render_fbo;
        QOpenGLFramebufferObject *m_texture_fbo;
        QImage *m_background;

        //Haptic environment for rendering cursor
        HapticEnvironment *m_hEnv;

        //almacenar el mapa en el que estamos
        QString m_currentPattern;

        //Puntero a Maindow
        MainWindow *m_mainWindow;

};
#endif // GLWIDGET_H

