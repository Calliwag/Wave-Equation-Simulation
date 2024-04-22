// Wave Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Resources.hpp"
#include "Pool.hpp"

int main()
{
    window.SetTargetFPS(60);

    Pool pool;
	pool.PoolX = 1000;
	pool.PoolY = 1000;
	pool.DeltaTime = 0.5;
	pool.DeltaPos = 1;
	pool.RenderScale = 7.0 / 10.0;
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
	for (int y = 0; y < pool.PoolY; y++)
	{
		pool.Sources[0][y] = 1;
	}
	pool.CourantRect(.25 * pool.PoolX, .25 * pool.PoolX + 1, 0, .4 * pool.PoolY, 0);
	pool.CourantRect(.25 * pool.PoolX, .25 * pool.PoolX + 1, .45 * pool.PoolY, .55 * pool.PoolY, 0);
	pool.CourantRect(.25 * pool.PoolX, .25 * pool.PoolX + 1, .6 * pool.PoolY, pool.PoolY, 0);
	for (int y = 0; y < pool.PoolY; y++)
	{
		pool.AbsorbantX[pool.PoolX - 1][y] = 1;
	}
	for (int x = .26 * pool.PoolX; x < pool.PoolX; x++)
	{
		pool.AbsorbantY[x][0] = 1;
		pool.AbsorbantY[x][pool.PoolY - 1] = 1;
	}

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
	//for (int y = 0; y < pool.PoolY; y++)
	//{
	//	pool.Sources[0][y] = 1;
	//}
	//pool.CourantRect(.4 * pool.PoolX, .6 * pool.PoolX, .4 * pool.PoolY, .6 * pool.PoolY, 0.5);
	//for (int y = 0; y < pool.PoolY; y++)
	//{
	//	pool.AbsorbantX[pool.PoolX - 1][y] = 1;
	//}
	//for (int x = 0; x < pool.PoolX; x++)
	//{
	//	pool.AbsorbantY[x][0] = 1;
	//	pool.AbsorbantY[x][pool.PoolY - 1] = 1;
	//}


	//Vector2 point = { pool.PoolX / 2,pool.PoolY / 2 };
	//for (int x = 0; x < pool.PoolX; x++)
	//{
	//	for (int y = 0; y < pool.PoolY; y++)
	//	{
	//		if (sqrt(pow((x - point.x), 2) + pow((y - point.y), 2)) < 200)
	//		{
	//			pool.Courant[x][y] = 0.25;
	//		}
	//	}
	//}

	//Vector2 point1 = { 0.05 * pool.PoolX,pool.PoolY / 2 };
	//Vector2 point2 = { 0.8 * pool.PoolX,pool.PoolY / 2 };
	//for (int x = 0; x < pool.PoolX; x++)
	//{
	//	for (int y = 0; y < pool.PoolY; y++)
	//	{
	//		if (sqrt(pow((x - point1.x), 2) + pow((y - point1.y), 2)) < 532 && sqrt(pow((x - point2.x), 2) + pow((y - point2.y), 2)) < 750)
	//		{
	//			pool.Courant[x][y] = 0.5;
	//		}
	//	}
	//}

	//for (int x = 0; x < pool.PoolX; x++)
	//{
	//	for (int y = 0; y < pool.PoolY; y++)
	//	{
	//		pool.Courant[x][y] = (float)GetRandomValue(500, 1000) / 1000.0f;
	//	}
	//}
	
	pool.CreateCourantColorSimple();

	int LimitFrames = 2500;

	SetTextureFilter(pool.CourantTexture, TEXTURE_FILTER_TRILINEAR);
	SetTextureFilter(pool.PoolTexture.texture, TEXTURE_FILTER_TRILINEAR);
	SetTextureFilter(pool.FullTexture.texture, TEXTURE_FILTER_TRILINEAR);

    while (!window.ShouldClose())
    {
		pool.VerletUpdate();
		pool.DrawToTexture();
		string name = "frame" + to_string(pool.frames) + ".png";

		//Full Screen Captured
		//Image ScreenShot = LoadImageFromTexture(pool.FullTexture.texture);
		//ImageFlipVertical(&ScreenShot);
		//ExportImage(ScreenShot, name.c_str());
		//UnloadImage(ScreenShot);

		//Only Pool Captured
		//Image ScreenShot = LoadImageFromTexture(pool.PoolTexture.texture);
		//ImageFlipVertical(&ScreenShot);
		//ExportImage(ScreenShot , name.c_str());
		//UnloadImage(ScreenShot);

		//ffmpeg -framerate 60 -i frame%d.png -vcodec libx264 -crf 18 -pix_fmt yuv420p output.mp4

		if (pool.frames > LimitFrames - 1 && LimitFrames != 0)
		{
			UnloadRenderTexture(pool.FullTexture);
			UnloadRenderTexture(pool.PoolTexture);
			CloseWindow();
			break;
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
