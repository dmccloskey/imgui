// # Low level plotting features and components
#include "imgui.h"
#include "imgui_internal.h"
namespace ImGui
{
    // ## Interpolation
    // - interpolate
    // - sample
    // - splines
    static inline time_t    ImLerp(const time_t& a, const time_t& b, const time_t& t);                      //TODO [DM]
    static inline float     ImBSBasis(float t1, float v0, float v1, float v2, float v3);                            //TODO [DM] BSpline basis function
    static inline ImVec4    ImBSBasis(const ImVec4& t1, const ImVec4& v0, const ImVec4& v1, const ImVec4& v2, const ImVec4& v3);                            //TODO [DM] BSpline basis function used for colors
    static inline int       ImBSerp(int* v, float t, bool closed = false);                            //TODO [DM] BSpline interpolate from [a, b) or [a, b] (closed=true)
    static inline float     ImBSerp(float* v, float t, bool closed = false);                        //TODO [DM] BSpline interpolate from [a, b) or [a, b] (closed=true)
    static inline ImVec4    ImBSerp(const ImVec4* v, const ImVec4& t, bool closed = false);                        //TODO [DM] BSpline interpolate used for colors from [a, b) or [a, b] (closed=true)
    static inline int*      ImQuaLerp(int a, int b, float t, int n);                        //TODO [DM] Quantized linear sampler
    static inline float*    ImQuaLerp(float a, float b, float t, int n);                        //TODO [DM] Quantized linear sampler
    static inline time_t*   ImQuaLerp(const time_t& a, const time_t& b, const time_t& t, int n);                        //TODO [DM] Quantized linear sampler
    static inline int*      ImQuaBSerp(int a, int b, float t, int n, bool closed = false);                        //TODO [DM] Quantized BSpline sampler
    static inline float*    ImQuaBSerp(float a, float b, float t, int n, bool closed = false);                        //TODO [DM] Quantized BSpline sampler
    static inline ImVec4*   ImQuaBSerp(const ImVec4& a, const ImVec4& b, const ImVec4& t, int n, bool closed = false);                        //TODO [DM] Quantized BSpline sampler used for colors

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
        void Domain(ImVec2 domain);     // input data ranges
        void Range(ImVec2 range);      // output data ranges
    };

    class ImLinearScales: public ImScales
    {
    public:
        // continuous scales
        void    Linear();
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
        void figure(ImVec2 plot_size, const char* title)

        {
            ImGuiWindow* window = GetCurrentWindow();
            if (window->SkipItems)
                return;

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;

            const ImVec2 title_size = CalcTextSize(title, NULL, true);
            if (plot_size.x == 0.0f)
                plot_size.x = CalcItemWidth();
            if (plot_size.y == 0.0f)
                plot_size.y = title_size.y + (style.FramePadding.y * 2);

            const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(plot_size.x, plot_size.y));
            const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
            const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(title_size.x > 0.0f ? style.ItemInnerSpacing.x + title_size.x : 0.0f, 0));
            ItemSize(total_bb, style.FramePadding.y);
            if (!ItemAdd(total_bb, 0, &frame_bb))
                return;
            const bool hovered = ItemHoverable(inner_bb, 0);

            RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
        }
        virtual void ShowPlot();
    private:
        ImVec2 plot_size_;
        const char* title_
    };

    class ImScatterPlot: public ImPlotter
    {
    public:
        virtual void ShowPlot() override;
        void ShowPlot(float x_data[], float y_data[], float r_data[], ImU32 stroke_col, float stroke_width,
            ImU32 fill_col, char* labels[], ImFont* label_font, ImU32 label_font_col,
            float label_font_size, float dx1[], float dx2[], float dy1[], float dy2[])
        {
            //TODO: draw circles
            if (IM_ARRAYSIZE(x_data) > 0)
            {
                int res_w = ImMin((int)graph_size.x, values_count) + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);
                int item_count = values_count + ((plot_type == ImGuiPlotType_Lines) ? -1 : 0);

                // Tooltip on hover
                int v_hovered = -1;
                if (hovered)
                {
                    const float t = ImClamp((g.IO.MousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
                    const int v_idx = (int)(t * item_count);
                    IM_ASSERT(v_idx >= 0 && v_idx < values_count);

                    const float v0 = values_getter(data, (v_idx + values_offset) % values_count);
                    const float v1 = values_getter(data, (v_idx + 1 + values_offset) % values_count);
                    if (plot_type == ImGuiPlotType_Lines)
                        SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx+1, v1);
                    else if (plot_type == ImGuiPlotType_Histogram)
                        SetTooltip("%d: %8.4g", v_idx, v0);
                    v_hovered = v_idx;
                }

                const float t_step = 1.0f / (float)res_w;
                const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

                float v0 = values_getter(data, (0 + values_offset) % values_count);
                float t0 = 0.0f;
                ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
                float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

                const ImU32 col_base = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLines : ImGuiCol_PlotHistogram);
                const ImU32 col_hovered = GetColorU32((plot_type == ImGuiPlotType_Lines) ? ImGuiCol_PlotLinesHovered : ImGuiCol_PlotHistogramHovered);

                for (int n = 0; n < res_w; n++)
                {
                    const float t1 = t0 + t_step;
                    const int v1_idx = (int)(t0 * item_count + 0.5f);
                    IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
                    const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
                    const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

                    // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
                    ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
                    ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, (plot_type == ImGuiPlotType_Lines) ? tp1 : ImVec2(tp1.x, histogram_zero_line_t));
                    if (plot_type == ImGuiPlotType_Lines)
                    {
                        window->DrawList->AddLine(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
                    }
                    else if (plot_type == ImGuiPlotType_Histogram)
                    {
                        if (pos1.x >= pos0.x + 2.0f)
                            pos1.x -= 1.0f;
                        window->DrawList->AddRectFilled(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
                    }

                    t0 = t1;
                    tp0 = tp1;
                }
            }

            //TODO: draw labels

            //TODO: add tool tip

            //TODO: add zoom
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