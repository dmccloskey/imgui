// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the sdl_opengl3_example/ folder**
// See imgui_impl_sdl.cpp for details.

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
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

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

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
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }

        // Start the ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

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
            ImGui::Begin("Scatter plot", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(720, 720);
            figure_properties.margin_bottom = 100.0f;
            figure_properties.margin_top = 100.0f;
            figure_properties.margin_left = 100.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "Scatter Plot";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Markers 1-3
            ImU32 hover_color = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            ImGui::ImMarkerProperties marker_properties;
            marker_properties.marker_hovered_col = hover_color;
            marker_properties.marker_stroke_width = 1.0f;
            ImGui::ImMarkers<float, float> Markers;

            marker_properties.marker_stroke_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            marker_properties.marker_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            Markers.SetProperties(marker_properties);
            Markers.DrawMarkers(Figure, x_data, y_data1, r_data, n_data, series[0]);
            marker_properties.marker_stroke_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            marker_properties.marker_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            Markers.SetProperties(marker_properties);
            Markers.DrawMarkers(Figure, x_data, y_data2, r_data, n_data, series[1]);
            marker_properties.marker_stroke_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            marker_properties.marker_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            Markers.SetProperties(marker_properties);
            Markers.DrawMarkers(Figure, x_data, y_data3, r_data, n_data, series[2]);

            // Labels 1-3
            ImGui::ImLabelProperties label_properties;
            label_properties.label_font = io.FontDefault;
            label_properties.label_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));  ///< Label font color
            label_properties.label_font_size = 12.0f;
            label_properties.label_offset_pos = ImVec2(2.0f, 2.0f);

            ImGui::ImLabels<float, float> Labels;
            Labels.SetProperties(label_properties);
            Labels.DrawLabels(Figure, x_data, y_data1, n_data, labels);
            Labels.DrawLabels(Figure, x_data, y_data2, n_data, labels);
            Labels.DrawLabels(Figure, x_data, y_data3, n_data, labels);

            // Lines 1-3
            ImGui::ImLineProperties line_properties;
            line_properties.line_stroke_width = 1.5f;
            line_properties.line_stroke_dash = 0.0f;
            line_properties.line_stroke_gap = 0.0f;
            line_properties.line_interp = "None";
            ImGui::ImLines<float, float> Lines;

            line_properties.line_stroke_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            Lines.SetProperties(line_properties);
            Lines.DrawLines(Figure, x_data, y_data1, n_data);
            line_properties.line_stroke_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            Lines.SetProperties(line_properties);
            Lines.DrawLines(Figure, x_data, y_data2, n_data);
            line_properties.line_stroke_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            Lines.SetProperties(line_properties);
            Lines.DrawLines(Figure, x_data, y_data3, n_data);

            // Axes
            ImGui::ImAxisProperties axis_properties;
            axis_properties.axis_title = "x-axis bottom";
            axis_properties.axis_title_font = io.FontDefault;
            axis_properties.axis_title_font_size = 18.0f;
            axis_properties.axis_title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_format = "%4.2f";
            axis_properties.axis_thickness = 2.0f;
            axis_properties.axis_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_font = io.FontDefault;
            axis_properties.axis_tick_font_size = 12.0f;
            axis_properties.axis_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.grid_lines_thickness = 0.5f;
            axis_properties.grid_lines_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImAxis<float, float> Axis;
            Axis.SetProperties(axis_properties);
            Axis.DrawXAxis(Figure, "Bottom", 0.0f, 6.0f, 1.0f);
            Axis.DrawYAxis(Figure, "Left", 0.0f, 6.0f, 1.0f);
            Axis.DrawXGridLines(Figure, 0.0f, 6.0f, 1.0f);
            Axis.DrawYGridLines(Figure, 0.0f, 6.0f, 1.0f);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.stroke_width = 1.0f;
            legend_properties.fill_col = 0;
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", series, series_color, 3);

            ImGui::End();
        }

        // 2. Bar plot demo (staggered)
        {
            // Data
            const float y_data1[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const float y_data2[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
            const float y_data3[] = {6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
            const float x_data[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

            const float dy_data1_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data2_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data3_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data_l[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

            const char* labels[] = {"1", "2", "3", "4", "5", "6", "7"};
            const char* series[] = {"series1", "series2", "series3"};
            const int n_data = 7;

            float y_data1_bottoms[7];
            for (int n=0; n<n_data; ++n)
            {
                y_data1_bottoms[n] = 0.0f; // bottom positions
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
            ImGui::Begin("Bar plot (staggered)", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(720, 720);
            figure_properties.margin_bottom = 100.0f;
            figure_properties.margin_top = 100.0f;
            figure_properties.margin_left = 100.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "Bar Plot";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Bars 1-3
            ImGui::ImBarProperties bar_properties;
            bar_properties.bar_width = 10.0f;
            bar_properties.bar_stroke_col = 0;
            bar_properties.bar_stroke_width = 1.0f;

            ImGui::ImBars<float, float> Bars;

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBars(Figure, y_data1, n_data, 0*bar_properties.bar_width, y_data1_bottoms, series[0]);

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBars(Figure, y_data2, n_data, 1*bar_properties.bar_width, y_data1_bottoms, series[1]);

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBars(Figure, y_data3, n_data, 2*bar_properties.bar_width, y_data1_bottoms, series[2]);

            // Error bars 1-3
            ImGui::ImErrorBarProperties error_properties;
            error_properties.error_bar_stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            error_properties.error_bar_stroke_width = 1.0f;
            error_properties.error_bar_cap_style = "Straight";
            error_properties.error_bar_cap_width = 8.0f;

            ImGui::ImErrorBars<float, float> ErrorBars;
            ErrorBars.SetProperties(error_properties);
            ErrorBars.DrawErrorBarsY(Figure, x_data, y_data1, n_data, dy_data1_h, dy_data_l,
                0*bar_properties.bar_width + 0.5*bar_properties.bar_width, y_data1_bottoms);
            ErrorBars.DrawErrorBarsY(Figure, x_data, y_data2, n_data, dy_data2_h, dy_data_l,
                1*bar_properties.bar_width + 0.5*bar_properties.bar_width, y_data1_bottoms);
            ErrorBars.DrawErrorBarsY(Figure, x_data, y_data3, n_data, dy_data3_h, dy_data_l,
                2*bar_properties.bar_width + 0.5*bar_properties.bar_width, y_data1_bottoms);

            // Axes
            ImGui::ImAxisProperties axis_properties;
            axis_properties.axis_title = "x-axis bottom";
            axis_properties.axis_title_font = io.FontDefault;
            axis_properties.axis_title_font_size = 18.0f;
            axis_properties.axis_title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_thickness = 2.0f;
            axis_properties.axis_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_font = io.FontDefault;
            axis_properties.axis_tick_font_size = 12.0f;
            axis_properties.axis_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImAxis<float, float> Axis;
            axis_properties.axis_tick_format = "%s";
            Axis.SetProperties(axis_properties);
            Axis.DrawXAxis(Figure, "Bottom", x_data, labels, n_data);
            axis_properties.axis_tick_format = "%4.2f";
            Axis.SetProperties(axis_properties);
            Axis.DrawYAxis(Figure, "Left", 0.0f, 6.0f, 1.0f);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.stroke_width = 1.0f;
            legend_properties.fill_col = 0;
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", series, series_color, 3);

            ImGui::End();
        }

        // 2b. BarH plot demo (staggered)
        {
            // Data
            const float y_data1[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const float y_data2[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
            const float y_data3[] = {6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
            const float x_data[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

            const float dy_data1_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data2_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data3_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data_l[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

            const char* labels[] = {"1", "2", "3", "4", "5", "6", "7"};
            const char* series[] = {"series1", "series2", "series3"};
            const int n_data = 7;

            float y_data1_bottoms[7];
            for (int n=0; n<n_data; ++n)
            {
                y_data1_bottoms[n] = 0.0f; // bottom positions
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
            ImGui::Begin("BarH plot (staggered)", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(720, 720);
            figure_properties.margin_bottom = 100.0f;
            figure_properties.margin_top = 100.0f;
            figure_properties.margin_left = 100.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "BarH Plot";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Bars 1-3
            ImGui::ImBarProperties bar_properties;
            bar_properties.bar_width = 10.0f;
            bar_properties.bar_stroke_col = 0;
            bar_properties.bar_stroke_width = 1.0f;

            ImGui::ImBars<float, float> Bars;

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBarsH(Figure, y_data1, n_data, 0*bar_properties.bar_width, y_data1_bottoms, series[0]);

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBarsH(Figure, y_data2, n_data, 1*bar_properties.bar_width, y_data1_bottoms, series[1]);

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBarsH(Figure, y_data3, n_data, 2*bar_properties.bar_width, y_data1_bottoms, series[2]);

            // Error bars 1-3
            ImGui::ImErrorBarProperties error_properties;
            error_properties.error_bar_stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            error_properties.error_bar_stroke_width = 1.0f;
            error_properties.error_bar_cap_style = "Straight";
            error_properties.error_bar_cap_width = 8.0f;

            ImGui::ImErrorBars<float, float> ErrorBars;
            ErrorBars.SetProperties(error_properties);
            ErrorBars.DrawErrorBarsX(Figure, y_data1, x_data, n_data, dy_data1_h, dy_data_l,
                0*bar_properties.bar_width + 0.5*bar_properties.bar_width, y_data1_bottoms);
            ErrorBars.DrawErrorBarsX(Figure, y_data2, x_data, n_data, dy_data2_h, dy_data_l,
                1*bar_properties.bar_width + 0.5*bar_properties.bar_width, y_data1_bottoms);
            ErrorBars.DrawErrorBarsX(Figure, y_data3, x_data, n_data, dy_data3_h, dy_data_l,
                2*bar_properties.bar_width + 0.5*bar_properties.bar_width, y_data1_bottoms);

            // Axes
            ImGui::ImAxisProperties axis_properties;
            axis_properties.axis_title = "x-axis bottom";
            axis_properties.axis_title_font = io.FontDefault;
            axis_properties.axis_title_font_size = 18.0f;
            axis_properties.axis_title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_thickness = 2.0f;
            axis_properties.axis_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_font = io.FontDefault;
            axis_properties.axis_tick_font_size = 12.0f;
            axis_properties.axis_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImAxis<float, float> Axis;
            axis_properties.axis_tick_format = "%s";
            Axis.SetProperties(axis_properties);
            Axis.DrawYAxis(Figure, "Left", x_data, labels, n_data);
            axis_properties.axis_tick_format = "%4.2f";
            Axis.SetProperties(axis_properties);
            Axis.DrawXAxis(Figure, "Top", 0.0f, 6.0f, 1.0f);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.stroke_width = 1.0f;
            legend_properties.fill_col = 0;
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", series, series_color, 3);

            ImGui::End();
        }

        // 3. Bar plot demo (stacked)
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
            float y_max = 0;
            for (int n=0; n<n_data; ++n)
            {
                y_data1_bottoms[n] = 0.0f;
                y_data2_bottoms[n] = y_data1[n];
                y_data3_bottoms[n] = y_data1[n] + y_data2[n];
                const float y_sum = y_data1[n] + y_data2[n] + y_data3[n];
                if (y_sum > y_max) y_max = y_sum;
            }

            // Data scales
            ImGui::ImLinearScales<float, float> scales_x;
            ImGui::ImLinearScales<float, float> scales_y;
            scales_x.SetDomain(0.0f, 6.0f);
            scales_y.SetDomain(0.0f, y_max);

            // Color scales
            ImGui::ImLinearScales<float, ImVec4> scales_color;
            scales_color.SetDomain(0.0f, 2.0f);
            scales_color.SetRange(ImVec4(255.0f, 0.0f, 0.0f, 255.0f), ImVec4(0.0f, 0.0f, 255.0f, 255.0f));

            // ImGui::SetNextWindowPos(ImVec2(0,0));
            // ImGui::SetNextWindowSize(io.DisplaySize);
            bool show_plot_test = true;
            ImGui::Begin("Bar plot (stacked)", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(720, 720);
            figure_properties.margin_bottom = 100.0f;
            figure_properties.margin_top = 100.0f;
            figure_properties.margin_left = 100.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "Bar Plot";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Bars 1-3
            ImGui::ImBarProperties bar_properties;
            bar_properties.bar_width = 20.0f;
            bar_properties.bar_stroke_col = 0;
            bar_properties.bar_stroke_width = 1.0f;

            ImGui::ImBars<float, float> Bars;

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBars(Figure, y_data1, n_data, 0.0f, y_data1_bottoms, series[0]);

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBars(Figure, y_data2, n_data, 0.0f, y_data2_bottoms, series[1]);

            bar_properties.bar_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            bar_properties.bar_hovered_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            Bars.SetProperties(bar_properties);
            Bars.DrawBars(Figure, y_data3, n_data, 0.0f, y_data3_bottoms, series[2]);

            // Axes
            ImGui::ImAxisProperties axis_properties;
            axis_properties.axis_title = "x-axis bottom";
            axis_properties.axis_title_font = io.FontDefault;
            axis_properties.axis_title_font_size = 18.0f;
            axis_properties.axis_title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_thickness = 2.0f;
            axis_properties.axis_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_font = io.FontDefault;
            axis_properties.axis_tick_font_size = 12.0f;
            axis_properties.axis_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImAxis<float, float> Axis;
            axis_properties.axis_tick_format = "%s";
            Axis.SetProperties(axis_properties);
            Axis.DrawXAxis(Figure, "Bottom", x_data, labels, n_data);
            axis_properties.axis_tick_format = "%4.2f";
            Axis.SetProperties(axis_properties);
            Axis.DrawYAxis(Figure, "Left", 0.0f, y_max, 1.0f);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.stroke_width = 1.0f;
            legend_properties.fill_col = 0;
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", series, series_color, 3);

            ImGui::End();
        }

        // 4. Pie chart demo
        {
            // Data
            const float x_data[] = {45.0f, 15.0f, 20.0f, 5.0f, 5.0f, 8.0f, 2.0f};

            const char* labels[] = {"1", "2", "3", "4", "5", "6", "7"};
            const char* series[] = {"series1", "series2", "series3"};
            const int n_data = 7;

            // Data scales
            ImGui::ImLinearScales<float, float> scales_x;
            ImGui::ImLinearScales<float, float> scales_y;
            // scales_x.SetDomain(0.0f, 6.0f);
            // scales_y.SetDomain(0.0f, 0.0f);

            // Color scales
            ImGui::ImLinearScales<float, ImVec4> scales_color;
            scales_color.SetDomain(0.0f, 7.0f);
            scales_color.SetRange(ImVec4(255.0f, 0.0f, 0.0f, 255.0f), ImVec4(0.0f, 0.0f, 255.0f, 255.0f));

            // ImVec4 series_color_vec4[n_data] = {
            //     ImVec4(0.0f, 0.0f, 255.0f, 0.0f), // yellow
            //     // ImVec4(255.0f, 255.0f, 0.0f, 0.0f), // blue
            //     ImVec4(255.0f, 0.0f, 255.0f, 0.0f), // green
            //     ImVec4(255.0f, 0.0f, 0.0f, 255.0f), // red
            //     ImVec4(0.0f, 0.0f, 255.0f, 255.0f),  // blue
            //     ImVec4(0.0f, 255.0f, 0.0f, 0.0f), // magenta
            //     ImVec4(255.0f, 0.0f, 0.0f, 0.0f), // cyan
            //     ImVec4(0.0f, 89.25f, 255.0f, 0.0f) // orange
            // };
            ImU32 series_color[n_data];
            for (int i=0; i<n_data; ++i)
                series_color[i] = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(float(i)));
                // series_color[i] = ImGui::ColorConvertFloat4ToU32(series_color_vec4[i]);

            // ImGui::SetNextWindowPos(ImVec2(0,0));
            // ImGui::SetNextWindowSize(io.DisplaySize);
            bool show_plot_test = true;
            ImGui::Begin("Pie chart", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(720, 720);
            figure_properties.margin_bottom = 100.0f;
            figure_properties.margin_top = 100.0f;
            figure_properties.margin_left = 100.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "Pie chart";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Pie 1
            ImGui::ImPieProperties pie_properties;
            pie_properties.inner_radius = 10.0f;
            pie_properties.outer_radius = 100.0f;
            pie_properties.pie_stroke_col = 0;
            pie_properties.pie_stroke_width = 1.0f;
            pie_properties.pie_hovered_col = 0;
            pie_properties.pie_segments = 256;

            ImGui::ImPie<float, float> Pie;

            Pie.SetProperties(pie_properties);
            Pie.DrawPie(Figure, x_data, series_color, n_data);
            // Pie.DrawPie(Figure, x_data, series_color, 1);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.stroke_width = 1.0f;
            legend_properties.fill_col = 0;
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", labels, series_color, n_data);

            ImGui::End();
        }

        // 5a. Area plot demo
        {
            // Data
            const float y_data1[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const float y_data2[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
            const float y_data3[] = {6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
            const float x_data[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

            const float dy_data1_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data2_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data3_h[] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
            const float dy_data_l[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

            const char* labels[] = {"1", "2", "3", "4", "5", "6", "7"};
            const char* series[] = {"series1", "series2", "series3"};

            const size_t n_data = 7;
            float y_data_bottoms[n_data] = {0.0f};

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
            ImGui::Begin("Area plot", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(500, 300);
            figure_properties.margin_bottom = 50.0f;
            figure_properties.margin_top = 50.0f;
            figure_properties.margin_left = 50.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "Area Plot";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Areas 1-3
            ImGui::ImAreaProperties area_properties;

            ImGui::ImArea<float, float> Areas;

            area_properties.area_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            // area_properties.hover_color = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            area_properties.tooltip = "series 1";
            Areas.SetProperties(area_properties);
            Areas.DrawArea(Figure, x_data, y_data1, n_data);

            area_properties.area_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            // area_properties.hover_color = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            area_properties.tooltip = "series 2";
            Areas.SetProperties(area_properties);
            Areas.DrawArea(Figure, x_data, y_data2, n_data);

            area_properties.area_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            // area_properties.hover_color = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 0.0f, 0.0f, 255.0f));
            area_properties.tooltip = "series 3";
            Areas.SetProperties(area_properties);
            Areas.DrawArea(Figure, x_data, y_data3, n_data);

            // Axes
            ImGui::ImAxisProperties axis_properties;
            axis_properties.axis_title = "x-axis bottom";
            axis_properties.axis_title_font = io.FontDefault;
            axis_properties.axis_title_font_size = 18.0f;
            axis_properties.axis_title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_thickness = 2.0f;
            axis_properties.axis_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_font = io.FontDefault;
            axis_properties.axis_tick_font_size = 12.0f;
            axis_properties.axis_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImAxis<float, float> Axis;
            axis_properties.axis_tick_format = "%s";
            Axis.SetProperties(axis_properties);
            Axis.DrawXAxis(Figure, "Bottom", x_data, labels, n_data);
            axis_properties.axis_tick_format = "%2.1f";
            Axis.SetProperties(axis_properties);
            Axis.DrawYAxis(Figure, "Left", 0.0f, 6.0f, 1.0f);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.stroke_width = 1.0f;
            legend_properties.fill_col = 0;
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", series, series_color, 3);

            ImGui::End();
        }

        // 5b. Area plot (stacked) demo
        {
            // Data
            const float y_data1[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const float y_data2[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
            const float y_data3[] = {6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f};
            const float x_data[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
            const char* series[] = {"series1", "series2", "series3"};
            const size_t n_data = 7;
            float y_data_bottoms[n_data] = {0.0f};

            // Data scales
            ImGui::ImLinearScales<float, float> scales_x;
            ImGui::ImLinearScales<float, float> scales_y;
            scales_x.SetDomain(0.0f, 6.0f);
            scales_y.SetDomain(0.0f, 8.0f);

            // Color scales
            ImGui::ImLinearScales<float, ImVec4> scales_color;
            scales_color.SetDomain(0.0f, 3.0f);
            scales_color.SetRange(ImVec4(255.0f, 0.0f, 0.0f, 255.0f), ImVec4(0.0f, 0.0f, 255.0f, 255.0f));

            // ImGui::SetNextWindowPos(ImVec2(0,0));
            // ImGui::SetNextWindowSize(io.DisplaySize);
            bool show_plot_test = true;
            ImGui::Begin("Area (stacked) plot", &show_plot_test);

            // Figure
            ImGui::ImPlotProperties figure_properties;
            figure_properties.plot_size = ImVec2(500, 300);
            figure_properties.margin_bottom = 50.0f;
            figure_properties.margin_top = 50.0f;
            figure_properties.margin_left = 50.0f;
            figure_properties.margin_right = 100.0f;
            figure_properties.title = "Area (stacked) Plot";
            figure_properties.title_font = io.FontDefault;
            figure_properties.title_font_size = 18.0f;
            figure_properties.title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImPlot<float, float> Figure;
            Figure.SetProperties(figure_properties);
            Figure.SetScales(&scales_x, &scales_y);
            Figure.DrawFigure();

            // Areas 1-3
            ImGui::ImAreaProperties area_properties;

            ImGui::ImArea<float, float> Areas;

            area_properties.area_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0));
            area_properties.area_hover_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(3));
            area_properties.tooltip = "series 1";
            Areas.SetProperties(area_properties);
            Areas.DrawArea(Figure, x_data, y_data1, n_data, y_data_bottoms);

            area_properties.area_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1));
            area_properties.area_hover_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(3));
            area_properties.tooltip = "series 2";
            Areas.SetProperties(area_properties);
            Areas.DrawArea(Figure, x_data, y_data2, n_data, y_data_bottoms);

            area_properties.area_fill_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2));
            area_properties.area_hover_col = ImGui::ColorConvertFloat4ToU32(scales_color.Scale(3));
            area_properties.tooltip = "series 3";
            Areas.SetProperties(area_properties);
            Areas.DrawArea(Figure, x_data, y_data3, n_data, y_data_bottoms);

            // Axes
            ImGui::ImAxisProperties axis_properties;
            axis_properties.axis_title = "x-axis bottom";
            axis_properties.axis_title_font = io.FontDefault;
            axis_properties.axis_title_font_size = 18.0f;
            axis_properties.axis_title_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_thickness = 2.0f;
            axis_properties.axis_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            axis_properties.axis_tick_font = io.FontDefault;
            axis_properties.axis_tick_font_size = 12.0f;
            axis_properties.axis_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            ImGui::ImAxis<float, float> Axis;
            axis_properties.axis_tick_format = "%2.1f";
            Axis.SetProperties(axis_properties);
            Axis.DrawXAxis(Figure, "Bottom", 0.0f, 6.0f, 1.0f);
            Axis.DrawYAxis(Figure, "Left", 0.0f, 8.0f, 1.0f);

            // Legend
            ImGui::ImLegendProperties legend_properties;
            legend_properties.stroke_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));
            legend_properties.series_font = io.FontDefault;
            legend_properties.series_font_size = 18.0f;
            legend_properties.series_font_col = ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f));

            const ImU32 series_color[] = {
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(0)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(1)),
                ImGui::ColorConvertFloat4ToU32(scales_color.Scale(2))
            };
            ImGui::ImLegend<float, float> Legend;
            Legend.SetProperties(legend_properties);
            Legend.DrawLegend(Figure, "TR", series, series_color, 3);

            ImGui::End();
        }

        {
            bool show_metrics_window = true;
            ImGui::ShowMetricsWindow(&show_metrics_window);
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
