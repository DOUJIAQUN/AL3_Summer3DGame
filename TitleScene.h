#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Skydome.h"

class TitleScene {
private:
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;


	uint32_t gametitletextureHandle_ = 0;

	Sprite* gametitleSprite_ = nullptr;


	ViewProjection viewProjection_;

		// 天球
	Skydome* _skydome = nullptr;

	Model* _modelSkydemo = nullptr;

	

public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }
};
