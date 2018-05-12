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

    // ## Plot legends and other features

    struct ImColorBar
    {
    };

    // ## Scales
    template<class Ta, class Tb>
    class ImScales
    {
    public:
        void SetDomain(const Ta& min, const Ta& max){domain_min_ = min; domain_max_ = max;}     // input data ranges
        void SetRange(const Tb min, const Tb max){range_min_ = min; range_max_ = max;}     // output data ranges
        Ta GetDomainMin(){return domain_min_;}
        Ta GetDomainMax(){return domain_max_;}
        Tb GetRangeMin(){return range_min_;}
        Tb GetRangeMax(){return range_max_;}
        virtual Tb Scale(const Ta& t) = 0;
    protected:
        Ta domain_min_;
        Ta domain_max_;
        Tb range_min_;
        Tb range_max_;
    };


    /**
     * @brief Continuous linear scales used for ImVec2 and ImVec4 (colors
     * 
     */
    template<class Ta, class Tb>
    class ImLinearScales: public ImScales<Ta, Tb>
    {
    public:
        Tb Scale(const Ta& t)
        {
            const Ta tx = (t - domain_min_)/(domain_max_ - domain_min_);
            const Tb ty = ImLerp(range_min_, range_max_, tx);
            // printf("tx (%f,%f), ty(%f,%f)", tx.x, tx.y, ty.x, ty.y);
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

    template<class Ta, class Tc>
    class ImPlot2D
    {
    public:
        void SetScales(ImScales<Ta, float>* scales_x, ImScales<Tc, float>* scales_y){scales_x_ = scales_x; scales_y_ = scales_y;}
        void ShowFigure(const ImVec2& plot_size,
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
            const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
            const ImRect figure_bb(ImVec2(inner_bb.Min.x + margin_left, inner_bb.Min.y + margin_top),
                ImVec2(inner_bb.Max.x - margin_right, inner_bb.Max.y - margin_bottom));
            const ImRect total_bb(frame_bb.Min, frame_bb.Max);
            RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

            // Draw the title
            const ImVec2 title_size = CalcTextSize(title, NULL, true);
            if (title_size.x > 0.0f)
            {
                // centered by default (add parameter for title position)
                const ImVec2 title_pos(
                    (inner_bb.Max.x - inner_bb.Min.x)*0.5f + title_size.x*0.5,
                    inner_bb.Min.y);
                window->DrawList->AddText(title_font, title_font_size, title_pos, title_font_col, title);
            }

            // update scales range
            scales_x_->SetRange(figure_bb.Min.x, figure_bb.Max.x);
            scales_y_->SetRange(figure_bb.Max.y, figure_bb.Min.y);
        };

        /**
         * @brief Draw Top X Axis
         * 
         * TODO: add method to make nice round numbers for tick labels and spacing
         * TODO: add option to orient tick labels
         * TODO: fix shifting axis title
         * 
         * @param TODO...
         */
        void ShowXAxisTop(const char* axis_title, const ImFont* axis_title_font, 
            const float& axis_title_font_size, const ImU32& axis_title_font_col,
            const int& axis_ticks, const char* axis_tick_format,
            const float& axis_thickness, const ImU32& axis_col,
            const ImFont* axis_tick_font, const float& axis_tick_font_size, 
            const ImU32& axis_font_col)
        {            
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;
            
            const Ta tick_span = (scales_x_->GetDomainMax() - scales_x_->GetDomainMin())/axis_ticks;
            
            for (int n=0; n<=axis_ticks; ++n)
            {
                // Tick label
                char tick_label[64];
                const Ta tick_value = tick_span * n + scales_x_->GetDomainMin();

                // Tick Position
                sprintf(tick_label, axis_tick_format, tick_value);
                ImVec2 tick_pos = ImVec2(scales_x_->Scale(tick_value), scales_y_->GetRangeMax() - tick_size.y); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }

            // Axis
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMax()),
                ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMax()), axis_col, axis_thickness);

            // Axis title
            const ImVec2 title_size = CalcTextSize(axis_title, NULL, true);
            const float title_height_spacing = title_size.y*0.8;
            ImVec2 title_pos = ImVec2((scales_x_->GetRangeMax() - scales_x_->GetRangeMin())*0.5f + title_size.x*0.5f,
                scales_y_->GetRangeMax() - (title_size.y + tick_size.y));
                // scales_y_->GetRangeMax() - (title_size.y + tick_size.y + title_height_spacing + tick_height_spacing));
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        };

        /**
         * @brief Draw Bottom X Axis
         * 
         * @param TODO...
         */
        void ShowXAxisBottom(const char* axis_title, const ImFont* axis_title_font, 
            const float& axis_title_font_size, const ImU32& axis_title_font_col,
            const int& axis_ticks, const char* axis_tick_format,
            const float& axis_thickness, const ImU32& axis_col,
            const ImFont* axis_tick_font, const float& axis_tick_font_size, 
            const ImU32& axis_font_col)
        {            
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;
            
            const Ta tick_span = (scales_x_->GetDomainMax() - scales_x_->GetDomainMin())/axis_ticks;
            
            for (int n=0; n<=axis_ticks; ++n)
            {
                // Tick label
                char tick_label[64];
                const Ta tick_value = tick_span * n + scales_x_->GetDomainMin();

                // Tick Position
                sprintf(tick_label, axis_tick_format, tick_value);
                ImVec2 tick_pos = ImVec2(scales_x_->Scale(tick_value), scales_y_->GetRangeMin() + tick_size.y); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }

            // Axis
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMin()),
                ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMin()), axis_col, axis_thickness);

            // Axis title
            const ImVec2 title_size = CalcTextSize(axis_title, NULL, true);
            const float title_height_spacing = title_size.y*0.8;
            ImVec2 title_pos = ImVec2((scales_x_->GetRangeMax() - scales_x_->GetRangeMin())*0.5f + title_size.x*0.5f,
                scales_y_->GetRangeMin() + (title_size.y + tick_size.y));
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        };

        /**
         * @brief Draw Left Y Axis
         * 
         * @param TODO...
         */
        void ShowYAxisLeft(const char* axis_title, const ImFont* axis_title_font, 
            const float& axis_title_font_size, const ImU32& axis_title_font_col,
            const int& axis_ticks, const char* axis_tick_format,
            const float& axis_thickness, const ImU32& axis_col,
            const ImFont* axis_tick_font, const float& axis_tick_font_size, 
            const ImU32& axis_font_col)
        {            
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;
            
            const Ta tick_span = (scales_y_->GetDomainMax() - scales_y_->GetDomainMin())/axis_ticks;
            
            for (int n=0; n<=axis_ticks; ++n)
            {
                // Tick label
                char tick_label[64];
                const Ta tick_value = tick_span * n + scales_y_->GetDomainMin();

                // Tick Position
                sprintf(tick_label, axis_tick_format, tick_value);
                ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMin() - tick_size.y, scales_y_->Scale(tick_value)); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }

            // Axis
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMin()),
                ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMax()), axis_col, axis_thickness);

            // Axis title
            const ImVec2 title_size = CalcTextSize(axis_title, NULL, true);
            const float title_height_spacing = title_size.y*0.8;
            ImVec2 title_pos = ImVec2(
                scales_x_->GetRangeMin() - (title_size.y + tick_size.y),
                (scales_y_->GetRangeMax() - scales_y_->GetRangeMin())*0.5f + title_size.y*0.5f);
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        };

        /**
         * @brief Draw Right Y Axis
         * 
         * @param TODO...
         */
        void ShowYAxisRight(const char* axis_title, const ImFont* axis_title_font, 
            const float& axis_title_font_size, const ImU32& axis_title_font_col,
            const int& axis_ticks, const char* axis_tick_format,
            const float& axis_thickness, const ImU32& axis_col,
            const ImFont* axis_tick_font, const float& axis_tick_font_size, 
            const ImU32& axis_font_col)
        {            
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;
            
            const Ta tick_span = (scales_y_->GetDomainMax() - scales_y_->GetDomainMin())/axis_ticks;
            
            for (int n=0; n<=axis_ticks; ++n)
            {
                // Tick label
                char tick_label[64];
                const Ta tick_value = tick_span * n + scales_y_->GetDomainMin();

                // Tick Position
                sprintf(tick_label, axis_tick_format, tick_value);
                ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMax() + tick_size.y, scales_y_->Scale(tick_value)); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }

            // Axis
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMin()),
                ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMax()), axis_col, axis_thickness);

            // Axis title
            const ImVec2 title_size = CalcTextSize(axis_title, NULL, true);
            const float title_height_spacing = title_size.y*0.8;
            ImVec2 title_pos = ImVec2(
                scales_x_->GetRangeMin() + (title_size.y + tick_size.y),
                (scales_y_->GetRangeMax() - scales_y_->GetRangeMin())*0.5f + title_size.y*0.5f);
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        };

        /**
         * @brief Draw X axis grid lines
         * 
         * @param TODO...
         */
        void ShowXAxesGridLines();

        /**
         * @brief Draw y axis grid lines
         * 
         * @param TODO...
         */
        void ShowYAxesGridLines();
        
        /**@brief Show a plot legend
         * 
         * @param pos Pos of the legend (TL, TR, BL, BR)
         * @param col Background color of the legend
         * @param series List of series labels
         * @param series_color List of series colors 
         * @param series_font Font type for series labels
         * @param series_font_size Font size of the series labels
         * @param series_font_col Color of the series labels
         */
        void ShowLegend(const char* pos, const ImU32& col, 
            const char* series[], const ImU32 series_color[], const int& n_series,
            const ImFont* series_font, const float& series_font_size, const ImU32& series_font_col)
        {
            //assert(IM_ARRAYSIZE(series) == IM_ARRAYSIZE(series_color));
            
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImVec2 legend_pos = ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMax()); //TR
            if (pos == "TL") legend_pos = ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMin());
            else if (pos == "TR") legend_pos = ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMax());
            else if (pos == "BR") legend_pos = ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMax());
            else if (pos == "BL") legend_pos = ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMin());


            // Deduce the maximum text size
            ImVec2 series_size = ImVec2(0.0f, 0.0f);
            for (int n=0; n<n_series; ++n)
            {
                ImVec2 series_size_tmp = CalcTextSize(series[n], NULL, true);
                if (series_size_tmp.x > series_size.x) series_size = series_size_tmp;
            }

            // Legend attributes     
            const float series_spacing = 0.1*series_size.y;
            const float box_length = 0.9*series_size.y;
            const float height = n_series * series_size.y + n_series * series_spacing + series_spacing;
            const float width = series_size.x + box_length + 1.0f + 6*series_spacing;

            // Draw box
            window->DrawList->AddRect(ImVec2(legend_pos.x, legend_pos.y), ImVec2(legend_pos.x + width, legend_pos.y + height), col);

            for (int n=0; n<n_series; ++n)
            {
                const float start_y_pos = n*series_size.y + n*series_spacing + series_spacing;

                // Draw box and color for each series
                window->DrawList->AddRectFilled(
                    ImVec2(legend_pos.x + series_spacing, legend_pos.y + start_y_pos),
                    ImVec2(legend_pos.x + series_spacing + box_length, legend_pos.y + start_y_pos + box_length),
                    series_color[n]); 

                // Label colored box with series name
                window->DrawList->AddText(series_font, series_font_size,
                    ImVec2(legend_pos.x + series_spacing + box_length + 1.0f, legend_pos.y + start_y_pos),
                    series_font_col, series[n]);

            }
        };

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

        // virtual void ShowPlot() = 0; ? linking error

    protected:
        ImScales<Ta, float>* scales_x_;
        ImScales<Tc, float>* scales_y_;
    };

    template<class Ta, class Tc>
    class ImScatterPlot2D: public ImPlot2D<Ta, Tc>
    {
    public:
        // void ShowPlot() override; ?
        /**
         * @brief Shows the plot on the figure
         * 
         * @param x_data X data of length n (ordered from lowest to highest)
         * @param y_data y data of length n (ordered from lowest to highest)
         * @param r_data radius lengths of each point of length n (matching order of x/y_data)
         * @param marker_stroke_col circle (or other symbol) stroke color
         * @param marker_stroke_width circle (or other symbol) stroke width
         * @param marker_fill col circle (or other symbol) fill color
         * @param line_stroke_col line stroke color
         * @param line_stroke_width line stroke width
         * @param line_stroke_dash spacing of the dash
         * @param line_stroke_gap spacing between dashes
         * @param line_interp "None" for a straight line and "Bezier" for a curved line
         * @param labels Label for each data point of length n (matching order of x/y_data)
         * @param label_font Label font
         * @param label_font_col Label font color
         * @param label_font_size Label font size
         * @param dx1 Upper x data error of length n (matching order of x/y_data)
         * ...
         */
        void ShowPlot(const Ta* x_data, const Tc* y_data, const float* r_data, const int& n_data,
            const ImU32& marker_stroke_col, const float& marker_stroke_width,
            const ImU32& marker_fill_col, const ImU32& marker_hovered_col, 
            const ImU32& line_stroke_col, const float& line_stroke_width, 
            const float& line_stroke_dash, const float& line_stroke_gap, 
            const char* line_interp, 
            const char* series, const char* labels[], 
            const ImFont* label_font, const ImU32& label_font_col,
            const float& label_font_size, const float* dx1, const float* dx2, const float* dy1, const float* dy2)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            if (n_data > 0)
            {
                int t0 = 0;
                for (int n = 0; n < n_data; ++n)
                {
                    // Points
                    const int v1_idx = (int)(t0 * n_data);
                    const float centre_scaled_x = scales_x_->Scale(x_data[n]);
                    const float centre_scaled_y = scales_y_->Scale(y_data[n]);
                    window->DrawList->AddCircleFilled(ImVec2(centre_scaled_x, centre_scaled_y), r_data[n], marker_fill_col, 12);

                    // Line
                    if (n > 0 && strcmp(line_interp, "None") == 0)
                    {
                        window->DrawList->AddLine(
                            ImVec2(scales_x_->Scale(x_data[n-1]), scales_y_->Scale(y_data[n-1])),
                            ImVec2(scales_x_->Scale(x_data[n]), scales_y_->Scale(y_data[n])),
                            line_stroke_col, line_stroke_width);
                    }
                    else if (n > 0 && strcmp(line_interp, "Bezier") == 0)
                    {
                        // TODO
                    }

                    // Tooltip on hover
                    if (centre_scaled_x - r_data[n] <= g.IO.MousePos.x && 
                    centre_scaled_x + r_data[n] >= g.IO.MousePos.x &&
                    centre_scaled_y - r_data[n] <= g.IO.MousePos.y && 
                    centre_scaled_y + r_data[n] >= g.IO.MousePos.y)
                    {
                        SetTooltip("%s\n%s: %8.4g\n%s: %8.4g", series, "x", x_data[n], "y", y_data[n]);
                        window->DrawList->AddCircleFilled(ImVec2(centre_scaled_x, centre_scaled_y), r_data[n], marker_hovered_col, 12);
                    }
                    // labels
                    if (labels != NULL)
                        window->DrawList->AddText(label_font, label_font_size, ImVec2(centre_scaled_x, centre_scaled_y), label_font_col, labels[n]);
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