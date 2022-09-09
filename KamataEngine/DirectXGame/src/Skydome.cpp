#include "Skydome.h"
#include <cassert>

#include "Myfunc.h"

void Skydome::Initialize(Model* modelSkydome) {

	assert(modelSkydome);

	model_ = modelSkydome;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0 , 0 , 0};
	worldTransform_.scale_ = {100 , 100 , 100};

}

void Skydome::Update() {

	Myfunc::UpdateWorldTransform(worldTransform_);

}

void Skydome::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_ , viewProjection);

}