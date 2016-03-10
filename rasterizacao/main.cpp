#include "main.h"

//-----------------------------------------------------------------------------
std::vector<Pixel> list;

void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as funções do mygl.h
	//*************************************************************************
	int x, y, r, g, b, a;

	while(true) {
		std::cin >> x >> y >> r >> g >> b >> a;
		
		if(std::cin.eof())
			break;
		
		Pixel pixel = {.coord = {.x = x, .y = y}, .color = {.r = r, .g = g, .b = b, .a = a}};
		list.push_back(pixel);
	}
	
	if(list.size() == 2)
		DrawLine(list.at(0), list.at(1));
	else if(list.size() == 3)
		DrawTriangle(list.at(0), list.at(1), list.at(2));
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	std::cout << "Input (x y r g b a):" << std::endl;

	glutMainLoop();

	return 0;
}

