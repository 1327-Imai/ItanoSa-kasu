#pragma once

#include "WorldTransform.h"
#include "Model.h"

class Skydome {

public:
	void Initialize(Model* modelSkydome);

	void Update();

	void Draw(ViewProjection viewProjection);

private:
	
	WorldTransform worldTransform_;

	Model* model_;

};

