#include "Enemy.h"

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	//===================常规初始化========================
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 模型旋转二分之三Pi。调整转向
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	viewProjection_ = viewProjection;

	//========================动画相关初始化==============================

	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	// Move
	worldTransform_.translation_ += velocity_;

	// 动画计时器更新
	walkTimer_ += 1.0f / 60.0f;

	// 正弦回转动画

	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.z = std::sin(radian);

	// 更新模型的位置
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }