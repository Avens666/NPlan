#ifndef GLENV_H
#define GLENV_H

#include <GL/gl.h>
#include <GL/glu.h>

class CGLEvn
{
public:
	CGLEvn();
	~CGLEvn();
	//释放
	void glenvRelease();
	//初始化环境
	bool glenvInit(HDC hdc, int x,  int y,  int t, int b, bool b_mem = FALSE);
	//设为当前环境
	bool glenvActive();
	//上屏
	void SwapBuffers();

	HGLRC m_hrc;
	HDC m_hdc;

	RECT m_rectClient;

	kn_bool  m_b_init;
};

#endif //GLENV_H
