
#ifndef ITEMS_H
#define ITEMS_H

#include "imgui.h"

namespace example
{
    static void ShowAppWindow();
    static void ShowAppMainMenuBar();
    static void ShowMenuFile();
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
                //TODO
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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Export ..."))
        {
            //TODO
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Quit", "Alt+F4")) {}
    }
    
}
#endif