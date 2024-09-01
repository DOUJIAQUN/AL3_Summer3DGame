#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	_model = model;
	_worldTransform.Initialize();
	_viewProjection = viewProjection;
	_worldTransform.scale_ = {500.0f, 500.0f, 500.0f};	//天球の大きさを設定　元のサイズは1、今回は500倍に
}

void Skydome::Update() { _worldTransform.UpdateMatrix(); }	//自作関数で天球の位置を更新

void Skydome::Draw() { _model->Draw(_worldTransform, *_viewProjection); }
