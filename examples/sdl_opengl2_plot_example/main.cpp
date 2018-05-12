// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the sdl_opengl3_example/ folder**
// See imgui_impl_sdl.cpp for details.

#include "imgui.h"
#include "imgui_impl_sdl_gl2.h"
#include "imgui_plot.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window* window = SDL_CreateWindow("ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplSdlGL2_Init(window);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdlGL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdlGL2_NewFrame(window);

        // 1. Scatter plot demo
        {
            // Data
            const float y_data1[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const float y_data2[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
            const float y_data3[] = {6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
            const float x_data[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

            const float r_data[] = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 6.0f};
            const char* labels[] = {"1", "2", "3", "4", "5", "6", "7"};
            const char* series[] = {"series1", "series2", "series3"};
            const int n_data = 7;

            // Data scales         
            ImGui::ImLinearScales<float, float> scales_x;   
            ImGui::ImLinearScales<float, float> scales_y;   
            scales_x.SetDomain(0.0f, 6.0f);
            scales_y.SetDomain(0.0f, 6.0f);

            // Color scales            
            ImGui::ImLinearScales<float, ImVec4> scales_color; 
            scales_color.SetDomain(0.0f, 2.0f); 
            scales_color.SetRange(ImVec4(255.0f, 0.0f, 0.0f, 255.0f), ImVec4(0.0f, 0.0f, 255.0f, 255.0f)); 

            // ImGui::SetNextWindowPos(ImVec2(0,0));
            // ImGui::SetNextWindowSize(io.DisplaySize);
            bool show_plot_test = true;
            ImGui::Begin("Scatter plot", &show_plot_test, NULL);

            // Figure
            ImGui::ImScatterPlot2D<float, float> plotter;
            plotter.SetScales(&scales_x, &scales_y);
            plotter.ShowFigure(ImVec2(720, 720), 
                100.0f, 100.0f, 100.0f, 100.0f,
                "Scatter Plot", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // Plot 1
            ImU32 hover_color = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            plotter.ShowPlot(x_data, y_data1, r_data, n_data, NULL, NULL, 
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)), hover_color, 
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)), 1.0f, 0.0f, 0.0f, "None",
                series[0], labels,
                io.FontDefault, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)), 12.0f,
                NULL, NULL, NULL, NULL);
            
            // Plot 2
            plotter.ShowPlot(x_data, y_data2, r_data, n_data, NULL, NULL, 
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)), hover_color, 
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)), 1.0f, 0.0f, 0.0f, "None",
                series[1], labels,
                io.FontDefault, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)), 12.0f,
                NULL, NULL, NULL, NULL);
            
            // Plot 3
            plotter.ShowPlot(x_data, y_data3, r_data, n_data, NULL, NULL,
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2)), hover_color, 
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2)), 1.0f, 0.0f, 0.0f, "None",
                series[2], labels,
                io.FontDefault, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)), 12.0f,
                NULL, NULL, NULL, NULL);

            // Axes
            plotter.ShowXAxisBottom(
                "x-axis bottom", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                10, NULL, "%.2f", 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            plotter.ShowYAxisLeft(
                "y-axis left", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                0.0f, 6.0f, 1.0f, "%.2f",
                NULL, NULL,
                2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // plotter.ShowXAxisTop(
            //     "x-axis top", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     10, NULL, "%.2f", 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // plotter.ShowYAxisRight(
            //     "y-axis right", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     10, NULL, "%.2f", 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // Legend
            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            plotter.ShowLegend("TR", ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)), 
                series, series_color, 3,
                io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            ImGui::End();
        }

        // 2. Bar plot demo
        {
            // Data
            const float y_data1[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const float y_data2[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
            const float y_data3[] = {6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
            const float x_data[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

            const char* labels[] = {"1", "2", "3", "4", "5", "6", "7"};
            const char* series[] = {"series1", "series2", "series3"};
            const int n_data = 7;

            float y_data1_bottoms[7], y_data2_bottoms[7], y_data3_bottoms[7];
            for (int n=0; n<n_data; ++n)
            {
                y_data1_bottoms[n] = 0;
                y_data2_bottoms[n] = y_data1[n];
                y_data3_bottoms[n] = y_data1[n] + y_data2[n];
            }

            // Data scales         
            ImGui::ImLinearScales<float, float> scales_x;   
            ImGui::ImLinearScales<float, float> scales_y;   
            scales_x.SetDomain(0.0f, 6.0f);
            scales_y.SetDomain(0.0f, 6.0f);

            // Color scales            
            ImGui::ImLinearScales<float, ImVec4> scales_color; 
            scales_color.SetDomain(0.0f, 2.0f); 
            scales_color.SetRange(ImVec4(255.0f, 0.0f, 0.0f, 255.0f), ImVec4(0.0f, 0.0f, 255.0f, 255.0f)); 
            
            // ImGui::SetNextWindowPos(ImVec2(0,0));
            // ImGui::SetNextWindowSize(io.DisplaySize);
            bool show_plot_test = true;
            ImGui::Begin("Bar plot", &show_plot_test, NULL);

            // Figure
            ImGui::ImBarPlot2D<float, float> plotter;
            plotter.SetScales(&scales_x, &scales_y);
            plotter.ShowFigure(ImVec2(720, 720), 
                100.0f, 100.0f, 100.0f, 100.0f,
                "Bar Plot", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // Plot 1
            ImU32 hover_color = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            plotter.ShowPlot(5.0f, y_data1, 
                10.0f, y_data1_bottoms, n_data,
                NULL, NULL,
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)), hover_color,
                series[0], labels,
                io.FontDefault, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)), 12.0f,
                NULL, NULL);
            
            // Plot 2
            
            // Plot 3

            // Axes
            plotter.ShowXAxisBottom(
                "x-axis bottom", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                n_data, labels, "%.2f", 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            plotter.ShowYAxisLeft(
                "y-axis left", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                NULL, NULL, NULL, "%.2f",
                x_data, labels, 
                2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
                io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // plotter.ShowXAxisTop(
            //     "x-axis top", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     10, NULL, "%.2f", 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // plotter.ShowYAxisRight(
            //     "y-axis right", io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     10, NULL, "%.2f", 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)),
            //     io.FontDefault, 12.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            // Legend
            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            plotter.ShowLegend("TR", ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)), 
                series, series_color, 3,
                io.FontDefault, 18.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));

            ImGui::End();
        }

        {
            bool show_metrics_window = true;
            ImGui::ShowMetricsWindow(&show_metrics_window);
        }

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui::Render();
        ImGui_ImplSdlGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplSdlGL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
