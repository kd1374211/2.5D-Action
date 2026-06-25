#include "Void.h"

void Void::DrawUnLit()
{
	Math::Vector3 color = Math::Vector3::Zero;
	KdShaderManager::Instance().WorkAmbientController().SetFogEnable(false, true);
	KdShaderManager::Instance().WorkAmbientController().SetheightFog(color, -1.0f, -2.0f, 0.0f);
	
	//描画
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_model, m_mWorld);
}

void Void::Init()
{
	//モデルロード
	m_model = std::make_shared<KdModelData>();
	m_model->Load("Asset/Models/Void/Void.gltf");

	//マトリックス
	Math::Matrix scale = Math::Matrix::CreateScale(3.0f);
	Math::Matrix trans = Math::Matrix::CreateTranslation(Math::Vector3(0.0f, -3.0f, 0.0f));
	m_mWorld = scale * trans;
}

void Void::Release()
{}
