#include "GraphicsWidget.h"
#include "MainWindow.h"

#include <QtMath>
#include <QPainter>
#include <QOpenGLPaintDevice>
#include <QOpenGLFramebufferObjectFormat>

#include <QDebug>

GraphicsWidget::GraphicsWidget(HapticEnvironment *hEnv, MainWindow *mainWindow, QWidget *parent)
    : QOpenGLWidget(parent),
      m_timer(0),
      m_render_fbo(0),
      m_texture_fbo(0),
      m_hEnv(hEnv),
      m_mainWindow(mainWindow)
{
  makeCurrent();
  qreal  dpr = window()->devicePixelRatio();
  setMinimumSize(800/dpr,600/dpr);

  m_background = new QImage();
}

GraphicsWidget::~GraphicsWidget()
{
  m_timer->stop();
  delete m_texture_fbo;
  if(m_render_fbo != m_texture_fbo)
    delete m_render_fbo;
}

void GraphicsWidget::initializeGL()
{
  initializeOpenGLFunctions();

  initFrameBuffers();

  qglClearColor(Qt::black);
  glEnable(GL_NORMALIZE);

  //set timer for repainting/animation
  m_timer = new QBasicTimer();
  m_timer->start(10, this); //10 millisecond loop;
}

void GraphicsWidget::resizeGL(int width, int height)
{
    //setup view port again, taking into account user scaling of screen
    qreal  dpr = window()->devicePixelRatio();
    glViewport(0, 0, width * dpr, height* dpr);
}

void GraphicsWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //draw background
  draw();
  //draw stylus
  drawStylus();
}

void GraphicsWidget::initFrameBuffers()
{
  qreal  dpr = window()->devicePixelRatio();

  if(QOpenGLFramebufferObject::hasOpenGLFramebufferBlit())
  {
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(4);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    m_render_fbo  = new QOpenGLFramebufferObject(800/dpr, 600/dpr, format);
    m_texture_fbo = new QOpenGLFramebufferObject(800/dpr, 600/dpr);
  }
  else
  {
    m_render_fbo  = new QOpenGLFramebufferObject(800/dpr, 600/dpr);
    m_texture_fbo = m_render_fbo;
  }
}

void GraphicsWidget::timerEvent(QTimerEvent *e)
{
  update();
}

