#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}
void MyGLWidget::initializeGL(){
  
  LL2GLWidget::initializeGL();

  //Angles
  headPos = glm::vec3(0,0,0);
  tailPos = glm::vec3(-2,0,0);

  //Cámara
  factorAngleY = 0;
  factorAngleX = 90*(M_PI/360);
  obs = glm::vec3(0,55,0);
  vrp = glm::vec3(0,0,0);
  up = glm::vec3(0,0,-1);
  fovIdeal = float(M_PI*60)/180;
  fov = fovIdeal;
  l = b = -25;
  r = t = 25;
  znear = 1;
  zfar = 80;
  radiEscena = sqrt(450);
  ortho = false;
  viewTransform();
  

}

void MyGLWidget::paintGL(){
 // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);
  
  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {  
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }
  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);
  
  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipe
  for(int i = 0; i < 30; ++i){
    
    glBindVertexArray (VAO_models[PIPE]);
    pipeTransform(glm::vec3(14.5,0,14.5-i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

    pipeTransform(glm::vec3(14.5-i,0,14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
    
    pipeTransform(glm::vec3(-14.5,0,14.5-i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

    pipeTransform(glm::vec3(-14.5+i,0,-14.5));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
    
  }

  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);

}

void MyGLWidget::SnakeHeadTransform(){
  
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, headPos);
  TG = glm::rotate(TG,glm::radians(headAngle-90), glm::vec3(0.0,1.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::SnakeTailTransform(){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::rotate(TG,glm::radians(tailAngle), glm::vec3(0.0,1.0,0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::pipeTransform(glm::vec3 pos){

  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE],escalaModels[PIPE]*3,escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  
}
void MyGLWidget::MarbleTransform(){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(0.5*escalaModels[MARBLE],0.5*escalaModels[MARBLE],0.5*escalaModels[MARBLE]));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::TerraTransform(){

  LL2GLWidget::TerraTransform();
  glm::mat4 TG(1.0f);
  TG = scale(TG, glm::vec3(3,0,3));
  TG = translate(TG, glm::vec3(0,0,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::resizeGL (int w, int h) 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);
   
  if(ra < 1.0){
     if(not ortho)fov = 2.0*atan(tan(fovIdeal/2.0)/ra);
     else{
      
      l = -radiEscena;
      r = radiEscena;
      b = -radiEscena / ra;
      t = radiEscena / ra;

     }
  }
  if(ra > 1.0 and ortho){
        
        l = -ra * radiEscena;
        r = ra * radiEscena;
        b = -radiEscena;
        t = radiEscena;

  }
  
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;
  projectTransform();
}
//Cámara------------------------------------------------------------------

void MyGLWidget::viewTransform(){
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  if(ortho){
      View = glm::lookAt(obs,vrp,up);
  }
  else{   
    View = glm::translate(View, glm::vec3(0,0, -(2*radiEscena)));
    View = glm::rotate(View, factorAngleX, glm::vec3(1., 0., 0.));
    View = glm::rotate(View, -factorAngleY, glm::vec3(0., 1., 0.));
    View = glm::translate(View, glm::vec3(-centreEscena));
  }
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if(not ortho)Proj = glm::perspective (fov, ra, znear, zfar);
  else Proj = glm::ortho(l,r,b,t,znear,zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
} 
void MyGLWidget::updateSnakeGame()
{
	glm::vec3 newPos = headPos + direction;
	
        if (checkPosition(newPos)) // check boundary limits, check if eats himself
	{
		bodyPos.push_back(headPos);	
		headPos = newPos;
		
		if (direction == glm::vec3(0,0,-1))
			headAngle = 90;
		if (direction == glm::vec3(0,0,1))
			headAngle = -90;
		if (direction == glm::vec3(-1,0,0))
			headAngle = 180;
		if (direction == glm::vec3(1,0,0))
			headAngle = 0;			
	
		if (glm::distance(newPos,marblePos) > 0) // check if doesn't eat marble 
		{
			tailPos = bodyPos.front();
			bodyPos.erase(bodyPos.begin());	
			glm::vec3 tailMov = bodyPos.front() - tailPos;
			tailAngle = signedAngleBetweenVectors(glm::vec3(1,0,0),tailMov);
		}
		else
			computeRandomMarblePosition();
	}
}


bool MyGLWidget::checkPosition(glm::vec3 pos)
{
	bool valid = true;

  if((pos.x >= 14) or (pos.x <= -14)
  or(pos.z >= 14) or (pos.z <= -14)){
    valid = false;
  }
  if(pos == tailPos) valid = false;
  else{
    for(int i = 0; i < bodyPos.size() and valid; ++i){
        if(pos == bodyPos[i]) valid = false;
    }
  }
	return valid; 
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      direction = glm::vec3(0,0,-1);
      updateSnakeGame();
      break;
    }
    case Qt::Key_Down: {
      direction = glm::vec3(0,0,1); 
      updateSnakeGame();
      
      break;
        }
    case Qt::Key_Left: { 
      direction = glm::vec3(-1,0,0);
      updateSnakeGame();
      
      break;
        }  
    case Qt::Key_Right: { 
      direction = glm::vec3(1,0,0);
      updateSnakeGame();
      
    
      break;
        }
    case Qt::Key_C: { 
      if(!ortho) ortho = true;
      else ortho = false;
      viewTransform();
      projectTransform();

      break;
        }           
    case Qt::Key_R: { 

      initializeGL();
      projectTransform();
      break;
        }  
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event){
  LL2GLWidget::mousePressEvent (event);

}
void MyGLWidget::mouseMoveEvent(QMouseEvent* event)
{

  makeCurrent();
  if (DoingInteractive == ROTATE) {
    //Dividimos entre alt y ample para hacer la rotación en base a nuestro tamaño del viewport
    factorAngleX += M_PI*(event->y()-yClick)/float(alt); 
    factorAngleY -= M_PI*(event->x()-xClick)/float(ample);
    viewTransform();
  }

  xClick = event->x();
  yClick = event->y();

  update ();
}

void MyGLWidget::mouseReleaseEvent (QMouseEvent *event) {
  DoingInteractive = NONE;
}

void MyGLWidget::carregaShaders(){
  LL2GLWidget::carregaShaders();

}




