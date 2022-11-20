#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MatCalc.h"
#include "ParticleManager.h"
#include "GrassManager.h"
#include "BillboardManager.h"

const float PI = 3.14159265f;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//カメラ
	ViewProjection view_;
	int angleX_ = 0;
	int angleY_ = 30;

	//床
	WorldTransform worldTransform_;
	Model* model_ = nullptr;


	//パーティクル
	WorldTransform parTransform_;
	ParticleManager* particleM_ = nullptr;
	//草
	GrassManager* grassM_ = nullptr;
	BillboardManager* billboardM_ = nullptr;

	int scene = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
