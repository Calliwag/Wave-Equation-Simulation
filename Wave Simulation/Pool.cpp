#include "Pool.hpp"

void Pool::CreateGrids()
{
	for (int x = 0; x < PoolX; x++)
	{
		Displacement[x].resize(PoolY);
	}
	LastDisplacement.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		LastDisplacement[x].resize(PoolY);
	}
	Velocity.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		Velocity[x].resize(PoolY);
	}
	Acceleration.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		Acceleration[x].resize(PoolY);
	}
	Courant.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		Courant[x].resize(PoolY);
		for (int y = 0; y < PoolY; y++)
		{
			Courant[x][y] = 1;
		}
	}
	AbsorbantX.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		AbsorbantX[x].resize(PoolY);
	}
	AbsorbantY.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		AbsorbantY[x].resize(PoolY);
	}
	Sources.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		Sources[x].resize(PoolY);
	}
}

void Pool::CreateCourantColor()
{
	MaxCourant = 0;
	for (int x = 0; x < PoolX; x++)
	{
		for (int y = 0; y < PoolY; y++)
		{
			MaxCourant = max(MaxCourant, abs(1 - Courant[x][y]));
		}
	}
	CourantColor.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		CourantColor[x].resize(PoolY);
		for (int y = 0; y < PoolY; y++)
		{
			float C = (MaxCourant - abs(1 - Courant[x][y])) / MaxCourant;
			Color Medium = { 255,255,255,255.0f * (1 - C) };
			CourantColor[x][y] = Medium;
		}
	}
	RenderTexture2D NewCourantTexture = LoadRenderTexture(PoolX, PoolY);
	BeginTextureMode(NewCourantTexture);
	for (int x = 0; x < PoolX; x++)
	{
		for (int y = 0; y < PoolY; y++)
		{
			DrawPixel(x, y, CourantColor[x][y]);
		}
	}
	EndTextureMode();
	CourantTexture = NewCourantTexture.texture;
}

void Pool::CreateCourantColorSimple()
{
	CourantColor.resize(PoolX);
	for (int x = 0; x < PoolX; x++)
	{
		CourantColor[x].resize(PoolY);
		for (int y = 0; y < PoolY; y++)
		{
			Color Medium = GREEN;
			if (Courant[x][y] == 0)
			{
				Medium = { 255,255,255,255 };
			}
			else if (Courant[x][y] == 1)
			{
				Medium = { 255,255,255,0 };
			}
			else
			{
				Medium = { 255,255,255,128 };
			}
			CourantColor[x][y] = Medium;
		}
	}
	RenderTexture2D NewCourantTexture = LoadRenderTexture(PoolX, PoolY);
	BeginTextureMode(NewCourantTexture);
	for (int x = 0; x < PoolX; x++)
	{
		for (int y = 0; y < PoolY; y++)
		{
			DrawPixel(x, y, CourantColor[x][y]);
		}
	}
	EndTextureMode();
	CourantTexture = NewCourantTexture.texture;
}

void Pool::Draw()
{
	window.BeginDrawing();
	window.ClearBackground(BLACK);
	for (int x = 0; x < PoolX; x++)
	{
		for (int y = 0; y < PoolY; y++)
		{
			DrawRectangle(x * DeltaPos * RenderScale, y * DeltaPos * RenderScale, DeltaPos * RenderScale, DeltaPos * RenderScale, GetColor(x, y));
			DrawRectangle(x * DeltaPos * RenderScale, y * DeltaPos * RenderScale, DeltaPos * RenderScale, DeltaPos * RenderScale, CourantColor[x][y]);
			//DrawPixel(x * DeltaPos * RenderScale, y * DeltaPos * RenderScale, GetColor(x, y));
			//DrawPixel(x * DeltaPos * RenderScale, y * DeltaPos * RenderScale, CourantColor[x][y]);
		}
	}
	DrawRectangleLines(0, 0, PoolX * DeltaPos * RenderScale, PoolY * DeltaPos * RenderScale, WHITE);
	DrawText(("Frame Time : " + to_string(window.GetFrameTime()) + "s").c_str(), 3, 3, 18, WHITE);
	window.EndDrawing();
}

void Pool::CreatePoolTexture()
{
	PoolTexture = LoadRenderTexture(PoolX, PoolY);
}

