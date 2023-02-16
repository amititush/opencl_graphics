// opencl_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
//#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

#define COMPLEX_MULTIPLICATION "complex_multiplication.cl"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define WIDTH 2000
#define HEIGHT 2000

#define COLORS 64

#include <chrono>
using namespace std::chrono;

double lerp(double t, double a, double b)
{
    return a + t * (b - a);
}

void ColorMap(int iteration, unsigned char& r, unsigned char& g, unsigned char& b, float t)
{
    float fr;
    float fg;
    float fb;

    //double t = (double)iteration / maxIterations;
    if (t >= 0 && t < 0.16)
    {
        float r1 = 0;
        float g1 = 7.0 / 255;
        float b1 = 100.0 / 255;
        float r2 = 32.0 / 255;
        float g2 = 107.0 / 255;
        float b2 = 203.0 / 255;

        //t = 0.16;
        fr = r1 + (t / 0.16) * (r2 - r1);
        fg = g1 + (t / 0.16) * (g2 - g1);
        fb = b1 + (t / 0.16) * (b2 - b1);
    }
    else if (t >= 0.16 && t < 0.42)
    {
        float r1 = 32.0 / 255;
        float g1 = 107.0 / 255;
        float b1 = 203.0 / 255;
        float r2 = 237.0 / 255;
        float g2 = 1;
        float b2 = 1;
        //t = 0.42;
        fr = r1 + ((t - 0.16) / (0.42 - 0.16)) * (r2 - r1);
        fg = g1 + ((t - 0.16) / (0.42 - 0.16)) * (g2 - g1);
        fb = b1 + ((t - 0.16) / (0.42 - 0.16)) * (b2 - b1);
    }
    else if (t >= 0.42 && t < 0.6425)
    {
        float r1 = 237.0 / 255;
        float g1 = 1;
        float b1 = 1;
        float r2 = 1;
        float g2 = 170.0 / 255;
        float b2 = 0;

        //t = 0.6425;
        fr = r1 + ((t - 0.42) / (0.6425 - 0.42)) * (r2 - r1);
        fg = g1 + ((t - 0.42) / (0.6425 - 0.42)) * (g2 - g1);
        fb = b1 + ((t - 0.42) / (0.6425 - 0.42)) * (b2 - b1);
    }
    else if (t >= 0.6425 && t < 0.8575)
    {
        float r1 = 1;
        float g1 = 170.0 / 255;
        float b1 = 0;
        float r2 = 0;
        float g2 = 2.0 / 255;
        float b2 = 0;
        // t = 0.8575;
        fr = r1 + ((t - 0.6425) / (0.8575 - 0.6425)) * (r2 - r1);
        fg = g1 + ((t - 0.6425) / (0.8575 - 0.6425)) * (g2 - g1);
        fb = b1 + ((t - 0.6425) / (0.8575 - 0.6425)) * (b2 - b1);
    }
    else
    {   
        float r1 = 0;
        float g1 = 2.0 / 255;
        float b1 = 0;
        float r2 = 0;
        float g2 = 7.0 / 255;
        float b2 = 100.0 / 255;
        //t = 1;
        fr = r1 + ((t - 0.8575) / (1 - 0.8575)) * (r2 - r1);
        fg = g1 + ((t - 0.8575) / (1 - 0.8575)) * (g2 - g1);
        fb = b1 + ((t - 0.8575) / (1 - 0.8575)) * (b2 - b1);
    }

    r = 255*fr;
    g = 255*fg;
    b = 255*fb;

    return;
}

