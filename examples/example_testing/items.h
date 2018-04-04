
#ifndef ITEMS_H
#define ITEMS_H

#include "imgui.h"
#include <stdio.h> // vsnprintf, sscanf, printf

namespace example
{
  static void ShowAppWindow();
  static void ShowAppMainMenuBar();
  static void ShowMenuFile();
  static void ShowSequenceTable(bool* p_open);
  static void ShowGenericTable(bool* p_open);
  static void ShowTableFilterPopup(const char* popuop_id);

  static void ShowAppWindow()
  {
    ShowAppMainMenuBar();
  }

  // Demonstrate creating a fullscreen menu bar and populating it.
  static void ShowAppMainMenuBar()
  {
    static bool show_app_about = false;
    if (show_app_about)
    {
      ImGui::Begin("About SmartPeak", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("SmartPeak %s", ImGui::GetVersion()); //TODO: define version function
      ImGui::Separator();
      ImGui::Text("By Douglas McCloskey and contributors.");
      ImGui::Text("SmartPeak is licensed under the MIT License, see LICENSE for more information.");
      ImGui::End();
    }

    static bool show_sequence_table = false;
    if (show_sequence_table) ShowSequenceTable(&show_sequence_table);

    static bool show_generic_table = true;
    if (show_generic_table) ShowGenericTable(&show_generic_table);

    

    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        ShowMenuFile();
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit"))
      {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View"))
      {
        ImGui::MenuItem("Sequence table", NULL, &show_sequence_table);
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Settings"))
      {
        //TODO
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Tools"))
      {
        //TODO
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Help"))
      {
        ImGui::MenuItem("About", NULL, &show_app_about);
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  static void ShowMenuFile()
  {
    ImGui::MenuItem("Main menu", NULL, false, false);
    if (ImGui::MenuItem("New"))
    {
      //TODO
    }
    if (ImGui::MenuItem("Open", "Ctrl+O")) 
    {
      //TODO
    }
    if (ImGui::BeginMenu("Open Recent"))
    {
      //TODO: populate with recent
      ImGui::MenuItem("fish_hat.c");
      ImGui::MenuItem("fish_hat.inl");
      ImGui::MenuItem("fish_hat.h");
      ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S"))
    {
      //TODO
    }
    if (ImGui::MenuItem("Save As.."))
    {
      //TODO: open save as modal
    }
    ImGui::Separator();
    if (ImGui::BeginMenu("Import ..."))
    {
      //TODO
      // sequence, parameters, and all other tables
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Export ..."))
    {
      //TODO
      // sequence, parameters, and all other tables
      ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
  }

  static void ShowSequenceTable(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Sequence list", p_open, NULL))
    {
      // left
      static int selected = 0;
      ImGui::BeginChild("left pane", ImVec2(150, 0), true);
      for (int i = 0; i < 100; i++)
      {
        char label[128];
        sprintf(label, "MyObject %d", i);
        if (ImGui::Selectable(label, selected == i))
          selected = i;
      }
      ImGui::EndChild();
      ImGui::SameLine();

      // right
      ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
          ImGui::Text("MyObject: %d", selected);
          ImGui::Separator();
          ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
        ImGui::EndChild();
        if (ImGui::Button("Revert")) {}
        ImGui::SameLine();
        if (ImGui::Button("Save")) {}
      ImGui::EndGroup();
    }
    ImGui::End();
  }  
  static void ShowGenericTable(bool* p_open)
  {
    //TODO: add input for columns and tables (i.e., std::map)
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Generic table", p_open))
    {
      // ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);

      // headers
      const char* columns[] = {"ID", "Name", "Path", "Hovered"};
      ImGui::Columns(IM_ARRAYSIZE(columns), "mycolumns", true); // 4-ways, with border
      ImGui::Separator();
      for (int col = 0; col < IM_ARRAYSIZE(columns); ++col)
      {
        if (ImGui::Button(columns[col]))
          ImGui::OpenPopup(&static_cast<char>(col));
          ShowTableFilterPopup(&static_cast<char>(col));
        ImGui::NextColumn();
      }

      // body
      ImGui::Separator();
      const char* names[3] = { "One", "Two", "Three" };
      const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
      static int selected = -1;
      for (int i = 0; i < 3; i++)
      {
        char label[32];
        sprintf(label, "%04d", i);
        if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
            selected = i;
        bool hovered = ImGui::IsItemHovered();
        ImGui::NextColumn();
        ImGui::Text(names[i]); ImGui::NextColumn();
        ImGui::Text(paths[i]); ImGui::NextColumn();
        ImGui::Text("%d", hovered); ImGui::NextColumn();
      }
      // ImGui::EndChild();
    }
    ImGui::End();
  }  

  static void ShowTableFilterPopup(const char* popuop_id)
  {
    if (ImGui::BeginPopup(popuop_id))
    { 
      // Sorting
      if (ImGui::Button("Asc"))
      {
        //TODO filter ascending
      }
      ImGui::SameLine();
      if (ImGui::Button("Desc"))
      {
        //TODO filter descending
      }

      // Filtering and selecting
      ImGui::Separator();
      static ImGuiTextFilter filter;
      filter.Draw();
      const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
      static bool checked[] = { true, true, true, true, true, true, true, true }; 
      static bool check_all = true;
      if (ImGui::Button("check all"))
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++) checked[i] = true;
      ImGui::SameLine();
      if (ImGui::Button("uncheck all"))
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++) checked[i] = false;
      for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
        if (filter.PassFilter(lines[i]))
          ImGui::Checkbox(lines[i], &checked[i]);

      // Apply filters
      ImGui::Separator();
      if (ImGui::Button("Accept"))
      {
        //TODO: apply filter and exit
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel"))
      {
        //TODO: exit without applying filter
      }
      ImGui::EndPopup();
    }

  }
}
#endif