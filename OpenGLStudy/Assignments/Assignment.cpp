#include "Assignment.h"
#include "Application/Window/Window.h"
#include "Application/Window/WindowInput.h"
#include "Constants/Constants.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Graphics/Graphics.h"
#include "Time/Time.h"

#include "Graphics/Camera/EditorCamera/EditorCamera.h"
#include "Graphics/Graphics.h"
#include "Material/Material.h"
#include "Actor/Actor.h"
#include "Transform/Transform.h"
#include "Engine/Graphics/Model/Model.h"
#include "Material/Blinn/MatBlinn.h"
#include "Material/PBR_MR/MatPBRMR.h"
#include "Material/Cubemap/MatCubemap.h"
#include "Graphics/Texture/Texture.h"
#include "Assets/Handle.h"

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

	Graphics::cMatCubemap* _cubemapMat = dynamic_cast<Graphics::cMatCubemap*>(Graphics::cModel::s_manager.Get(m_cubemap->GetModelHandle())->GetMaterialAt());

	Graphics::cMatBlinn* _floorMat = dynamic_cast<Graphics::cMatBlinn*>(Graphics::cModel::s_manager.Get(m_mirror->GetModelHandle())->GetMaterialAt());
	auto cameraViewTextureHandle = Graphics::GetCameraCaptureFrameBuffer()->GetTextureHandle();
	//_floorMat->UpdateDiffuseTexture(cameraViewTextureHandle);
	//_floorMat->UpdateSpecularTexture(cameraViewTextureHandle);
	//_floorMat->UpdateReflectionTexture(cameraViewTextureHandle);
	//_floorMat->UpdateCubemapTexture(_cubemapMat->GetCubemapHandle());

	Graphics::cMatBlinn* _teapot2Mat = dynamic_cast<Graphics::cMatBlinn*>(Graphics::cModel::s_manager.Get(m_teapot2->GetModelHandle())->GetMaterialAt());
	//_teapot2Mat->UpdateCubemapTexture(_cubemapMat->GetCubemapHandle());

	Graphics::cMatPBRMR* _spaceHolderMat = dynamic_cast<Graphics::cMatPBRMR*>(Graphics::cModel::s_manager.Get(m_spaceHolder->GetModelHandle())->GetMaterialAt());


	return result;
}

void Assignment::CreateActor()
{
	m_teapot = new cActor();
	m_teapot->Initialize();
	m_teapot->Transform()->SetTransform(glm::vec3(0, 0, -300), glm::quat(glm::vec3(-glm::radians(90.f), 0, 0)), glm::vec3(5, 5, 5));
	m_teapot->SetModel("Contents/models/teapot.model");
	m_teapot->UpdateUniformVariables(Graphics::GetCurrentEffect());

	m_teapot2 = new cActor();
	m_teapot2->Initialize();
	m_teapot2->Transform()->SetTransform(glm::vec3(150, 0, -200), glm::quat(glm::vec3(-glm::radians(90.f), 0, 0)), glm::vec3(3, 3, 3));
	m_teapot2->SetModel("Contents/models/teapot.model");
	m_teapot2->UpdateUniformVariables(Graphics::GetCurrentEffect());

	m_mirror = new cActor();
	m_mirror->Initialize();
	m_mirror->Transform()->SetTransform(glm::vec3(0, 0, 0), glm::quat(1, 0, 0, 0), glm::vec3(800, 1, 800));
	m_mirror->SetModel("Contents/models/wall.model");
	m_mirror->UpdateUniformVariables(Graphics::GetCurrentEffect());



	m_cubemap = new cActor();
	m_cubemap->Initialize();
	m_cubemap->SetModel("Contents/models/cubemap.model");
	m_cubemap->UpdateUniformVariables(Graphics::GetEffectByKey("CubemapEffect"));

	m_spaceHolder = new cActor();
	m_spaceHolder->Initialize();
	m_spaceHolder->Transform()->SetTransform(glm::vec3(0, 150.f, 0), glm::quat(1, 0, 0, 0), glm::vec3(5, 5, 5));
	m_spaceHolder->SetModel("Contents/models/spaceHolder.model");
	m_spaceHolder->UpdateUniformVariables(Graphics::GetEffectByKey("PBR_MR"));
	m_spaceHolder->Transform()->Update();

	m_sphereList.reserve(25);
	for (int i = 0 ; i < 5 ; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			cActor * _sphere = new cActor();
			_sphere->Initialize();
			_sphere->Transform()->SetTransform(glm::vec3(-200.f + i * 100, 25.f + j * 50, 0), glm::quat(1, 0, 0, 0), glm::vec3(2.5f, 2.5f, 2.5f));
			_sphere->SetModel("Contents/models/pbrSphere.model");
			_sphere->UpdateUniformVariables(Graphics::GetEffectByKey("PBR_MR"));
			_sphere->Transform()->Update();
			m_sphereList.push_back( _sphere);
		}
	}
}

