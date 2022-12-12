// Esse projeto foi adaptado de TeaPot3D.cpp - Isabel H. Manssour
// Um programa OpenGL que exemplifica a visualização de objetos 3D.

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

GLfloat angle, fAspect, rotX, rotY;

typedef float color[3];

void DesenhaTerreno()
{
	float L = 500.0;
	float incr = 1.0;
	float y = -0.5;
	glColor3f(1.0f, 1.0f, 1.0f);
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

void DesenhaPorta(float d, color cor)
{
	glColor3fv(cor);
	glBegin(GL_QUADS);
		glVertex3f(-3, 0, d);  //1
		glVertex3f(-3, -d, d); //2
		glVertex3f(0, -d, d); //3
		glVertex3f(0, 0, d); //4
	glEnd();
}

void DesenhaCasa(void)
{
	float d = 10 / 2.0;

	// Define vertices
	float v1[3] = {-d, d, d};
	float v2[3] = {-d, -d, d};
	float v3[3] = {d+15, -d, d};
	float v4[3] = {d+15, d, d};

	float v5[3] = {d+15, d, -d};
	float v6[3] = {d+15, -d, -d};
	float v7[3] = {-d, -d, -d};
	float v8[3] = {-d, d, -d};

	color cinza = {0.67, 0.96, 0.71};
	color vermelho = {0.95, 0.51, 0.36};
	color marrom = {0.26, 0.01, 0.02};

	// Frente
	DesenhaParede(v1, v2, v3, v4, cinza);
	// Direita
	DesenhaParede(v4, v3, v6, v5, cinza);
	// Back
	DesenhaParede(v5, v8, v7, v6, cinza);
	// Esquerda
	DesenhaParede(v1, v8, v7, v2, cinza);
	// Topo
	DesenhaParede(v1, v4, v5, v8, vermelho);
	// Bottom
	DesenhaParede(v2, v7, v6, v3, marrom);

	// Desenha porta
	DesenhaPorta(d, marrom);
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	// Limpa a janela de visualização com a cor de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DesenhaTerreno();

	// Remove as faces de dentro da casa
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glPushMatrix();
	glTranslated(0.0, 0.0, -50.0);
	DesenhaCasa();
	glPopMatrix();

	glDisable(GL_CULL_FACE);

	// Execução dos comandos de desenho
	glutSwapBuffers();
}

// Inicialização
void Inicializa(void)
{
	// Fundo preto
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

// Programa Principal
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	// Define do modo de operacao da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5, 5);

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(1280, 720);

	// Cria a janela passando como argumento o titulo da mesma
	glutCreateWindow("Minecraft de centavos");

	// Registra a funcao callback de redesenho da janela de visualizacao
	glutDisplayFunc(Desenha);

	// Registra a funcao callback para tratamento do redimensionamento da janela
	glutReshapeFunc(AlteraTamanhoJanela);

	Inicializa();
	glutMainLoop();
}
