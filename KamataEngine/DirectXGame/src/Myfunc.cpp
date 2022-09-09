#include "Myfunc.h"

#pragma region//ワールド変換行列を設定する関数

//スケーリング行列を設定する関数
void Myfunc::Affine::SetMatScale(Matrix4& affineMat , Vector3 scale) {
	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	affineMat *= matScale;

}

//回転行列を設定する関数
void Myfunc::Affine::SetMatRotation(Matrix4& affineMat , Vector3 rotation) {

	//回転行列を宣言
	//Z軸回転
	Matrix4 matRotZ = {
		cos(rotation.z) , sin(rotation.z) , 0 , 0 ,
		-sin(rotation.z) , cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X軸回転
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , cos(rotation.x) , sin(rotation.x) , 0 ,
		0 , -sin(rotation.x) , cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y軸回転
	Matrix4 matRotY = {
		cos(rotation.y) , 0 , -sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		sin(rotation.y) , 0 , cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	affineMat *= matRotZ;
	affineMat *= matRotX;
	affineMat *= matRotY;

}

//平行移動行列の設定をする関数
void Myfunc::Affine::SetMatTranslation(Matrix4& affineMat , Vector3 translation) {

	//平行移動行列の宣言
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//行列の計算
	affineMat *= matTranslation;
}
#pragma endregion

#pragma region//ワールド変換行列を生成する関数
//スケーリング行列を生成する関数
Matrix4 Myfunc::Affine::CreateMatScale(Matrix4 affineMat , Vector3 scale) {

	//スケーリング行列を宣言
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	return matScale;

}

//回転行列を生成する関数
Matrix4 Myfunc::Affine::CreateMatRotation(Matrix4 affineMat , Vector3 rotation) {

	//回転行列を宣言
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//Z軸回転
	Matrix4 matRotZ = {
		cos(rotation.z) , sin(rotation.z) , 0 , 0 ,
		-sin(rotation.z) , cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X軸回転
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , cos(rotation.x) , sin(rotation.x) , 0 ,
		0 , -sin(rotation.x) , cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y軸回転
	Matrix4 matRotY = {
		cos(rotation.y) , 0 , -sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		sin(rotation.y) , 0 , cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//行列の計算
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;

}

//平行移動行列の生成をする関数
Matrix4 Myfunc::Affine::CreateMatTranslation(Matrix4 affineMat , Vector3 translation) {

	//平行移動行列の宣言
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//行列の計算
	return matTranslation;
}

#pragma endregion

#pragma region//度数法⇔弧度法の関数

//度数法を弧度法に変換する関数
float Myfunc::MyMathUtility::Deg2Rad(float Deg) {

	const float PI = 3.141592f;

	return Deg * PI / 180;

}

//弧度法を度数法に変換する関数
float Myfunc::MyMathUtility::Rad2Deg(float rad) {

	const float PI = 3.141592;

	return rad / PI * 180;

}

#pragma endregion

//ベクトルと行列の掛け算をする関数
Vector3  Myfunc::MyMathUtility::MulVector3AndMatrix4(Vector3 vec , Matrix4 mat) {

	Vector3 ans;

	ans.x =
		mat.m[0][0] * vec.x +
		mat.m[1][0] * vec.y +
		mat.m[2][0] * vec.z +
		mat.m[3][0] * 0;

	ans.y =
		mat.m[0][1] * vec.x +
		mat.m[1][1] * vec.y +
		mat.m[2][1] * vec.z +
		mat.m[3][1] * 0;

	ans.z =
		mat.m[0][2] * vec.x +
		mat.m[1][2] * vec.y +
		mat.m[2][2] * vec.z +
		mat.m[3][2] * 0;

	return ans;

}

Vector3 Myfunc::MyMathUtility::CrossProduction(Vector3 v1 , Vector3 v2) {
	Vector3 ans;

	ans.x = v1.y * v2.z - v1.z * v2.y;
	ans.y = v1.z * v2.x - v1.x * v2.z;
	ans.z = v1.x * v2.y - v1.y * v2.x;

	return ans;

}

Vector3 Myfunc::MyMathUtility::SubVector3(Vector3 v1 , Vector3 v2) {
	Vector3 ans;

	ans.x = v1.x - v2.x;
	ans.y = v1.y - v2.y;
	ans.z = v1.z - v2.z;

	return ans;

}

//WorldTransformを更新する関数
void Myfunc::UpdateWorldTransform(WorldTransform& worldTransform) {
	//ワールド変換行列を用意
	Matrix4 affineMat = MathUtility::Matrix4Identity();

	//ワールド変換行列にスケーリング,回転,平行移動の行列を合成
	Myfunc::Affine::SetMatScale(affineMat , worldTransform.scale_);
	Myfunc::Affine::SetMatRotation(affineMat , worldTransform.rotation_);
	Myfunc::Affine::SetMatTranslation(affineMat , worldTransform.translation_);

	//ワールド行列に単位行列を代入
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//行列の計算
	worldTransform.matWorld_ *= affineMat;

	//もし親がある場合
	if (worldTransform.parent_) {
		//親のワールド行列との計算を行う
		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	}

	//行列の転送
	worldTransform.TransferMatrix();
}