void Assignment::CreateCamera()
{
	m_editorCamera = new  cEditorCamera(glm::vec3(0, 150, 300), 5, 0, 300, 10.f);
	float _aspect = (float)(GetCurrentWindow()->GetBufferWidth()) / (float)(GetCurrentWindow()->GetBufferHeight());
	m_editorCamera->CreateProjectionMatrix(glm::radians(60.f), _aspect, 1.f, 1000.0f);
	m_editorCamera->Transform()->Update();
}

void Assignment::CreateLight()
{
	Graphics::CreateAmbientLight(Color(0.001f, 0.001f, 0.001f), aLight);
	Graphics::CreatePointLight(glm::vec3(0, 50.f, 100.f), Color(1.8, 1.8, 1.8), 1.5f, 0.3f, 2.f, true, pLight1);
	//Graphics::CreatePointLight(glm::vec3(-100, 40, -100), Color(0.8, 0.8, 0.8), 1.f, 0.7f, 1.8f, true, pLight2);
	//Graphics::CreateDirectionalLight(Color(.6, .6, .58f), glm::vec3(-1, -1, 0), true, dLight);
	//Graphics::CreateSpotLight(glm::vec3(0, 150, 0), glm::vec3(0, 1, 1), Color(1), 65.f, 1.5f, 0.3f, 5.f, true, spLight);
	//Graphics::CreateSpotLight(glm::vec3(100, 150, 0), glm::vec3(1, 1, 0), Color(1), 65.f, 1.f, 0.7f, 1.8f, true, spLight2);

}

void Assignment::Run()
{
	// loop until window closed
	while (!m_shouldApplicationLoopExit)
	{
		// Poll input events
		glfwPollEvents();

		// Render frame
		Graphics::RenderFrame();

		// Swap buffers
		m_window->SwapBuffers();
	}
}