void Pool::DrawToTexture()
{
	UnloadRenderTexture(FullTexture);
	UnloadRenderTexture(PoolTexture);
	PoolTexture = LoadRenderTexture(PoolX, PoolY);
	FullTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	BeginTextureMode(PoolTexture);
	ClearBackground(BLACK);
	for (int x = 0; x < PoolX; x++)
	{
		for (int y = 0; y < PoolY; y++)
		{
			DrawPixel(x, PoolY - y - 1, GetColor(x, y)); //GetColorEnergy
		}
	}
	DrawTexture(CourantTexture, 0, 0, WHITE);
	EndTextureMode();
	BeginTextureMode(FullTexture);
	ClearBackground(BLACK);
	DrawTextureEx(PoolTexture.texture, { 0,0 }, 0, DeltaPos * RenderScale, WHITE);
	DrawRectangleLines(0, 0, PoolX * DeltaPos * RenderScale, PoolY * DeltaPos * RenderScale, WHITE);
	DrawText(("Frame Time : " + to_string(window.GetFrameTime()) + "s").c_str(), 5, 3, 18, WHITE);
	DrawGraphY(PoolX - 10, 750, 0, 50);
	DrawGraphX(PoolY / 2, 0, 750, 50);
	EndTextureMode();
	window.BeginDrawing();
	window.ClearBackground(BLACK);
	DrawTextureRec(FullTexture.texture, {0,0,float(GetScreenWidth()),float(-GetScreenHeight())}, {0,0}, WHITE);
	window.EndDrawing();
}

void Pool::DrawGraphX(int YSlice, int XPos, int YPos, float Width)
{
	float Scale = Width / MaxDisplacement;
	vector<float> Graph;
	Graph.resize(PoolX);
	DrawLine(XPos, YPos, XPos + (PoolX - 1) * DeltaPos * RenderScale, YPos, GREEN);
	Graph[0] = Displacement[0][YSlice];
	for (int x = 1; x < PoolX; x++)
	{
		Graph[x] = Displacement[x][YSlice];
		DrawLine(XPos + (x - 1) * DeltaPos * RenderScale, YPos - Graph[x - 1] * Scale, XPos + x * DeltaPos * RenderScale, YPos - Graph[x] * Scale, WHITE);
	}
}

void Pool::DrawGraphY(int XSlice, int XPos, int YPos, float Width)
{
	float Scale = Width / MaxDisplacement;
	vector<float> Graph;
	Graph.resize(PoolX);
	DrawLine(XPos, YPos, XPos, YPos + (PoolY - 1) * DeltaPos * RenderScale, GREEN);
	Graph[0] = Displacement[XSlice][0];
	for (int y = 1; y < PoolY; y++)
	{
		Graph[y] = Displacement[XSlice][y];
		DrawLine(XPos - Graph[y - 1] * Scale, YPos + (y - 1) * DeltaPos * RenderScale, XPos - Graph[y] * Scale, YPos + y * DeltaPos * RenderScale, WHITE);
	}
}

Color Pool::GetColor(int x, int y)
{
	Color color;
	if (Displacement[x][y] == 0)
	{
		color = BLACK;
	}
	else if (Displacement[x][y] > 0)
	{
		color = BLACK;
		color.r = 255 * min(Displacement[x][y], 1.0f);
	}
	else if (Displacement[x][y] < 0)
	{
		color = BLACK;
		color.b = 255 * min(-Displacement[x][y], 1.0f);
	}
	else
	{
		color = YELLOW;
		cout << frames << ": " << x << " ," << y << ": " << Displacement[x][y] << " ," << Velocity[x][y] << " ," << Acceleration[x][y] << endl;
	}

	return color;
}

Color Pool::GetColorEnergy(int x, int y)
{
	Color color;
	float K = pow((Displacement[x][y] - LastDisplacement[x][y]) / DeltaTime, 2);
	float P = Courant[x][y] * Courant[x][y] * GetGradientNormedSq(x, y);
	float Energy = 1.0f * min(20.0f * (K + P), 1.0f);
	color = BLACK;
	color.r = 255.0f * min(Energy, 1.0f);
	color.g = 255.0f * min(Energy, 1.0f);
	//color.b = 255.0f * min(Energy, 1.0f);

	return color;
}

