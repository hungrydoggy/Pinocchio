
#include "stdafx.h"
#include "Engine.h"

#include <GL/glew.h>

#include "kinect/Kinect.h"
#include "kinect/KinectManager.h"


Engine::Engine() : runningState(true)
{
}

/// 메인 루프
void Engine::run()
{
	float deltaTime;

	// 동기화
	deltaTime = syncronize();

	// 업데이트
	update(deltaTime);

	// 그리기
	draw();
}

/// 동기화
float Engine::syncronize()
{
	static uint oldTime = timeGetTime();
	uint deltaTime;

	while(true)
	{
		deltaTime = timeGetTime() - oldTime;

		if (deltaTime > 100)
		{
			oldTime += deltaTime;
			continue;

		}else if (deltaTime < 16)
		{
			continue;
		}

		break;
	}

	oldTime += deltaTime;

	return (float)deltaTime * 0.001f;
}

void Engine::update(float deltaTime)
{
	/*
	// 키넥트에서 센서값 받아와서 갱신하기
	for (int i=0; i<KINECT_MANAGER.getKinectCount(); ++i)
	{
		bool depthIsRefreshed = KINECT_MANAGER.getKinect(i)->refreshDepthBuffer() >= 0;
		bool colorIsRefreshed = KINECT_MANAGER.getKinect(i)->refreshColorBuffer() >= 0;
		if (depthIsRefreshed && colorIsRefreshed)
			KINECT_MANAGER.getKinect(i)->mapColorToDepth();
	}
	//*/
}

void Engine::draw()
{
	/// 그리기
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	
    static const float xyScale = tanf(deg2rad(58.5f) * 0.5f) / (640.f * 0.5f);

	glPushMatrix();

	camera.applyViewMatrix();

	glTranslatef(0, 0, -2);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4f(0.f, 1.f, 0.f, 1.f);

	static float r = 0.f;
	glRotatef(camera.getRotation().getY() + r, 0, 1, 0);
	r += 2.f;
	
	glTranslatef(0, 0, 2);

	glPointSize(1.f);
	
	float radius = 1.f;
	Kinect *kinect = KINECT_MANAGER.getKinect(0);

	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);
	{
		for (int y=0; y<480; ++y)
		{
			for (int x=0; x<640; ++x)
			{
				byte *p = &kinect->getMappedColorBuffer()[(x + y*kinect->getColorWidth())*4];
				
				glColor4ub(p[2], p[1], p[0], 255);
				/*
				ushort depth = kinect->getDepthBuffer()[(x + y*kinect->getColorWidth())] >> 3;
				if (depth >= 300 && depth <= 4000)
				{
					float realDepth = depth / 1000.f;
					glVertex3f((x - 320) * realDepth * xyScale, (480 - y - 240) * realDepth * xyScale, -realDepth);
				}
				//*/
				const Vector3 &vec = kinect->getPointCloud()[x + y*kinect->getColorWidth()];
				glVertex3f(vec.getX(), vec.getY(), vec.getZ());
			}
		}
	}
	glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void myGLPerspectivef(float fovy, float aspect, float nearValue, float farValue)
{
	float h = 2 * nearValue * tanf(deg2rad(fovy/2));
	float top = h;
	float bottom = -h;
	float left = aspect * bottom;
	float right = aspect * top;
	glFrustum(left, right, bottom, top, nearValue, farValue);
}

/// 리사이즈
void Engine::resize(int width, int height)
{
	this->width = width;
	this->height = height;

	/*
	/// 직교 투영
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, width, height, 0, -1000.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	/// 원근 투영
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluPerspective(45, (float)width / (float)height, 1000, -1000);
	//glFrustum(-width / 2, width / 2, -height / 2, height / 2, 0, 100);
	//glFrustum(-1, 1, -1, 1, 0, 10);
	myGLPerspectivef(45, (float)width / (float)height, 1, 10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    glViewport(0, 0, width, height);

	/// open gl 설정
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
