// # Low level plotting features and components for creating basic to advanced plots and charts

// BUGS:
// - titles does not move in both x/y directions correctly relative to the figure

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#include <iostream> //delete when finished debugging

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

    // ## Scales
    template<typename Ta, typename Tb>
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
    template<typename Ta, typename Tb>
    class ImLinearScales: public ImScales<Ta, Tb>
    {
    public:
        Tb Scale(const Ta& t)
        {
            const Ta tx = (t - domain_min_)/(domain_max_ - domain_min_);
            const Tb ty = ImLerp(range_min_, range_max_, tx);
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
    
    // ## Plot element
    struct ImPlotProperties
    {
        ImVec2 plot_size = ImVec2(480.0f, 480.0f);        
        float margin_bottom = 50.0f;
        float margin_top = 50.0f; 
        float margin_left = 50.0f;
        float margin_right = 50.0f;
        char* title = NULL;
        ImFont* title_font = NULL;
        float title_font_size = 18.0f;
        ImU32 title_font_col = NULL;
    };

    template<typename Ta, typename Tb>
    class ImPlot
    {
    public:
        /**
         * @brief Draw the figure with an optional title
         * 
         */
        void DrawFigure()
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;

            // Create the window
            const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(properties_.plot_size.x, properties_.plot_size.y));
            const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
            const ImRect figure_bb(ImVec2(inner_bb.Min.x + properties_.margin_left, inner_bb.Min.y + properties_.margin_top),
                ImVec2(inner_bb.Max.x - properties_.margin_right, inner_bb.Max.y - properties_.margin_bottom));
            const ImRect total_bb(frame_bb.Min, frame_bb.Max);
            RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

            // Draw the title
            const ImVec2 title_size = CalcTextSize(properties_.title, NULL, true);
            if (title_size.x > 0.0f)
            {
                // centered by default (add parameter for title position)
                const ImVec2 title_pos(
                    (inner_bb.Max.x - inner_bb.Min.x)*0.5f + title_size.x*0.5,
                    inner_bb.Min.y);
                window->DrawList->AddText(properties_.title_font, properties_.title_font_size, title_pos, properties_.title_font_col, properties_.title);
            }

            // update scales range
            scales_x_->SetRange(figure_bb.Min.x, figure_bb.Max.x);
            scales_y_->SetRange(figure_bb.Max.y, figure_bb.Min.y);
        };

        void SetProperties(ImPlotProperties& properties){properties_ = properties;}
        void SetScales(ImScales<Ta, float>* scales_x, ImScales<Tb, float>* scales_y){scales_x_ = scales_x; scales_y_ = scales_y;}
        ImScales<Ta, float>* GetScalesX(){return scales_x_;}
        ImScales<Tb, float>* GetScalesY(){return scales_y_;}

    protected:
        ImScales<Ta, float>* scales_x_;
        ImScales<Tb, float>* scales_y_;
        ImPlotProperties properties_;
    };

    // ## Axes
    struct ImAxisProperties
    {
        char* axis_title = NULL;
        ImFont* axis_title_font = NULL;
        float axis_title_font_size = NULL;
        ImU32 axis_title_font_col = NULL;
        char* axis_tick_format = "%4.2f"; ///< string format
        float axis_thickness = 1.0f;
        ImU32 axis_col = NULL;
        ImFont* axis_tick_font = NULL;
        float axis_tick_font_size = 12.0f; 
        ImU32 axis_font_col = NULL;
        float grid_lines_thickness = 1.0f;
        ImU32 grid_lines_col = NULL;
    };

    template<typename Ta, typename Tb>
    class ImAxis
    {
    public:
        /**
        * @brief Draw X axis
        * 
        * @param figure The figure to draw on
        * @param orientation Options are Top, Bottom
        * @param axis_tick_min Minimum axis tick value
        * @param axis_tick_max Maximum axis tick value
        * @param axis_tick_span Spacing between axis ticks
        *
        */
        void DrawXAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
            const Ta& axis_tick_min, const Ta& axis_tick_max, const Ta& axis_tick_span)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;

            Ta tick_value = axis_tick_min;           
            while (tick_value <= axis_tick_max)
            {
                // Tick label
                char tick_label[64];
                sprintf(tick_label, properties_.axis_tick_format, tick_value);

                // Tick Position
                if (strcmp(orientation, "Top") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->Scale(tick_value), figure.GetScalesY()->GetRangeMax() - tick_size.y); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }
                else if (strcmp(orientation, "Bottom") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->Scale(tick_value), figure.GetScalesY()->GetRangeMin() + tick_size.y); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }

                tick_value += axis_tick_span;
            }

            _DrawXAxisAxis(figure, orientation);
            _DrawXAxisTitle(figure, orientation);
        };

        /**
        * @brief Draw X axis
        * 
        * @param figure The figure to draw on
        * @param orientation Options are Top, Bottom
        * @param axis_tick_pos Positions of the axis ticks
        * @param axis_tick_labels Labels of the axis ticks
        *
        */
        void DrawXAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
            const Ta* axis_tick_pos, const char* axis_tick_labels[], const int& n_ticks)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8; 

            for (int n=0; n < n_ticks; ++n)
            {
                // Tick label
                char tick_label[64];
                sprintf(tick_label, properties_.axis_tick_format, axis_tick_labels[n]);

                // Tick Position
                if (strcmp(orientation, "Top") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->Scale(axis_tick_pos[n]), figure.GetScalesY()->GetRangeMax() - tick_size.y); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }
                else if (strcmp(orientation, "Bottom") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->Scale(axis_tick_pos[n]), figure.GetScalesY()->GetRangeMin() + tick_size.y); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }
            }

            _DrawXAxisAxis(figure, orientation);
            _DrawXAxisTitle(figure, orientation);
        };

        void _DrawXAxisAxis(ImPlot<Ta, Tb>& figure, const char* orientation)
        {      
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Axis
            if (strcmp(orientation, "Top") == 0)
            {
                window->DrawList->AddLine(ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->GetRangeMax()),
                    ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMax()), properties_.axis_col, properties_.axis_thickness);
            }
            else if (strcmp(orientation, "Bottom") == 0)
            {
                window->DrawList->AddLine(ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->GetRangeMin()),
                    ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMin()), properties_.axis_col, properties_.axis_thickness);
            }
        };
        void _DrawXAxisTitle(ImPlot<Ta, Tb>& figure, const char* orientation)
        {      
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;

            // Axis title
            const ImVec2 title_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float title_height_spacing = title_size.y*0.8;

            if (strcmp(orientation, "Top") == 0)
            {
                ImVec2 title_pos = ImVec2((figure.GetScalesX()->GetRangeMax() - figure.GetScalesX()->GetRangeMax())*0.5f + title_size.x*0.5f,
                    figure.GetScalesY()->GetRangeMax() - (title_size.y + tick_size.y));
                window->DrawList->AddText(properties_.axis_title_font, properties_.axis_title_font_size, title_pos, properties_.axis_title_font_col, properties_.axis_title);
            }
            else if (strcmp(orientation, "Bottom") == 0)
            {
                ImVec2 title_pos = ImVec2((figure.GetScalesX()->GetRangeMax() - figure.GetScalesX()->GetRangeMin())*0.5f + title_size.x*0.5f,
                    figure.GetScalesY()->GetRangeMin() + (title_size.y + tick_size.y));
                window->DrawList->AddText(properties_.axis_title_font, properties_.axis_title_font_size, title_pos, properties_.axis_title_font_col, properties_.axis_title);
            }
        };

        /**
        * @brief Draw Y axis
        * 
        * @param figure The figure to draw on
        * @param orientation Options are Left, Right
        * @param axis_tick_min Minimum axis tick value
        * @param axis_tick_max Maximum axis tick value
        * @param axis_tick_span Spacing between axis ticks
        *
        */
        void DrawYAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
            const Tb& axis_tick_min, const Tb& axis_tick_max, const Tb& axis_tick_span)
        {        
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;

            Tb tick_value = axis_tick_min;           
            while (tick_value <= axis_tick_max)
            {
                // Tick label
                char tick_label[64];
                sprintf(tick_label, properties_.axis_tick_format, tick_value);

                // Tick Position
                if (strcmp(orientation, "Left") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->GetRangeMin() - tick_size.y, figure.GetScalesY()->Scale(tick_value)); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }
                else if (strcmp(orientation, "Right") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->GetRangeMax() + tick_size.y, figure.GetScalesY()->Scale(tick_value)); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }

                tick_value += axis_tick_span;
            }

            _DrawYAxisAxis(figure, orientation);
            _DrawYAxisTitle(figure, orientation);
        };

        /**
        * @brief Draw Y axis
        * 
        * @param figure The figure to draw on
        * @param orientation Options are Left, Right
        * @param axis_tick_pos Positions of the axis ticks
        * @param axis_tick_labels Labels of the axis ticks
        *
        */
        void DrawYAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
            const Tb* axis_tick_pos, const char* axis_tick_labels[], const int& n_ticks)
        {      
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;
    
            for (int n=0; n < n_ticks; ++n)
            {
                // Tick label
                char tick_label[64];
                sprintf(tick_label, properties_.axis_tick_format, axis_tick_labels[n]);

                if (strcmp(orientation, "Left") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->GetRangeMin() - tick_size.y, figure.GetScalesY()->Scale(axis_tick_pos[n])); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }
                else if (strcmp(orientation, "Right") == 0)
                {
                    ImVec2 tick_pos = ImVec2(figure.GetScalesX()->GetRangeMax() + tick_size.y, figure.GetScalesY()->Scale(axis_tick_pos[n])); // interpolate the position
                    window->DrawList->AddText(properties_.axis_tick_font, properties_.axis_tick_font_size, tick_pos, properties_.axis_font_col, tick_label);
                }
            }

            _DrawYAxisAxis(figure, orientation);
            _DrawYAxisTitle(figure, orientation);
        };

        void _DrawYAxisAxis(ImPlot<Ta, Tb>& figure, const char* orientation)
        {      
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Axis
            if (strcmp(orientation, "Left") == 0)
            {
                window->DrawList->AddLine(ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->GetRangeMin()),
                    ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->GetRangeMax()), properties_.axis_col, properties_.axis_thickness);
            }
            else if (strcmp(orientation, "Right") == 0)
            {
                window->DrawList->AddLine(ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMin()),
                    ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMax()), properties_.axis_col, properties_.axis_thickness);
            }
        };
        void _DrawYAxisTitle(ImPlot<Ta, Tb>& figure, const char* orientation)
        {      
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            // Tick major
            const ImVec2 tick_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float tick_height_spacing = tick_size.y*0.8;

            // Axis title
            const ImVec2 title_size = CalcTextSize(properties_.axis_title, NULL, true);
            const float title_height_spacing = title_size.y*0.8;

            if (strcmp(orientation, "Left") == 0)
            {
                ImVec2 title_pos = ImVec2(
                    figure.GetScalesX()->GetRangeMin() - (title_size.y + tick_size.y),
                    (figure.GetScalesY()->GetRangeMin() - figure.GetScalesY()->GetRangeMax())*0.5f + title_size.y*0.5f);
                window->DrawList->AddText(properties_.axis_title_font, properties_.axis_title_font_size, title_pos, properties_.axis_title_font_col, properties_.axis_title);
            }
            else if (strcmp(orientation, "Right") == 0)
            {
                ImVec2 title_pos = ImVec2(
                    figure.GetScalesX()->GetRangeMax() + (title_size.y + tick_size.y),
                    (figure.GetScalesY()->GetRangeMin() - figure.GetScalesY()->GetRangeMax())*0.5f + title_size.y*0.5f);
                window->DrawList->AddText(properties_.axis_title_font, properties_.axis_title_font_size, title_pos, properties_.axis_title_font_col, properties_.axis_title);
            }
        };
        
        /**
        * @brief Draw X Axis Gridlines
        * 
        * @param figure The figure to draw on
        *
        */
        void DrawXGridLines(ImPlot<Ta, Tb>& figure,
            const Ta& axis_tick_min, const Ta& axis_tick_max, const Ta& axis_tick_span)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            Ta tick_value = axis_tick_min;           
            while (tick_value <= axis_tick_max)
            {
                window->DrawList->AddLine(ImVec2(figure.GetScalesX()->Scale(tick_value), figure.GetScalesY()->GetRangeMax()),
                    ImVec2(figure.GetScalesX()->Scale(tick_value), figure.GetScalesY()->GetRangeMin()),
                    properties_.grid_lines_col, properties_.grid_lines_thickness);

                tick_value += axis_tick_span;
            }
        };
        
        /**
        * @brief Draw Y Axis Gridlines
        * 
        * @param figure The figure to draw on
        *
        */
        void DrawYGridLines(ImPlot<Ta, Tb>& figure,
            const Tb& axis_tick_min, const Tb& axis_tick_max, const Tb& axis_tick_span)
        {        
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            Tb tick_value = axis_tick_min;           
            while (tick_value <= axis_tick_max)
            {
                window->DrawList->AddLine(ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->Scale(tick_value)),
                    ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->Scale(tick_value)),
                    properties_.grid_lines_col, properties_.grid_lines_thickness);

                tick_value += axis_tick_span;
            }
        };
        
        void SetProperties(ImAxisProperties& properties){properties_ = properties;}

    private:
        ImAxisProperties properties_;
    };
    
    // ## Plot legends and other features
    struct ImColorBarProperties
    {
        // TODO
    };

    struct ImLegendProperties
    {
        ImU32 stroke_col = NULL;
        float stroke_width = 1.0f;
        ImU32 fill_col = NULL; ///< Background color of the legend
        ImFont* series_font = NULL; ///< Font type for series labels
        float series_font_size = NULL; ///< Font size of the series labels
        ImU32 series_font_col = NULL; ///< Color of the series labels
    };

    template<typename Ta, typename Tb>
    class ImLegend
    {
    public:
        
        /**@brief Draw a plot legend
         * 
         * @param figure The figure to draw on
         * @param pos Pos of the legend (TL, TR, BL, BR)
         * @param col Background color of the legend
         * @param series List of series labels
         * @param series_color List of series colors 
         */
        void DrawLegend(ImPlot<Ta, Tb>& figure, const char* pos, 
            const char* series[], const ImU32 series_color[], const int& n_series)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            //assert(IM_ARRAYSIZE(series) == IM_ARRAYSIZE(series_color));

            ImVec2 legend_pos = ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMax()); //TR
            if (pos == "TL") legend_pos = ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMin());
            else if (pos == "TR") legend_pos = ImVec2(figure.GetScalesX()->GetRangeMax(), figure.GetScalesY()->GetRangeMax());
            else if (pos == "BR") legend_pos = ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->GetRangeMax());
            else if (pos == "BL") legend_pos = ImVec2(figure.GetScalesX()->GetRangeMin(), figure.GetScalesY()->GetRangeMin());


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
            window->DrawList->AddRect(ImVec2(legend_pos.x, legend_pos.y), ImVec2(legend_pos.x + width, legend_pos.y + height), properties_.stroke_col);

            for (int n=0; n<n_series; ++n)
            {
                const float start_y_pos = n*series_size.y + n*series_spacing + series_spacing;

                // Draw box and color for each series
                window->DrawList->AddRectFilled(
                    ImVec2(legend_pos.x + series_spacing, legend_pos.y + start_y_pos),
                    ImVec2(legend_pos.x + series_spacing + box_length, legend_pos.y + start_y_pos + box_length),
                    series_color[n]); 

                // Label colored box with series name
                window->DrawList->AddText(properties_.series_font, properties_.series_font_size,
                    ImVec2(legend_pos.x + series_spacing + box_length + 1.0f, legend_pos.y + start_y_pos),
                    properties_.series_font_col, series[n]);

            }
        };

        void SetProperties(ImLegendProperties& properties){properties_ = properties;}

    private:
        ImLegendProperties properties_;
    };

    // ## Error Bars
    struct ImErrorBarProperties
    {
        ImU32 error_bar_stroke_col = NULL;
        float error_bar_stroke_width = 1.0f;
        char* error_bar_cap_style = "Straight"; ///< Options are "Straight", "Circular"
        float error_bar_cap_width = 4.0f;
    };

    template<typename Ta, typename Tb>
    class ImErrorBars
    {
    public:
        /**
        * @brief Draw Error Bars
        * 
        * @param figure The figure to draw on
        * @param dx1 Upper error bar lengths
        * @param dx2 Lower error bar lengths
        *
        */
        void DrawErrorBarsX(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data,
            const Ta* dx1, const Ta* dx2,
            const Tb& error_offset, const Ta* error_bottoms)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            for (int n = 0; n < n_data; ++n)
            {            
                // error bars
                const float centre_scaled_x = figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n]);
                const float centre_scaled_y = figure.GetScalesY()->Scale(y_data[n]) + error_offset;

                const ImVec2 point = ImVec2(centre_scaled_x, centre_scaled_y);
                const ImVec2 error_high = ImVec2(figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n] + dx1[n]), centre_scaled_y);
                const ImVec2 error_low = ImVec2(figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n] - dx2[n]), centre_scaled_y);
                
                window->DrawList->AddLine(point, error_high, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);
                window->DrawList->AddLine(point, error_low, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);

                // caps
                if (strcmp(properties_.error_bar_cap_style, "Straight")==0)
                {                
                    if (dx1[n] > 0)
                    {
                        const ImVec2 error_high_down = ImVec2(figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n] + dx1[n]), centre_scaled_y - properties_.error_bar_cap_width * 0.5);
                        const ImVec2 error_high_up = ImVec2(figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n] + dx1[n]), centre_scaled_y + properties_.error_bar_cap_width * 0.5);
                        window->DrawList->AddLine(error_high_down, error_high_up, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);
                    }

                    if (dx2[n] > 0)
                    {
                        const ImVec2 error_low_down = ImVec2(figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n] - dx2[n]), centre_scaled_y - properties_.error_bar_cap_width * 0.5);
                        const ImVec2 error_low_up = ImVec2(figure.GetScalesX()->Scale(x_data[n] + error_bottoms[n] - dx2[n]), centre_scaled_y + properties_.error_bar_cap_width * 0.5);
                        window->DrawList->AddLine(error_low_down, error_low_up, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);
                    }
                }
            }
        };

        /**
        * @brief Draw Error Bars
        * 
        * @param figure The figure to draw on
        * @param dy1 Upper error bar lengths
        * @param dy2 Lower error bar lengths
        *
        */
        void DrawErrorBarsY(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data,
            const Tb* dy1, const Tb* dy2,
            const Ta& error_offset, const Tb* error_bottoms)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            for (int n = 0; n < n_data; ++n)
            {            
                // error bars
                const float centre_scaled_x = figure.GetScalesX()->Scale(x_data[n]) + error_offset;
                const float centre_scaled_y = figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n]);

                const ImVec2 point = ImVec2(centre_scaled_x, centre_scaled_y);
                const ImVec2 error_high = ImVec2(centre_scaled_x, figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n] + dy1[n]));
                const ImVec2 error_low = ImVec2(centre_scaled_x, figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n] - dy2[n]));
                
                window->DrawList->AddLine(point, error_high, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);
                window->DrawList->AddLine(point, error_low, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);

                // caps
                if (strcmp(properties_.error_bar_cap_style, "Straight")==0)
                {                
                    if (dy1[n] > 0)
                    {
                    const ImVec2 error_high_left = ImVec2(centre_scaled_x - properties_.error_bar_cap_width * 0.5,
                        figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n] + dy1[n]));
                    const ImVec2 error_high_right = ImVec2(centre_scaled_x + properties_.error_bar_cap_width * 0.5,
                        figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n] + dy1[n]));
                    window->DrawList->AddLine(error_high_left, error_high_right, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);
                    }

                    if (dy2[n] > 0)
                    {
                    const ImVec2 error_low_left = ImVec2(centre_scaled_x - properties_.error_bar_cap_width * 0.5,
                        figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n] - dy2[n]));
                    const ImVec2 error_low_right = ImVec2(centre_scaled_x + properties_.error_bar_cap_width * 0.5,
                        figure.GetScalesY()->Scale(y_data[n] + error_bottoms[n] - dy2[n]));
                    window->DrawList->AddLine(error_low_left, error_low_right, properties_.error_bar_stroke_col, properties_.error_bar_stroke_width);
                    }
                }
            }
        };
        
        void SetProperties(ImErrorBarProperties& properties){properties_ = properties;}

    private:
        ImErrorBarProperties properties_;
    };

    // ## Labels (for e.g., scatter plot)
    struct ImLabelProperties
    {
        ImFont* label_font = NULL;  ///< Label font
        ImU32 label_font_col = NULL;  ///< Label font color
        float label_font_size = 12.0f;  ///< Label font size
        ImVec2 label_offset_pos = ImVec2(0.0f, 0.0f); ///< Offset position of the label
    };

    template<typename Ta, typename Tb>
    class ImLabels
    {
    public:
        /**
        * @brief Draw Labels
        * 
        * @param figure The figure to draw on
        * @param x_data
        * @param y_data
        * @param n_data
        * @param labels Label for each data point of length n (matching order of x/y_data)
        *
        */
        void DrawLabels(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data, const char* labels[],
            const Ta& x_offset, const Tb& y_offset, const Ta* x_bottoms, const Ta* y_bottoms)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            if (labels == NULL) return;

            for (int n = 0; n < n_data; ++n)
            {            
                // labels
                const float centre_scaled_x = figure.GetScalesX()->Scale(x_data[n] + x_bottoms[0]) + x_offset;
                const float centre_scaled_y = figure.GetScalesY()->Scale(y_data[n] + y_bottoms[0]) + y_offset;
                window->DrawList->AddText(properties_.label_font, properties_.label_font_size, ImVec2(centre_scaled_x, centre_scaled_y), properties_.label_font_col, labels[n]);
            }
        };
        void DrawLabels(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data, const char* labels[])
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            if (labels == NULL) return;

            for (int n = 0; n < n_data; ++n)
            {            
                // labels
                const float centre_scaled_x = figure.GetScalesX()->Scale(x_data[n]);
                const float centre_scaled_y = figure.GetScalesY()->Scale(y_data[n]);
                window->DrawList->AddText(properties_.label_font, properties_.label_font_size, ImVec2(centre_scaled_x, centre_scaled_y), properties_.label_font_col, labels[n]);
            }
        };

        void SetProperties(ImLabelProperties& properties){properties_ = properties;}
    private:
        ImLabelProperties properties_;
    };

    // ## Markers (for e.g., scatter plot)
    struct ImMarkerProperties
    {
        ImU32 marker_stroke_col = NULL;  ///< circle (or other symbol) stroke color
        float marker_stroke_width = 1.0f;  ///< circle (or other symbol) stroke width
        ImU32 marker_fill_col = NULL;  ///< circle (or other symbol) fill color
        ImU32 marker_hovered_col = NULL;  ///< circle (or other symbol) fill color on hover
        // char* tool_tip_format = "%4.2f"; ///< tooltip format
    };

    template<typename Ta, typename Tb>
    class ImMarkers
    {
    public:
        /**
        * @brief Draw Markers
        * 
        * @param figure The figure to draw on
        * @param x_data
        * @param y_data
        * @param r_data Radius of the markers
        * @param n_data
        * @param series Name of the marker series (used for tooltip)
        *
        */
        void DrawMarkers(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const float* r_data, const int& n_data,
            const char* series)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            int t0 = 0;
            for (int n = 0; n < n_data; ++n)
            {
                // Points
                const float centre_scaled_x = figure.GetScalesX()->Scale(x_data[n]);
                const float centre_scaled_y = figure.GetScalesY()->Scale(y_data[n]);
                window->DrawList->AddCircleFilled(ImVec2(centre_scaled_x, centre_scaled_y), r_data[n], properties_.marker_fill_col, 12);

                // Tooltip on hover
                if (centre_scaled_x - r_data[n] <= g.IO.MousePos.x && 
                centre_scaled_x + r_data[n] >= g.IO.MousePos.x &&
                centre_scaled_y - r_data[n] <= g.IO.MousePos.y && 
                centre_scaled_y + r_data[n] >= g.IO.MousePos.y)
                {
                    SetTooltip("%s\n%s: %8.4g\n%s: %8.4g", series, "x", x_data[n], "y", y_data[n]);
                    window->DrawList->AddCircleFilled(ImVec2(centre_scaled_x, centre_scaled_y), r_data[n], properties_.marker_hovered_col, 12);
                }
            }
        };

        void SetProperties(ImMarkerProperties& properties){properties_ = properties;}
    private:
        ImMarkerProperties properties_;
    };

    // ## Lines (for e.g., line plot)
    struct ImLineProperties
    {
        ImU32 line_stroke_col = NULL;  ///< line stroke color
        float line_stroke_width = 1.5f;  ///< line stroke width
        float line_stroke_dash = 0.0f;  ///< spacing of the dash
        float line_stroke_gap = 0.0f;   ///< spacing between dashes
        char* line_interp = "None";  ///< "None" for a straight line and "Bezier" for a curved line
    };

    template<typename Ta, typename Tb>
    class ImLines
    {
    public:
        /**
        * @brief Draw Lines
        * 
        * @param figure The figure to draw on
        * @param x_data
        * @param y_data
        * @param n_data
        *
        */
        void DrawLines(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            int t0 = 0;
            for (int n = 0; n < n_data; ++n)
            {
                // Line
                if (n > 0 && strcmp(properties_.line_interp, "None") == 0)
                {
                    window->DrawList->AddLine(
                        ImVec2(figure.GetScalesX()->Scale(x_data[n-1]), figure.GetScalesY()->Scale(y_data[n-1])),
                        ImVec2(figure.GetScalesX()->Scale(x_data[n]), figure.GetScalesY()->Scale(y_data[n])),
                        properties_.line_stroke_col, properties_.line_stroke_width);
                }
                else if (n > 0 && strcmp(properties_.line_interp, "Bezier") == 0)
                {
                    // TODO
                }
            }
        };

        void SetProperties(ImLineProperties& properties){properties_ = properties;}
    private:
        ImLineProperties properties_;
    };

    // ## Bars (for e.g., bar plot)
    struct ImBarProperties
    {
        float bar_width = 10.0f;
        ImU32 bar_stroke_col = NULL;
        float bar_stroke_width = 1.0f;
        ImU32 bar_fill_col = NULL;
        ImU32 bar_hovered_col = NULL;
    };

    /**
    * @brief Bar plots.  The orientiation of the bars is specified
    *   by the `orientiation` parameter.  Stacked or Staggered bar
    *   representations are controlled manually by the user via
    *   the `bar_bottoms` and `bar_offset` parameters.
    */
    template<typename Ta, typename Tb>
    class ImBars
    {
    public:
        /**
        * @brief Draw vertical Bars
        * 
        * @param figure The figure to draw on
        * @param y_data
        * @param n_data
        * @param series Name of the marker series (used for tooltip)
        *
        */
        void DrawBars(ImPlot<Ta, Tb>& figure,
            const Tb* y_data, const int& n_data,
            const Ta& bar_offset, const Tb* bar_bottoms,
            const char* series)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            const float bar_span = (figure.GetScalesX()->GetRangeMax() - figure.GetScalesX()->GetRangeMin())/(n_data - 1);
            for (int n = 0; n < n_data; ++n)
            {
                // Bars
                // const ImVec2 bar_BL = ImVec2(figure.GetScalesX()->GetRangeMin() + n*bar_span + bar_offset,
                //     figure.GetScalesY()->GetRangeMin() + figure.GetScalesY()->Scale(bar_bottoms[n]));
                // const ImVec2 bar_TR = ImVec2(figure.GetScalesX()->GetRangeMin() + n*bar_span + properties_.bar_width + bar_offset,
                //     figure.GetScalesY()->GetRangeMin() + figure.GetScalesY()->Scale(bar_bottoms[n] + y_data[n]));               
                const ImVec2 bar_BL = ImVec2(figure.GetScalesX()->GetRangeMin() + n*bar_span + bar_offset,
                    figure.GetScalesY()->Scale(bar_bottoms[n]));
                const ImVec2 bar_TR = ImVec2(figure.GetScalesX()->GetRangeMin() + n*bar_span + properties_.bar_width + bar_offset,
                    figure.GetScalesY()->Scale(bar_bottoms[n] + y_data[n]));
                window->DrawList->AddRectFilled(bar_BL, bar_TR, properties_.bar_fill_col);

                // printf("BL: %f, %f; TR: %f, %f", bar_BL.x, bar_BL.y, bar_TR.x, bar_TR.y);
                // system("pause");

                // Tooltip on hover
                if (bar_BL.x <= g.IO.MousePos.x &&
                bar_TR.x >= g.IO.MousePos.x &&
                bar_BL.y >= g.IO.MousePos.y &&
                bar_TR.y <= g.IO.MousePos.y)
                {
                    SetTooltip("%s\n%s: %8.4g", series, "y", y_data[n]);
                    window->DrawList->AddRectFilled(bar_BL, bar_TR, properties_.bar_hovered_col);
                }
            }
        };

        /**
        * @brief Draw horizontal Bars
        * 
        * @param figure The figure to draw on
        * @param y_data
        * @param n_data
        *
        */
        void DrawBarsH(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const int& n_data,
            const Tb& bar_offset, const Ta* bar_bottoms,
            const char* series)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            const float bar_span = (figure.GetScalesY()->GetRangeMax() - figure.GetScalesY()->GetRangeMin())/(n_data - 1);
            for (int n = 0; n < n_data; ++n)
            {
                // Bars
                // const ImVec2 bar_BL = ImVec2(figure.GetScalesX()->GetRangeMin() + n*bar_span + bar_offset,
                //     figure.GetScalesY()->GetRangeMin() + figure.GetScalesY()->Scale(bar_bottoms[n]));
                // const ImVec2 bar_TR = ImVec2(figure.GetScalesX()->GetRangeMin() + n*bar_span + properties_.bar_width + bar_offset,
                //     figure.GetScalesY()->GetRangeMin() + figure.GetScalesY()->Scale(bar_bottoms[n] + y_data[n]));               
                const ImVec2 bar_BL = ImVec2(figure.GetScalesX()->Scale(bar_bottoms[n]),
                    figure.GetScalesY()->GetRangeMin() + n*bar_span + bar_offset);
                const ImVec2 bar_TR = ImVec2(figure.GetScalesX()->Scale(bar_bottoms[n] + x_data[n]),
                    figure.GetScalesY()->GetRangeMin() + n*bar_span + properties_.bar_width + bar_offset);
                window->DrawList->AddRectFilled(bar_BL, bar_TR, properties_.bar_fill_col);

                // printf("BL: %f, %f; TR: %f, %f", bar_BL.x, bar_BL.y, bar_TR.x, bar_TR.y);
                // system("pause");

                // Tooltip on hover
                if (bar_BL.x <= g.IO.MousePos.x &&
                bar_TR.x >= g.IO.MousePos.x &&
                bar_BL.y <= g.IO.MousePos.y &&
                bar_TR.y >= g.IO.MousePos.y)
                {
                    SetTooltip("%s\n%s: %8.4g", series, "x", x_data[n]);
                    window->DrawList->AddRectFilled(bar_BL, bar_TR, properties_.bar_hovered_col);
                }
            }
        };

        void SetProperties(ImBarProperties& properties){properties_ = properties;}
    private:
        ImBarProperties properties_;
    };

    // ## Pie (for e.g., pie or donught plot)
    struct ImPieProperties
    {
        float inner_radius = 0.0f; ///< change to create a donught plot
        float outer_radius = 100.0f; ///< controls the size of the pie
        ImU32 pie_stroke_col = NULL;
        float pie_stroke_width = 1.0f;
        ImU32 pie_hovered_col = NULL;
        int pie_segments = 128; ///< number of segments to use when drawing the circle
                                // if this is too low, the segment will not be drawn!
    };

    template<typename Ta, typename Tb>
    class ImPie
    {
    public:
        /**
        * @brief Draw Pie
        * 
        * @param figure The figure to draw on
        * @param x_data Numerical values
        * @param y_data Color used for each of the pie segments
        * @param n_data
        *
        */
        void DrawPie(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const ImU32* colors, const int& n_data)
        {                
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            // calculate the total x
            float x_data_total = 0;
            for (int n = 0; n < n_data; ++n)
                x_data_total += x_data[n];

            // float x_data_total = 100.0f; // DELETE when finished testing

            // add each arc
            float x_data_prev_rad = 0.0f;
            for (int n = 0; n < n_data; ++n)
            {                
                const float x_data_rad = IM_PI*2.0f*x_data[n]/x_data_total + x_data_prev_rad;  // convert x_data to radians
                const int n_segments = (int)ceilf(x_data[n]/x_data_total*(float)properties_.pie_segments);  // determine the number of segments
                const ImVec2 centre = ImVec2(
                    (figure.GetScalesX()->GetRangeMin() + figure.GetScalesX()->GetRangeMax())/2,
                    (figure.GetScalesY()->GetRangeMin() + figure.GetScalesY()->GetRangeMax())/2
                );

                // // line 1
                // const ImVec2 vec_line_start = ImVec2(
                //     ImCos(x_data_prev_rad)*properties_.inner_radius + centre.x, 
                //     ImSin(x_data_prev_rad)*properties_.inner_radius + centre.y);
                // const ImVec2 vec_line_end = ImVec2(
                //     ImCos(x_data_prev_rad)*properties_.outer_radius + centre.x, 
                //     ImSin(x_data_prev_rad)*properties_.outer_radius + centre.y);

                // start: end of the outer arc.  end: start of the inner arc
                const ImVec2 vec1 = ImVec2(
                    ImCos(x_data_rad)*properties_.inner_radius + centre.x, 
                    ImSin(x_data_rad)*properties_.inner_radius + centre.y);

                // start: end of the inner arc.  end: start of the outer arc
                const ImVec2 vec2 = ImVec2(
                    ImCos(x_data_prev_rad)*properties_.outer_radius + centre.x, 
                    ImSin(x_data_prev_rad)*properties_.outer_radius + centre.y);

                // draw the pie segment
                // window->DrawList->AddLine(vec_line_start, vec_line_end, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));
                window->DrawList->PathArcTo(centre, properties_.outer_radius, x_data_prev_rad, x_data_rad, n_segments);  // outer arc
                window->DrawList->PathLineTo(vec1);  // start outer to inner arc line
                window->DrawList->PathArcTo(centre, properties_.inner_radius, x_data_rad, x_data_prev_rad, n_segments);  // inner arc 
                window->DrawList->PathLineTo(vec2);  // end inner arc to outer arc line
                window->DrawList->PathFillConvex(colors[n]);

                // add labels

                // [TODO: Tooltip on hover]

                x_data_prev_rad = x_data_rad;
            }
        }

        void SetProperties(ImPieProperties& properties){properties_ = properties;}
    private:
        ImPieProperties properties_;
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
    // - Histogram (density and binning)
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