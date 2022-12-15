#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "Camera.h"

GLfloat angle, fAspect, rotX, rotY;

typedef float color[3];

Camera camera(vec3(0, -2, 0));

static float lastMousePos = 0.0;
static bool firstTimeMouse = true;
float x_pos = 0;

GLfloat posLuz[4] = {100.0, 50.0, 50.0, 1.0};

void DesenhaTerreno()
{
	float L = 500.0;
	float incr = 1.0;
	float y = -5;
	glColor3f(0.14f,0.65f,0.05f);
	glBegin(GL_LINES);
	for (float i = -L; i <= L; i += incr)
	{
		// Verticais
		glVertex3f(i, y, -L);
		glVertex3f(i, y, L);

		// Horizontais
		glVertex3f(-L, y, i);
		glVertex3f(L, y, i);
	}
	glEnd();
}

void DesenhaCeu()
{
	float L = 500.0;
	float incr = 1.0;
	float y = 5;
	glColor3f(0.25,0.69,0.92);
	glBegin(GL_LINES);
	for (float i = -L; i <= L; i += incr)
	{
		// Verticais
		glVertex3f(i, y, -L);
		glVertex3f(i, y, L);

		// Horizontais
		glVertex3f(-L, y, i);
		glVertex3f(L, y, i);
	}
	glEnd();
}

void DesenhaParede(float p1[3], float p2[3], float p3[3], float p4[3], color cor)
{
	glColor3fv(cor);
	glBegin(GL_QUADS);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p4);
	glEnd();
}

void DesenhaJanelaFrente(float d, color cor) {
	glColor3fv(cor);
	glBegin(GL_QUADS);
	glVertex3f(d + 5, 2, d);  // 1
	glVertex3f(d + 5, -2, d); // 2
	glVertex3f(d, -2, d);  // 3
	glVertex3f(d, 2, d);   // 4
	glEnd();
}

void DesenhaPorta(float d, color cor)
{
	glColor3fv(cor);
	glBegin(GL_QUADS);
	glVertex3f(-3, 0, d);  // 1
	glVertex3f(-3, -d, d); // 2
	glVertex3f(0, -d, d);  // 3
	glVertex3f(0, 0, d);   // 4
	glEnd();
}

void DesenhaCasa(void)
{
	float d = 10 / 2.0;

	// Define vertices
	float v1[3] = {-d, d, d};
	float v2[3] = {-d, -d, d};
	float v3[3] = {d + 15, -d, d};
	float v4[3] = {d + 15, d, d};

	float v5[3] = {d + 15, d, -d};
	float v6[3] = {d + 15, -d, -d};
	float v7[3] = {-d, -d, -d};
	float v8[3] = {-d, d, -d};

	color cinza = {0.67, 0.96, 0.71};
	color vermelho = {0.95, 0.51, 0.36};
	color marrom = {0.26, 0.01, 0.02};
	color azul = {0.25,0.69,0.92};

	// Desenha porta
	glNormal3f(0.f, 0.f, 1.f);
	DesenhaPorta(d, marrom);

	// Desenha porta
	glNormal3f(0.f, 0.f, 1.f);
	DesenhaJanelaFrente(d, azul);

	// Frente
	glNormal3f(0.f, 0.f, 1.f);
	DesenhaParede(v1, v2, v3, v4, cinza);

	// Direita
	glNormal3f(1.f, 0.f, 0.f);
	DesenhaParede(v4, v3, v6, v5, cinza);

	// Back
	glNormal3f(0.f, 0.f, -1.f);
	DesenhaParede(v5, v8, v7, v6, cinza);

	// Esquerda
	glNormal3f(-1.f, 0.f, 0.f);
	DesenhaParede(v1, v8, v7, v2, cinza);

	// Topo
	glNormal3f(0.f, 1.f, 0.f);
	DesenhaParede(v1, v4, v5, v8, vermelho);

	// Bottom
	glNormal3f(0.f, -1.f, 0.f);
	DesenhaParede(v2, v7, v6, v3, marrom);

}

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao(void)
{
	GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0};
	GLfloat luzDifusa[4] = {0.7, 0.7, 0.7, 1.0};	// "cor"
	GLfloat luzEspecular[4] = {1.0, 1.0, 1.0, 1.0}; // "brilho"

	// Capacidade de brilho do material
	GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};
	GLint especMaterial = 60;

	// Define a refletância do material
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posLuz);
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	// Limpa a janela de visualização com a cor de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	camera.ativar();

	DesenhaTerreno();

	// DesenhaCeu();

	// Chama a função que especifica os parâmetros de iluminação
	DefineIluminacao();

	glPushMatrix();
	glTranslated(0.0, 0.0, -50.0);
	DesenhaCasa();
	glPopMatrix();

	// Execução dos comandos de desenho
	glutSwapBuffers();
}

// Inicialização
void Inicializa(void)
{
	// Fundo preto
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	// Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	DefineIluminacao();
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);

	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva(angulo,aspecto,zMin,zMax)
	gluPerspective(45.0, fAspect, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if (h == 0)
		h = 1;

	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

void TeclasEspeciais(int tecla, int x, int y)
{
	switch (tecla)
	{
	case GLUT_KEY_LEFT:
		camera.left();
		break;
	case GLUT_KEY_RIGHT:
		camera.right();
		break;
	case GLUT_KEY_UP:
		camera.forward();
		break;
	case GLUT_KEY_DOWN:
		camera.back();
		break;
	}
	glutPostRedisplay();
}

void spinDisplay(void)
{
	float dx;

	if (firstTimeMouse)
	{
		dx = 0;
		lastMousePos = x_pos;
		firstTimeMouse = false;
	}
	dx = x_pos - lastMousePos;
	lastMousePos = x_pos;
	camera.updateYaw(dx);
	camera.update();
	glutPostRedisplay();
}

void MouseEvento(int x, int y)
{
	x_pos = x;
	glutIdleFunc(spinDisplay);
}

// Programa Principal
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	// Define do modo de operacao da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5, 5);

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(1280, 720);

	// Cria a janela passando como argumento o titulo da mesma
	glutCreateWindow("Minecraft de centavos");

	// Registra a funcao callback de redesenho da janela de visualizacao
	glutDisplayFunc(Desenha);

	// Registra a funcao callback para tratamento das teclas especiais
	glutSpecialFunc(TeclasEspeciais);

	// Registra a função callback para tratamento de eventos do mouse
	glutMotionFunc(MouseEvento);

	// Registra a funcao callback para tratamento do redimensionamento da janela
	glutReshapeFunc(AlteraTamanhoJanela);

	Inicializa();
	glutMainLoop();
}
