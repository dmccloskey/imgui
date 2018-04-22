
#ifndef ITEMS_H
#define ITEMS_H

#include "imgui.h"
#include <stdio.h> // vsnprintf, sscanf, printf

#include <iostream>
#include <string>

// TODO:
// refactor each widget into its own class
// parameterize each widget class with attributes that define the contents of the widget
// add widget for the plotting element (protoype using the lines plots in demo)

namespace example
{
  // typdefs
  typedef char element[256];
  typedef element row[2];
  typedef bool row_checked[3];
  
  // helpers

  // main widgets and widget elements
  static void ShowAppWindow();
  static void ShowAppMainMenuBar();
  static void ShowMenuFile();
  static void SequenceTableWidget(bool* p_open);
  static void TableWidget(bool* p_open);
  static void TableFilterPopup(const char* popuop_id, ImGuiTextFilter filter[], element lines[], bool checked[]);
  static void FileBrowserWidget(bool* p_open);  
  static void PlotWidget(bool* p_open);  

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
    if (show_sequence_table) SequenceTableWidget(&show_sequence_table);

    static bool show_generic_table = true;
    if (show_generic_table) TableWidget(&show_generic_table);
    
    static bool show_file_browser = true;
    if(show_file_browser) FileBrowserWidget(&show_file_browser);
    
    static bool show_plot = true;
    if(show_plot) PlotWidget(&show_plot);

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
      //TODO: open file browser modal
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

  static void SequenceTableWidget(bool* p_open)
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
      for (int i = 0; i < 100; ++i)
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

  static void TableWidget(bool* p_open)
  {
    //TODO: add input for columns and tables (i.e., std::map)
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Generic table", p_open))
    {
      // ImGui::BeginChild("##ScrollingRegion", ImVec2(0, ImGui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);

      // table data
      const char* headers[] = {"Name", "Path"};
      typedef element row[2];
      row columns[] = {
        {"One",  "/path/one"},
        {"Two",  "/path/two"},
        {"Thre",  "/path/three"},
      };
      row_checked columns_checked[] = {
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
      };
      static ImGuiTextFilter filter[3];

      // headers
      ImGui::Columns(IM_ARRAYSIZE(headers) + 1, "mycolumns", true); // 4-ways, with border
      ImGui::Separator();
      ImGui::Text("Index");
      ImGui::NextColumn();
      for (int col = 0; col < IM_ARRAYSIZE(headers); ++col)
      {
        if (ImGui::Button(headers[col]))
          ImGui::OpenPopup(&static_cast<char>(col));
          element elements[IM_ARRAYSIZE(columns)]; // extract out column data
          bool checked[IM_ARRAYSIZE(columns)]; // extract out checked columns
          for (int i = 0; i < IM_ARRAYSIZE(columns); ++i)
          {
            *elements[i] = *columns[i][col];
            checked[i] = columns[i][col];
          }
          TableFilterPopup(&static_cast<char>(col), filter, columns, columns_checked);
        ImGui::NextColumn();
      }

      // body
      ImGui::Separator();
      static int selected = -1;
      for (int i = 0; i < IM_ARRAYSIZE(columns); ++i)
      {
        bool pass_all_columns = true;
        for (int j = 0; j < IM_ARRAYSIZE(headers); ++j)
          if (!filter[j].PassFilter(columns[i][j]))
            pass_all_columns = false;

        if (pass_all_columns)
        {
          char label[32];
          sprintf(label, "%04d", i);
          if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
              selected = i;
          bool hovered = ImGui::IsItemHovered();
          ImGui::NextColumn();
          for (int j = 0; j < IM_ARRAYSIZE(headers); ++j)
          {
            ImGui::Text(columns[i][j]);
            ImGui::NextColumn();
          }
        }
      }
      // ImGui::EndChild();
    }
    ImGui::End();
  }  

  static void TableFilterPopup(const char* popuop_id, ImGuiTextFilter filter[], row columns[], bool checked[])
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
      filter.Draw();
      static bool check_all = true;
      if (ImGui::Button("check all"))
        for (int i = 0; i < IM_ARRAYSIZE(lines); ++i) checked[i] = true;
      ImGui::SameLine();
      if (ImGui::Button("uncheck all"))
        for (int i = 0; i < IM_ARRAYSIZE(lines); ++i) checked[i] = false;
      for (int i = 0; i < IM_ARRAYSIZE(lines); ++i)
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

  static void FileBrowserWidget(bool* p_open)
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
      ImGui::BeginChild("Directories", ImVec2(120, -ImGui::GetFrameHeightWithSpacing()), true);
      for (int i = 0; i < 10; ++i)
      {
        char label[128];
        sprintf(label, "MyObject %d", i);
        if (ImGui::Selectable(label, selected_dir == i))
          selected_dir = i;
      }
      ImGui::EndChild();
      ImGui::SameLine();

