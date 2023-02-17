#include "Device.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;
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

    r = 255 * fr;
    g = 255 * fg;
    b = 255 * fb;

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

    r = 255 * fr;
    g = 255 * fg;
    b = 255 * fb;

    return;
}

void PreComputeColors(unsigned char* colors, int colorCount, bool black)
{
    vector<unsigned char> color = { 0, 0, 0, 255 };
    for (int i = 0; i < colorCount; i++)
    {
        if (black)
            ColorMap(i, color[0], color[1], color[2], float(i) / colorCount);
        else
            ColorMapNoBlack(i, color[0], color[1], color[2], float(i) / colorCount);
        colors[i * 4] = color[0];
        colors[i * 4 + 1] = color[1];
        colors[i * 4 + 2] = color[2];
        colors[i * 4 + 3] = 255;
    }
}

int Device::initDevice()
{
    PreComputeColors(colors, COLORS, false);

    cl_platform_id* platforms;
    cl_uint numPlatforms = 0;
    clGetPlatformIDs(32, NULL, &numPlatforms);
    platforms = (cl_platform_id*)malloc(numPlatforms * sizeof(cl_platform_id));
    clGetPlatformIDs(numPlatforms, platforms, NULL);

    cl_uint cores = 0;
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
                    deviceId = devices[j];
                    cores = temp_cores;
                    memcpy(deviceName, tempDeviceName, 32);
                    break;
                }

                if (temp_cores > cores)
                {
                    deviceId = devices[j];
                    cores = temp_cores;
                    memcpy(deviceName, tempDeviceName, 32);
                }
            }
        }
    }

    std::cout << "Chosen device: " << deviceName << ", cores: " << cores << std::endl;

    size_t size_extensions = 0;
    char* extensions;
    clGetDeviceInfo(deviceId, CL_DEVICE_EXTENSIONS, 0, NULL, &size_extensions);
    extensions = (char*)malloc(size_extensions * sizeof(char));
    clGetDeviceInfo(deviceId, CL_DEVICE_EXTENSIONS, size_extensions, extensions, NULL);
    std::cout << extensions << std::endl;
    char desired_extension[] = "cl_khr_fp64";
    if (strstr(extensions, desired_extension))
    {
        std::cout << "Found extension: " << desired_extension << std::endl;
        use_fp64 = true;
    }
    else
    {
        std::cout << "Extension not found: " << desired_extension << std::endl;

    }
    //use_fp64 = false;

    cl_int err = 0;
    context = clCreateContext(NULL, 1, &deviceId, nullptr, nullptr, &err);
    if (err != CL_SUCCESS)
    {
        std::cout << "Error creating context" << std::endl;

        switch (err)
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

    program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);

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
        char option[] = "-DFP_64";
        options = (char*)malloc(sizeof(option));
        memcpy(options, option, sizeof(option));
    }

    if (!use_fp64)
        built_program = clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
    else
        built_program = clBuildProgram(program, 1, &deviceId, options, NULL, NULL);

    if (built_program != CL_SUCCESS)
    {
        size_t log_size;
        clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size * sizeof(char));
        clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        std::cout << "Error building program." << std::endl;
        std::cout << log << std::endl;
        return -1;
    }

    clCreateKernelsInProgram(program, 0, NULL, &num_kernels);
    kernels = (cl_kernel*)malloc(num_kernels * sizeof(cl_kernel));
    clCreateKernelsInProgram(program, num_kernels, kernels, NULL);

    cl_queue_properties queue_properties[] = { CL_QUEUE_PROPERTIES | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE };
    command_queue = clCreateCommandQueueWithProperties(context, deviceId, NULL, &err);

    if (err != CL_SUCCESS)
    {
        std::cout << "Failed to create command queue!" << std::endl;
        return -1;
    }

    cl_image_format imageFormat = {};
    imageFormat.image_channel_order = CL_RGBA;
    imageFormat.image_channel_data_type = CL_UNSIGNED_INT8;
    cl_image_desc imageDesc = {};
    imageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
    imageDesc.image_width = width;
    imageDesc.image_height = height;
    imageDesc.image_depth = 0;
    imageDesc.image_array_size = 0;
    imageDesc.image_row_pitch = 0;
    imageDesc.num_mip_levels = 0;
    imageDesc.num_samples = 0;
    imageDesc.buffer = 0;
    imageDesc.mem_object = 0;
    //unsigned char* imageData;

    imageData = (unsigned char*)malloc(width * height * 4);
    image = clCreateImage(context, CL_MEM_WRITE_ONLY, &imageFormat, &imageDesc, NULL, &err);

    cl_sampler_properties samplerProperties[] = { CL_SAMPLER_NORMALIZED_COORDS, CL_FALSE, CL_SAMPLER_ADDRESSING_MODE, CL_ADDRESS_CLAMP, CL_SAMPLER_FILTER_MODE, CL_FILTER_NEAREST,0 };

    cl_sampler sampler = clCreateSamplerWithProperties(context, samplerProperties, &err);

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
    paletteImageData = (unsigned char*)malloc(COLORS * 4 * sizeof(unsigned char));
    //memcpy(paletteImageData, colors, COLORS * 4 * sizeof(unsigned char));
    paletteImage = clCreateImage(context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, &paletteImageFormat, &paletteImageDesc, colors, &err);
    paletteSampler = clCreateSamplerWithProperties(context, samplerProperties, &err);

    if (!use_fp64)
    {
        positionBuffer = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(cl_float4), &position, &err);

        err = clSetKernelArg(kernels[2], 0, sizeof(image), &image);
        err = clSetKernelArg(kernels[2], 1, sizeof(paletteImage), &paletteImage);
        err = clSetKernelArg(kernels[2], 2, sizeof(paletteSampler), &paletteSampler);
        err = clSetKernelArg(kernels[2], 3, sizeof(positionBuffer), &positionBuffer);
    }
    else
    {
        positionBuffer = clCreateBuffer(context, CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, sizeof(cl_double4), &dPosition, &err);
        err = clSetKernelArg(kernels[3], 0, sizeof(image), &image);
        err = clSetKernelArg(kernels[3], 1, sizeof(paletteImage), &paletteImage);
        err = clSetKernelArg(kernels[3], 2, sizeof(paletteSampler), &paletteSampler);
        err = clSetKernelArg(kernels[3], 3, sizeof(positionBuffer), &positionBuffer);
    }
}

