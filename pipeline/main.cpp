#include "main.h"
#include "objLoader.h"
#include "pipeline.h"

objLoader* objData;
double angle = 0.0f;

void MyGlDraw(void)
{
	
	myglInit();				// Inicialização do Pipeline
	
	myglLookAt(
		0.0f, 0.0f, 5.0f,	// posição da câmera
		0.0f, 0.0f, 0.0f,	// ponto para o qual a camera está olhando
		0.0f, 1.0f, 0.0f	// vetor "up"
	);
	
	myglViewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);		// constroi a janela de projecao
	
	myglViewPlaneDistance(2.4);		// Seleciona a distancia d do viewplane

	myglRotate(angle, 0.0f, 1.0f, 0.0f);

	angle += 0.1f;
	
	myglClearColor(0, 0, 0, 255);	// Limpa o frame buffer
	
	Pixel pixel[3];
	
	for(int f = 0; f < objData->faceCount; f++) {
		obj_face* obj = objData->faceList[f];
		
		for(int i = 0; i < 3; i++) {
			Point p = myglPipeline(
				objData->vertexList[obj->vertex_index[i]]->e[0],
				objData->vertexList[obj->vertex_index[i]]->e[1],
				objData->vertexList[obj->vertex_index[i]]->e[2]
			);
			
			pixel[i].coord.x = p.x;
			pixel[i].coord.y = p.y;
			pixel[i].color.r = 255;
			pixel[i].color.g = 255;
			pixel[i].color.b = 255;
			pixel[i].color.a = 0;
		}
		
		DrawTriangle(pixel[0], pixel[1], pixel[2]);
	}
}

//********************************************************************************

int main(int argc, char **argv)
{
	if(argc < 2) {
		std::cout << "\nDigite o nome do arquivo OBJ.\nExiting...\n" << std::endl;
		return 1;
	}
	
	objData = new objLoader();			// cria o objeto que carrega o modelo
	objData->load(argv[1]);				// a carga do modelo é indicada atraves do nome do arquivo. 

	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

