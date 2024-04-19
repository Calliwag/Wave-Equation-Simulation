// Wave Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Resources.hpp"
#include "Pool.hpp"

int main()
{
    window.SetTargetFPS(60);

    Pool pool;
	pool.PoolX = 100;
	pool.PoolY = 100;
	pool.DeltaTime = 0.5;
	pool.DeltaPos = 7;
	pool.RenderScale = 1;
	pool.BoundaryConstant = 1;
	pool.DisplacementDamping = 0;
	pool.VelocityDamping = 0;
	pool.Displacement.resize(pool.PoolX);
	
	pool.CreateGrids();
	//pool.CreatePoolTexture();

	//Centered Sources
	//pool.Sources[pool.PoolX / 2 - 1][pool.PoolY / 2 - 1] = 1;
	//pool.Sources[pool.PoolX / 2 - 1][pool.PoolY / 2] = 1;
	//pool.Sources[pool.PoolX / 2][pool.PoolY / 2 - 1] = 1;
	//pool.Sources[pool.PoolX / 2][pool.PoolY / 2] = 1;

	//Double Slit Experiment >100x100
	//for (int y = 0; y < pool.PoolY; y++)
	//{
	//	pool.Sources[0][y] = 1;
	//}
	//pool.CourantRect(.25 * pool.PoolX, .25 * pool.PoolX + 1, 0, .4 * pool.PoolY, 0);
	//pool.CourantRect(.25 * pool.PoolX, .25 * pool.PoolX + 1, .45 * pool.PoolY, .55 * pool.PoolY, 0);
	//pool.CourantRect(.25 * pool.PoolX, .25 * pool.PoolX + 1, .6 * pool.PoolY, pool.PoolY, 0);
	//for (int y = 0; y < pool.PoolY; y++)
	//{
	//	pool.AbsorbantX[pool.PoolX - 1][y] = 1;
	//}
	//for (int x = .26 * pool.PoolX; x < pool.PoolX; x++)
	//{
	//	pool.AbsorbantY[x][0] = 1;
	//	pool.AbsorbantY[x][pool.PoolY - 1] = 1;
	//}

	//for (int y = 0; y < pool.PoolY; y++)
	//{
	//	pool.AbsorbantX[0][y] = 1;
	//	pool.AbsorbantX[pool.PoolX - 1][y] = 1;
	//}
	//for (int x = 0; x < pool.PoolX; x++)
	//{
	//	pool.AbsorbantY[x][0] = 1;
	//	pool.AbsorbantY[x][pool.PoolY - 1] = 1;
	//}

	//Light Through Square Prism
	for (int y = 0; y < pool.PoolY; y++)
	{
		pool.Sources[0][y] = 1;
	}
	pool.CourantRect(.4 * pool.PoolX, .6 * pool.PoolX, .4 * pool.PoolY, .6 * pool.PoolY, 0.5);
	for (int y = 0; y < pool.PoolY; y++)
	{
		pool.AbsorbantX[pool.PoolX - 1][y] = 1;
	}
	for (int x = 0; x < pool.PoolX; x++)
	{
		pool.AbsorbantY[x][0] = 1;
		pool.AbsorbantY[x][pool.PoolY - 1] = 1;
	}

	//for (int x = 0; x < pool.PoolX; x++)
	//{
	//	for (int y = 0; y < pool.PoolY; y++)
	//	{
	//		pool.Courant[x][y] = (float)GetRandomValue(500, 1000) / 1000.0f;
	//	}
	//}

	pool.CreateCourantColor();

	int LimitFrames = 10000;

    while (!window.ShouldClose())
    {
		pool.VerletUpdate();
		pool.DrawToTexture();
		string name = "frame" + to_string(pool.frames) + ".png";
		//TakeScreenshot(name.c_str()); //Full Screen Captured

		//Full Screen Captured
		//Image ScreenShot = LoadImageFromTexture(pool.FullTexture.texture);
		//ImageFlipVertical(&ScreenShot);
		//ExportImage(ScreenShot, name.c_str());

		//Only Pool Captured
		//Image ScreenShot = LoadImageFromTexture(pool.PoolTexture.texture);
		//ImageFlipVertical(&ScreenShot);
		//ExportImage(ScreenShot , name.c_str());

		if (pool.frames > LimitFrames - 1 && LimitFrames != 0)
		{
			UnloadRenderTexture(pool.FullTexture);
			UnloadRenderTexture(pool.PoolTexture);
			CloseWindow();
		}
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
