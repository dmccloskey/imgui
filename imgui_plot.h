// # Low level plotting features and components
#include "imgui.h"
#include "imgui_internal.h"

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
    void ShowAxesTop(char* title, float x_tick_major, float x_tick_minor);
    void ShowAxesBottom(char* , float x_tick_major, float x_tick_minor);
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
    virtual void ShowPlot();
};

class ImScatterPlot: public ImPlotter
{
public:
    virtual void ShowPlot() override;
    void ShowPlot()
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