      // right: files/folders in highlighted directory
      ImGui::BeginChild("Current Directory", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);      
      // headers
      const char* headers[] = {"Name", "Date Modified", "Type", "Size"};
      ImGui::Columns(IM_ARRAYSIZE(headers) + 1, "mycolumns", true); // with border
      ImGui::Separator();      
      ImGui::Text("index");
      ImGui::NextColumn();
      for (int col = 0; col < IM_ARRAYSIZE(headers); ++col)
      {
        ImGui::Text(headers[col]);
        ImGui::NextColumn();
      }
      // Body
      ImGui::Separator();
      typedef element row[4];
      row columns[] = {
        {"file1", "04/15/2018 2:00 PM", ".csv", "1MB"},
        {"file2", "04/15/2018 2:00 PM", ".txt", "2MB"},
        {"file3", "04/15/2018 2:00 PM", ".mzML", "3MB"},
      };
      static int selected_file = -1;
      for (int i = 0; i < IM_ARRAYSIZE(columns); ++i)
      {
        if (filter.PassFilter(columns[i][0]))
        {
          char label[32];
          sprintf(label, "%04d", i);
          if (ImGui::Selectable(label, selected_file == i, ImGuiSelectableFlags_SpanAllColumns))
              selected_file = i;
          bool hovered = ImGui::IsItemHovered();
          ImGui::NextColumn();
          for (int j = 0; j < IM_ARRAYSIZE(headers); ++j)
          {
            ImGui::Text(columns[i][j]);
            ImGui::NextColumn();
          }
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
        for (int i = 0; i < IM_ARRAYSIZE(file_types); ++i)
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

  static void PlotWidget(bool* p_open)
  {
    //TODO: add input for columns and tables (i.e., std::map)
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Generic plot", p_open))
    {
      // left: filters
      static int selected_dir = 0;
      char* headers[] = {"sample_name", "sample_type", "component_name"}; // feature or sample columns to filter on
      typedef element row[3];
      row columns[] = {
        {"S1", "A", "C1"},
        {"S1", "A", "C2"},
        {"S2", "A", "C1"},
        {"S2", "A", "C2"},
        {"S3", "B", "C1"},
        {"S3", "B", "C2"},
        {"S4", "B", "C1"},
        {"S4", "B", "C2"},
      };
      typedef bool row_checked[3];
      row_checked columns_checked[] = {
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
        {true, true, true},
      };
      static ImGuiTextFilter filter[3];
      ImGui::BeginChild("Filters", ImVec2(200, -ImGui::GetFrameHeightWithSpacing()), true);
      for (int j = 0; j < IM_ARRAYSIZE(headers); ++j)
      {
        if (ImGui::Button(headers[j]))
        {
          ImGui::OpenPopup(headers[j]);
        }
        if (ImGui::BeginPopup(headers[j]))
        {
          filter[j].Draw();
          if (ImGui::Button("check all"))
            for (int i = 0; i < IM_ARRAYSIZE(columns); ++i)
              columns_checked[i][j] = true;
          ImGui::SameLine();
          if (ImGui::Button("uncheck all"))
            for (int i = 0; i < IM_ARRAYSIZE(columns); ++i)
              columns_checked[i][j] = false;           
          for (int i = 0; i < IM_ARRAYSIZE(columns); ++i)
          {
            bool pass_other_filters = true;
            for (int k = 0; k < IM_ARRAYSIZE(headers); ++k)
              if (!columns_checked[i][k])
                bool pass_other_filters = false; 
            if (filter[j].PassFilter(columns[i][j]) && pass_other_filters)
              ImGui::Checkbox(columns[i][j], &columns_checked[i][j]);
          }
          ImGui::EndPopup();
        }
        ImGui::Separator();
      }
      ImGui::EndChild();
      ImGui::SameLine();

      // right (Tab 1)
      ImGui::BeginChild("Figure", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
      ImGui::TextWrapped("TODO: add plot");
      ImGui::EndChild();

      // // right (Tab 2)
      // ImGui::BeginChild("Figure", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
      // ImGui::TextWrapped("TODO: add table");
      // ImGui::EndChild();

      // bottom: selectd filename, filetypes, open/cancel
      ImGui::BeginGroup();
      if (ImGui::Button("Download"))
      {
        // TODO download the plot
      }
      ImGui::SameLine();
      if (ImGui::Button("Options"))
      {
        // TODO open a table with the raw data
      }
      ImGui::SameLine();
      if (ImGui::Button("New Tab"))
      {
        // TODO open a table with the raw data
      }
      ImGui::EndGroup();
    }
    ImGui::End();
  } 
}
#endif