int Device::shutdownDevice()
{
    for (unsigned int i = 0; i < num_kernels; i++)
    {
        clReleaseKernel(kernels[i]);
    }
    clReleaseCommandQueue(command_queue);
    clReleaseProgram(program);

    clReleaseContext(context);

    return 0;
}

int Device::runMandelbrot()
{
    size_t global_work_size[] = { 1 };
    size_t local_work_size[] = { 1 };

    auto startTime = high_resolution_clock::now();
    size_t pixel_global_work_size[] = { width, height };
    cl_int err;
    if (!use_fp64)
        err = clEnqueueNDRangeKernel(command_queue, kernels[2], 2, NULL, pixel_global_work_size, NULL, 0, NULL, NULL);
    else
        err = clEnqueueNDRangeKernel(command_queue, kernels[3], 2, NULL, pixel_global_work_size, NULL, 0, NULL, NULL);
    //err = clEnqueueReadBuffer(command_queue, image, CL_TRUE, 0, sizeof(freal_out_p), &freal_out_p, 0, NULL, NULL);
    size_t origin[] = { 0, 0, 0 };
    size_t region[] = { width, height, 1 };
    size_t size = width * 4;
    void* mapped_memory = clEnqueueMapImage(command_queue, image, CL_TRUE, CL_MAP_READ, origin, region, &size, 0, 0, nullptr, nullptr, &err);
    memcpy(imageData, mapped_memory, width * height * 4);
    clEnqueueUnmapMemObject(command_queue, image, mapped_memory, 0, NULL, NULL);
    //err = clSetEventCallback(kernel_execution, CL_COMPLETE, &kernel_complete, kernel_message);
    if (err != CL_SUCCESS)
    {
        std::cout << "Failed to enqueue task" << std::endl;
        return -1;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - startTime);

    std::cout << "Position: (" << position.x << ", " << position.z << "), (" << position.y << ", " << position.w << ")" << std::endl;

    cout << "Frame time: " << duration.count() << endl;
    //cout << "FPS: " << 1000.0f / duration.count() << endl;

    return 0;
}

void Device::updatePosition()
{
    if (!use_fp64)
    {
        void* mapped_memory = clEnqueueMapBuffer(command_queue, positionBuffer, CL_TRUE, CL_MAP_WRITE, 0, sizeof(cl_float4), NULL, NULL, NULL, NULL);
        memcpy(mapped_memory, &position, sizeof(cl_float4));
        clEnqueueUnmapMemObject(command_queue, positionBuffer, mapped_memory, 0, NULL, NULL);
    }
    else
    {
        void* mapped_memory = clEnqueueMapBuffer(command_queue, positionBuffer, CL_TRUE, CL_MAP_WRITE, 0, sizeof(cl_double4), NULL, NULL, NULL, NULL);
        memcpy(mapped_memory, &dPosition, sizeof(cl_double4));
        clEnqueueUnmapMemObject(command_queue, positionBuffer, mapped_memory, 0, NULL, NULL);
    }
}

int Device::saveImage()
{
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
    stbi_write_png(fileName.c_str(), width, height, 4, imageData, width * 4);

    return 0;
}

void Device::restartDevice()
{
    shutdownDevice();
    initDevice();
}

void Device::setResolution(const int res[2])
{
    width = res[0];
    height = res[1];

    restartDevice();
}

void Device::setPosition(const float nPosition[4])
{
    position.x = nPosition[0];
    position.y = nPosition[1];
    position.z = nPosition[2];
    position.w = nPosition[3];

    updatePosition();
}

