#pragma once
#include "Engine/Cores/Core.h"
#include <string>
#include "Math/Transform/Transform.h"
#include "Engine/Graphics/Model/Model.h"
/** Forward deceleration*/
//----------------------------------------------

namespace Graphics {
	class cMaterial;
	class cEffect;
}
//----------------------------------------------
class cActor
{
public:
	cActor() {};
	// Rule of three
	virtual ~cActor() { CleanUp(); }

	void Initialize();
	void CleanUp();
	
	/** Setters */
	void SetModel(const std::string i_modelPath);

	/** Getters*/
	cTransform Transform;
	Graphics::cModel::HANDLE GetModelHandle() const { return m_modelHandle; }
private:
	Graphics::cModel::HANDLE  m_modelHandle;


};
