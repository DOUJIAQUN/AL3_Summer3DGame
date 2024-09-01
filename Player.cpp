#include "Player.h"
#include "GameScene.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, GameScene* gameScene) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 模型旋转二分之Pi。 但是我自己做的模型朝向是向左，所以不需要旋转
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;
	gameScene_ = gameScene;
}

void Player::Update() {

	// 1 移动入力
	Move();
	// 2 冲突判定
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	IsMapChipCollision(collisionMapInfo);
	// 3 反应冲突判定的结果
	CollisionResultMove(collisionMapInfo);
	// 4 天花板冲突
	CeilingCollision(collisionMapInfo);
	// 5 墙壁冲突
	WallCollision(collisionMapInfo);
	// 6 落地状态切换
	LandingSwitch(collisionMapInfo);
	// 7 旋回制御
	// 因为我自己的模型的原因 所以旋转角这样设置
	ConvolutionalControl();
	// 8 更新位置
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 移动
/// </summary>
void Player::Move() {

	// if (!onGround_) {
	//	velocity_ = Add(velocity_, {0.0f, -kGravityAcceleration, 0.0f});
	//	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	// }

	// 移动
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_D)) {
			// 旋转
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotation_ = std::numbers::pi_v<float>;
				turnTimer_ = 1.0f;
			}
			// 减速
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_A)) {
			// 旋转
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotation_ = 0.0f;
				turnTimer_ = 1.0f;
			}
			// 减速
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		velocity_ = Add(velocity_, acceleration);
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} // 非按键时速度衰减
	else {
		velocity_.x *= (1.0f - kAttenuation);
	}
	// 跳跃
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_W)) {
			velocity_ = Add(velocity_, {0.0f, kJumpAcceleration, 0.0f});
		}
	}
}

/// <summary>
/// 旋回制御
/// </summary>
void Player::ConvolutionalControl() {
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::clamp(turnTimer_ - 1 / 30.0f, 0.0f, turnTimer_);
		float destinationRotationYTable[] = {
		    0, std::numbers::pi_v<float>,
		    // std::numbers::pi_v<float> / 2.0f,
		    // std::numbers::pi_v<float> * 3.0f / 2.0f,
		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// worldTransform_.rotation_.y = destinationRotationY;
		worldTransform_.rotation_.y = std::lerp(destinationRotationY, turnFirstRotation_, turnTimer_);
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

/// <summary>
/// 获得玩家的世界位置坐标
/// </summary>
Vector3 Player::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

/// <summary>
/// 获得世界变换
/// </summary>
/// <returns></returns>
WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

/// <summary>
/// 获得玩家的AABB
/// </summary>
/// <returns></returns>
AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// velocity_ += Vector3(0, 2.0f, 0);

	gameScene_->SetIsDead(true);
}

#pragma region 玩家和地图块的碰撞

/// <summary>
/// 获得角点位置
/// </summary>
/// <param name="center">中心</param>
/// <param name="corner">角点</param>
/// <returns></returns>
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorners] = {
	    {kWidth / 2.0f,  -kHeight / 2.0f, 0.0f}, // 右下
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f}, // 左下
	    {kWidth / 2.0f,  kHeight / 2.0f,  0.0f}, // 右上
	    {-kWidth / 2.0f, kHeight / 2.0f,  0.0f}  // 左上
	};

	return Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

/// <summary>
/// map冲突判定
/// </summary>
/// <param name="info"></param>
void Player::IsMapChipCollision(CollisionMapInfo& info) {
	IsMapChipUPCollision(info);
	IsMapChipDownCollision(info);
	IsMapChipRightCollision(info);
	IsMapChipLeftCollision(info);
}

void Player::IsMapChipUPCollision(CollisionMapInfo& info) {
	// 上升？ 早期return
	if (info.move.y <= 0) {
		return;
	}

	// 移动后四角坐标计算
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorners)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {0.0f, kHeight / 2.0f, 0.0f};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.move) + offset);
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);

		float moveY = (rect.bottom - worldTransform_.translation_.y + kBlank) - (kHeight / 2.0f);
		info.move.y = std::max(0.0f, moveY);
		info.ceiling = true;
	}
}

void Player::IsMapChipDownCollision(CollisionMapInfo& info) {
	// if player up return
	if (info.move.y >= 0) {
		return;
	}

	// 移动后四角坐标计算
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorners)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {0.0f, kHeight / 2.0f, 0.0f};

		// indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.move) - offset);
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);

		float moveY = (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f) + kBlank;

		info.move.y = std::min(0.0f, moveY);
		info.landing = true;

	} else {
		info.landing = false;
	}
}

void Player::IsMapChipRightCollision(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}

	// 移动后四角坐标计算
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorners)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {kWidth / 2.0f, 0.0f, 0.0f};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.move) + offset);
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.left - worldTransform_.translation_.x - kWidth / 2 - kBlank;
		info.move.x = std::max(0.0f, moveX);
		info.hitWall = true;
	}
}

void Player::IsMapChipLeftCollision(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}

	// 移动后四角坐标计算
	std::array<Vector3, static_cast<uint32_t>(Corner::kNumCorners)> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {kWidth / 2.0f, 0.0f, 0.0f};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition((worldTransform_.translation_ + info.move) - offset);
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.right - worldTransform_.translation_.x + kWidth / 2 + kBlank;
		info.move.x = std::min(0.0f, moveX);
		info.hitWall = true;
	}
}

void Player::CollisionResultMove(CollisionMapInfo& info) { worldTransform_.translation_ = Add(worldTransform_.translation_, info.move); }

void Player::CeilingCollision(Player::CollisionMapInfo& info) {
	if (info.ceiling) {
		velocity_.y = 0.0f;
	}
	if (info.landing) {
		velocity_.y = 0.0f;
	}
}

void Player::WallCollision(Player::CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1 - kAttenuationWall);
	}
}

void Player::LandingSwitch(CollisionMapInfo& info) {
	velocity_ = Add(velocity_, {0.0f, -kGravityAcceleration, 0.0f});
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else /*if (!info.landing)*/ {
			// 移动后四角坐标计算
			std::array<Vector3, kNumCorners> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
			}

			// if (info.move.y <= 0) {
			// return;
			//}

			// 下落判定和切换
			MapChipType mapChipType;
			bool hit = false;
			MapChipField::IndexSet indexSet;

			// 左下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] - Vector3{0, kBlank, 0});
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] - Vector3{0, kBlank, 0});
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			if (!hit) {
				onGround_ = false;
			}
		}
	} else {

		if (info.landing) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
		}
	}
}

#pragma endregion