void Device::setPosition(const double nPosition[4])
{
    dPosition.x = nPosition[0];
    dPosition.y = nPosition[1];
    dPosition.z = nPosition[2];
    dPosition.w = nPosition[3];

    updatePosition();
}

void Device::zoom(float nzoomValue)
{
    if (!use_fp64)
    {
        //zoomValue = 1;
        float pixel_width = abs(position.y - position.x);
        float pixel_height = abs(position.w - position.z);
        float middleX = (position.x + position.y) / 2;
        float middleY = (position.z + position.w) / 2;

        position.x = middleX - pixel_width / 2 * nzoomValue;
        position.y = middleX + pixel_width / 2 * nzoomValue;

        position.z = middleY - pixel_height / 2 * nzoomValue;
        position.w = middleY + pixel_height / 2 * nzoomValue;
        realPosition = position;
    }
    else
    {
        double pixel_width = abs(dPosition.y - dPosition.x);
        double pixel_height = abs(dPosition.w - dPosition.z);
        double middleX = (dPosition.x + dPosition.y) / 2;
        double middleY = (dPosition.z + dPosition.w) / 2;

        dPosition.x = middleX - pixel_width / 2 * nzoomValue;
        dPosition.y = middleX + pixel_width / 2 * nzoomValue;

        dPosition.z = middleY - pixel_height / 2 * nzoomValue;
        dPosition.w = middleY + pixel_height / 2 * nzoomValue;
        drealPosition = dPosition;
    }
    zoomValue *= nzoomValue;
    updatePosition();
}

void Device::fixRatio(bool by_x)
{
    if (!use_fp64)
    {
        prevPosition = position;
        float ratio = (float)width / height;

        if (by_x)
        {
            float pixel_width = abs(position.w - position.z) * ratio;
            float middlePoint = (position.x + position.y) / 2;
            position.x = middlePoint - pixel_width / 2;
            position.y = middlePoint + pixel_width / 2;
        }
        else {
            float pixel_width = (position.y - position.x) / ratio;
            float middlePoint = (position.z + position.w) / 2;
            position.z = middlePoint - pixel_width / 2;
            position.w = middlePoint + pixel_width / 2;
        }
        realPosition = position;
        updatePosition();
        position = prevPosition;
    }
    else
    {
        double ratio = (double)width / height;

        if (by_x)
        {
            double pixel_width = abs(dPosition.w - dPosition.z) * ratio;
            double middlePoint = (dPosition.x + dPosition.y) / 2;
            dPosition.x = middlePoint - pixel_width / 2;
            dPosition.y = middlePoint + pixel_width / 2;
        }
        else {
            double pixel_width = (dPosition.y - dPosition.x) / ratio;
            double middlePoint = (dPosition.z + dPosition.w) / 2;
            dPosition.z = middlePoint - pixel_width / 2;
            dPosition.w = middlePoint + pixel_width / 2;
        }
        drealPosition = dPosition;
        updatePosition();
    }
}


void Device::setMiddlePosition(const int nPosition[2])
{
    if (!use_fp64)
    {
        float positionX = position.x + (float)nPosition[0] / width * (position.y - position.x);
        float positionY = position.z + (float)nPosition[1] / height * (position.w - position.z);

        float ratio = (float)width / height;
        float pixel_height = abs(position.w - position.z);
        float pixel_width = abs(position.y - position.x);

        position.x = positionX - pixel_width / 2;
        position.y = positionX + pixel_width / 2;

        position.z = positionY - pixel_height / 2;
        position.w = positionY + pixel_height / 2;

        float middleX = position.x + nPosition[0] * pixel_width;
        float middleY = position.z + nPosition[1] * pixel_height;

        //position.x = middleX - pixel_width / 2;
        //position.y = middleX + pixel_width / 2;
        //
        //position.z = middleY - pixel_height / 2;
        //position.w = middleY + pixel_height / 2;
        realPosition = position;
    }
    else
    {
        double positionX = dPosition.x + (double)nPosition[0] / width * (dPosition.y - dPosition.x);
        double positionY = dPosition.z + (double)nPosition[1] / height * (dPosition.w - dPosition.z);

        double ratio = (double)width / height;
        double pixel_height = abs(dPosition.w - dPosition.z);
        double pixel_width = abs(dPosition.y - dPosition.x);

        dPosition.x = positionX - pixel_width / 2;
        dPosition.y = positionX + pixel_width / 2;

        dPosition.z = positionY - pixel_height / 2;
        dPosition.w = positionY + pixel_height / 2;

        float middleX = dPosition.x + nPosition[0] * pixel_width;
        float middleY = dPosition.z + nPosition[1] * pixel_height;

        //position.x = middleX - pixel_width / 2;
        //position.y = middleX + pixel_width / 2;
        //
        //position.z = middleY - pixel_height / 2;
        //position.w = middleY + pixel_height / 2;
        drealPosition = dPosition;
    }

    fixRatio(true);

}