void ColorMapNoBlack(int iteration, unsigned char& r, unsigned char& g, unsigned char& b, float t)
{
    float fr;
    float fg;
    float fb;

    //double t = (double)iteration / maxIterations;
    if (t >= 0 && t < 0.16)
    {
        float r1 = 0;
        float g1 = 7.0 / 255;
        float b1 = 100.0 / 255;
        float r2 = 32.0 / 255;
        float g2 = 107.0 / 255;
        float b2 = 203.0 / 255;

        //t = 0.16;
        fr = r1 + (t / 0.16) * (r2 - r1);
        fg = g1 + (t / 0.16) * (g2 - g1);
        fb = b1 + (t / 0.16) * (b2 - b1);
    }
    else if (t >= 0.16 && t < 0.42)
    {
        float r1 = 32.0 / 255;
        float g1 = 107.0 / 255;
        float b1 = 203.0 / 255;
        float r2 = 237.0 / 255;
        float g2 = 1;
        float b2 = 1;
        //t = 0.42;
        fr = r1 + ((t - 0.16) / (0.42 - 0.16)) * (r2 - r1);
        fg = g1 + ((t - 0.16) / (0.42 - 0.16)) * (g2 - g1);
        fb = b1 + ((t - 0.16) / (0.42 - 0.16)) * (b2 - b1);
    }
    else if (t >= 0.42 && t < 0.6425)
    {
        float r1 = 237.0 / 255;
        float g1 = 1;
        float b1 = 1;
        float r2 = 1;
        float g2 = 170.0 / 255;
        float b2 = 0;

        //t = 0.6425;
        fr = r1 + ((t - 0.42) / (0.6425 - 0.42)) * (r2 - r1);
        fg = g1 + ((t - 0.42) / (0.6425 - 0.42)) * (g2 - g1);
        fb = b1 + ((t - 0.42) / (0.6425 - 0.42)) * (b2 - b1);
    }
    else if (t >= 0.6425 && t < 0.8575)
    {
        float r1 = 1;
        float g1 = 170.0 / 255;
        float b1 = 0;
        float r2 = 0;
        float g2 = 2.0 / 255;
        float b2 = 0.5;
        // t = 0.8575;
        fr = r1 + ((t - 0.6425) / (0.8575 - 0.6425)) * (r2 - r1);
        fg = g1 + ((t - 0.6425) / (0.8575 - 0.6425)) * (g2 - g1);
        fb = b1 + ((t - 0.6425) / (0.8575 - 0.6425)) * (b2 - b1);
    }
    else
    {
        float r1 = 0;
        float g1 = 2.0 / 255;
        float b1 = 0.5;
        float r2 = 0;
        float g2 = 7.0 / 255;
        float b2 = 100.0 / 255;
        //t = 1;
        fr = r1 + ((t - 0.8575) / (1 - 0.8575)) * (r2 - r1);
        fg = g1 + ((t - 0.8575) / (1 - 0.8575)) * (g2 - g1);
        fb = b1 + ((t - 0.8575) / (1 - 0.8575)) * (b2 - b1);
    }

    r = 255* fr;
    g = 255* fg;
    b = 255* fb;

    return;
}

void PreComputeColors(unsigned char *colors, int colorCount, bool black)
{
    vector<unsigned char> color = { 0, 0, 0, 255 };
    for (int i = 0; i < colorCount; i++)
    {
        if (black)
            ColorMap(i, color[0], color[1], color[2], float(i) / colorCount);
        else
            ColorMapNoBlack(i, color[0], color[1], color[2], float(i) / colorCount);
        colors[i*4] = color[0];
        colors[i * 4 + 1] = color[1];
        colors[i * 4 + 2] = color[2];
        colors[i * 4 + 3] = 255;
    }
}

void CL_CALLBACK kernel_complete(cl_event e, cl_int status, void* data)
{
    std::cout << (char*)data << std::endl;
}

std::complex<float> complex_power(std::complex<float> complex, int exponent)
{
    std::complex<float> result = { 0, 0 };
    for (int i = 0; i < exponent; i++)
    {
        result *= std::complex<float>(complex.real() * complex.real() - complex.imag() * complex.imag(), 2 * complex.real() * complex.imag());
    }

    return result;
}