void GraphicsWidget::qglColor(QColor color)
{
  glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void GraphicsWidget::qglClearColor(QColor clearColor)
{
  glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
}


void GraphicsWidget::loadPattern(const QString &s)
{
    if (!s.isEmpty())
    {
        if (m_background->load(s))
        {
            m_currentPattern = s;
            qDebug() << "Nuevo mapa:" << s;
            inTargetArea = false; // Reiniciar estado del área objetivo
            mapTimer.invalidate(); // Invalidar temporizador
        }
        else
        {
            qDebug() << "Error al cargar el mapa:" << s;
        }
    }
    else
    {
        m_background->fill(Qt::black);
        qDebug() << "Ningun mapa seleccionado";
    }
}

//funcion para iniciar y parar la simulcaion en si estamos en el lugar indicado de cada mapa durante 2 segundos
void GraphicsWidget::checkPosition()
{
    //Primer mapa
    if (m_currentPattern == "patterns/complexity/comp1.bmp")
    {
        //zona inicial
        if (pos.x >= 0.0525 && pos.x <= 0.2325 && pos.y <= -0.68666 && pos.y >= -0.914 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio del primer mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds << " ms" << std::endl;
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio del primer mapa";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= 0.6975 && pos.x <= 0.8775 && pos.y <= 0.8451 && pos.y >= 0.6171 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final del primer mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final del primer mapa";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }

    //Segundo Mapa
    if (m_currentPattern == "patterns/complexity/comp2.bmp")
    {
        //zona inicial
        if (pos.x >= 0.74 && pos.x <= 0.92 && pos.y <= -0.1277 && pos.y >= -0.3557 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio del segundo mapa";
            }
            else
            {
                qint64 elapsedMilliseconds2 = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds2 << " ms" << std::endl;
                if (elapsedMilliseconds2 >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio del segundo mapa";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= -0.9125 && pos.x <= -0.7325 && pos.y <= -0.6805 && pos.y >= -0.9085 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final del segundo mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final del segundo mapa";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }

    //Tercer Mapa
    if (m_currentPattern == "patterns/complexity/comp3.bmp")
    {
        //zona inicial
        if (pos.x >= -0.82 && pos.x <= -0.64 && pos.y <= 0.9539 && pos.y >= 0.7259 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio del tercer mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds << " ms" << std::endl;
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio del tercer mapa";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= 0.78 && pos.x <= 0.96 && pos.y <= 0.282 && pos.y >= 0.054 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final del tercer mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final del tercer mapa";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }

    //Cuarto Mapa
    if (m_currentPattern == "patterns/straight_to_curvy/stc1.bmp")
    {
        //zona inicial
        if (pos.x >= 0.42 && pos.x <= 0.6 && pos.y <= -0.7179 && pos.y >= -0.9459 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio del cuarto mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds << " ms" << std::endl;
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio del cuarto mapa";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= -0.685 && pos.x <= -0.505 && pos.y <= 0.957 && pos.y >= 0.729 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final del cuarto mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final del cuarto mapa";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }

    //Quinto Mapa
    if (m_currentPattern == "patterns/straight_to_curvy/stc2.bmp")
    {
        //zona inicial
        if (pos.x >= -0.1275 && pos.x <= 0.0525 && pos.y <= -0.7263 && pos.y >= -0.9543 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio del quinto mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds << " ms" << std::endl;
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio del quinto mapa";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= -0.7325 && pos.x <= -0.5525 && pos.y <= 0.9487 && pos.y >= 0.7207 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final del quinto mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final del quinto mapa";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }

    //Sexto Mapa
    if (m_currentPattern == "patterns/straight_to_curvy/stc3.bmp")
    {
        //zona inicial
        if (pos.x >= -0.802 && pos.x <= -0.622 && pos.y <= 0.9039 && pos.y >= 0.6759 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio del sexto mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds << " ms" << std::endl;
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio del sexto mapa";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= 0.7675 && pos.x <= 0.9475 && pos.y <= 0.5929 && pos.y >= 0.3649 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final del sexto mapa";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final del sexto mapa";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }
    //Septimo, Octavo y Noveno Mapa
    if (m_currentPattern == "patterns/width/wid1.bmp" || m_currentPattern == "patterns/width/wid2.bmp" || m_currentPattern == "patterns/width/wid3.bmp")
    {
        //zona inicial
        if (pos.x >= -0.9325 && pos.x <= -0.7525 && pos.y <= 0.2176 && pos.y >= -0.0104 && m_mainWindow->getRecording()==false)
        {
            if (!inTargetArea) // Si acabamos de entrar en el área objetivo
            {
                inTargetArea = true;
                mapTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto de inicio de los últimos mapas";
            }
            else
            {
                qint64 elapsedMilliseconds = mapTimer.elapsed();
                //std::cout << "Tiempo transcurrido: " << elapsedMilliseconds << " ms" << std::endl;
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto de inicio de los últimos mapas";
                    mapTimer.restart(); // Reiniciar el temporizador
                    //debemos mandar algo para que empieze el programa
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para iniciar la grabacion
                }
            }
        }
        else
        {
           inTargetArea = false; // Si salimos del área de salida
        }
        // Zona final
        if (pos.x >= 0.74 && pos.x <= 0.92 && pos.y <= 0.3045 && pos.y >= 0.0765 && m_mainWindow->getRecording()==true)
        {
            if (!inFinalArea) // Si acabamos de entrar en el área final
            {
                inFinalArea = true;
                finalTimer.restart(); // Reiniciar el temporizador
                qDebug() << "Entrando en el punto final de los últimos mapas";
            }
            else
            {
                qint64 elapsedMilliseconds = finalTimer.elapsed();
                if (elapsedMilliseconds >= 2000) // 2000 milisegundos = 2 s
                {
                    qDebug() << "Llevamos 2 segundos en el punto final de los últimos mapas";
                    m_mainWindow->AutoRecording(); // Llamar a la función de MainWindow para detener la grabación
                }
            }
        }
        else
        {
            inFinalArea = false; // Si salimos del área final
        }
    }
}


void GraphicsWidget::draw()
{

  QRect rect(0, 0, m_render_fbo->width(), m_render_fbo->height());

  // render the image file into our framebuffer object
  m_render_fbo->bind();
  QOpenGLPaintDevice device(m_render_fbo->size());
  QPainter fbo_painter;
  fbo_painter.begin(&device);
  fbo_painter.drawImage(rect, *m_background);
  fbo_painter.end();
  m_render_fbo->release();

  if(m_render_fbo != m_texture_fbo)
  {
    QOpenGLFramebufferObject::blitFramebuffer(m_texture_fbo, rect, m_render_fbo,
                                              rect);
  }

  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
  glPushMatrix();

    //setup view port again, taking into account user scaling of screen
  qreal  dpr = window()->devicePixelRatio();
  glViewport(0, 0, width()* dpr, height()* dpr);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, +1, -1.0, +1.0, 4.0, 10.0);

  //setup for texture background
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -4.0f);

  //draw background
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texture_fbo->texture());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0, -1.0,  0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, -1.0,  0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0,  1.0,  0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0,  1.0,  0.0f);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
  glPopAttrib();
}

void GraphicsWidget::drawStylus()
{
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
  glPushMatrix();

  //setup view port again, taking into account user scaling of screen
  qreal  dpr = window()->devicePixelRatio();
  glViewport(0, 0, width()* dpr, height()* dpr);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, +1, -1.0, +1.0, 4.0, 10.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //get probe position
  //API::Vec3 pos;
  if(m_hEnv!=0 && m_hEnv->isDeviceEnabled())
  {
    pos = m_hEnv->getNormPosition();
  }
  else
  {
    pos = getMouseNormPosition();
  }
  glTranslatef((GLfloat)pos.x,
               (GLfloat)pos.y,
               -4.0);

  // Llamar a checkPosition para verificar la posición actual
  checkPosition();

  //cout<<"xxx: "<<pos.x;
  //cout<<"  yyy: "<<pos.y<<endl;

  qglColor(Qt::red);
  glShadeModel(GL_SMOOTH);
  drawSphere(0.020, 10, 10);

  glPopMatrix();
  glPopAttrib();

  //vamos a ver si estamos en el inicio o final de cada mapa para saber cuando empezar
}


void GraphicsWidget::drawSphere(double r, int lats, int longs)
{
  int i, j;
  for(i = 0; i <= lats; i++)
  {
    double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
    double z0  = r * sin(lat0);
    double zr0 = r * cos(lat0);

    double lat1 = M_PI * (-0.5 + (double) i / lats);
    double z1 = r * sin(lat1);
    double zr1 = r * cos(lat1);

    glBegin(GL_QUAD_STRIP);
    for(j = 0; j <= longs; j++)
    {
      double lng = 2 * M_PI * (double) (j - 1) / longs;
      double x = cos(lng);
      double y = sin(lng);

      glNormal3f(x * zr0, y * zr0, z0);
      glVertex3f(x * zr0, y * zr0, z0);
      glNormal3f(x * zr1, y * zr1, z1);
      glVertex3f(x * zr1, y * zr1, z1);
    }
    glEnd();
  }
}

HAPI::Vec3 GraphicsWidget::getMouseNormPosition()
{
  //Get global mouse coordinate
  QPoint globalP = QCursor::pos();

  //Map to widget coordinate
  QPointF localP = this->mapFromGlobal(globalP);

  //Normalize the coordinate
  localP.setX(localP.x() / this->width() * 2 - 1);
  localP.setY(-localP.y() / this->height() * 2 + 1);

  HAPI::Vec3 normP(localP.x(), localP.y(), -1.0);

  //Cap at normalized bounds
  normP.x = H3DUtil::H3DMax(normP.x, -1.0);
  normP.y = H3DUtil::H3DMax(normP.y, -1.0);
  normP.z = H3DUtil::H3DMax(normP.z, -1.0);
  normP.x = H3DUtil::H3DMin(normP.x, 1.0);
  normP.y = H3DUtil::H3DMin(normP.y, 1.0);
  normP.z = H3DUtil::H3DMin(normP.z, 1.0);

  return normP;
}
