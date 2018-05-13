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
}