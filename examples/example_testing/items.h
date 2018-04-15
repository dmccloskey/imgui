
#ifndef ITEMS_H
#define ITEMS_H

#include "imgui.h"
#include <stdio.h> // vsnprintf, sscanf, printf

// TODO:
// refactor each widget into its own class
// parameterize each widget class with attributes that define the contents of the widget
// add widget for the plotting element (protoype using the lines plots in demo)

namespace example
{
  static void ShowAppWindow();
  static void ShowAppMainMenuBar();
  static void ShowMenuFile();
  static void ShowSequenceTable(bool* p_open);
  static void ShowGenericTable(bool* p_open);
  static void ShowTableFilterPopup(const char* popuop_id);
  static void ShowFileBrowser(bool* p_open);  

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

    static bool show_file_browser = true;
    if(show_file_browser) ShowFileBrowser(&show_file_browser);
    

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
      //TODO: SQL light interface
    }
    if (ImGui::MenuItem("Open...", "Ctrl+O")) 
    {
      //TODO: File browser modal
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
      //TODO: open save as File browser modal
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

      // TODO add ability to search by:
      // - sample_name
      // - sequence_group_name
      // - sample_group_name
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

  static void ShowFileBrowser(bool* p_open)
  {
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("File Browser", p_open, NULL))
    {
      // top: up level, current directory string, refresh icon, search
      // ImGui::BeginChild("Top");
      if (ImGui::Button("Up"))
      {
        // up one directory
      }
      ImGui::SameLine();      
      static char bufdirname[64] = "path/to/current/dir";
      ImGui::InputText("", bufdirname, 64);
      ImGui::SameLine(); 
      if (ImGui::Button("Refresh"))
      {
        // Refresh right panel and search
      }
      ImGui::SameLine();  
      static ImGuiTextFilter filter;
      filter.Draw();
      // ImGui::EndChild();
      ImGui::Separator();

      // left: current directory folders (recursive tree)
      static int selected_dir = 0;
      ImGui::BeginChild("Directories", ImVec2(120, 0), true);
      for (int i = 0; i < 10; i++)
      {
        char label[128];
        sprintf(label, "MyObject %d", i);
        if (ImGui::Selectable(label, selected_dir == i))
          selected_dir = i;
      }
      ImGui::EndChild();
      ImGui::SameLine();

      // right: files/folders in highlighted directory
      ImGui::BeginChild("Current Directory");      
      // headers
      const char* columns[] = {"Name", "Date Modified", "Type", "Size"};
      ImGui::Columns(IM_ARRAYSIZE(columns), "mycolumns", true); // 4-ways, with border
      ImGui::Separator();
      for (int col = 0; col < IM_ARRAYSIZE(columns); ++col)
      {
        ImGui::Text(columns[col]);
        ImGui::NextColumn();
      }
      // Body
      ImGui::Separator();
      const char* current_dir_names[] = { "file1", "file2", "file3" };
      const char* date_modified[] = { "04/15/2018 2:00 PM", "04/15/2018 2:00 PM", "04/15/2018 2:00 PM" };
      const char* type[] = { ".csv", ".txt", ".mzML" };
      const char* size[] = { "1MB", "2MB", "3MB" };
      static int selected_file = -1;
      for (int i = 0; i < IM_ARRAYSIZE(current_dir_names); i++)
      {
        if (filter.PassFilter(current_dir_names[i]))
        {
          char label[64];
          sprintf(label, current_dir_names[i]);
          if (ImGui::Selectable(label, selected_file == i, ImGuiSelectableFlags_SpanAllColumns))
              selected_file = i;
          bool hovered = ImGui::IsItemHovered();
          ImGui::NextColumn();
          ImGui::Text(date_modified[i]); ImGui::NextColumn();
          ImGui::Text(type[i]); ImGui::NextColumn();
          ImGui::Text(size[i]); ImGui::NextColumn();
        }
      }
      ImGui::EndChild();

      // bottom: selectd filename, filetypes, open/cancel
      ImGui::BeginGroup();
      // Filtering and selecting
      ImGui::Separator();
      ImGui::Text("File Name:");
      ImGui::SameLine();
      static char buffilename[64] = "file_name";
      ImGui::InputText("", buffilename, 64);
      ImGui::SameLine();            
      const char* file_types[] = { ".csv", ".txt", ".mzML", "." };
      if (ImGui::BeginMenu("File Type"))
      {
        for (int i = 0; i < IM_ARRAYSIZE(file_types); i++)
        {
          ImGui::MenuItem(file_types[i]);
        }
        // ImGui::SameLine(); //TODO: display selected filetype
        // ImGui::Text(file_types[i]);
        ImGui::EndMenu();
      }
      if (ImGui::Button("Accept"))
      {
        // TODO read in the file
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel"))
      {
        // TODO close the modal
      }
      ImGui::EndGroup();
    }
    ImGui::End();
  }  

  // Show plotting widget
  // Provide filters for the following:
  // - sample_name, sequence_group_name, sample_group_name
  // - component_name, component_group_name
  // - sample status: Pass, Warn, Fail
  // Provide a scatter plot with the following
  // - chromatographic points in blue
  // - integrated points in red
  // Bonus:
  // - allows users to click points include/exclude
  //   from the peak
}
#endif