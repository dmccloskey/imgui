// # Low level plotting features and components
#include "imgui.h"
#include "imgui_internal.h"

// ## Data
template<T a, T b>
struct PlotDatum
{
	a       x;                  // x data (float, int, date, etc)
    b       y;                  // y data (float, int, date, etc)
	// ImVec2  pos;                // x and y data (would restrict only to float)
    float   r;                  // circle radius or box half-width
    ImU32   stroke_col;
    float   stroke_width;
    ImU32   fill_col;
	int     caption_id;
    char*   label;              // string identifier for the data point
    ImFont* label_font;
    ImU32   label_font_col;
    float   label_font_size;
    char*   series;
};

template<T a, T b>
typedef ImVector<PlotDatum<a,b>> PlotData; // vector type to store a series of plot data

// ## Plot legends and other features
struct Legend
{
	ImVec2  pos;
    float   width;
    float   height;
    ImU32   col;
    char*   series;
    ImFont* series_font;
    ImU32   series_font_col;
    float   series_font_size;
};

struct ColorBar
{
	float  span;
	ImU32  min;
	ImU32  max;
};

template<T a, T b>
struct ErrorBar
{
	a       dx1; // left
	a       dx2; // right
	b       dy1; // top
	b       dy2; // bottom
    bool   shade; // error shades instead of error bars
};
template<T a, T b>
typedef ImVector<ErrorBar<a,b>> ErrorBars; // vector type to store a series of plot data errors

// ## Interpolation
// moved to math section in imgui_internal.h
// - interpolate
// - sample
// - splines

// ## Scales
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
    void    continuous(); // used for colors
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