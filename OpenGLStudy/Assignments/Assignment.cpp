#include "Assignment.h"
#include "Application/Window/Window.h"
#include "Application/Window/WindowInput.h"
#include "Constants/Constants.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Graphics/Graphics.h"
#include "Time/Time.h"

#include "Graphics/Camera/EditorCamera/EditorCamera.h"
#include "Light/DirectionalLight/DirectionalLight.h"
#include "FrameBuffer/cFrameBuffer.h"
#include "Material/Material.h"
#include "Actor/Actor.h"
#include "Transform/Transform.h"
#include "Engine/Graphics/Model/Model.h"
#include "Material/Blinn/MatBlinn.h"
#include "Graphics/Texture/Texture.h"
#include <map>

bool Assignment::Initialize(GLuint i_width, GLuint i_height, const char* i_windowName /*= "Default Window"*/)
{
	auto result = true;
	if (!(result = cApplication::Initialize(i_width, i_height, i_windowName))) {
		assert(false);
		// TODO: LogError
		printf("Failed to initialize Application!");
		return false;
	}

	CreateActor();
	CreateCamera();
	CreateLight();

	Graphics::cMatBlinn* _wallMat = dynamic_cast<Graphics::cMatBlinn*>(Graphics::cModel::s_manager.Get(m_wall->GetModelHandle())->GetMaterialAt());
	auto cameraViewTextureHandle = Graphics::GetCameraCaptureFrameBuffer()->GetTextureHandle();
	_wallMat->UpdateDiffuseTexture(cameraViewTextureHandle);
	_wallMat->UpdateSpecularTexture(cameraViewTextureHandle);
	return result;
}

void Assignment::CreateActor()
{
	m_teapot = new cActor();
	m_teapot->Initialize();
	m_teapot->Transform()->Translate(glm::vec3(0,0, -2.f));
	m_teapot->Transform()->Rotate(glm::vec3(1, 0, 0), -90.f);
	m_teapot->Transform()->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	m_teapot->SetModel("Contents/models/teapot.model");
	m_teapot->UpdateUniformVariables(Graphics::GetCurrentEffect());

	m_teapot2 = new cActor();
	m_teapot2->Initialize();
	m_teapot2->Transform()->Translate(glm::vec3(2, 0, -3.f));
	m_teapot2->Transform()->Rotate(glm::vec3(1, 0, 0), -90.f);
	m_teapot2->Transform()->Scale(glm::vec3(0.03f, 0.03f, 0.03f));

	m_teapot2->SetModel("Contents/models/teapot.model");
	m_teapot2->UpdateUniformVariables(Graphics::GetCurrentEffect());

	m_plane = new cActor();
	m_plane->Initialize();
	m_plane->SetModel("Contents/models/plane.model");
	m_plane->UpdateUniformVariables(Graphics::GetCurrentEffect());
	m_plane->Transform()->Translate(glm::vec3(0,-0.2f, -2.f));
	m_plane->Transform()->Rotate(glm::vec3(0, 1, 0), 180);
	m_plane->Transform()->Scale(glm::vec3(20, 1, 20));

	m_wall = new cActor();
	m_wall->Initialize();
	m_wall->SetModel("Contents/models/wall.model");
	m_wall->UpdateUniformVariables(Graphics::GetCurrentEffect());
	m_wall->Transform()->Translate(glm::vec3(0, 1,-5.f));
	m_wall->Transform()->Rotate(glm::vec3(1, 0, 0), 90);
	m_wall->Transform()->Rotate(glm::vec3(0, 1, 0), 180);
	m_wall->Transform()->Scale(glm::vec3(-5, 1, 3.75f));

}

void Assignment::CreateCamera()
{
	m_editorCamera = new  cEditorCamera(glm::vec3(0, 1.f, 0), -10.f, 0, 3, 10.f);
	float _aspect = (float)(GetCurrentWindow()->GetBufferWidth()) / (float)(GetCurrentWindow()->GetBufferHeight());
	m_editorCamera->CreateProjectionMatrix(45.0f, _aspect, 0.1f, 150.0f);
}

void Assignment::CreateLight()
{
	Graphics::CreateAmbientLight(Color(0.1f, 0.1f, 0.1f), aLight);
	//Graphics::CreatePointLight(glm::vec3(0, 1.5f, 0), Color(1.f, 1.f, 1.f), 0.3f, 0.1f, 0.1f, false,pLight1);
	Graphics::CreatePointLight(glm::vec3(-3, 0, -3), Color(0.8, 0.2, 0.2), 0.5f, 0.2f, 0.1f, false,pLight2);
	Graphics::CreateDirectionalLight(Color(1, 1, 1), glm::vec3(-0.3f,0.3, 0), true, dLight);
}

