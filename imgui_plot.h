// # Low level plotting features and components
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <iostream>

namespace ImGui
{
    // ## Interpolation
    // - interpolate
    // - sample
    // - splines
    // static inline time_t    ImLerp(const time_t& a, const time_t& b, const time_t& t);                      //TODO [DM]
    // static inline float     ImBSBasis(float t1, float v0, float v1, float v2, float v3);                            //TODO [DM] BSpline basis function
    // static inline ImVec4    ImBSBasis(const ImVec4& t1, const ImVec4& v0, const ImVec4& v1, const ImVec4& v2, const ImVec4& v3);                            //TODO [DM] BSpline basis function used for colors
    // static inline int       ImBSerp(int* v, float t, bool closed = false);                            //TODO [DM] BSpline interpolate from [a, b) or [a, b] (closed=true)
    // static inline float     ImBSerp(float* v, float t, bool closed = false);                        //TODO [DM] BSpline interpolate from [a, b) or [a, b] (closed=true)
    // static inline ImVec4    ImBSerp(const ImVec4* v, const ImVec4& t, bool closed = false);                        //TODO [DM] BSpline interpolate used for colors from [a, b) or [a, b] (closed=true)
    // static inline int*      ImQuaLerp(int a, int b, float t, int n);                        //TODO [DM] Quantized linear sampler
    // static inline float*    ImQuaLerp(float a, float b, float t, int n);                        //TODO [DM] Quantized linear sampler
    // static inline time_t*   ImQuaLerp(const time_t& a, const time_t& b, const time_t& t, int n);                        //TODO [DM] Quantized linear sampler
    // static inline int*      ImQuaBSerp(int a, int b, float t, int n, bool closed = false);                        //TODO [DM] Quantized BSpline sampler
    // static inline float*    ImQuaBSerp(float a, float b, float t, int n, bool closed = false);                        //TODO [DM] Quantized BSpline sampler
    // static inline ImVec4*   ImQuaBSerp(const ImVec4& a, const ImVec4& b, const ImVec4& t, int n, bool closed = false);                        //TODO [DM] Quantized BSpline sampler used for colors

    // ## Data
    // - plotting interfaces should except the 
    //   following plot elements:
    // a       x;                  // x data (float, int, date, etc)
    // b       y;                  // y data (float, int, date, etc)
    // float   r;                  // circle radius or box half-width
    // ImU32   stroke_col;
    // float   stroke_width;
    // ImU32   fill_col;
    // int     caption_id;
    // char*   label;              // string identifier for the data point
    // ImFont* label_font;
    // ImU32   label_font_col;
    // float   label_font_size;
    // char*   series;	
    // a       dx1; // left
    // a       dx2; // right
    // b       dy1; // top
    // b       dy2; // bottom
    // bool   shade; // error shades instead of error bars

    // ## Plot legends and other features

    struct ImLegend
    {
        /**@brief Show a plot legend
         * 
         * @param pos Pos of the legend
         * @param width Width of the legend
         * @param height Height of the legend
         * @param col Background color of the legend
         * @param series List of series labels
         * @param series_color List of series colors 
         * @param series_font Font type for series labels
         * @param series_font_col Color of the series labels
         * @param series_font_size Font size of the series labels
         */
        void ShowLegend(ImVec2 pos, float width, float height, ImU32 col, 
            char* series[], ImU32 series_color[],
            ImFont* series_font, ImU32 series_font_col, float series_font_size)
        {
            assert(IM_ARRAYSIZE(series) == IM_ARRAYSIZE(series_color));
            //TODO
        };
    };

    struct ImColorBar
    {
    /**@brief Show a color bar for e.g., a heatmap
     * 
     * @param span Range of values to span
     * @param min Minimum color value
     * @param max Maximum color value 
     */
        void ShowColorBar(const float& span, const ImU32& min, const ImU32& max)
        {
            //TODO
        };
    };

