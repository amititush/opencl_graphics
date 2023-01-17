#ifdef PF_64
#pragma OPENCL EXTENSION cl_khr_fp64: enable
#endif

float2 multiply_complex_inner(float real1, float img1, float real2, float img2)
{
	float2 result;
	result[0] = real1 * real2 - img1 * img2;
	result[1] = real1 * img2 + real2 * img1;

	return result;
}


kernel void multiply_complex(__global float* real1_in, __global float* img1_in, __global float* real2_in, __global float* img2_in, __global float* real_out, __global float* img_out)
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
	float2 result = multiply_complex_inner(*real1_in, *img1_in, *real2_in, *img2_in);
	*real_out = result[0];
	*img_out = result[1];
}

kernel void power_complex(__global float* real_in, __global float* img_in, __global int* exponent, __global float* real_out, __global float* img_out)
{
	float2 result = { 0,0 };
	int i;
	for (i = 0; i < exponent; i++)
	{
		float2 tempResult = multiply_complex_inner(*real_in, *img_in, *real_in, *img_in);
		result[0] += tempResult[0];
		result[1] += tempResult[1];
	}
	*real_out = result[0];
	*img_out = result[1];
}