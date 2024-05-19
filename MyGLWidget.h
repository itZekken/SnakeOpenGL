#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);

    //Inicialización
    virtual void initializeGL();
    virtual void TerraTransform();
    virtual void carregaShaders();
    virtual void paintGL();
    
    virtual void MarbleTransform();
    virtual void pipeTransform(glm::vec3 pos);
    virtual void SnakeHeadTransform();
    virtual void SnakeTailTransform();

    virtual void updateSnakeGame();

    virtual void mousePressEvent (QMouseEvent *event); // nou pel ratolí

    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void mouseReleaseEvent (QMouseEvent *event);

    virtual void resizeGL(int w, int h);

    virtual void viewTransform();
    virtual void projectTransform();
  //Angulos cámara
    float psi; 
    float theta;
    bool ortho;
    float l,r,b,t;
    float fovIdeal;

  

  public slots:


  private:
  
    int printOglError(const char file[], int line, const char func[]);
   
};
