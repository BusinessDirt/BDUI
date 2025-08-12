#include "Mixture/Renderer/ImGuiWidgets.hpp"

namespace Mixture
{
    ImGuiID ImGuiWidgets::Dockspace(const std::string_view& windowTitle, bool fullscreen, ImGuiWindowFlags windowFlagsOverride,
                            ImGuiDockNodeFlags dockspaceFlags, bool* pOpen)
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            // Optional: remove window rounding and border for fullscreen
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        }

        // Apply override flags if provided
        window_flags |= windowFlagsOverride;

        ImGui::Begin(windowTitle.data(), pOpen, window_flags);

        if (fullscreen)
            ImGui::PopStyleVar(2);

        // Create dockspace
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspaceFlags);

        ImGui::End();

        return dockspace_id;
    }
}
