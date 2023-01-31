#ifdef PF_64
#pragma OPENCL EXTENSION cl_khr_fp64: enable
#endif

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
		result.x = 1;
		result.y = 1;
		*real_out = result;
		return;
	}
	else if (*exponent == 1)
	{
		*real_out = *real_in;
		return;
	}
	result = *real_in;
	float2 tempResult = { 0,0 };
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

kernel void image_proc(write_only image2d_t image)
{
	uint2 dims = (uint2)(get_image_width(image),get_image_height(image));
	int2 coord = (int2)(get_global_id(0),get_global_id(1));
	float2 fraction = (float2)((float)coord[0]/dims[0], (float)coord[1]/dims[1]);
	uint4 color = (uint4)((uint)(fraction.x*255), (uint)(fraction.x*255), (uint)(fraction.x*255), 255);
	//color = (uint4)(30, 30, 30, 30);
	write_imageui(image, coord, color);
}