int opencl_render(unsigned char** imageData)
{
    // vector<vector<unsigned char>> colors;
    unsigned char colors[COLORS * 4];
    //colors.resize(COLORS);
    PreComputeColors(colors, COLORS, false);

    cl_platform_id* platforms;
    cl_uint numPlatforms = 0;
    clGetPlatformIDs(32, NULL, &numPlatforms);
    platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
    clGetPlatformIDs(numPlatforms, platforms, NULL);

    cl_device_id device = 0;
    cl_uint cores = 0;
    char deviceName[32];
    for (unsigned int i = 0; i < numPlatforms; i++)
    {
        char vendorName[32];
        clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 32, vendorName, NULL);
        if (strcmp(vendorName, "stubvendorxxx") == 0)
            continue;
        std::cout << "Vendor: " << vendorName << std::endl;

        cl_device_id* devices;
        cl_uint num_devices = 0;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        if (num_devices > 0)
        {
            devices = (cl_device_id*)malloc(num_devices * sizeof(cl_device_id));
            clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);
            for (unsigned int j = 0; j < num_devices; j++)
            {
                char tempDeviceName[32];
                cl_uint temp_cores = 0;
                clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &temp_cores, NULL);
                clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 32, tempDeviceName, NULL);
                std::cout << "Found device: " << tempDeviceName << ", cores: " << temp_cores << std::endl;

                // force intel
                std::string s1 = (std::string)tempDeviceName;
                if (s1.find("Intel") != std::string::npos && false)
                {
                    device = devices[j];
                    cores = temp_cores;
                    memcpy(deviceName, tempDeviceName, 32);
                    break;
                }

                if (temp_cores > cores)
                {
                    device = devices[j];
                    cores = temp_cores;
                    memcpy(deviceName, tempDeviceName, 32);
                }
            }
        }
    }

    std::cout << "Chosen device: " << deviceName << ", cores: " << cores << std::endl;

    size_t size_extensions = 0;
    char* extensions;
    clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, NULL, &size_extensions);
    extensions = (char*)malloc(size_extensions * sizeof(char));
    clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, size_extensions, extensions, NULL);
    std::cout << extensions << std::endl;
    char desired_extension[] = "cl_khr_fp64";
    bool use_fp64 = false;
    if (strstr(extensions, desired_extension))
    {
        std::cout << "Found extension: " << desired_extension << std::endl;
        use_fp64 = true;
    }
    else
    {
        std::cout << "Extension not found: " << desired_extension << std::endl;

    }


    cl_int errCode = 0;
    cl_context context = clCreateContext(NULL, 1, &device, nullptr, nullptr, &errCode);
    if (errCode != CL_SUCCESS)
    {
        std::cout << "Error creating context" << std::endl;

        switch (errCode)
        {
        case CL_INVALID_PLATFORM:
            std::cout << "Invalid platform" << std::endl;
            break;
        case CL_INVALID_VALUE:
            std::cout << "Invalid value" << std::endl;
            break;
        case CL_INVALID_DEVICE:
            std::cout << "Invalid device" << std::endl;
            break;
        case CL_DEVICE_NOT_AVAILABLE:
            std::cout << "Device unavailable" << std::endl;
            break;
        case CL_OUT_OF_HOST_MEMORY:
            std::cout << "Out of host memory" << std::endl;
            break;
        }
        return -1;
    }

    if (context == 0)
        return -1;

    cl_uint numDevices;
    clGetContextInfo(context, CL_CONTEXT_NUM_DEVICES, sizeof(size_t), &numDevices, nullptr);

    cl_device_id* contextDeviceId;
    contextDeviceId = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
    clGetContextInfo(context, CL_CONTEXT_DEVICES, numDevices * sizeof(cl_device_id), contextDeviceId, nullptr);
    char contextDeviceName[32];
    cl_int err = 0;
    if (numDevices > 0)
        err = clGetDeviceInfo(contextDeviceId[0], CL_DEVICE_NAME, 32, contextDeviceName, NULL);
    if (err != CL_SUCCESS)
    {
        switch (err)
        {
        case CL_INVALID_CONTEXT:
            std::cout << "Invalid context" << std::endl;
            break;
        case CL_INVALID_VALUE:
            std::cout << "Invalid value" << std::endl;
            break;
        case CL_OUT_OF_RESOURCES:
            std::cout << "Context out of resources" << std::endl;
            break;
        case CL_OUT_OF_HOST_MEMORY:
            std::cout << "Context out of host memory" << std::endl;
            break;
        }
    }
    std::cout << "Created context with " << numDevices << " devices: " << contextDeviceName << std::endl;

    FILE* program_handle;
    errno_t error = fopen_s(&program_handle, COMPLEX_MULTIPLICATION, "rb");

    fseek(program_handle, 0, SEEK_END);
    size_t program_size = ftell(program_handle);
    rewind(program_handle);

    if (program_size == 0)
    {
        std::cout << "Program size 0" << std::endl;
        return -1;
    }
    char* program_buffer = (char*)malloc(program_size + (long)1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);

    if (err != CL_SUCCESS)
    {
        switch (err)
        {
        case CL_INVALID_CONTEXT:
            std::cout << "Program creation failed - Invalid context" << std::endl;
            break;
        case CL_INVALID_VALUE:
            std::cout << "Program creation failed - Invalid value" << std::endl;
            break;
        case CL_OUT_OF_HOST_MEMORY:
            std::cout << "Program creation failed - out of host memory" << std::endl;
            break;
        }
        return -1;
    }
    std::cout << "Created program: " << COMPLEX_MULTIPLICATION << std::endl;

    char* options;
    if (use_fp64)
    {
        char option[] = "-DPF_64";
        options = (char*)malloc(sizeof(option));
        memcpy(options, option, sizeof(option));
    }
    cl_int built_program = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    if (built_program != CL_SUCCESS)
    {
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size * sizeof(char));
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        std::cout << "Error building program." << std::endl;
        std::cout << log << std::endl;
        return -1;
    }
    cl_uint num_kernels;
    clCreateKernelsInProgram(program, 0, NULL, &num_kernels);
    cl_kernel* kernels = (cl_kernel*)malloc(num_kernels * sizeof(cl_kernel));
    clCreateKernelsInProgram(program, num_kernels, kernels, NULL);

    cl_queue_properties queue_properties[] = { CL_QUEUE_PROPERTIES | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE };
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device, NULL, &err);

    if (err != CL_SUCCESS)
    {
        std::cout << "Failed to create command queue!" << std::endl;
        return -1;
    }

    cl_double2 real1_in = { -0.8, 0.156 };
    cl_double2 real2_in = { -(double)M_PI / 2, 5 };
    cl_double2 real_out = { 0,0 };
    cl_float2 freal_out = { 0,0 };

    cl_mem real1_in_buff;
    cl_mem real2_in_buff;
    cl_mem real_out_buff;
    use_fp64 = false;
    if (use_fp64)
    {
        real1_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(real1_in), &real1_in, &err);
        real2_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(real2_in), &real2_in, &err);
        real_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(real_out), &real_out, &err);
    }
    else
    {
        cl_float2 freal1_in = { (float)real1_in.x, (float)real1_in.y };
        cl_float2 freal2_in = { (float)real2_in.x, (float)real2_in.y };
        cl_float2 freal_out = { (float)real_out.x, (float)real_out.y };
        real1_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal1_in), &freal1_in, &err);
        real2_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal2_in), &freal2_in, &err);
        real_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal_out), &freal_out, &err);
    }

    err = clSetKernelArg(kernels[0], 0, sizeof(real1_in_buff), &real1_in_buff);
    err = clSetKernelArg(kernels[0], 1, sizeof(real2_in_buff), &real2_in_buff);
    err = clSetKernelArg(kernels[0], 2, sizeof(real_out_buff), &real_out_buff);

    cl_float2 freal_in = { (float)real1_in.x, (float)real1_in.y };
    cl_int exponent = 8;
    cl_float2 freal_out_p = { 0,0 };

    cl_mem real_in_buff;
    cl_mem exponent_buff;
    cl_mem real_out_p_buff;

    real_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal_in), &freal_in, &err);
    exponent_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(exponent), &exponent, &err);
    real_out_p_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, sizeof(freal_out_p), &freal_out_p, &err);

    err = clSetKernelArg(kernels[1], 0, sizeof(real_in_buff), &real_in_buff);
    err = clSetKernelArg(kernels[1], 1, sizeof(exponent_buff), &exponent_buff);
    err = clSetKernelArg(kernels[1], 2, sizeof(real_out_p_buff), &real_out_p_buff);

    cl_mem image;
    cl_image_format imageFormat = {};
    imageFormat.image_channel_order = CL_RGBA;
    imageFormat.image_channel_data_type = CL_UNSIGNED_INT8;
    cl_image_desc imageDesc = {};
    imageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
    imageDesc.image_width = WIDTH;
    imageDesc.image_height = HEIGHT;
    imageDesc.image_depth = 0;
    imageDesc.image_array_size = 0;
    imageDesc.image_row_pitch = 0;
    imageDesc.num_mip_levels = 0;
    imageDesc.num_samples = 0;
    imageDesc.buffer = 0;
    imageDesc.mem_object = 0;
    //unsigned char* imageData;

    *imageData = (unsigned char*)malloc(WIDTH * HEIGHT * 4);
    image = clCreateImage(context, CL_MEM_WRITE_ONLY, &imageFormat, &imageDesc, NULL, &err);

    cl_sampler_properties samplerProperties[] = { CL_SAMPLER_NORMALIZED_COORDS, CL_FALSE, CL_SAMPLER_ADDRESSING_MODE, CL_ADDRESS_CLAMP, CL_SAMPLER_FILTER_MODE, CL_FILTER_NEAREST,0 };


    cl_sampler sampler = clCreateSamplerWithProperties(context, samplerProperties, &err);

    cl_mem paletteImage;
    cl_image_format paletteImageFormat = {};
    paletteImageFormat.image_channel_order = CL_RGBA;
    paletteImageFormat.image_channel_data_type = CL_UNSIGNED_INT8;
    cl_image_desc paletteImageDesc = {};
    paletteImageDesc.image_type = CL_MEM_OBJECT_IMAGE1D;
    paletteImageDesc.image_width = COLORS;
    paletteImageDesc.image_height = 1;
    paletteImageDesc.image_depth = 0;
    paletteImageDesc.image_array_size = 0;
    paletteImageDesc.image_row_pitch = 0;
    paletteImageDesc.num_mip_levels = 0;
    paletteImageDesc.num_samples = 0;
    paletteImageDesc.buffer = 0;
    paletteImageDesc.mem_object = 0;
    unsigned char* paletteImageData;
    paletteImageData = (unsigned char*)malloc(COLORS * 4 * sizeof(unsigned char));
    //memcpy(paletteImageData, colors, COLORS * 4 * sizeof(unsigned char));
    paletteImage = clCreateImage(context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, &paletteImageFormat, &paletteImageDesc, colors, &err);
    cl_sampler paletteSampler = clCreateSamplerWithProperties(context, samplerProperties, &err);

    err = clSetKernelArg(kernels[2], 0, sizeof(image), &image);
    err = clSetKernelArg(kernels[2], 1, sizeof(paletteImage), &paletteImage);
    err = clSetKernelArg(kernels[2], 2, sizeof(paletteSampler), &paletteSampler);

    size_t global_work_size[] = { 1 };
    size_t local_work_size[] = { 1 };
    cl_event kernel_execution = 0;
    //err = clEnqueueNDRangeKernel(command_queue, kernels[0], 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    err = clEnqueueNDRangeKernel(command_queue, kernels[1], 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

    err = clEnqueueReadBuffer(command_queue, real_out_p_buff, CL_TRUE, 0, sizeof(freal_out_p), &freal_out_p, 0, NULL, NULL);

    auto startTime = high_resolution_clock::now();
    size_t pixel_global_work_size[] = { WIDTH, HEIGHT };
    err = clEnqueueNDRangeKernel(command_queue, kernels[2], 2, NULL, pixel_global_work_size, NULL, 0, NULL, NULL);
    //err = clEnqueueReadBuffer(command_queue, image, CL_TRUE, 0, sizeof(freal_out_p), &freal_out_p, 0, NULL, NULL);
    size_t origin[] = { 0, 0, 0 };
    size_t region[] = { WIDTH, HEIGHT, 1 };
    size_t size = WIDTH * 4;
    void* mapped_memory = clEnqueueMapImage(command_queue, image, CL_TRUE, CL_MAP_READ, origin, region, &size, 0, 0, nullptr, nullptr, &err);
    memcpy(*imageData, mapped_memory, WIDTH * HEIGHT * 4);
    char kernel_message[] = "Kernel finished\n";
    //err = clSetEventCallback(kernel_execution, CL_COMPLETE, &kernel_complete, kernel_message);
    if (err != CL_SUCCESS)
    {
        std::cout << "Failed to enqueue task" << std::endl;
        return -1;
    }
    std::cout << "Computing: (" << real1_in.x << "+" << real1_in.y << "i)^" << exponent << std::endl;
    /*
    if (use_fp64)
    {
        err = clEnqueueReadBuffer(command_queue, real_out_buff, CL_TRUE, 0, sizeof(real_out), &real_out, 0, NULL, NULL);
        err = clEnqueueReadBuffer(command_queue, img_out_buff, CL_TRUE, 0, sizeof(img_out), &img_out, 0, NULL, NULL);
        std::cout << "Result: " << real_out << "+" << img_out << "i" << std::endl;
    }
    else
    {
        err = clEnqueueReadBuffer(command_queue, real_out_buff, CL_TRUE, 0, sizeof(freal_out), &freal_out, 0, NULL, NULL);
        err = clEnqueueReadBuffer(command_queue, img_out_buff, CL_TRUE, 0, sizeof(fimg_out), &fimg_out, 0, NULL, NULL);
        std::cout << "Multiplication result: " << freal_out << "+" << fimg_out << "i" << std::endl;
    }*/


    std::cout << "Power result: " << freal_out_p.x << "+" << freal_out_p.y << "i" << std::endl;

    if (use_fp64)
    {
        std::complex<double> expected_result = std::complex<double>(real1_in.x, real1_in.y) * std::complex<double>(real2_in.x, real2_in.y);
        std::cout << "Expected result: " << expected_result.real() << "+" << expected_result.imag() << "i" << std::endl;

        std::complex<double> exponent_result = pow(std::complex<double>((double)freal_in.x, (double)freal_in.y), exponent);
        std::cout << "Expected power result: " << exponent_result.real() << "+" << exponent_result.imag() << "i" << std::endl;
    }
    else
    {
        std::complex<float> expected_result = std::complex<float>((float)real1_in.x, (float)real1_in.y) * std::complex<float>((float)real2_in.x, (float)real2_in.y);
        //std::cout << "Expected multiplication result: " << expected_result.real() << "+" << expected_result.imag() << "i" << std::endl;

        std::complex<double> exponent_result = complex_power(std::complex<float>(freal_in.x, freal_in.y), exponent);
        exponent_result = exponent_result = pow(std::complex<double>((double)freal_in.x, (double)freal_in.y), exponent);
        std::cout << "Expected power result: " << exponent_result.real() << "+" << exponent_result.imag() << "i" << std::endl;
    }

    auto now = system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream datetime;
    datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    string extension = ".png";
    string fileName = "pics/Mandelbrot_";
    fileName.append(datetime.str());
    //fileName.append(std::to_string(k).c_str());
    fileName.append(extension.c_str());
    std::replace(fileName.begin(), fileName.end(), ':', '_');
    //stbi_write_png(fileName.c_str(), WIDTH, HEIGHT, 4, *imageData, WIDTH * 4);

    clReleaseEvent(kernel_execution);

    err = clReleaseMemObject(real_in_buff);
    err = clReleaseMemObject(exponent_buff);
    err = clReleaseMemObject(real_out_p_buff);

    err = clReleaseMemObject(real1_in_buff);
    err = clReleaseMemObject(real2_in_buff);
    err = clReleaseMemObject(real_out_buff);

    for (unsigned int i = 0; i < num_kernels; i++)
    {
        clReleaseKernel(kernels[i]);
    }
    clReleaseCommandQueue(command_queue);
    clReleaseProgram(program);

    clReleaseContext(context);
    //clReleaseDevice(device);

   
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - startTime);

    cout << "Frame time: " << duration.count() << endl;
    cout << "FPS: " << 1000.0f / duration.count() << endl;
}

void cleanups()
{

}


int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        
        unsigned char* imageData;
        opencl_render(&imageData);

        /*
        auto now = system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream datetime;
        datetime << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

        string extension = ".png";
        string fileName = "pics/Mandelbrot_";
        fileName.append(datetime.str());
        //fileName.append(std::to_string(k).c_str());
        fileName.append(extension.c_str());
        std::replace(fileName.begin(), fileName.end(), ':', '_');
        stbi_write_png(fileName.c_str(), WIDTH, HEIGHT, 4, imageData, WIDTH * 4);
        */

        {
            GLuint image_texture;
            glGenTextures(1, &image_texture);
            glBindTexture(GL_TEXTURE_2D, image_texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

            // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
            //stbi_image_free(image_data);

            ImGui::Begin("OpenGL Texture Text");
            ImGui::Text("size = %d x %d", WIDTH, HEIGHT);
            ImGui::Image((void*)(intptr_t)image_texture, ImVec2(WIDTH, HEIGHT));
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}
