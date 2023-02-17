// opencl_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include "Device.h"

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

int main()
{
    Device device;
    device.initDevice();

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
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
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
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    device.runMandelbrot();

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, device.width, device.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, device.imageData);

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

        static float position[4] = { 0 };
        static double dPosition[4] = { 0 };
        {
            ImGui::Begin("Mandelbrot Render settings");
            static int res[2] = { WIDTH, HEIGHT };
            ImGui::InputInt2("Resolution", res);
            if (ImGui::Button("Set resolution"))
            {
                device.setResolution(res);
                device.runMandelbrot();
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, device.width, device.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, device.imageData);
            }

            
            position[0] = device.realPosition.x;
            position[1] = device.realPosition.y;
            position[2] = device.realPosition.z;
            position[3] = device.realPosition.w;

            dPosition[0] = device.drealPosition.x;
            dPosition[1] = device.drealPosition.y;
            dPosition[2] = device.drealPosition.z;
            dPosition[3] = device.drealPosition.w;

            ImGui::InputFloat4("Position", position);
            if (ImGui::Button("Set position"))
            {
                if (!device.use_fp64)
                    device.setPosition(position);
                else
                {
                    dPosition[0] = position[0];
                    dPosition[1] = position[1];
                    dPosition[2] = position[2];
                    dPosition[3] = position[3];
                    device.setPosition(dPosition);
                }
                device.runMandelbrot();
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, device.width, device.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, device.imageData);
                position[0] = device.position.x;
                position[1] = device.position.y;
                position[2] = device.position.z;
                position[3] = device.position.w;
            }
            static bool by_x = true;
            ImGui::Checkbox("Fix by X", &by_x);
            if (ImGui::Button("Fix ratio"))
            {
                device.fixRatio(by_x);
                device.runMandelbrot();
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, device.width, device.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, device.imageData);
            }
            const std::string zoomValue = "Zoom value x" + std::to_string(1/device.zoomValue);
            ImGui::Text(zoomValue.c_str());
            ImGui::End();
        }

        {
            ImGui::Begin("Mandelbrot Render", NULL, ImGuiWindowFlags_NoMove);
            ImGui::Image((void*)(intptr_t)image_texture, ImVec2(device.width, device.height));
            
            if (ImGui::IsItemHovered())
            {
                float wheel = ImGui::GetIO().MouseWheel;
                bool click = ImGui::IsMouseReleased(ImGuiMouseButton_Left);
                if (wheel)
                {
                    float zoom = wheel<0 ? 1.25 : 0.8;
                    device.zoom(zoom);

                    position[0] = device.realPosition.x;
                    position[1] = device.realPosition.y;
                    position[2] = device.realPosition.z;
                    position[3] = device.realPosition.w;
                    device.runMandelbrot();
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, device.width, device.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, device.imageData);
                }
                if (click)
                {
                    ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
                    ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
                    ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
                    int newPosition[2] = { mousePositionRelative.x, mousePositionRelative.y };
                    device.setMiddlePosition(newPosition);
                    position[0] = device.realPosition.x;
                    position[1] = device.realPosition.y;
                    position[2] = device.realPosition.z;
                    position[3] = device.realPosition.w;
                    device.runMandelbrot();
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, device.width, device.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, device.imageData);
                }
            }

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

    device.shutdownDevice();

    return 0;
}
