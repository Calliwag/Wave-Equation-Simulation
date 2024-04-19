#pragma once

#include "Resources.hpp"

using namespace std;

class Pool
{
public:
	int frames = 0;
	int PoolX = 0;
	int PoolY = 0;
	float BoundaryConstant = 0;
	float MaxCourant = 1;
	float MaxDisplacement = 0;
	float VelocityDamping = 0;
	float DisplacementDamping = 0;
	float DeltaTime = 1;
	float DeltaPos = 1;
	float RenderScale = 1;
	vector<vector<float>> Displacement = {};
	vector<vector<float>> LastDisplacement = {};
	vector<vector<float>> Velocity = {};
	vector<vector<float>> Acceleration = {};
	vector<vector<float>> Courant = {};
	vector<vector<Color>> CourantColor = {};
	vector<vector<float>> AbsorbantX = {};
	vector<vector<float>> AbsorbantY = {};
	vector<vector<int>> Sources = {};
	RenderTexture2D PoolTexture;
	RenderTexture2D FullTexture;
	Texture CourantTexture;

	void CreateGrids();

	void CreateCourantColor();

	void CreatePoolTexture();

	void Draw();

	void DrawToTexture();

	void DrawGraphX(int YSlice, int XPos, int YPos, float Width);

	void DrawGraphY(int YSlice, int XPos, int YPos, float Width);

	Color GetColor(int x, int y);

	void EulerUpdate();

	void VerletUpdate();

	void SourceUpdate(int x, int y);

	void AbsorbantUpdateX(int x, int y);

	void AbsorbantUpdateY(int x, int y);

	float GetAcceleration(int x, int y);

	float GetSecondDerivative(int x, int y);

	void CourantRect(int x1, int x2, int y1, int y2, float NewCourant);
};