void Pool::EulerUpdate()
{
	frames++;
	for (int x = 0; x < PoolX; x++)
	{
		for (int y = 0; y < PoolY; y++)
		{
			Acceleration[x][y] = GetAcceleration(x, y);
			Velocity[x][y] += Acceleration[x][y] * DeltaTime;
			Displacement[x][y] += Velocity[x][y] * DeltaTime;
			//Displacement[x][y] *= .99;
		}
	}
}

void Pool::VerletUpdate()
{
	frames++;
	cout << "Frame " << frames << endl;
	MaxDisplacement *= 0.9999;
	for (int i = 0; i < 1 / DeltaTime; i++)
	{
		vector<vector<float>> NewDisplacement = Displacement;
		for (int x = 0; x < PoolX; x++)
		{
			for (int y = 0; y < PoolY; y++)
			{
				if (!Sources[x][y])
				{
					Acceleration[x][y] = GetAcceleration(x, y);
				}
			}
		}
		for (int x = 0; x < PoolX; x++)
		{
			for (int y = 0; y < PoolY; y++)
			{
				if (Sources[x][y])
				{
					SourceUpdate(x, y);
				}
				else
				{
					if (AbsorbantX[x][y])
					{
						AbsorbantUpdateX(x, y);
					}
					else if (AbsorbantY[x][y])
					{
						AbsorbantUpdateY(x, y);
					}
					Velocity[x][y] = (1 - VelocityDamping * DeltaTime) * (Displacement[x][y] - LastDisplacement[x][y]);
					NewDisplacement[x][y] = (1 - DisplacementDamping * DeltaTime) * Displacement[x][y] + Velocity[x][y] + Acceleration[x][y] * DeltaTime * DeltaTime;
				}
			}
		}
		for (int x = 0; x < PoolX; x++)
		{
			for (int y = 0; y < PoolY; y++)
			{
				if (Sources[x][y] == 0)
				{
					LastDisplacement[x][y] = Displacement[x][y];
					Displacement[x][y] = NewDisplacement[x][y];
					MaxDisplacement = max(MaxDisplacement, abs(Displacement[x][y]));
				}
			}
		}
	}
}

void Pool::SourceUpdate(int x, int y)
{
	Displacement[x][y] = Sources[x][y] * (sin(0.1 * frames));
}

void Pool::AbsorbantUpdateX(int x, int y)
{
	float AccelerationX = 0;
	if (x == 0)
	{
		AccelerationX = -(Courant[x][y] / (DeltaPos * DeltaTime)) * ((Displacement[x][y] - LastDisplacement[x][y]) - (Displacement[x + 1][y] - LastDisplacement[x + 1][y]));
	}
	else if (x == PoolX - 1)
	{
		AccelerationX = (float) - (Courant[x][y] / (DeltaPos * DeltaTime)) * ((Displacement[x][y] - LastDisplacement[x][y]) - (Displacement[x - 1][y] - LastDisplacement[x - 1][y]));
	}
	else
	{
		AccelerationX = -(Courant[x][y] / (2 * DeltaPos * DeltaTime)) * ((Displacement[x + 1][y] - LastDisplacement[x + 1][y]) - (Displacement[x - 1][y] - LastDisplacement[x - 1][y]));
	}

	float AccelerationY = 0;
	if (y == 0)
	{
		AccelerationY = (0.5 * Courant[x][y] * Courant[x][y] / (DeltaPos * DeltaPos)) * (Displacement[x][y + 1] - 2 * Displacement[x][y]);
	}
	else if (y == PoolY - 1)
	{
		AccelerationY = (0.5 * Courant[x][y] * Courant[x][y] / (DeltaPos * DeltaPos)) * (Displacement[x][y - 1] - 2 * Displacement[x][y]);
	}
	else
	{
		AccelerationY = (0.5 * Courant[x][y] * Courant[x][y] / (DeltaPos * DeltaPos)) * (Displacement[x][y + 1] + Displacement[x][y - 1] - 2 * Displacement[x][y]);
	}
	
	Acceleration[x][y] = AccelerationX + AccelerationY;
}