void Assignment::Run()
{

	// loop until window closed
	while (!m_shouldApplicationLoopExit)
	{
		glfwPollEvents();
		/*
				// clear window
				glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.f);
				// A lot of things can be cleaned like color buffer, depth buffer, so we need to specify what to clear
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwPollEvents();

				m_currentEffect->UseEffect();

				// Update camera location
				// --------------------------------------------------
				m_editorCamera->UpdateUniformLocation(m_currentEffect->GetProgramID());
				glUniformMatrix4fv(m_currentEffect->GetProjectionMatrixUniformID(), 1, GL_FALSE, glm::value_ptr(m_editorCamera->GetProjectionMatrix()));
				glUniformMatrix4fv(m_currentEffect->GetViewMatrixUniformID(), 1, GL_FALSE, glm::value_ptr(m_editorCamera->GetViewMatrix()));

				// Set up lighting
				// --------------------------------------------------
				{
					m_ambientLight->Illuminate();
					m_currentEffect->SetPointLightCount(2);
					m_PointLight->Illuminate();
					m_PointLight2->Illuminate();
				}
				// main draw happens in this scope
				// --------------------------------------------------
				{
					m_teapot->Update(m_currentEffect);
					m_teapot2->Update(m_currentEffect);
					m_plane->Update(m_currentEffect);
				}
				// --------------------------------------------------

				// clear program
				glUseProgram(0);
		*/
		// ----------------------
		// Submit data to be render
		m_teapot->Transform()->Update();
		m_teapot2->Transform()->Update();
		m_plane->Transform()->Update();
		m_wall->Transform()->Update();

		std::vector<std::pair<Graphics::cModel::HANDLE, cTransform*>> _renderingMap;
		_renderingMap.push_back({ m_plane->GetModelHandle(), m_plane->Transform() });
		_renderingMap.push_back({ m_teapot->GetModelHandle(), m_teapot->Transform() });
		_renderingMap.push_back({ m_teapot2->GetModelHandle(), m_teapot2->Transform() });
		_renderingMap.push_back({ m_wall->GetModelHandle(), m_wall->Transform() });
		
		Graphics::SubmitDataToBeRendered(m_editorCamera, _renderingMap);
		Graphics::ShadowMap_Pass();
		// ----------------------
		// Rendering

		std::vector<std::pair<Graphics::cModel::HANDLE, cTransform*>> _renderingMap2;
		_renderingMap2.push_back({ m_plane->GetModelHandle(), m_plane->Transform() });
		_renderingMap2.push_back({ m_teapot->GetModelHandle(), m_teapot->Transform() });
		_renderingMap2.push_back({ m_teapot2->GetModelHandle(), m_teapot2->Transform() });
		Graphics::SubmitDataToBeRendered(m_editorCamera, _renderingMap2);
		Graphics::Render_Pass_CaptureCameraView();

		std::vector<std::pair<Graphics::cModel::HANDLE, cTransform*>> _renderingMap3;
		_renderingMap3.push_back({ m_teapot->GetModelHandle(), m_teapot->Transform() });
		_renderingMap3.push_back({ m_teapot2->GetModelHandle(), m_teapot2->Transform() });
		_renderingMap3.push_back({ m_plane->GetModelHandle(), m_plane->Transform() });
		_renderingMap3.push_back({ m_wall->GetModelHandle(), m_wall->Transform() });
		Graphics::SubmitDataToBeRendered(m_editorCamera, _renderingMap3);
		Graphics::Render_Pass();
		// ----------------------
		// Swap buffers
		m_window->SwapBuffers();
	}
}

void Assignment::CleanUp()
{

	safe_delete(m_editorCamera);
	safe_delete(m_teapot);
	safe_delete(m_teapot2);
	safe_delete(m_plane);
	safe_delete(m_wall);

}

void Assignment::Tick(float second_since_lastFrame)
{

	sWindowInput* _windowInput = m_window->GetWindowInput();

	// get + handle user input events
	{
		m_editorCamera->CameraControl(_windowInput, second_since_lastFrame);

		m_editorCamera->MouseControl(_windowInput, 0.01667f);
	}

	// for recompile shader
	if (m_window->GetWindowInput()->IsKeyDown(GLFW_KEY_F6)) {
		Graphics::GetCurrentEffect()->RecompileShader(Constants::CONST_PATH_DEFAULT_VERTEXSHADER, GL_VERTEX_SHADER);
	}

	if (pLight1) {
		glm::vec3 _toLight = (pLight1->Transform()->GetWorldLocation() - m_teapot->Transform()->GetWorldLocation());
		glm::vec3 _toForward = glm::normalize(glm::cross(glm::vec3(0, 1, 0), _toLight)) * (5 * second_since_lastFrame);
		pLight1->Transform()->Translate(_toForward);
	}
	if (m_teapot) {
		m_teapot->Transform()->Rotate(glm::vec3(0, 0, 1), 100 * second_since_lastFrame);
	}
	/*
		m_teapot->Transform()->PrintEulerAngle();

		if (_windowInput->IsKeyDown(GLFW_KEY_LEFT)) {
			m_teapot->Transform()->Rotate(glm::vec3(0, 0, 1), (50 * second_since_lastFrame));
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_RIGHT)) {
			m_teapot->Transform()->Rotate(glm::vec3(0, 0, 1), (-50 * second_since_lastFrame));
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_UP)) {
			m_teapot->Transform()->Rotate(glm::vec3(1, 0, 0), (50 * second_since_lastFrame));
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_DOWN)) {
			m_teapot->Transform()->Rotate(glm::vec3(1, 0, 0), (-50 * second_since_lastFrame));
		}*/
}

void Assignment::FixedTick()
{

}
