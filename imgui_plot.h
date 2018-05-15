// # Low level plotting features and components for creating basic to advanced plots and charts

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

    template<class Ta, class Tb>
    class ImPlot
    {
    public:
        /**
         * @brief Draw the figure with an optional title
         * 
         */
        void DrawFigure();

        void SetProperties(ImPlotProperties& properties){properties_ = properties;}
        void SetScales(ImScales<Ta, float>* scales_x, ImScales<Tb, float>* scales_y){scales_x_ = scales_x; scales_y_ = scales_y;}
        ImScales<Ta, float>* GetScalesX(){return scales_x_;}
        ImScales<Tb, float>* GetScalesY(){return scales_y_;}
        ImGuiWindow* GetCurrentWindow(){return GetCurrentWindow();}

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
    };

    template<class Ta, class Tb>
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
            const Ta& axis_tick_min, const Ta& axis_tick_max, const Ta& axis_tick_span);

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
            const Ta* axis_tick_pos, const char* axis_tick_labels[]);

        void _DrawXAxisAxis(ImPlot<Ta, Tb>& figure, const char* orientation);
        void _DrawXAxisTitle(ImPlot<Ta, Tb>& figure, const char* orientation);

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
            const Tb& axis_tick_min, const Tb& axis_tick_max, const Tb& axis_tick_span);

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
            const Tb* axis_tick_pos, const char* axis_tick_labels[]);

        void _DrawYAxisAxis(ImPlot<Ta, Tb>& figure, const char* orientation);
        void _DrawYAxisTitle(ImPlot<Ta, Tb>& figure, const char* orientation);
        
        /**
        * @brief Draw Gridlines
        * 
        * @param figure The figure to draw on
        * @param orientation Options are vertical, horizontal
        *
        */
        void DrawGridLines(ImPlot<Ta, Tb>& figure, const char* orientation);
        
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

    template<class Ta, class Tb>
    class ImLegend
    {
    public:
        
        /**@brief Show a plot legend
         * 
         * @param figure The figure to draw on
         * @param pos Pos of the legend (TL, TR, BL, BR)
         * @param col Background color of the legend
         * @param series List of series labels
         * @param series_color List of series colors 
         */
        void ShowLegend(ImPlot<Ta, Tb>& figure, const char* pos, 
            const char* series[], const ImU32 series_color[], const int& n_series);

        void SetProperties(ImLegendProperties& properties){properties_ = properties;}

    private:
        ImLegendProperties properties_;
    };

    // ## Error Bars
    struct ImErrorBarProperties
    {
        ImU32 error_bar_stroke_col = 1.0f;
        float error_bar_stroke_width = 1.0f;
        char* error_bar_cap_style = "Straight"; ///< Options are "Straight", "Circular"
        float error_bar_cap_width = 4.0f;
    };

    template<class Ta, class Tb>
    class ImErrorBar
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
        void DrawErrorBars(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data,
            const Ta* dx1, const Ta* dx2);

        /**
        * @brief Draw Error Bars
        * 
        * @param figure The figure to draw on
        * @param dy1 Upper error bar lengths
        * @param dy2 Lower error bar lengths
        *
        */
        void DrawErrorBars(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data,
            const Tb* dy1, const Tb* dy2);
        
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

    template<class Ta, class Tb>
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
            const Ta* x_data, const Tb* y_data, const int& n_data, const char* labels[]);

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
    };

    template<class Ta, class Tb>
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
            const char* series);

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

    template<class Ta, class Tb>
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
            const Ta* x_data, const Tb* y_data, const int& n_data);

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
    template<class Ta, class Tb>
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
            const char* series);

        /**
        * @brief Draw horizontal Bars
        * 
        * @param figure The figure to draw on
        * @param y_data
        * @param n_data
        *
        */
        void DrawBars(ImPlot<Ta, Tb>& figure,
            const Ta* x_data, const int& n_data,
            const Tb& bar_offset, const Ta* bar_bottoms);

        void SetProperties(ImBarProperties& properties){properties_ = properties;}
    private:
        ImBarProperties properties_;
    };

    // ## Pie (for e.g., pie or donught plot)
    struct ImPieProperties
    {
        float inner_radius = 0.0f; ///< change to create a donught plot
        float outer_radius = 100.0f; ///< controls the size of the pie
    };

    template<class Ta, class Tb>
    class ImPie
    {
    public:
        /**
        * @brief Draw Pie
        * 
        * @param figure The figure to draw on
        * @param x_data
        * @param y_data
        * @param n_data
        *
        */
        void DrawPie(ImPlot& figure,
            const Ta* x_data, const Tb* y_data, const int& n_data);

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
            const int& axis_ticks, const char* axis_tick_labels[], const char* axis_tick_format,
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
            const int& axis_ticks, const char* axis_tick_labels[], const char* axis_tick_format,
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

            if (axis_tick_labels!=NULL)
            {
                const Ta tick_span = (scales_x_->GetRangeMax() - scales_x_->GetRangeMin())/axis_ticks;            
                for (int n=0; n<axis_ticks-1; ++n)
                {
                    // Tick label
                    char tick_label[64];
                    const Ta tick_pos_x = tick_span * n + tick_span;

                    // Tick Position
                    sprintf(tick_label, axis_tick_format, axis_tick_labels[n]);
                    ImVec2 tick_pos = ImVec2(tick_pos_x, scales_y_->GetRangeMin() + tick_size.y); // interpolate the position
                    window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
                }
            }
            else
            {
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
         * @param axis_ticks The number of axis ticks to show
         *      [min, max] if axis_tick_labels are not specified (e.g., scatter or line plot)
         *      [min + span, max - span] if axis_tick_labels are specified (e.g., bar plot)
         * @param axis_tick_labels labels to use (optional)
         */
        void ShowYAxisLeft(const char* axis_title, const ImFont* axis_title_font, 
            const float& axis_title_font_size, const ImU32& axis_title_font_col,
            const Tc& axis_tick_min, const Tc& axis_tick_max, const Tc& axis_tick_span, const char* axis_tick_format,
            const Tc* axis_tick_pos, const char* axis_tick_labels[], 
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

            if (axis_tick_labels!=NULL && axis_tick_pos!=NULL)
            {         
                for (int n=0; n < IM_ARRAYSIZE(axis_tick_pos); ++n)
                {
                    // Tick label
                    char tick_label[64];
                    sprintf(tick_label, axis_tick_format, axis_tick_labels[n]);

                    // Tick Position
                    ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMin() - tick_size.y, scales_y_->Scale(axis_tick_pos[n])); // interpolate the position
                    window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
                }
            }
            else
            {
                Tc tick_value = axis_tick_min;           
                while (tick_value <= axis_tick_max)
                {
                    // Tick label
                    char tick_label[64];

                    // Tick Position
                    sprintf(tick_label, axis_tick_format, tick_value);
                    ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMin() - tick_size.y, scales_y_->Scale(tick_value)); // interpolate the position
                    window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);

                    tick_value += axis_tick_span;
                }
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
            const int& axis_ticks, const char* axis_tick_labels[], const char* axis_tick_format,
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

    template<class Ta, class Tc>
    class ImBarPlot2D: public ImPlot2D<Ta, Tc>
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
        void ShowPlot(const float x_offset, const Tc* y_data, 
            const float bar_width, const Tc* bar_bottoms, const int& n_data,
            const ImU32& bar_stroke_col, const float& bar_stroke_width,
            const ImU32& bar_fill_col, const ImU32& bar_hovered_col,
            const char* series, const char* labels[], 
            const ImFont* label_font, const ImU32& label_font_col,
            const float& label_font_size,
            const float* dy1, const float* dy2)
        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;

            if (n_data > 0)
            {
                const float bar_span = (scales_x_->GetRangeMax() - scales_x_->GetRangeMin())/n_data;   
                for (int n = 0; n < n_data; ++n)
                {
                    // Bars
                    const ImVec2 bar_BL = ImVec2(scales_x_->GetRangeMin() + n*bar_span + x_offset,
                        scales_y_->GetRangeMin() + scales_y_->Scale(bar_bottoms[n]));
                    const ImVec2 bar_TR = ImVec2(scales_x_->GetRangeMin() + n*bar_span + x_offset,
                        scales_y_->GetRangeMin() + scales_y_->Scale(bar_bottoms[n]) + scales_y_->Scale(y_data[n]));
                    window->DrawList->AddRectFilled(bar_BL, bar_TR, bar_fill_col);

                    // Tooltip on hover
                    if (bar_TR.x <= g.IO.MousePos.x && 
                    bar_BL.x >= g.IO.MousePos.x &&
                    bar_TR.y >= g.IO.MousePos.y && 
                    bar_BL.y <= g.IO.MousePos.y)
                    {
                        SetTooltip("%s\n%s: %8.4g", series, "y", y_data[n]);
                        window->DrawList->AddRectFilled(bar_BL, bar_TR, bar_hovered_col);
                    }

                    // labels
                    if (labels != NULL)
                    {                        
                        const float centre_scaled_x = n*bar_span + x_offset + scales_x_->GetRangeMin() * 0.5;
                        const float centre_scaled_y = scales_y_->GetRangeMin() - scales_y_->Scale(bar_bottoms[n]) - scales_y_->Scale(y_data[n]);
                        window->DrawList->AddText(label_font, label_font_size, ImVec2(centre_scaled_x, centre_scaled_y), label_font_col, labels[n]);
                    }

                    // Error bars
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