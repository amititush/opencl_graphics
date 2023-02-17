#pragma once

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <CL/cl.h>
#include <math.h>
#include <complex>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <chrono>

#define COMPLEX_MULTIPLICATION "complex_multiplication.cl"

#define WIDTH 1000
#define HEIGHT 1000

#define COLORS 64

class Device
{
public:
	int initDevice();
	int shutdownDevice();
	int runMandelbrot();
	int saveImage();
	void fixRatio(bool by_x);
	void restartDevice();
	void setResolution(const int res[2]);
	void setPosition(const float position[4]);
	void zoom(float zoomValue);
	void setMiddlePosition(const int position[2]);
public:
	cl_device_id deviceId;
	char deviceName[32];
	char contextDeviceName[32];

	int width = WIDTH;
	int height = HEIGHT;
	
	unsigned char colors[COLORS * 4];
	cl_context context;
	cl_program program;
	cl_int built_program;
	cl_uint num_kernels;
	cl_kernel* kernels;
	cl_command_queue command_queue;

	cl_mem image;
	unsigned char* imageData;
	cl_mem paletteImage;
	unsigned char* paletteImageData;
	cl_sampler paletteSampler;

	cl_float4 position = { -2.25, 0.75, -1.5, 1.5 };
	cl_float4 prevPosition = { -2.25, 0.75, -1.5, 1.5 };
	cl_float4 realPosition = { -2.25, 0.75, -1.5, 1.5 };
};