void Assignment::CleanUp()
{
	safe_delete(m_editorCamera);
	safe_delete(m_teapot);
	safe_delete(m_teapot2);
	safe_delete(m_mirror);
	safe_delete(m_cubemap);
	safe_delete(m_spaceHolder);
	for (int i =0; i < m_sphereList.size(); ++i)
	{
		safe_delete(m_sphereList[i]);
	}
	m_sphereList.clear();
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
		Graphics::GetEffectByKey(Constants::CONST_DEFAULT_EFFECT_KEY)->RecompileShader(Constants::CONST_PATH_BLINNPHONG_FRAGMENTSHADER, GL_FRAGMENT_SHADER);
	}
	//dLight->Transform()->Rotate(-cTransform::WorldUp, 0.01677f);
	if (m_teapot) {
		m_teapot->Transform()->gRotate(glm::vec3(0, 1.f, 0), second_since_lastFrame);
	}

	cTransform* controledActor = nullptr;
	controledActor = pLight1->Transform();
	//controledActor = m_sphere->Transform();
	if (controledActor) {
		if (_windowInput->IsKeyDown(GLFW_KEY_J)) {
			controledActor->Translate(-cTransform::WorldRight * 100.f * second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_L)) {
			controledActor->Translate(cTransform::WorldRight* 100.f  * second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_I)) {
			controledActor->Translate(-cTransform::WorldForward* 100.f  * second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_K)) {
			controledActor->Translate(cTransform::WorldForward* 100.f  * second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_SPACE)) {
			controledActor->Translate(cTransform::WorldUp* 100.f* second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			controledActor->Translate(-cTransform::WorldUp* 100.f * second_since_lastFrame);
		}

	}

	cTransform* rotateControl = nullptr;
	if (spLight)
		rotateControl = spLight->Transform();
	//if (dLight)
		//rotateControl = dLight->Transform();
	if (rotateControl)
	{
		if (_windowInput->IsKeyDown(GLFW_KEY_LEFT)) {
			rotateControl->Rotate(cTransform::WorldUp, second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_RIGHT)) {
			rotateControl->Rotate(-cTransform::WorldUp, second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_UP)) {
			rotateControl->Rotate(cTransform::WorldRight, second_since_lastFrame);
		}
		if (_windowInput->IsKeyDown(GLFW_KEY_DOWN)) {
			rotateControl->Rotate(-cTransform::WorldRight, second_since_lastFrame);
		}
	}
	m_teapot->Transform()->Update();
	m_teapot2->Transform()->Update();
	m_mirror->Transform()->Update();

	if (pLight1)
		pLight1->Transform()->Update();
	if (pLight2)
	{
		pLight2->Transform()->Update();
#ifdef _DEBUG
		//pLight2->Transform()->PrintPosition();
#endif // DEBUG
	}

	if (dLight)
		dLight->Transform()->Update();
	if (spLight)
		spLight->Transform()->Update();
	if (spLight2)
		spLight2->Transform()->Update();

	// Submit data
	{

		/** 1. Wait until render thread is ready for receiving new graphic data */
		Graphics::MakeApplicationThreadWaitForSwapingData(m_applicationMutex);

		/** 2. Clear the application thread data and submit new one */
		{
			// Submit geometry data
			{
				Graphics::ClearApplicationThreadData();
				std::vector<std::pair<Graphics::cModel::HANDLE, cTransform>> _renderingMap;
				// Submit data to be render
				{
					_renderingMap.reserve(32);
					_renderingMap.push_back({ m_teapot->GetModelHandle(), *m_teapot->Transform() });
					_renderingMap.push_back({ m_teapot2->GetModelHandle(), *m_teapot2->Transform() });
					//_renderingMap.push_back({ m_mirror->GetModelHandle(), *m_mirror->Transform() });
					_renderingMap.push_back({ m_spaceHolder->GetModelHandle(), *m_spaceHolder->Transform() });
					for (int i = 0; i < m_sphereList.size(); ++i)
					{
						_renderingMap.push_back({ m_sphereList[i]->GetModelHandle(), *m_sphereList[i]->Transform() });
					}
					{// Spot light shadow map pass
						Graphics::SubmitDataToBeRendered(Graphics::UniformBufferFormats::sFrame(), _renderingMap, &Graphics::SpotLightShadowMap_Pass);
					}
					{ // Point light shadow map pass
						Graphics::SubmitDataToBeRendered(Graphics::UniformBufferFormats::sFrame(), _renderingMap, &Graphics::PointLightShadowMap_Pass);
					}
					
					// Frame data from directional light
					if (dLight && dLight->IsShadowEnabled()) {
						Graphics::UniformBufferFormats::sFrame _frameData_Shadow(dLight->CalculateLightTransform());
						Graphics::SubmitDataToBeRendered(_frameData_Shadow, _renderingMap, &Graphics::DirectionalShadowMap_Pass);
					}
				}
				// Frame data from camera
				if (false)
				{
					cEditorCamera _mirroredCamera = *m_editorCamera;
					_mirroredCamera.MirrorAlongPlane(*m_mirror->Transform());
					Graphics::UniformBufferFormats::sFrame _frameData_Mirrored(_mirroredCamera.GetProjectionMatrix(), _mirroredCamera.GetViewMatrix());
					_frameData_Mirrored.ViewPosition = _mirroredCamera.CamLocation();
					_renderingMap.clear();
					_renderingMap.push_back({ m_teapot->GetModelHandle(), *m_teapot->Transform() });
					_renderingMap.push_back({ m_teapot2->GetModelHandle(), *m_teapot2->Transform() });
					glm::vec4 _plane = glm::vec4(m_mirror->Transform()->Up(), m_mirror->Transform()->Position().y);
					Graphics::SubmitClipPlaneData(_plane);
					Graphics::SubmitDataToBeRendered(_frameData_Mirrored, _renderingMap, &Graphics::Reflection_Pass);
				}

				// Actual scene to render
				Graphics::UniformBufferFormats::sFrame _frameData_Camera(m_editorCamera->GetProjectionMatrix(), m_editorCamera->GetViewMatrix());
				_frameData_Camera.ViewPosition = m_editorCamera->CamLocation();
				{
					_renderingMap.clear();
					_renderingMap.push_back({ m_teapot->GetModelHandle(), *m_teapot->Transform() });
					_renderingMap.push_back({ m_teapot2->GetModelHandle(), *m_teapot2->Transform() });
					//_renderingMap.push_back({ m_mirror->GetModelHandle(), *m_mirror->Transform() });
					
					Graphics::SubmitDataToBeRendered(_frameData_Camera, _renderingMap, &Graphics::Render_Pass);
				}
				// PBR pass
				{
					_renderingMap.clear();
					_renderingMap.reserve(m_sphereList.size() + 1);
					_renderingMap.push_back({ m_spaceHolder->GetModelHandle(), *m_spaceHolder->Transform() });
					for (int i = 0; i < m_sphereList.size(); ++i )
					{
						_renderingMap.push_back({ m_sphereList[i]->GetModelHandle(), *m_sphereList[i]->Transform() });
					}
					Graphics::SubmitDataToBeRendered(_frameData_Camera, _renderingMap, &Graphics::PBR_Pass);
				}

				// Cube map
				{
					_renderingMap.clear();
					_renderingMap.push_back({ m_cubemap->GetModelHandle(), *m_cubemap->Transform() });
					Graphics::UniformBufferFormats::sFrame _frameData_Cubemap(m_editorCamera->GetProjectionMatrix(), glm::mat4(glm::mat3(m_editorCamera->GetViewMatrix())));
					Graphics::SubmitDataToBeRendered(_frameData_Camera, _renderingMap, &Graphics::CubeMap_Pass);
				}

				// Transform Gizmo
				{
					_renderingMap.clear();
					cTransform _worldTransform;
					Assets::cHandle<Graphics::cModel> unneccessaryHandle;
					_renderingMap.push_back({ unneccessaryHandle, _worldTransform });
					_renderingMap.push_back({ unneccessaryHandle, *m_teapot->Transform() });

					if (pLight1)
						_renderingMap.push_back({ unneccessaryHandle, *pLight1->Transform() });
					if (pLight2)
						_renderingMap.push_back({ unneccessaryHandle, *pLight2->Transform() });
					if (spLight)
						_renderingMap.push_back({ unneccessaryHandle, *spLight->Transform() });
					if (spLight2)
						_renderingMap.push_back({ unneccessaryHandle, *spLight2->Transform() });
					Graphics::SubmitDataToBeRendered(_frameData_Camera, _renderingMap, &Graphics::Gizmo_RenderTransform);
				}

				// Normal gizmo
				if(false)
				{
					_renderingMap.clear();
					_renderingMap.push_back({ m_teapot2->GetModelHandle(), *m_teapot2->Transform() });
					Graphics::SubmitDataToBeRendered(_frameData_Camera, _renderingMap, &Graphics::Gizmo_RenderVertexNormal);
				}
			}

			// Submit lighting data
			{
				std::vector<Graphics::cPointLight> _pLights;
				std::vector<Graphics::cSpotLight> _spLights;
				if (pLight1)
				{
					pLight1->UpdateLightIndex(_pLights.size());
					_pLights.push_back(*pLight1);
				}

				if (pLight2)
				{
					pLight2->UpdateLightIndex(_pLights.size());
					_pLights.push_back(*pLight2);
				}

				if (spLight)
				{
					spLight->UpdateLightIndex(_spLights.size());
					_spLights.push_back(*spLight);
				}
				if (spLight2)
				{
					spLight2->UpdateLightIndex(_spLights.size());
					_spLights.push_back(*spLight2);
				}
				Graphics::SubmitLightingData(_pLights, _spLights, *aLight, *dLight);
			}
		}

	}
}

void Assignment::FixedTick()
{

}
