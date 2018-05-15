// # Low level plotting features and components for creating basic to advanced plots and charts

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "imgui_plot.h"

#include <iostream> //delete when finished debugging


namespace ImGui
{    
    template<class Ta, class Tb>
    void ImPlot<Ta, Tb>::DrawFigure()
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

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::DrawXAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
        const Ta& axis_tick_min, const Ta& axis_tick_max, const Ta& axis_tick_span)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Tick major
        const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
        const float tick_height_spacing = tick_size.y*0.8;

        Ta tick_value = axis_tick_min;           
        while (tick_value <= axis_tick_max)
        {
            // Tick label
            char tick_label[64];
            sprintf(tick_label, axis_tick_format, tick_value);

            // Tick Position
            if (strcmp(orientation, "Top") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->Scale(tick_value), scales_y_->GetRangeMin() + tick_size.y); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }
            else if (strcmp(orientation, "Bottom") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->Scale(tick_value), scales_y_->GetRangeMin() + tick_size.y); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }

            tick_value += axis_tick_span;
        }

        _DrawXAxisAxis(figure, orientation);
        _DrawXAxisTitle(figure, orientation);
    };

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::DrawXAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
        const Ta* axis_tick_pos, const char* axis_tick_labels[])
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Tick major
        const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
        const float tick_height_spacing = tick_size.y*0.8; 

        for (int n=0; n < IM_ARRAYSIZE(axis_tick_pos); ++n)
        {
            // Tick label
            char tick_label[64];
            sprintf(tick_label, axis_tick_format, axis_tick_labels[n]);

            // Tick Position
            if (strcmp(orientation, "Top") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->Scale(axis_tick_pos[n]), scales_y_->GetRangeMax() - tick_size.y); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }
            else if (strcmp(orientation, "Bottom") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->Scale(axis_tick_pos[n]), scales_y_->GetRangeMin() + tick_size.y); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }
        }

        _DrawXAxisAxis(figure, orientation);
        _DrawXAxisTitle(figure, orientation);
    };
    

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::_DrawXAxisAxis(ImPlot<Ta, Tb>& figure, const char* orientation)
    {      
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Axis
        if (strcmp(orientation, "Top") == 0)
        {
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMax()),
                ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMax()), axis_col, axis_thickness);
        }
        else if (strcmp(orientation, "Bottom") == 0)
        {
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMin()),
                ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMin()), axis_col, axis_thickness);
        }
    };

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::_DrawXAxisTitle(ImPlot<Ta, Tb>& figure, const char* orientation)
    {      
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Tick major
        const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
        const float tick_height_spacing = tick_size.y*0.8;

        // Axis title
        const ImVec2 title_size = CalcTextSize(axis_title, NULL, true);
        const float title_height_spacing = title_size.y*0.8;

        if (strcmp(orientation, "Top") == 0)
        {
            ImVec2 title_pos = ImVec2((scales_x_->GetRangeMax() - scales_x_->GetRangeMin())*0.5f + title_size.x*0.5f,
                scales_y_->GetRangeMax() - (title_size.y + tick_size.y));
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        }
        else if (strcmp(orientation, "Bottom") == 0)
        {
            ImVec2 title_pos = ImVec2((scales_x_->GetRangeMax() - scales_x_->GetRangeMin())*0.5f + title_size.x*0.5f,
                scales_y_->GetRangeMin() + (title_size.y + tick_size.y));
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        }
    };

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::DrawYAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
        const Tb& axis_tick_min, const Tb& axis_tick_max, const Tb& axis_tick_span)
    {        
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Tick major
        const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
        const float tick_height_spacing = tick_size.y*0.8;

        Tc tick_value = axis_tick_min;           
        while (tick_value <= axis_tick_max)
        {
            // Tick label
            char tick_label[64];
            sprintf(tick_label, axis_tick_format, tick_value);

            // Tick Position
            if (strcmp(orientation, "Left") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMin() - tick_size.y, scales_y_->Scale(tick_value)); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }
            else if (strcmp(orientation, "Right") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMax() + tick_size.y, scales_y_->Scale(tick_value)); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }

            tick_value += axis_tick_span;
        }

        _DrawYAxisAxis(figure, orientation);
        _DrawYAxisTitle(figure, orientation);
    };

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::DrawYAxis(ImPlot<Ta, Tb>& figure, const char* orientation,
        const Tb* axis_tick_pos, const char* axis_tick_labels[])
    {      
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Tick major
        const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
        const float tick_height_spacing = tick_size.y*0.8;
   
        for (int n=0; n < IM_ARRAYSIZE(axis_tick_pos); ++n)
        {
            // Tick label
            char tick_label[64];
            sprintf(tick_label, axis_tick_format, axis_tick_labels[n]);

            if (strcmp(orientation, "Left") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMin() - tick_size.y, scales_y_->Scale(axis_tick_pos[n])); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }
            else if (strcmp(orientation, "Right") == 0)
            {
                ImVec2 tick_pos = ImVec2(scales_x_->GetRangeMax() + tick_size.y, scales_y_->Scale(axis_tick_pos[n])); // interpolate the position
                window->DrawList->AddText(axis_tick_font, axis_tick_font_size, tick_pos, axis_font_col, tick_label);
            }
        }

        _DrawYAxisAxis(figure, orientation);
        _DrawYAxisTitle(figure, orientation);
    };

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::_DrawYAxisAxis(ImPlot<Ta, Tb>& figure, const char* orientation)
    {      
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Axis
        if (strcmp(orientation, "Left") == 0)
        {
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMin()),
                ImVec2(scales_x_->GetRangeMin(), scales_y_->GetRangeMax()), axis_col, axis_thickness);
        }
        else if (strcmp(orientation, "Right") == 0)
        {
            window->DrawList->AddLine(ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMin()),
                ImVec2(scales_x_->GetRangeMax(), scales_y_->GetRangeMax()), axis_col, axis_thickness);
        }
    };

    template<class Ta, class Tb>
    void ImAxis<Ta, Tb>::_DrawYAxisTitle(ImPlot<Ta, Tb>& figure, const char* orientation)
    {      
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        // Tick major
        const ImVec2 tick_size = CalcTextSize(axis_title, NULL, true);
        const float tick_height_spacing = tick_size.y*0.8;

        // Axis title
        const ImVec2 title_size = CalcTextSize(axis_title, NULL, true);
        const float title_height_spacing = title_size.y*0.8;

        if (strcmp(orientation, "Left") == 0)
        {
            ImVec2 title_pos = ImVec2(
                scales_x_->GetRangeMin() - (title_size.y + tick_size.y),
                (scales_y_->GetRangeMax() - scales_y_->GetRangeMin())*0.5f + title_size.y*0.5f);
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        }
        else if (strcmp(orientation, "Right") == 0)
        {
            ImVec2 title_pos = ImVec2(
                scales_x_->GetRangeMin() - (title_size.y + tick_size.y),
                (scales_y_->GetRangeMax() - scales_y_->GetRangeMin())*0.5f + title_size.y*0.5f);
            window->DrawList->AddText(axis_title_font, axis_title_font_size, title_pos, axis_title_font_col, axis_title);
        }
    };

    template<class Ta, class Tb>
    void ImLegend<Ta, Tb>::ShowLegend(ImPlot<Ta, Tb>& figure, const char* pos, 
            const char* series[], const ImU32 series_color[], const int& n_series)
    {
        ImGuiWindow* window = figure.GetCurrentWindow();
        if (window->SkipItems)
            return;

        //assert(IM_ARRAYSIZE(series) == IM_ARRAYSIZE(series_color));

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
    
    template<class Ta, class Tb>
    void ImLabels<Ta, Tb>::DrawLabels(ImPlot<Ta, Tb>& figure,
        const Ta* x_data, const Tb* y_data, const int& n_data, const char* labels[])
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;

        if (labels == NULL) return;

        int t0 = 0;
        for (int n = 0; n < n_data; ++n)
        {            
            // labels
            const float centre_scaled_x = scales_x_->Scale(x_data[n]);
            const float centre_scaled_y = scales_y_->Scale(y_data[n]);
            window->DrawList->AddText(label_font, label_font_size, ImVec2(centre_scaled_x, centre_scaled_y), label_font_col, labels[n]);
            t0 += 1;
        }
    };
    
    template<class Ta, class Tb>
    void ImMarkers<Ta, Tb>::DrawMarkers(ImPlot<Ta, Tb>& figure,
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
            const float centre_scaled_x = scales_x_->Scale(x_data[n]);
            const float centre_scaled_y = scales_y_->Scale(y_data[n]);
            window->DrawList->AddCircleFilled(ImVec2(centre_scaled_x, centre_scaled_y), r_data[n], marker_fill_col, 12);

            // Tooltip on hover
            if (centre_scaled_x - r_data[n] <= g.IO.MousePos.x && 
            centre_scaled_x + r_data[n] >= g.IO.MousePos.x &&
            centre_scaled_y - r_data[n] <= g.IO.MousePos.y && 
            centre_scaled_y + r_data[n] >= g.IO.MousePos.y)
            {
                SetTooltip("%s\n%s: %8.4g\n%s: %8.4g", series, "x", x_data[n], "y", y_data[n]);
                window->DrawList->AddCircleFilled(ImVec2(centre_scaled_x, centre_scaled_y), r_data[n], marker_hovered_col, 12);
            }
        }
    };

    template<class Ta, class Tb>
    void ImLines<Ta, Tb>::DrawLines(ImPlot<Ta, Tb>& figure,
        const Ta* x_data, const Tb* y_data, const int& n_data)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        int t0 = 0;
        for (int n = 0; n < n_data; ++n)
        {
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
        }
    };

    template<class Ta, class Tb>
    void ImLines<Ta, Tb>::DrawBars(ImPlot<Ta, Tb>& figure,
        const Tb* y_data, const int& n_data,
        const Ta& bar_offset, const Tb* bar_bottoms,
        const char* series)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        ImGuiContext& g = *GImGui;

        const float bar_span = (scales_x_->GetRangeMax() - scales_x_->GetRangeMin())/n_data;   
        for (int n = 0; n < n_data; ++n)
        {
            // Bars
            const ImVec2 bar_BL = ImVec2(scales_x_->GetRangeMin() + n*bar_span + bar_offset,
                scales_y_->GetRangeMin() + scales_y_->Scale(bar_bottoms[n]));
            const ImVec2 bar_TR = ImVec2(scales_x_->GetRangeMin() + n*bar_span + bar_offset,
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

            // // labels
            // if (labels != NULL)
            // {                        
            //     const float centre_scaled_x = n*bar_span + bar_offset + scales_x_->GetRangeMin() * 0.5;
            //     const float centre_scaled_y = scales_y_->GetRangeMin() - scales_y_->Scale(bar_bottoms[n]) - scales_y_->Scale(y_data[n]);
            //     window->DrawList->AddText(label_font, label_font_size, ImVec2(centre_scaled_x, centre_scaled_y), label_font_col, labels[n]);
            // }
        }
    }
}