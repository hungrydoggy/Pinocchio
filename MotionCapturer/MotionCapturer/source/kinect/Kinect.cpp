
#include "stdafx.h"
#include "Kinect.h"

#include <NuiApi.h>


Kinect::Kinect(INuiSensor *sensor) : sensor(sensor)
{
	HRESULT hr;

	// 키넥트 초기화
	hr = sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX);
	if (FAILED(hr))
	{
		releaseSensor();
		return;
	}

	// depth event 생성
	nextDepthFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// depth image stream을 생성
	hr = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_640x480, 0, 2, nextDepthFrameEvent, &depthStreamHandle);
	if (FAILED(hr))
	{
		releaseSensor();
		return;
	}

	// color event 생성
	nextColorFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// color image stream을 생성
	hr = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, nextColorFrameEvent, &colorStreamHandle);
	if (FAILED(hr))
	{
		releaseSensor();
		return;
	}
	
	// 사이즈 얻기
	DWORD width, height;

	NuiImageResolutionToSize(NUI_IMAGE_RESOLUTION_640x480, width, height);
	depthWidth = (long)width;
	depthHeight = (long)height;

	NuiImageResolutionToSize(NUI_IMAGE_RESOLUTION_640x480, width, height);
	colorWidth = (long)width;
	colorHeight = (long)height;


	// 버퍼 할당
	depthBuffer = new ushort[depthWidth * depthHeight];
	memset(depthBuffer, 0, sizeof(ushort) * depthWidth * depthHeight);
	colorBuffer = new byte[colorWidth * colorHeight * 4];
	memset(depthBuffer, 0, sizeof(byte) * colorWidth * colorHeight * 4);
	colorCoordinates = new long[depthWidth * depthHeight * 2];
	memset(depthBuffer, 0, sizeof(long) * depthWidth * depthHeight * 2);
}


Kinect::~Kinect()
{
	releaseSensor();

	DELETE_ARRAY(depthBuffer)
	DELETE_ARRAY(colorBuffer)
	DELETE_ARRAY(colorCoordinates)
}

/// 키넥트 센서 해제
void Kinect::releaseSensor()
{
	if (sensor)
	{
		sensor->Release();
	}
}

/// 깊이 버퍼 갱신
int Kinect::refreshDepthBuffer()
{
	NUI_IMAGE_FRAME imageFrame;
	HRESULT hr;

	if (WaitForSingleObject(nextDepthFrameEvent, 0) != WAIT_OBJECT_0)
		return -1;

	hr = sensor->NuiImageStreamGetNextFrame(depthStreamHandle, 0, &imageFrame);
	if (FAILED(hr))
		return hr;

	NUI_LOCKED_RECT lockedRect;
	hr = imageFrame.pFrameTexture->LockRect(0, &lockedRect, null, 0);
	if (FAILED(hr))
		return hr;

	memcpy(depthBuffer, lockedRect.pBits, lockedRect.size);

	hr = imageFrame.pFrameTexture->UnlockRect(0);
	if (FAILED(hr))
		return hr;

	hr = sensor->NuiImageStreamReleaseFrame(depthStreamHandle, &imageFrame);
	if (FAILED(hr))
		return hr;

	return 0;
}

/// 컬러 버퍼 갱신
int Kinect::refreshColorBuffer()
{
	NUI_IMAGE_FRAME imageFrame;
	HRESULT hr;
	
	if (WaitForSingleObject(nextColorFrameEvent, 0) != WAIT_OBJECT_0)
		return -1;

	hr = sensor->NuiImageStreamGetNextFrame(colorStreamHandle, 0, &imageFrame);
	if (FAILED(hr))
		return hr;

	NUI_LOCKED_RECT lockedRect;
	hr = imageFrame.pFrameTexture->LockRect(0, &lockedRect, null, 0);
	if (FAILED(hr))
		return hr;

	memcpy(colorBuffer, lockedRect.pBits, lockedRect.size);

	hr = imageFrame.pFrameTexture->UnlockRect(0);
	if (FAILED(hr))
		return hr;

	hr = sensor->NuiImageStreamReleaseFrame(colorStreamHandle, &imageFrame);
	if (FAILED(hr))
		return hr;

	return 0;
}