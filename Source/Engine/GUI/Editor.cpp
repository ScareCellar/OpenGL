#include "Editor.h"

namespace neu {
	void Editor::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
	}
	void Editor::UpdateGui(Scene& scene)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) m_active = !m_active;
		if (!m_active) return;

		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4{ 1, 1, 0, 1 });

		ImGui::Begin("Scene");
		scene.UpdateGUI();
		ImGui::Separator();

		int index = 0;
		for (auto& actor : scene.m_actors)
		{
			bool done = false;
			ImGui::PushID(index++);
			if (ImGui::Selectable(actor->name.c_str(), actor.get() == m_selected))
			{
				m_selected = actor.get();
			}
			ImGui::PopID();
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_selected) {
			m_selected->UpdateGUI();
		}
		ImGui::End();
		ImGui::PopStyleColor();
	}
}