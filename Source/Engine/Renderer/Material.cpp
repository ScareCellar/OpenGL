#include "Material.h"
#include "Resources/Program.h"


namespace neu{

	bool Material::Load(const std::string& filename) {
		// load program document
		serial::document_t document;
		if (!serial::Load(filename, document)) {
			LOG_WARNING("Could not load program file: {}", filename);
			return false;
		}

		std::string programName;
		SERIAL_READ_NAME(document, "program", programName);

		program = Resources().Get<Program>(programName);

		std::string textureName;
		SERIAL_READ_NAME(document, "baseMap", textureName);
		baseMap = Resources().Get<Texture>(textureName);
		SERIAL_READ_NAME(document, "specularMap", textureName);
		if (!textureName.empty()) specularMap = Resources().Get<Texture>(textureName);

		baseMap = Resources().Get<Texture>(textureName);
		SERIAL_READ(document, shininess);

		/*SERIAL_READ(document, tiling);
		SERIAL_READ(document, offset);*/


		return true;
	}
	void Material::Bind() {
		program->Use();
		if (baseMap) {
			baseMap->SetActive(GL_TEXTURE0);
			baseMap->Bind();
		}

		if (specularMap) {
			specularMap->SetActive(GL_TEXTURE1);
			specularMap->Bind();
		}
		
		program->SetUniform("u_material.shininess", shininess);
		program->SetUniform("u_material.tiling", tiling);
		program->SetUniform("u_material.offset", offset);
	}
	void Material::UpdateGUI()
	{
		// start new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		// set ImGui
		ImGui::Begin("Editor");
		ImGui::DragFloat("Shininess", &shininess, 0.1f);
		ImGui::DragFloat2("Tiling", glm::value_ptr(tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);
		ImGui::End();
	}
}