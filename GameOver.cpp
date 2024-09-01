#include "GameOver.h"
#include "TextureManager.h"

GameOver::GameOver() {}

GameOver::~GameOver() {
	delete _modelSkydemo;
	delete gameOverSprite_;
}

void GameOver::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();
	
	//====================天球==========================
	_modelSkydemo = Model::CreateFromOBJ("skydome", true); // 天球モデル

	_skydome = new Skydome();

	_skydome->Initialize(_modelSkydemo, &viewProjection_);


	gameOvertextureHandle_ = TextureManager::Load("gameover.png");

	
	gameOverSprite_ = Sprite::Create(gameOvertextureHandle_, {0, 0});

}

void GameOver::Update() {
	_skydome->Update();
	if (Input::GetInstance()->PushKey(DIK_B)) {
		finished_ = true;
	}
}

void GameOver::Draw() {
	// コマンドリストの取得	获取命令列表
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	_skydome->Draw();
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	gameOverSprite_->Draw();
	Sprite::PostDraw();
	
	
}
