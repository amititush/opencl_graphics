// opencl_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <CL/cl.h>
#include <math.h>
#include <complex>

#define COMPLEX_MULTIPLICATION "complex_multiplication.cl"

int main()
{
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
    cl_int err = CL_INVALID_DEVICE;
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

    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device, NULL, &err);

    if (err != CL_SUCCESS)
    {
        std::cout << "Failed to create command queue!" << std::endl;
        return -1;
    }

    cl_double real1_in = (double)3 / M_PI;
    cl_double img1_in = -(double)pow(8, M_E);
    cl_double real2_in = -(double)M_PI / 2;
    cl_double img2_in = 5;
    cl_double real_out = 0;
    cl_double img_out = 0;
    cl_float freal_out = 0;
    cl_float fimg_out = 0;

    cl_mem real1_in_buff;
    cl_mem img1_in_buff;
    cl_mem real2_in_buff;
    cl_mem img2_in_buff;
    cl_mem real_out_buff;
    cl_mem img_out_buff;

    if (use_fp64)
    {
        real1_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(real1_in), &real1_in, &err);
        img1_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(img1_in), &img1_in, &err);
        real2_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(real2_in), &real2_in, &err);
        img2_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(img2_in), &img2_in, &err);
        real_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(real_out), &real_out, &err);
        img_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(img_out), &img_out, &err);
    }
    else
    {
        cl_float freal1_in = (float)real1_in;
        cl_float fimg1_in = (float)img1_in;
        cl_float freal2_in = (float)real2_in;
        cl_float fimg2_in = (float)img2_in;
        cl_float freal_out = (float)real_out;
        cl_float fimg_out = (float)img_out;
        real1_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal1_in), &freal1_in, &err);
        img1_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(fimg1_in), &fimg1_in, &err);
        real2_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal2_in), &freal2_in, &err);
        img2_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(fimg2_in), &fimg2_in, &err);
        real_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal_out), &freal_out, &err);
        img_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(fimg_out), &fimg_out, &err);
    }

    err = clSetKernelArg(kernels[0], 0, sizeof(real1_in_buff), &real1_in_buff);
    err = clSetKernelArg(kernels[0], 1, sizeof(img1_in_buff), &img1_in_buff);
    err = clSetKernelArg(kernels[0], 2, sizeof(real2_in_buff), &real2_in_buff);
    err = clSetKernelArg(kernels[0], 3, sizeof(img2_in_buff), &img2_in_buff);
    err = clSetKernelArg(kernels[0], 4, sizeof(real_out_buff), &real_out_buff);
    err = clSetKernelArg(kernels[0], 5, sizeof(img_out_buff), &img_out_buff);

    cl_float freal_in = (float)real1_in;
    cl_float fimg_in = (float)img1_in;
    cl_int exponent = 8;
    cl_float freal_out_p = 0;
    cl_float fimg_out_p = 0;

    cl_mem real_in_buff;
    cl_mem img_in_buff;
    cl_mem exponent_buff;
    cl_mem real_out_p_buff;
    cl_mem img_out_p_buff;

    real_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal_in), &freal_in, &err);
    img_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(fimg_in), &fimg_in, &err);
    img_in_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(exponent_buff), &exponent_buff, &err);
    real_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(freal_out_p), &freal_out_p, &err);
    img_out_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(fimg_out_p), &fimg_out_p, &err);

    size_t global_work_size[] = { 1 };
    size_t local_work_size[] = { 1 };
    err = clEnqueueNDRangeKernel(command_queue, kernels[0], 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    err = clEnqueueNDRangeKernel(command_queue, kernels[1], 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

    if (err != CL_SUCCESS)
    {
        std::cout << "Failed to enqueue task" << std::endl;
        return -1;
    }
    std::cout << "Computing: " << real1_in << "+" << img1_in << "i * " << real2_in << "+" << img2_in << "i" << std::endl;

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
        std::cout << "Result: " << freal_out << "+" << fimg_out << "i" << std::endl;
    }

    err = clEnqueueReadBuffer(command_queue, real_out_buff, CL_TRUE, 0, sizeof(freal_out_p), &freal_out_p, 0, NULL, NULL);
    err = clEnqueueReadBuffer(command_queue, img_out_buff, CL_TRUE, 0, sizeof(fimg_out_p), &fimg_out_p, 0, NULL, NULL);
    std::cout << "Result: " << freal_out << "+" << fimg_out << "i" << std::endl;

    if (use_fp64)
    {
        std::complex<double> expected_result = std::complex<double>(real1_in, img1_in) * std::complex<double>(real2_in, img2_in);
        std::cout << "Expected result: " << expected_result.real() << "+" << expected_result.imag() << "i" << std::endl;
    }
    else
    {
        std::complex<float> expected_result = std::complex<float>((float)real1_in, (float)img1_in) * std::complex<float>((float)real2_in, (float)img2_in);
        std::cout << "Expected result: " << expected_result.real() << "+" << expected_result.imag() << "i" << std::endl;
    }

    err = clReleaseMemObject(real1_in_buff);
    err = clReleaseMemObject(img1_in_buff);
    err = clReleaseMemObject(real2_in_buff);
    err = clReleaseMemObject(img2_in_buff);
    err = clReleaseMemObject(real_out_buff);
    err = clReleaseMemObject(img_out_buff);

    for (unsigned int i = 0; i < num_kernels; i++)
    {
        clReleaseKernel(kernels[i]);
    }
    clReleaseCommandQueue(command_queue);
    clReleaseProgram(program);

    clReleaseContext(context);
    //clReleaseDevice(device);


    return 0;
}
