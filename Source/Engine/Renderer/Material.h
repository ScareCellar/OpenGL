#pragma once
#include "Resources/Resource.h"
#include "GUI/GUI.h"
#include <glm/glm.hpp>

namespace neu {
	class Program;
	class Texture;

	class Material : public Resource, public GUI {
	public:
		enum class Parameters : uint32_t{
			None		= 0,
			BaseMap		= (1 << 0),
			SpecularMap = (1 << 1),
			EmissiveMap = (1 << 2),
			NormalMap	= (1 << 3)
		};
	
	public:



		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

	public:
		res_t<Program> program;
		res_t<Texture> baseMap;
		glm::vec3 baseColor{ 1 };
		res_t<Texture> specularMap;
		res_t<Texture> emissiveMap;
		glm::vec3 emissiveColor{ 0,0,0 };

		float shininess = 2.0f;
		glm::vec2 tiling{ 1,1 };
		glm::vec2 offset{ 0,0 };
		
		Parameters parameters;

		// Inherited via GUI
		void UpdateGUI() override;
	};

}