void Pool::AbsorbantUpdateY(int x, int y)
{
	float AccelerationX = 0;
	if (x == 0)
	{
		AccelerationX = (0.5 * Courant[x][y] * Courant[x][y] / (DeltaPos * DeltaPos)) * (Displacement[x + 1][y] - 2 * Displacement[x][y]);
	}
	else if (x == PoolX - 1)
	{
		AccelerationX = (0.5 * Courant[x][y] * Courant[x][y] / (DeltaPos * DeltaPos)) * (Displacement[x - 1][y] - 2 * Displacement[x][y]);
	}
	else
	{
		AccelerationX = (0.5 * Courant[x][y] * Courant[x][y] / (DeltaPos * DeltaPos)) * (Displacement[x + 1][y] + Displacement[x - 1][y] - 2 * Displacement[x][y]);
	}

	float AccelerationY = 0;
	if (y == 0)
	{
		AccelerationY = -(Courant[x][y] / (DeltaPos * DeltaTime)) * ((Displacement[x][y] - LastDisplacement[x][y]) - (Displacement[x][y + 1] - LastDisplacement[x][y + 1]));
	}
	else if (y == PoolY - 1)
	{
		AccelerationY = -(Courant[x][y] / (DeltaPos * DeltaTime)) * ((Displacement[x][y] - LastDisplacement[x][y]) - (Displacement[x][y - 1] - LastDisplacement[x][y - 1]));
	}
	else
	{
		AccelerationY = -(Courant[x][y] / (2 * DeltaPos * DeltaTime)) * ((Displacement[x][y + 1] - LastDisplacement[x][y + 1]) - (Displacement[x][y - 1] - LastDisplacement[x][y - 1]));
	}

	Acceleration[x][y] = AccelerationX + AccelerationY;
}

float Pool::GetAcceleration(int x, int y)
{
	return GetSecondDerivative(x, y) * Courant[x][y] * Courant[x][y];
}

float Pool::GetGradientNormedSq(int x, int y)
{
	float DerivativeX = 0;
	if (x == 0)
	{
		DerivativeX = (1.0 / DeltaPos) * (Displacement[x + 1][y] - Displacement[x][y]);
	}
	else if (x == PoolX - 1)
	{
		DerivativeX = (1.0 / DeltaPos) * (Displacement[x][y] - Displacement[x - 1][y]);
	}
	else
	{
		DerivativeX = (0.5 / DeltaPos) * (Displacement[x + 1][y] - Displacement[x - 1][y]);
	}

	float DerivativeY = 0;
	if (y == 0)
	{
		DerivativeY = (1.0 / DeltaPos) * (Displacement[x][y + 1] - Displacement[x][y]);
	}
	else if (y == PoolY - 1)
	{
		DerivativeY = (1.0 / DeltaPos) * (Displacement[x][y] - Displacement[x][y - 1]);
	}
	else
	{
		DerivativeY = (0.5 / DeltaPos) * (Displacement[x][y + 1] - Displacement[x][y - 1]);
	}

	return DerivativeX * DerivativeX + DerivativeY * DerivativeY;
}

float Pool::GetSecondDerivative(int x, int y)
{
	float SecondDerivativeX = 0;
	if (x == 0)
	{
		SecondDerivativeX = (1 + BoundaryConstant) * Displacement[x + 1][y] - 2 * Displacement[x][y];
	}
	else if (x == PoolX - 1)
	{
		SecondDerivativeX = (1 + BoundaryConstant) * Displacement[x - 1][y] - 2 * Displacement[x][y];
	}
	else
	{
		SecondDerivativeX = Displacement[x+1][y] + Displacement[x - 1][y] - 2 * Displacement[x][y];
	}

	float SecondDerivativeY = 0;
	if (y == 0)
	{
		SecondDerivativeY = (1 + BoundaryConstant) * Displacement[x][y + 1] - 2 * Displacement[x][y];
	}
	else if (y == PoolY - 1)
	{
		SecondDerivativeY = (1 + BoundaryConstant) * Displacement[x][y - 1] - 2 * Displacement[x][y];
	}
	else
	{
		SecondDerivativeY = Displacement[x][y + 1] + Displacement[x][y - 1] - 2 * Displacement[x][y];
	}

	return (SecondDerivativeX + SecondDerivativeY) / (DeltaPos * DeltaPos);
}

void Pool::CourantRect(int x1, int x2, int y1, int y2, float NewCourant)
{
	for (int x = x1; x < x2; x++)
	{
		for (int y = y1; y < y2; y++)
		{
			Courant[x][y] = NewCourant;
		}
	}
}