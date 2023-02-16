#ifdef PF_64
#pragma OPENCL EXTENSION cl_khr_fp64: enable
#endif

// mandelbrot
//float X_MIN = -2.25;
//float X_MAX = 0.75;
//float Y_MIN = -1.5;
//float Y_MAX = 1.5;

// seahorse tail zoom
__constant float X_MIN = -0.7435669 - 0.0011439;
__constant float X_MAX = -0.7435669 + 0.0011439;
__constant float Y_MIN = -0.1314023 - 0.0011439;
__constant float Y_MAX = -0.1314023 + 0.0011439;

// seahorse tail
//float X_MIN = -0.7435669 - 0.0022878;
//float X_MAX = -0.7435669 + 0.0022878;
//float Y_MIN = -0.1314023 - 0.0022878;
//float Y_MAX = -0.1314023 + 0.0022878;

__constant int MAX_ITERATIONS = 1000;
__constant int RADIUS = 256;
		   
__constant int COLORS = 64;


float3 HSVtoRGB(float fH, float fS, float fV) {
	float fR;
	float fG;
	float fB;

	float fC = fV * fS; // Chroma
	//double fHPrime = fmod(fH / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fH / 60, 2) - 1));
	float fM = fV - fC;

	if (0 <= fH && fH < 60) {
		//if (0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (60 <= fH && fH < 120) {
		//else if (1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (120 <= fH && fH < 180) {
		//else if (2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (180 <= fH && fH < 240) {
		//else if (3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (240 <= fH && fH < 300) {
		//else if (4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (300 <= fH && fH < 360) {
		//else if (5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else {
		fR = fC;
		fG = 0;
		fB = fX;
	}

	fR += fM;
	fG += fM;
	fB += fM;

	return (float3)(fR, fG, fB);
}


float2 multiply_complex_inner(float2 real1, float2 real2)
{
	float2 result;
	result.x = real1.x * real2.x - real1.y * real2.y;
	result.y = real1.x * real2.y + real2.x * real1.y;

	return result;
}


kernel void multiply_complex(__global float2* real1_in, __global float2* real2_in, __global float2* real_out)
{
	//#ifdef FP_64
	//double dreal_out = *real1_in * *real2_in - *img1_in * *img2_in;
	//*real_out = (float)dreal_out;
	//double dimg_out = *real1_in * *img2_in + *real2_in * *img1_in;
	//*img_out = (float)dimg_out;
	//
	//#else
	//*real_out = *real1_in * *real2_in - *img1_in * *img2_in;
	//*img_out = *real1_in * *img2_in + *real2_in * *img1_in;
	//#endif
	float2 result = multiply_complex_inner(*real1_in, *real2_in);
	*real_out = result;
}

kernel void power_complex(__global float2* real_in, __global int* exponent, __global float2* real_out)
{
	float2 result;
	if (*exponent == 0)
	{
		result = (float2)(1, 1);
		*real_out = result;
		return;
	}
	else if (*exponent == 1)
	{
		*real_out = *real_in;
		return;
	}
	result = *real_in;
	float2 tempResult = (float2)(0, 0);
	int i;

	for (i = 2; i <= *exponent; i++)
	{
		result = multiply_complex_inner(result, *real_in);
	}
	*real_out = result;
	//*real_out = tempResult;
	//*real_out = 1111;
	//*img_out = 2222;
}

float complexLength(float2 number)
{
	return number.x * number.x + number.y * number.y;
}

//kernel void create_palette(write_only image2d_t image, float3 color)
//{
//	int2 coord = (int2)(0, get_global_id(0));
//	write_imageui(image, coord, (uint4)(color, 255));
//}

kernel void image_proc(write_only image2d_t image, read_only image1d_t palette, sampler_t paletteSampler)
{
	uint2 dims = (uint2)(get_image_width(image), get_image_height(image));
	int2 coord = (int2)(get_global_id(0), get_global_id(1));
	float2 num = (float2)(X_MIN + (float)coord.x / dims.x * (X_MAX - X_MIN), Y_MIN + (float)coord.y / dims.y * (Y_MAX - Y_MIN));

	float2 fraction = (float2)((float)coord[0]/dims[0], (float)coord[1]/dims[1]);

	float2 z0 = (float2)(0, 0);
	float2 zk = (float2)(0, 0);
	int i;
	for (i = 0; i < MAX_ITERATIONS; i++)
	{
		z0 = multiply_complex_inner(z0, z0);
		z0.x += num.x;
		z0.y += num.y;

		if (complexLength(z0) > RADIUS)
		{
			break;
		}
	}
	float frac = (float)i / MAX_ITERATIONS;
	//uint4 color = (uint4)((uint)(fraction.x*255), (uint)(fraction.x*255), (uint)(fraction.x*255), 255);
	uint4 color;// = (uint4)((uint)(frac * 255), (uint)(frac * 255), (uint)(frac * 255), 255);

	if (i < MAX_ITERATIONS)
	{
		float nsmooth = log(z0.x * z0.x + z0.y * z0.y) / 2;
		float nu = log(nsmooth / log(2.0f)) / log(2.0f);
		//i = floor(i + 1 - nu);
		uint4 tColor = read_imageui(palette, paletteSampler, i % COLORS);
		//color *= 255;
		//float3 tColor = HSVtoRGB(0.95f + 10 * ((float)i/MAX_ITERATIONS), 0.6f, 1.0f);
		//color.x = 255 * (uint)tColor.x;
		//color.y = 255 * (uint)tColor.y;
		//color.z = 255 * (uint)tColor.z;
		//color.w = 255;
		//color = (uint4)(255 * tColor);
		color = tColor;
		//color.w = 255;
	}
	else
	{
		color = (uint4)(0, 0, 0, 255);
	}
	
	//color = (uint4)(100, 150, 50, 255);
	write_imageui(image, coord, color);
}