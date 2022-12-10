/*
Graficos por Computadora
Nombre: Proyecto 2
Nombre del archivo: main.c
Fecha:  09-12-2022
Integrantes:
  Amavizca Cuellar Elizabeth / Morales Flores Gael Alexis / Manriquez Cobian Rogelio

Objetivo: Implementar un videojuego en 2D.

Compilar:
  gcc -c main.c
Enlazar:
  gcc main.o -o run -l glut32 -l opengl32
Ejecutar:
  ./run
*/

// IMPORTACIÓN DE LIBRERÍAS UTILIZADAS EN EL PROGRAMA.
#include<gl\glut.h>
#include<math.h>
#include<stdio.h>
#include <windows.h>
#define tamVentana 800

// DEFINICIÓN DE PALETA DE COLORES
typedef enum{NEGRO, AZUL, VERDE, CYAN, ROJO, MAGENTA, AMARILLO, BLANCO, GRIS,
             GRIS_OSCURO, GRIS_CLARO}COLOR;

float PALETA_COLORES[11][3] = { 
  0, 0, 0, //NEGRO
  0, 0, 1, //AZUL
  0, 1, 0, //VERDE
  0, 1, 1, //CYAN
  1, 0, 0, //ROJO
  1, 0, 1, //MAGENTA
  1, 1, 0, //AMARILLO
  1, 1, 1, //BLANCO
  0.5,0.5,0.5, //GRIS
  0.25,0.25,0.25, //GRIS_OSC
  0.8,0.8,0.8 //GRIS_CLARO
};

// DEFINICIÓN DE ESTRUCTURAS DE DATOS
typedef struct{
  float x;
  float y;
}PUNTO;

typedef struct{
  PUNTO piv;
  float ancho;
  float alto;  
  COLOR color;
}RECTANGULO;

typedef struct{
  PUNTO c;
  float radio;  
  COLOR color;
}CIRCULO;


// FUNCIONES DE DIBUJADO DE FIGURAS
void circulo(CIRCULO CI);

// FUNCIONES RATÓN
void raton(int boton, int estado, int x, int y);
void clicIzquierdo(PUNTO pt);

// FUNCIONES DEL PLANO
void dibujarTexto(char *Texto, float x, float y);
void AsignaColor(COLOR c);
void display(void);

// FUNCIONES GENERALES
PUNTO CVent_a_Cplano(int x, int y,int AnchoV, int AltoV, float xini, float xfin, float yini, float yfin);
void setGame(void);
void mostrarGUI(void);
void animar(void);

// FUNCIONES PARA TECLADO
void teclasNormales(unsigned char tecla, int x, int y);
void teclasEspeciales(int tecla, int x, int y);
void teclaIzquierda(void);
void teclaArriba(void);
void teclaAbajo(void);

// VARIABLES GLOBALES
CIRCULO bola;
RECTANGULO P1, P2;

char play = 0;
char modo = 1;
char puntos[2];
char cadena[5];
char cadena2[5];
char cadena3[5];
char modoVelocidad = 1;

float deltaX = 0.2;
float deltaY = 0.25;

unsigned int velocidad = 60;
unsigned int time = 0;

// FUNCIÓN MAIN
int main(int argc, char **argv){
  glutInit(&argc,argv);
  glutInitWindowSize(tamVentana, tamVentana);
  glutCreateWindow("Proyecto 2: Implementar un videojuego en 2D");
  // CONFIGURACIÓN FIGURAS GUI
  setGame();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // DIVISIÓN DEL LIENZO EN 20x20
  glOrtho(-10, 10, -10, 10, 1.0, -1.0); 
  glMatrixMode(GL_MODELVIEW);
  // FUNCIÓN DE DISPLAY
  glutDisplayFunc(display);
  // FUNCIÓN DE ANIMAR
  glutIdleFunc(animar);
  // FUNCIONES PARA EL RATÓN
  glutMouseFunc(raton);
  // FUNCIONES PARA EL TECLADO
  glutSpecialFunc(teclasEspeciales);
  glutKeyboardFunc(teclasNormales);
  glutMainLoop();
  return 0;
}
  
