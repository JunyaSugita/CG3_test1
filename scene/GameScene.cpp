#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	view_.Initialize();
	view_.eye = { 0,10,-10 };
	view_.UpdateMatrix();
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,2,0 };
	MatCalc(worldTransform_);

	parTransform_.Initialize();
	parTransform_.scale_ = { 20,0,20 };
	MatCalc(parTransform_);

	model_ = Model::Create();

	particleM_ = ParticleManager::Create();
	particleM_->Initialize();
	particleM_->Update();

	grassM_ = GrassManager::Create();
	grassM_->Update();

	billboardM_ = BillboardManager::Create();
	billboardM_->Update();
}

void GameScene::Update() {

	//カメラ
	view_.eye = { 
		cos(angleX_ * PI / 180) * cos(angleY_ * PI / 180) * 10,
		sin(angleY_ * PI / 180) * 10,
		sin(angleX_ * PI / 180) * cos(angleY_ * PI / 180) * 10};
	view_.UpdateMatrix();
	particleM_->CameraMoveEyeVector(view_);
	grassM_->CameraMoveEyeVector(view_);
	billboardM_->CameraMoveEyeVector(view_);

	if (input_->PushKey(DIK_LEFT)) {
		angleX_--;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		angleX_++;
	}
	if (input_->PushKey(DIK_UP)) {
		if (angleY_ < 89) {
			angleY_++;
		}
	}
	if (input_->PushKey(DIK_DOWN)) {
		if (angleY_ > 20) {
			angleY_--;
		}
	} 

	Vector3 ray = view_.eye - view_.target;
	ray.normalize();
	ray.y = 0;

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_ -= ray * 0.1f;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_ += ray * 0.1f;
	}
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_ -= ray.cross(view_.up) * 0.1f;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_ += ray.cross(view_.up) * 0.1f;
	}
	MatCalc(worldTransform_);
	if (input_->TriggerKey(DIK_SPACE)) {
		scene++;
		if (scene > 2) {
			scene = 0;
		}
	}

	//パーティクル
	if (scene == 2) {
		particleM_->ParticleGenerate({0,2,0});
		particleM_->ParticleGenerate(worldTransform_.translation_);
	}
	particleM_->Update();
	
	//草
	grassM_->Update();

	billboardM_->Update();

	switch (scene)
	{
	case 0:
		debugText_->SetPos(400,600);
		debugText_->Printf("Billbord");
		break;
	case 1:
		debugText_->SetPos(400, 600);
		debugText_->Printf("Billbord Y : Grass");
		break;
	case 2:
		debugText_->SetPos(0, 0);
		debugText_->Printf("Particle : emitter");
		debugText_->SetPos(0, 60);
		debugText_->Printf("W,A,S,D key : emitterMove");
		break;
	}
	debugText_->SetPos(0, 20);
	debugText_->Printf("SPACE key : nextScene");
	debugText_->SetPos(0, 40);
	debugText_->Printf("ARROW key : cameraMove");
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(parTransform_,view_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion



#pragma region パーティクルの描画
	// パーティクル描画前処理
	ParticleManager::PreDraw(commandList);

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>
	if (scene == 2) {
		particleM_->Draw();
	}
	// パーティクル描画後処理
	ParticleManager::PostDraw();
#pragma endregion

#pragma ビルボードの描画
	// パーティクル描画前処理
	BillboardManager::PreDraw(commandList);

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>
	if (scene == 0) {
		billboardM_->Draw();
	}
	// パーティクル描画後処理
	BillboardManager::PostDraw();
#pragma endregion

#pragma 草の描画
	// パーティクル描画前処理
	GrassManager::PreDraw(commandList);

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>
	if (scene == 1) {
		grassM_->Draw();
	}
	// パーティクル描画後処理
	GrassManager::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