    // ## Scales
    class ImScales
    {
    public:
        void SetDomain(ImVec2 min, ImVec2 max){domain_min_ = min; domain_max_ = max;}     // input data ranges
        void SetRange(ImVec2 min, ImVec2 max){range_min_ = min; range_max_ = max;}     // output data ranges
        virtual ImVec2 Scale(const ImVec2& t) = 0;
        virtual float ScaleX(const float& t) = 0;
        virtual float ScaleY(const float& t) = 0;
    protected:
        ImVec2 domain_min_;
        ImVec2 domain_max_;
        ImVec2 range_min_;
        ImVec2 range_max_;
    };

    class ImLinearScales: public ImScales
    {
    public:
        // continuous scales
        ImVec2 Scale(const ImVec2& t)
        {
            const ImVec2 tx = (t - domain_min_)/(domain_max_ - domain_min_);
            const ImVec2 ty = (range_max_ - range_min_)*tx + range_min_;
            // printf("tx (%f,%f), ty(%f,%f)", tx.x, tx.y, ty.x, ty.y);
            return ty;
        };
        float ScaleX(const float& t)
        {
            const float tx = (t - domain_min_.x)/(domain_max_.x - domain_min_.x);
            const float ty = (range_max_.x - range_min_.x)*tx + range_min_.x;
            return ty;
        };
        float ScaleY(const float& t)
        {
            const float tx = (t - domain_min_.y)/(domain_max_.y - domain_min_.y);
            const float ty = (range_max_.y - range_min_.y)*tx + range_min_.y;
            return ty;
        };
        // void    Pow();
        // void    Log();
        // void    Time();
        // void    Sequential();
        // void    Continuous(); // used for colors
        // // quantized scales
        // void    Quantize(); // maps a continouus variable to a discrete scale
        // void    Quantile(ImVec2& range, float& val); // maps a continuous varible to a sampled domain
        // void    Threshold(); 
        // // discrete scales
        // void    Ordered();
        // void    Categorical();
        // void    Band();
        // void    Round();
        // void    Point();
    };

    // ## Axes
    class ImAxes
    {
    public:
        void ShowAxesTop(char* title, float x_tick_major, float x_tick_minor);
        void ShowAxesBottom(char* title, float x_tick_major, float x_tick_minor);
        void ShowAxesLeft(char* title, float y_tick_major, float y_tick_minor);
        void ShowAxesRight(char* title, float y_tick_major, float y_tick_minor);
        void ShowXAxesGridLines();
        void ShowYAxesGridLines();
    };

    // # High level plotting functions

    // ## Charts
    // ### Basics
    // - Line
    // - Scatter
    // - Bar (stacked)
    // - Barh
    // - BoxPlot
    // - Pie
    // - Area (stacked)
    // - Heatmap
    // - Histogram (densiting and binning)
    // ### Advanced
    // - Contour
    // - Stem
    // - Stream
    // - Polar
    // - Radar
    // - Hexagonal binning
    // - Histogram 2D
    // - Violin
    // - Joint
    // - Strip
    // - Swarm

    class ImPlotter
    {
    public:
        ImPlotter(){};
        ~ImPlotter(){};
        void SetScales(ImScales* scales){scales_ = scales;}
        void ShowFigure(ImVec2 plot_size,
            const float& margin_bottom, const float& margin_top, 
            const float& margin_left, const float& margin_right,
            const char* title, ImFont* title_font, const float& title_font_size, const ImU32& title_font_col)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;

