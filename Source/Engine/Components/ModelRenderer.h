#pragma once
#include "RendererComponent.h"

namespace neu {
	class ModelRenderer : public RendererComponent {
		CLASS_PROTOTYPE(ModelRenderer);
	public:
	
		

		// Inherited via RendererComponent
		void Update(float dt) override;

		void Draw(Renderer& renderer) override;

		void Read(const serial_data_t& value);
		void UpdateGUI() override;

		res_t<Model> model;
		res_t<Material> material;
	};
}