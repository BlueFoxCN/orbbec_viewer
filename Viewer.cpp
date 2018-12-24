/*****************************************************************************
*                                                                            *
*  OpenNI 2.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
// Undeprecate CRT functions
#ifndef _CRT_SECURE_NO_DEPRECATE 
	#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "Viewer.h"
///for me
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <cstring>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
// #include "depth_to_color.h"
// #include "flash_bin_parser.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


using namespace std;
int fileNumber = 0;

static const double fx_l = 578.500977;
static const double fy_l = 578.500977;
static const double cx_l = 323.388000;
static const double cy_l = 252.487000;
static const double fx_r = 518.468018;
static const double fy_r = 518.468018;
static const double cx_r = 312.657990;
static const double cy_r = 239.076004;
static const double r0 = 0.999996;
static const double r1 = -0.002270;
static const double r2 = 0.001785;
static const double r3 = 0.002274;
static const double r4 = 0.999995;
static const double r5 = -0.001989;
static const double r6 = -0.001780;
static const double r7 = 0.001993;
static const double r8 = 0.999996;
static const double t0 = -25.179001;
static const double t1 = -0.102628;
static const double t2 = 0.314967;
static const double lk0 = -0.101344;
static const double lk1 = 0.385942;
static const double lk2 = -0.554521;
static const double lk3 = -0.004084;
static const double lk4 = 0.001130;
static const double rk0 = 0.046869;
static const double rk1 = -0.197088;
static const double rk2 = 0.094192;
static const double rk3 = -0.004304;
static const double rk4 = 0.000334;

float data[16] = {  0.895261, -0.000957499, 24.3073, -12956, \
					0.00130183, 0.897046, 11.1307, 22.0917, \
					-3.07752e-006, 3.44437e-006, 1.00012, 0.314967, \
					0, 0, 0, 1};

Mat final_mat = Mat(4, 4, CV_32F, data);

// float fx_l = 577.265991; 
// float fy_l = 577.265991; 
// float cx_l = 330.873993; 
// float cy_l = 272.842987;
// float fx_r = 512.622986; 
// float fy_r = 512.622986; 
// float cx_r = 331.141998; 
// float cy_r = 265.320007;
// float r0 = 0.999940; 
// float r1 = -0.010819; 
// float r2 = 0.001910;
// float r3 = 0.010824; 
// float r4 = 0.999938; 
// float r5 = -0.002646; 
// float r6 = -0.001881; 
// float r7 = 0.002667;
// float r8 = 0.999995;
// float t0 = -25.478901; 
// float t1 = -0.447018;
// float t2 = 3.110210; 
// float lk0 = -0.084280; 
// float lk1 = 0.242008;
// float lk2 = -0.256409; 
// float lk3 = -0.004250; 
// float lk4 = -0.013207; 
// float rk0 = 0.007452;
// float rk1 = -0.021535; 
// float rk2 = -0.053989;
// float rk3 = 0.001267; 
// float rk4 = -0.012922;

Mat mat(480, 640, CV_8UC4);
int plyFlag = 0;
char * getDate()
{
	time_t tt; 
	time( &tt ); 
	tt = tt + 8*3600; // transform the time zone 
	tm* t= gmtime( &tt ); 
	cout<<asctime(t)<<endl;
	return asctime(t);
}
char* Generate5charRandom()
{

	// srand(time(NULL));
	// int retINT5 = rand()%100000;
	// static char retINT5str[10] ;
	// sprintf(retINT5str,"%d",retINT5);
	// return retINT5str;
	return getDate();
}

#if (ONI_PLATFORM == ONI_PLATFORM_MACOSX)
		#include <GLUT/glut.h>
#else
		#include <GL/glut.h>
#endif

#include "OniSampleUtilities.h"

// #define GL_WIN_SIZE_X	1280
// #define GL_WIN_SIZE_Y	1024
#define GL_WIN_SIZE_X	1280
#define GL_WIN_SIZE_Y	960
#define TEXTURE_SIZE	512

#define DEFAULT_DISPLAY_MODE	DISPLAY_MODE_DEPTH

#define MIN_NUM_CHUNKS(data_size, chunk_size)	((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)	(MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))

SampleViewer* SampleViewer::ms_self = NULL;

void SampleViewer::glutIdle()
{
	glutPostRedisplay();
}
void SampleViewer::glutDisplay()
{
	SampleViewer::ms_self->display();
}
void SampleViewer::glutKeyboard(unsigned char key, int x, int y)
{
	SampleViewer::ms_self->onKey(key, x, y);
}



SampleViewer::SampleViewer(const char* strSampleName, openni::Device& device, openni::VideoStream& depth, openni::VideoStream& color) :
	m_device(device), m_depthStream(depth), m_colorStream(color), m_streams(NULL), m_eViewState(DEFAULT_DISPLAY_MODE), m_pTexMap(NULL)

{
	ms_self = this;
	strncpy(m_strSampleName, strSampleName, ONI_MAX_STR);
}
SampleViewer::~SampleViewer()
{
	delete[] m_pTexMap;

	ms_self = NULL;

	if (m_streams != NULL)
	{
		delete []m_streams;
	}
}

openni::Status SampleViewer::init(int argc, char **argv)
{
	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;

	m_depthStream.setMirroringEnabled(0);
	m_colorStream.setMirroringEnabled(0);


	if (m_depthStream.isValid() && m_colorStream.isValid())
	{
		depthVideoMode = m_depthStream.getVideoMode();
		colorVideoMode = m_colorStream.getVideoMode();

		int depthWidth = depthVideoMode.getResolutionX();
		int depthHeight = depthVideoMode.getResolutionY();
		int colorWidth = colorVideoMode.getResolutionX();
		int colorHeight = colorVideoMode.getResolutionY();

		if (depthWidth == colorWidth &&
			depthHeight == colorHeight)
		{
			m_width = depthWidth;
			m_height = depthHeight;
		}
		else
		{
			printf("Error - expect color and depth to be in same resolution: D: %dx%d, C: %dx%d\n",
				depthWidth, depthHeight,
				colorWidth, colorHeight);
			return openni::STATUS_ERROR;
		}
	}
	else if (m_depthStream.isValid())
	{
		depthVideoMode = m_depthStream.getVideoMode();
		m_width = depthVideoMode.getResolutionX();
		m_height = depthVideoMode.getResolutionY();
	}
	else if (m_colorStream.isValid())
	{
		colorVideoMode = m_colorStream.getVideoMode();
		m_width = colorVideoMode.getResolutionX();
		m_height = colorVideoMode.getResolutionY();
	}
	else
	{
		printf("Error - expects at least one of the streams to be valid...\n");
		return openni::STATUS_ERROR;
	}

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &m_depthStream;
	m_streams[1] = &m_colorStream;

	// Texture map init
	m_nTexMapX = MIN_CHUNKS_SIZE(m_width, TEXTURE_SIZE);
	m_nTexMapY = MIN_CHUNKS_SIZE(m_height, TEXTURE_SIZE);
	m_pTexMap = new openni::RGB888Pixel[m_nTexMapX * m_nTexMapY];

	return initOpenGL(argc, argv);

}
char plyfileName[1024];
char jpgfileName[1024];
char*char5Random=NULL;
openni::Status SampleViewer::run()	//Does not return
{
	char5Random =  Generate5charRandom();
	mkdir(char5Random,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	glutMainLoop();//让所有的与“事件”有关的函数调用无限循环

	return openni::STATUS_OK;
}

void SampleViewer::display()
{

	int changedIndex;
	openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
	if (rc != openni::STATUS_OK)
	{
		printf("Wait failed\n");
		return;
	}

	switch (changedIndex)
	{
		case 0:
			m_depthStream.readFrame(&m_depthFrame); break;
		case 1:
			m_colorStream.readFrame(&m_colorFrame); break;
		default:
			printf("Error in wait\n");
	}

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, GL_WIN_SIZE_X, GL_WIN_SIZE_Y, 0, -1.0, 1.0);

	if (m_depthFrame.isValid())
	{
		
		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
		int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);
		
		if(plyFlag == 1) 
		{
			sprintf(plyfileName, "%s%s%d%s", char5Random, "/", fileNumber,".ply");
			sprintf(jpgfileName, "%s%s%d%s", char5Random, "/", fileNumber,".jpg");
			fileNumber++;

			int counter = 0;
			
			std::ofstream outfile (plyfileName,std::ofstream::binary);
			outfile << "ply\n" \
					<< "format ascii 1.0\n" \
					<< "comment Author: CloudCompare (TELECOM PARISTECH/EDF R&D)\n" \
					<< "obj_info Generated by CloudCompare!\n" \
					<< "element vertex 2048\n" \
					<< "property float x\n" \
					<< "property float y\n" \
					<< "property float z\n" \
					<< "property uchar red\n" \
					<< "property uchar green\n" \
					<< "property uchar blue\n" \
					<< "element face 0\n" \
					<< "property list uchar int vertex_indices\n" \
					<< "end_header\n";
			
			double x = 0.0, y = 0.0, z = 0.0;

			const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)m_colorFrame.getData();
			const openni::RGB888Pixel* pStart = (const openni::RGB888Pixel*)m_colorFrame.getData();
			plyFlag = 0;
			float point_data[4] = {};
			for (int i = 0; i < m_depthFrame.getHeight(); i++) {
				for(int j = 0; j < m_depthFrame.getWidth(); j++)
				{
					if (i > 0 || j > 0) {
						pImageRow += 1;
					}
					Vec4b &rgba = mat.at<Vec4b>(i, j); 
					rgba[3] = UCHAR_MAX; 
					rgba[0] = saturate_cast<uchar>(pImageRow->b); 
					rgba[1] = saturate_cast<uchar>(pImageRow->g); 
					rgba[2] = saturate_cast<uchar>(pImageRow->r);
					if(0 != (pDepthRow + i * rowSize)[j])
					{
						counter++;
						z = (pDepthRow + i * rowSize)[j];
						point_data[0] = j * z;
						point_data[1] = i * z;
						point_data[2] = z;
						point_data[3] = 1;
						Mat point = Mat(4, 1, CV_32F, point_data);

						Mat map_point = final_mat * point;
						int u_color = int(map_point.at<float>(0, 0) / map_point.at<float>(2, 0));
						int v_color = int(map_point.at<float>(1, 0) / map_point.at<float>(2, 0));
						
						x = ((j-cx_l) * ((pDepthRow + i * rowSize)[j])) / fx_l;
						y = (((i-cy_l) * ((pDepthRow + i * rowSize)[j])) / fy_l);
						const openni::RGB888Pixel* pCur = pStart + rowSize * v_color + u_color;

						outfile << x << " ";
						outfile << y << " ";
						outfile << z << " ";
						outfile << (int)(pCur->r) << " ";
						outfile << (int)(pCur->g) << " ";
						outfile << (int)(pCur->b) << "\n";
					}
				}
			}
			
			outfile.seekp(0);
			outfile << "ply\n" \
					<< "format ascii 1.0\n" \
					<< "comment Author: CloudCompare (TELECOM PARISTECH/EDF R&D)\n" \
					<< "obj_info Generated by CloudCompare!\n" \
					<< "element vertex " << counter << "\n" \
					<< "property float x\n" \
					<< "property float y\n" \
					<< "property float z\n" \
					<< "property uchar red\n" \
					<< "property uchar green\n" \
					<< "property uchar blue\n" \
					<< "element face 0\n" \
					<< "property list uchar int vertex_indices\n" \
					<< "end_header\n";
			outfile.close();
			imwrite(jpgfileName, mat);
		}
	}

	memset(m_pTexMap, 0, m_nTexMapX*m_nTexMapY*sizeof(openni::RGB888Pixel));
	
	// check if we need to draw image frame to texture
	if ((m_eViewState == DISPLAY_MODE_OVERLAY ||
		m_eViewState == DISPLAY_MODE_IMAGE) && m_colorFrame.isValid())
	{
		const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)m_colorFrame.getData();
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_colorFrame.getCropOriginY() * m_nTexMapX;
		int rowSize = m_colorFrame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

		for (int y = 0; y < m_colorFrame.getHeight(); ++y)
		{
			const openni::RGB888Pixel* pImage = pImageRow;
			openni::RGB888Pixel* pTex = pTexRow + m_colorFrame.getCropOriginX();

			for (int x = 0; x < m_colorFrame.getWidth(); ++x, ++pImage, ++pTex)
			{
				*pTex = *pImage;

			}

			pImageRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}

	if ((m_eViewState == DISPLAY_MODE_OVERLAY || 
		m_eViewState == DISPLAY_MODE_DEPTH) && m_depthFrame.isValid())
	{
		const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*)m_depthFrame.getData();
		
		calculateHistogram(m_pDepthHist, MAX_DEPTH, m_depthFrame);
		openni::RGB888Pixel* pTexRow = m_pTexMap + m_depthFrame.getCropOriginY() * m_nTexMapX;
		int rowSize = m_depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel);

		if (m_bAlign)
		{
			openni::DepthPixel pDepthAlign[m_depthFrame.getHeight() * m_depthFrame.getWidth()] = {};
			double z = 0.0;
			float point_data[4] = {};
			for (int i = 0; i < m_depthFrame.getHeight(); i++) {
				for(int j = 0; j < m_depthFrame.getWidth(); j++)
				{
					if(0 != (pDepthRow + i * rowSize)[j])
					{
						z = (pDepthRow + i * rowSize)[j];
						point_data[0] = j * z;
						point_data[1] = i * z;
						point_data[2] = z;
						point_data[3] = 1;
						Mat point = Mat(4, 1, CV_32F, point_data);

						Mat map_point = final_mat * point;
						int u_color = int(map_point.at<float>(0, 0) / map_point.at<float>(2, 0));
						int v_color = int(map_point.at<float>(1, 0) / map_point.at<float>(2, 0));
						
						*(pDepthAlign + v_color * rowSize + u_color) = (openni::DepthPixel)z;
					}
				}
			}

			pDepthRow = pDepthAlign;
		}



		for (int y = 0; y < m_depthFrame.getHeight(); ++y)
		{
			const openni::DepthPixel* pDepth = pDepthRow;
			openni::RGB888Pixel* pTex = pTexRow + m_depthFrame.getCropOriginX();

			for (int x = 0; x < m_depthFrame.getWidth(); ++x, ++pDepth, ++pTex)
			{
				if (*pDepth != 0)
				{
					int nHistValue = m_pDepthHist[*pDepth];
					if (m_eViewState == DISPLAY_MODE_OVERLAY)
					{
						pTex->r = pTex->r * 0.5 + nHistValue * 0.5;
						pTex->g = pTex->g * 0.5 + nHistValue * 0.5;
						pTex->b = 0;
					} else {
						pTex->r = nHistValue;
						pTex->g = nHistValue;
						pTex->b = 0;
					}

				}
			}

			pDepthRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nTexMapX, m_nTexMapY, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pTexMap);

	// Display the OpenGL texture map
	glColor4f(1,1,1,1);

	glBegin(GL_QUADS);

	int nXRes = m_width;
	int nYRes = m_height;

	// upper left
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	// upper right
	glTexCoord2f((float)nXRes/(float)m_nTexMapX, 0);
	glVertex2f(GL_WIN_SIZE_X, 0);
	// bottom right
	glTexCoord2f((float)nXRes/(float)m_nTexMapX, (float)nYRes/(float)m_nTexMapY);
	glVertex2f(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	// bottom left
	glTexCoord2f(0, (float)nYRes/(float)m_nTexMapY);
	glVertex2f(0, GL_WIN_SIZE_Y);

	glEnd();

	glutSwapBuffers();

}

void SampleViewer::onKey(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key)
	{
		
	case 27:
		m_depthStream.stop();
		m_colorStream.stop();
		m_depthStream.destroy();
		m_colorStream.destroy();
		m_device.close();
		openni::OpenNI::shutdown();
		exit (1);
	case '0':
		m_eViewState = DISPLAY_MODE_OVERLAY;
		m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_OFF);
		m_bAlign = true;
		break;
	case '1':
		m_eViewState = DISPLAY_MODE_OVERLAY;
		m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_OFF);
		m_bAlign = false;
		break;
	case '2':
		m_eViewState = DISPLAY_MODE_DEPTH;
		m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_OFF);
		m_bAlign = false;
		break;
	case '3':
		m_eViewState = DISPLAY_MODE_IMAGE;
		m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_OFF);
		m_bAlign = false;
		break;
	case 'm':
		m_depthStream.setMirroringEnabled(!m_depthStream.getMirroringEnabled());
		m_colorStream.setMirroringEnabled(!m_colorStream.getMirroringEnabled());
		break;
	case '4':
		plyFlag = 1;
		break;
	case '5':
		vector<int> compression_params; 
		compression_params.push_back(IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		//显示图片 
		imwrite("1.png", mat, compression_params); 
		imshow("1.png", mat); 
		waitKey(0);
		break;
	}

}

openni::Status SampleViewer::initOpenGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow (m_strSampleName);
	// 	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	initOpenGLHooks();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	return openni::STATUS_OK;

}
void SampleViewer::initOpenGLHooks()
{
	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
}