            // Create the window
            const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(plot_size.x, plot_size.y));
            const ImRect inner_bb_(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
            const ImRect figure_bb_(ImVec2(inner_bb_.Min.x + margin_bottom, inner_bb_.Min.y + margin_left),
                ImVec2(inner_bb_.Max.x - margin_right, inner_bb_.Max.y - margin_top));
            const ImRect total_bb(frame_bb.Min, frame_bb.Max);
            RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

            // Draw the title
            const ImVec2 title_size = CalcTextSize(title, NULL, true);
            if (title_size.x > 0.0f)
            {
                // centered by default (add parameter for title position)
                const ImVec2 title_pos(
                    (inner_bb_.GetTR().x - inner_bb_.GetTL().x)*0.5f + title_size.x*0.5f,
                    inner_bb_.GetTR().y);
                window->DrawList->AddText(title_font, title_font_size, title_pos, title_font_col, title);
            }

            // update scales range
            scales_->SetRange(figure_bb_.GetBL(), figure_bb_.GetTR());
        }

        /**
         * @brief Draw top, bottom, left, and right axes depending on user input
         * 
         * Axes will be drawn starting at the figure bounding box and migrated outward
         *  through the margins
         * 
         * @param TODO...
         * @param x_tick # of tick strikes (includes min and max)
         */
        void ShowAxes(
            const float* x_data, const float* y_data, const int n_data,
            char* top_title, int top_x_ticks, 
                float top_x_axis_thickness, ImU32 top_x_axis_col,
                ImFont* top_x_axis_tick_font, float top_x_axis_tick_font_size, ImU32 top_x_axis_font_col,
            char* bottom_title, int bottom_x_ticks,
                float bottom_x_axis_thickness, ImU32 bottom_x_axis_col,
            char* left_title, int left_y_ticks,
                float left_y_axis_thickness, ImU32 left_y_axis_col,
            char* right_title, int right_y_ticks,
                float right_y_axis_thickness, ImU32 right_y_axis_col,
            bool top_x_axis = false, bool bottom_x_axis = false, 
            bool left_y_axis = false, bool right_y_axis = false,
            bool x_axes_grid_lines = false, bool y_axes_grid_lines = false);
        void ShowXAxisTop(
            const float* x_data, const float* y_data, const int n_data,
            char* top_title, ImFont* top_title_font, float top_title_font_size, ImU32 top_title_font_col,
                int top_x_ticks, char* top_x_tick_format,
                float top_x_axis_thickness, ImU32 top_x_axis_col,
                ImFont* top_x_axis_tick_font, float top_x_axis_tick_font_size, ImU32 top_x_axis_font_col,
            bool x_axes_grid_lines = false)
        {            
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(top_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;
            ImVec2 _pos = ImVec2((figure_bb_.GetTR().x - figure_bb_.GetTL().x)*0.5f+tick_size.x*0.5f,
                figure_bb_.GetTR().y + tick_size.y + tick_height_spacing);
            
            const float tick_span = (x_data[n_data-1] - x_data[0])/top_x_ticks;
            int n_ticks = (int)((x_data[n_data-1] - x_data[0])/tick_span);
            for (int n=0; n<n_ticks; ++n)
            {
                char tick_label[64];
                float tick_value = ImLerp(x_data[0], x_data[n_data-1], tick_span * n);
                ImVec2 tick_pos = ImVec2(scales_->ScaleX(tick_value), figure_bb_.GetTR().y); // interpolate the position
                sprintf(tick_label, top_x_tick_format, tick_value);
                window->DrawList->AddText(top_x_axis_tick_font, top_x_axis_tick_font_size, tick_pos, top_x_axis_font_col, tick_label);
            }

            // Axis
            window->DrawList->AddLine(figure_bb_.GetTL(), figure_bb_.GetTR(), top_x_axis_col, top_x_axis_thickness);
            window->DrawList->AddLine(figure_bb_.GetBL(), figure_bb_.GetTL(), top_x_axis_col, top_x_axis_thickness);
            window->DrawList->AddLine(figure_bb_.GetBL(), figure_bb_.GetBR(), top_x_axis_col, top_x_axis_thickness);
            window->DrawList->AddLine(figure_bb_.GetBR(), figure_bb_.GetTL(), top_x_axis_col, top_x_axis_thickness);

            // Axis title
            if (top_title != NULL)
            {
                const ImVec2 title_size = CalcTextSize(top_title, NULL, true);
                const float title_height_spacing = title_size.y*0.8;
                ImVec2 title_pos = ImVec2((figure_bb_.GetTR().x - figure_bb_.GetTL().x)*0.5f + title_size.x*0.5f,
                    figure_bb_.GetTR().y + title_size.y + tick_size.y + title_height_spacing + tick_height_spacing);
                window->DrawList->AddText(top_title_font, top_title_font_size, title_pos, top_title_font_col, top_title);
            }
        };

        // virtual void ShowPlot() = 0; ? linking error

    protected:
        ImVec2 plot_size_;
        ImRect inner_bb_;
        ImRect figure_bb_;
        ImScales* scales_;
    };

