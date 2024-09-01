#include "GameVictory.h"
#include "TextureManager.h"

GameVictory::GameVictory() {}

GameVictory::~GameVictory() {
	delete _modelSkydemo;
	delete gameVictorySprite_;
}

void GameVictory::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();

	//====================天球==========================
	_modelSkydemo = Model::CreateFromOBJ("skydome", true); // 天球モデル

	_skydome = new Skydome();

	_skydome->Initialize(_modelSkydemo, &viewProjection_);


	gameVictorytextureHandle_ = TextureManager::Load("victory.png");

	gameVictorySprite_ = Sprite::Create(gameVictorytextureHandle_, {0, 0});

}

void GameVictory::Update() {
	_skydome->Update();
	if (Input::GetInstance()->PushKey(DIK_B)) {
		finished_ = true;
	}
}

void GameVictory::Draw() {
	// コマンドリストの取得	获取命令列表
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);
	_skydome->Draw();
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	gameVictorySprite_->Draw();
	Sprite::PostDraw();

	
}
