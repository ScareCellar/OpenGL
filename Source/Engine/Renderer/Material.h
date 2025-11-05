#pragma once
#include "Resources/Resource.h"
#include "GUI/GUI.h"
#include <glm/glm.hpp>

namespace neu {
	class Program;
	class Texture;

	class Material : public Resource, public GUI {
	public:
		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

	public:
		float shininess = 2.0f;
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };
		res_t<Texture> baseMap;
		res_t<Texture> specularMap;
		res_t<Program> program;
		glm::vec3 baseColor{ 1 };

		// Inherited via GUI
		void UpdateGUI() override;
	};

}