// # Low level plotting features and components
#include "imgui.h"
#include "imgui_internal.h"

// # Data
struct PlotData
{
	ImVec2  pos;
	float   radius;
    ImU32   stroke_col;
    float   stroke_width;
    ImU32   fill_col;
	int     caption_id;
    char*   label;
    ImFont* label_font;
    ImU32   label_font_col;
    float   label_font_size;
    char*   series;
};

struct Legend
{
	ImVec2  pos;
    ImVec2  a;
    ImVec2  b;
    ImU32   col;
    char*   series;
    ImFont* series_font;
    ImU32   series_font_col;
    float   series_font_size;
};

struct ErrorBars
{
	float  dx1; // left
	float  dx2; // right
	float  dy1; // top
	float  dy2; // bottom
};

// ## Interpolation
// - interpolate
// - sample
// - splines

// ## Scales
// - band 
// - categorical 
// - colors 
// - continuous 
// - linear 
// - log 
// - nice 
// - ordinal 
// - pow 
// - quantile 
// - quantize 
// - sequential 
// - time
struct Scales
{
    ImVec2  domain;     // input data ranges
    ImVec2  range;      // output data ranges
    // continuous scales
    void    linear();
    void    pow();
    void    log();
    void    time();
    void    sequential();
    // quantized scales
    void    quantize(); // maps a continouus variable to a discrete scale
    void    quantile(ImVec2& range, float& val); // maps a continuous varible to a sampled domain
    void    threshold(); 
    // discrete scales
    void    ordered();
    void    categorical();
    void    band();
    void    round();
    void    point();
};

// ## Shapes
// - arc
// - area
// - areaRadial
// - line
// - lineRadial
// - pie 
// - point
// - pointRadial
// - stack
// - symbol

// ## Axes
struct Axes
{
    void axesTop();
    void axesBottom();
    void axesLeft();
    void axesRight();
    float xTickMajor;
    float xTickMinor;
    float yTickMajor;
    float yTickMinor;
    bool showXAxesGridLines;
    bool showYAxesGridLines;
    char* axesTopTitle;
    char* axesBottomTitle;
    char* axesLeftTitle;
    char* axesRightTitle;
};

// # High level plotting functions

// ## Charts
// - Line
// - Scatter
// - Bar
// - Barh
// - BoxPlot
// - Pie
// - Area
// - Heatmap
// - Contour
// - 


// ## Layouts and hierarchies
// - 
// - 
// - 