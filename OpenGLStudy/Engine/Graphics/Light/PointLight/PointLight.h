#pragma once
#include "Graphics/Light/Light.h"

namespace Graphics {

	class cPointLight : public cGenLight
	{
	public:
		cPointLight() : m_const(1), m_linear(0), m_quadratic(0), cGenLight()
		{}
		cPointLight(Color i_color, const glm::vec3& i_position, const GLfloat i_range,GLfloat i_const, GLfloat i_linear, GLfloat i_quadratic);
		virtual ~cPointLight() {
		}

		/** overriding virtual functions*/
		virtual void Illuminate() override;
		virtual void SetupLight(const GLuint& i_programID, GLuint i_lightIndex = 0) override;

		protected:
		// for attenuation calculation: c+bx+ax^2
		GLfloat m_range, m_const, m_linear, m_quadratic;
	};

}