    class ImScatterPlot: public ImPlotter
    {
    public:
        // void ShowPlot() override; ?
        /**
         * @brief Shows the plot on the figure
         * 
         * @param x_data X data of length n (ordered from lowest to highest)
         * @param y_data y data of length n (ordered from lowest to highest)
         * @param r_data radius lengths of each point of length n (matching order of x/y_data)
         * @param stroke_col circle (or other symbol) stroke color
         * @param stroke_width circle (or other symbol) stroke width
         * @param fill col circle (or other symbol) fill color
         * @param hovered_col circle (or other symbol) hover color
         * @param labels Label for each data point of length n (matching order of x/y_data)
         * @param label_font Label font
         * @param label_font_col Label font color
         * @param label_font_size Label font size
         * @param dx1 Upper x data error of length n (matching order of x/y_data)
         * ...
         */
        void ShowPlot(const float* x_data, const float* y_data, const float* r_data, const int n_data,
            const ImU32 stroke_col, const float stroke_width,
            const ImU32 fill_col, const ImU32 hovered_col, const char* series, const char* labels[], 
            const ImFont* label_font, const ImU32 label_font_col,
            const float label_font_size, const float* dx1, const float* dx2, const float* dy1, const float* dy2)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            // determine the domain from user input
            ImVec2 pos_min(x_data[0], y_data[0]);
            ImVec2 pos_max(x_data[n_data-1], y_data[n_data-1]);
            scales_->SetDomain(pos_min, pos_max);

            if (n_data > 0)
            {
                int t0 = 0;
                for (int n = 0; n < n_data; ++n)
                {

                    // Points
                    const int v1_idx = (int)(t0 * n_data);
                    ImVec2 centre(x_data[n], y_data[n]);
                    ImVec2 centre_scaled = scales_->Scale(centre);
                    window->DrawList->AddCircleFilled(centre_scaled, r_data[n], fill_col, 12);

                    // Tooltip on hover
                    if (centre_scaled.x - r_data[n] <= g.IO.MousePos.x && 
                    centre_scaled.x + r_data[n] >= g.IO.MousePos.x &&
                    centre_scaled.y - r_data[n] <= g.IO.MousePos.y && 
                    centre_scaled.y + r_data[n] >= g.IO.MousePos.y)
                    {
                        SetTooltip("%s\n%s: %8.4g\n%s: %8.4g", series, "x", x_data[n], "y", y_data[n]);
                        window->DrawList->AddCircleFilled(centre_scaled, r_data[n], hovered_col, 12);
                    }
                    // labels
                    if (labels != NULL)
                        window->DrawList->AddText(label_font, label_font_size, centre_scaled, label_font_col, labels[n]);
                    t0 += 1;
                }

            //TODO: add zoom
            }
        };
    };


    // ## Layouts and hierarchies
    // - circos layout
    // - swim lanes
    // - bundle diagram
    // - chord diagram
    // - force directed graph
    // - force layout
    // - indented tree layout
    // - pack layout
    // - partition layout
    // - radial dendrogram
    // - radial tree layout
    // - sankey diagram
    // - treemap layout
    // - vertical dendrogram
}