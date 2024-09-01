#include "CameraController.h"
#include "Player.h"

void CameraController::Initalize(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();
	endPosition = (targetWorldTransform.translation_ + targetOffset_) + (kVelocityBias * targetVelocity);

	//===============================================================
	// 调整镜头距离的东西
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		viewProjection_->translation_.z += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		viewProjection_->translation_.z -= 0.1f;
	}
	//===============================================================
	// 補間追従
	viewProjection_->translation_.x = std::lerp(viewProjection_->translation_.x, endPosition.x, kInterpolationRate);
	viewProjection_->translation_.y = std::lerp(viewProjection_->translation_.y, endPosition.y, kInterpolationRate);

	//  追従対象画面外补正
	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.left, targetWorldTransform.translation_.x + margin.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.bottom, targetWorldTransform.translation_.y + margin.top);

	// 限制移动范围
	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, moveableArea_.left, moveableArea_.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, moveableArea_.bottom, moveableArea_.top);

	viewProjection_->UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_ = Add(targetWorldTransform.translation_, targetOffset_);
}