void display(void){
  glClearColor(0.3, 0.3, 0.4, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  // MOSTRAR LOS ELEMENTOS GUI
  mostrarGUI();
  glFlush();  
}

void animar(void){
    if(play){
        if((time%velocidad) == 0){
            bola.c.x += deltaX;
            bola.c.y += deltaY;        
            // LADO IZQUIERDO
            if(bola.c.x - bola.radio < -9.5){
                bola.c.x = 0;
                bola.c.y = 1;
                P1.piv.x = -9.7;
                P1.piv.y = -1;
                P2.piv.x = 9.2;
                P2.piv.y = -1;
                deltaX *= -1;
                deltaY *= -1;
                puntos[0] ++;
                
            }
            // LADO DERECHO
            if(bola.c.x + bola.radio > 9.5){
                bola.c.x = 0;
                bola.c.y = 1;
                P1.piv.x = -9.7;
                P1.piv.y = -1;
                P2.piv.x = 9.2;
                P2.piv.y = -1;
                deltaX *= -1;
                deltaY *= -1;
                puntos[1] ++;
            }
            // COLISIÓN SUPERIOR            
            if(bola.c.y + bola.radio > 9.7)
                deltaY *= -1;
            // COLISIÓN INFERIOR
            if(bola.c.y - bola.radio < -8.5)
                deltaY *= -1;
            // COMPROBACIÓN COLISIÓN CON JUGADORES     
            // REBOTE IZQUIERDO
            if(bola.c.x - bola.radio <= -9.2 && bola.c.y >= P1.piv.y && bola.c.y <= P1.piv.y+P1.alto)
                deltaX *= -1;            
            else if(bola.c.x + bola.radio >= 9.2 && bola.c.y >= P2.piv.y && bola.c.y <= P2.piv.y+P2.alto)            
                deltaX *= -1;
            if(modo == 1)
                P2.piv.y = bola.c.y-1.5;
        }
        time++;
        if(time>1000000000)
            time=0;
    }    
    glutPostRedisplay();
}

void mostrarGUI(){
  float theta;
  // FONDO NEGRO PARA BOTONES
  AsignaColor(GRIS_OSCURO);
  // BORDE INFERIOR
  glRectf(-10.0, -10.0, 10.0, -8.8);
    
  // BOTÓN 1 JUGADOR
  AsignaColor(NEGRO);
  glRectf(-9.9, -9.8, -6.9, -9);
  AsignaColor(BLANCO);
  glRectf(-9.8, -9.7, -7.0, -9.1);
    
  // BOTÓN 2 JUGADORES    
  AsignaColor(NEGRO);
  glRectf(-6.7, -9.8, -3.7, -9);
  AsignaColor(BLANCO);
  glRectf(-6.6, -9.7, -3.8, -9.1);

  // BOTÓN DE VELOCIDAD
  AsignaColor(NEGRO);
  glRectf(6.7, -9.8, 3.7, -9);
  AsignaColor(BLANCO);
  glRectf(6.6, -9.7, 3.8, -9.1);

  // BOTÓN DE REINICIO      
  AsignaColor(NEGRO);
  glRectf(9.9, -9.8, 6.9, -9);
  AsignaColor(BLANCO);
  glRectf(9.8, -9.7, 7.0, -9.1);
    
  // LÍNEA MEDIO
  AsignaColor(BLANCO);
  glLineWidth(5);
  glBegin(GL_LINE_STRIP);
  glVertex2f(0, 10);
  glVertex2f(0, -8.8);      
  glEnd();  
  
  // CÍRCULO MEDIO    
  glBegin(GL_LINE_LOOP);
  for(theta=0; theta<6.28; theta+=0.1){
    glVertex2f(1.5*cos(theta),1+1.5*sin(theta));
  }    
  glEnd();
  glLineWidth(1);

  // LÍNEA NEGRA DIVISORA
  AsignaColor(NEGRO);
  glLineWidth(3);
  glBegin(GL_LINE_STRIP);
  glVertex2f(-10, -8.8);
  glVertex2f(10, -8.8);      
  glEnd();

  // JUGADOR 1  
  AsignaColor(P1.color);
  glRectf(P1.piv.x, P1.piv.y, P1.piv.x+P1.ancho, P1.piv.y+P1.alto);

  // JUGADOR 2
  AsignaColor(P2.color);
  glRectf(P2.piv.x, P2.piv.y, P2.piv.x+P2.ancho, P2.piv.y+P2.alto);

  // BOLA
  glLineWidth(4);
  circulo(bola);
  glLineWidth(1);

  // FONDO DE PUNTUACIONES
  AsignaColor(NEGRO);
  glRectf(-2, -9.9, 2, -8.9);
  AsignaColor(BLANCO);
  glRectf(-1.8, -9.7, 1.8, -9.1);

  // TEXTOS
  // TEXTOS DE PUNTUACIÓN
  AsignaColor(NEGRO);  
  sprintf(cadena,"%i",puntos[1]);
  dibujarTexto(cadena, -1.3, -9.55);
  sprintf(cadena,"%i",puntos[0]);
  dibujarTexto(cadena, 1, -9.55);  

  // TEXTOS DE MODO DE JUEGO
  AsignaColor(NEGRO);
  sprintf(cadena2,"1 PLAYER");
  dibujarTexto(cadena2, -9.5, -9.55);
  sprintf(cadena2,"2 PLAYERS");
  dibujarTexto(cadena2, -6.4, -9.55);

  // TEXTOS DE VELOCIDAD Y REINICIO
  sprintf(cadena3,"SPEED: %i", modoVelocidad);
  dibujarTexto(cadena3, 4.25, -9.55);
  sprintf(cadena3,"RESET");
  dibujarTexto(cadena3, 7.6 , -9.55);

  // BOTÓN DE INICIO
  if(!play){        
    AsignaColor(NEGRO);
    glRectf(-2.7, 0.3, 2.7, 1.7);     
    AsignaColor(BLANCO);
    glRectf(-2.5, 0.5, 2.5, 1.5);    
    AsignaColor(NEGRO);
    sprintf(cadena3,"PLAY");
    dibujarTexto(cadena3, -0.6, 0.8);
  }

  glutPostRedisplay();
}

void setGame(void){
    // INICIALIZACIÓN DE BOLA
    bola.c.x = 0;
    bola.c.y = 1;
    bola.color = GRIS;
    bola.radio = 0.5;

    // INICIALIZACIÓN DE JUGADOR 1
    P1.piv.x = -9.7;
    P1.piv.y = -1;
    P1.ancho = 0.5;
    P1.alto = 3;
    P1.color = BLANCO;
    // INICIALIZACIÓN DE JUGADOR 2
    P2.piv.x = 9.2;
    P2.piv.y = -1;
    P2.ancho = 0.5;
    P2.alto = 3;
    P2.color = BLANCO;

    puntos[0] = 0;
    puntos[1] = 0;

    play = 0;
}

// FUNCIONES DE DIBUJADO DE FIGURAS  

void circulo(CIRCULO CI){
  float x,y,theta;
  AsignaColor(CI.color);
  glBegin(GL_TRIANGLE_FAN);
  for(theta=0; theta<6.28; theta+=0.1){
    x=CI.c.x+CI.radio*cos(theta);
    y=CI.c.y+CI.radio*sin(theta);
    glVertex2f(x,y);
  }
  glEnd(); 
}

// FUNCIONES DEL RATÓN

void raton(int boton, int estado, int x, int y){
  PUNTO pt = CVent_a_Cplano(x,y,tamVentana,tamVentana,-10,10,-10,10);  
  if(estado == GLUT_DOWN)
    switch(boton){
      case GLUT_LEFT_BUTTON:        
        clicIzquierdo(pt);
        break;
     }
}

void clicIzquierdo(PUNTO pt){
  // REGIONES DE BOTONES
  // REGIÓN DE 1 JUGADOR    
  if(((pt.x >= -9.5)&&(pt.x <= -6.9)) && ((pt.y >= -9.8)&&(pt.y <= -9))){
    setGame();     
    modo = 1;        
  }        
  // REGIÓN DE 2 JUGADORES    
  else if(((pt.x >= -6.7)&&(pt.x <= -3.7)) && ((pt.y >= -9.8)&&(pt.y <= -9))){
    setGame();    
    modo = 2;
  }        
    
  // REGIÓN DE VELOCIDAD      
  if(((pt.x >= 3.7)&&(pt.x <= 6.7)) && ((pt.y >= -9.8)&&(pt.y <= -9))){
    if(modoVelocidad == 1){
      modoVelocidad = 2;
      velocidad = 40;
    }                 
    else if(modoVelocidad == 2){
      modoVelocidad = 3;
      velocidad = 20;
    }
    else if(modoVelocidad == 3){
      modoVelocidad = 1;
      velocidad = 60;
    }
  }

  // REGIÓN DE REINICIO    
  if(((pt.x >= 6.9)&&(pt.x <= 9.9)) && ((pt.y >= -9.8)&&(pt.y <= -9))){
    setGame();    
  }    

  // REGIÓN DE PLAY
  if(!play)
    if(((pt.x >= -2.7)&&(pt.x <= 2.7)) && ((pt.y >= 0.3)&&(pt.y <= 1.7)))
      play = 1;            
  
  glutPostRedisplay();
}

// FUNCIONES PARA TECLADO

void teclasNormales(unsigned char tecla, int x, int y){    
    switch(tecla){
        case 27:
            exit(0);
            break;
        case 'w':            
            if(play==1 && modo == 2)
                if(P2.piv.y < 6.5)
                    P2.piv.y += 0.5;
            break;
        case 's':            
            if(play==1 && modo == 2)
                if(P2.piv.y > -8.5)
                    P2.piv.y -= 0.5;
            break;
    }
    glutPostRedisplay();
}

void teclasEspeciales(int tecla, int x, int y){
    switch(tecla){
        case GLUT_KEY_UP:          
          teclaArriba();
          break;
        case GLUT_KEY_DOWN:
          teclaAbajo();
          break;
    }
    glutPostRedisplay();
}

void teclaArriba(void){
    if(play)
        if(P1.piv.y < 6.5)
            P1.piv.y += 0.5;
}

void teclaAbajo(void){
    if(play)
        if(P1.piv.y > -8.5)
            P1.piv.y -= 0.5;
}

// FUNCIONES ADICIONALES    

void dibujarTexto(char *Texto, float x, float y){
  int k=0;
  glRasterPos2f(x,y);
  while(Texto[k]!='\0'){
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Texto[k]);
    k++;
  }
}


PUNTO CVent_a_Cplano(int x, int y,int AnchoV, int AltoV, 
                     float xini, float xfin, float yini, float yfin){
  PUNTO pos;
  float RangoX = xfin - xini;
  float RangoY = yfin - yini;
  float porcentaje;
  porcentaje = ((float)x/AnchoV);
  pos.x = (porcentaje*(RangoX)) - (RangoX)/2;
  porcentaje = ((float)y/AltoV);
  pos.y = ((1-porcentaje) * (RangoY)) - (RangoX)/2;
  return pos;
}

void AsignaColor(COLOR c){
  glColor3f(PALETA_COLORES[c][0],PALETA_COLORES[c][1],PALETA_COLORES[c][2]);
}