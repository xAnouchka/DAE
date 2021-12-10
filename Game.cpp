#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	InitTextures();
	InitGrid();
	InitSnake();
	InitFruit();

	g_ShowInfo = false;
	g_UpdateFruit = true;
	g_Dir = Direction::none;
}

void Draw()
{
	ClearBackground();

	//DrawGrid();
	DrawSnakeHead();
	DrawFruit();
	ShowInfo();
}

void Update(float elapsedSec)
{
	float speedFactor{ 4.f };
	g_AccumulatedTime += elapsedSec;
	if (1.0f / speedFactor < g_AccumulatedTime)
	{
		g_AccumulatedTime -= 1.0f / speedFactor;
		MoveSnake(elapsedSec);
	}
}

void End()
{
	delete[] pCells;
	pCells = nullptr;

	DeleteTexture(g_SnakeGraphics);
	DeleteTexture(g_InfoTexture);
	DeleteTexture(g_Info2Texture);
	DeleteTexture(g_InfoBoxTexture);
	DeleteTexture(g_ScoreTexture);
	DeleteTexture(g_ScoreNrTexture);
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{

}

void OnKeyUpEvent(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_i:
		g_ShowInfo = !g_ShowInfo;
		PrintInfo();
		break;
	case SDLK_a:
	case SDLK_LEFT:
		g_Dir = Direction::left;
		break;
	case SDLK_d:
	case SDLK_RIGHT:
		g_Dir = Direction::right;
		break;
	case SDLK_w:
	case SDLK_UP:
		g_Dir = Direction::up;
		break;
	case SDLK_s:
	case SDLK_DOWN:
		g_Dir = Direction::down;
		break;
	}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	////std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//switch (e.button)
	//{
	//case SDL_BUTTON_LEFT:
	//{
	//	//std::cout << "Left mouse button released\n";
	//	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
	//	break;
	//}
	//case SDL_BUTTON_RIGHT:
	//	//std::cout << "Right mouse button released\n";
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	//std::cout << "Middle mouse button released\n";
	//	break;
	//}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
void InitTextures()
{
	bool success{ TextureFromFile("Resources/snake-graphics.png", g_SnakeGraphics) };
	if (!success) std::cout << "Failed to load snake texture";
	success = { TextureFromFile("Resources/info.png", g_InfoBoxTexture) };
	if (!success) std::cout << "Failed loading g_InfoBoxTexture";

	success = { TextureFromString("Controls", "Resources/DIN-light.otf", 50, g_White, g_InfoTexture) };
	if (!success) std::cout << "Failed loading g_InfoTexture";
	success = { TextureFromString("Credits", "Resources/DIN-light.otf", 50, g_White, g_Info2Texture) };
	if (!success) std::cout << "Failed loading g_Info2Texture";
	success = { TextureFromString("Score: ", "Resources/DIN-light.otf", 20, g_Black, g_ScoreTexture) };
	if (!success) std::cout << "Failed loading g_ScoreTexture";
}

/* Function to add all the cells to the array pCells */
void InitGrid()
{
	// for every row: for every column: start.x + column*10, start.y + row*10, 25, 25
	int cellnr{ 0 };

	for (int row{ 0 }; row < g_NrRows; row++)
	{
		for (int col{ 0 }; col < g_NrCols; col++)
		{
			pCells[cellnr] = Rectf{ col * g_CellSize, row * g_CellSize, g_CellSize, g_CellSize };
			++cellnr;
		}
	}
}

/* Function to set g_FruitIdx to a new number */
void InitFruit()
{
	Point2f point{ GetRand(0, int(g_WindowWidth)) / 1.f, GetRand(0, int(g_WindowHeight)) / 1.f };
	g_FruitIdx = GetCellIdx(point);
}

/* Function to set the snake in middle of the grid when program is opened */
void InitSnake()
{
	g_HeadIdx = GetLinearIndexFrom2DIndex(g_NrRows / 2, g_NrCols / 2, g_NrCols);
}

/* Function to get the cell index of a point in the screen, returns the index int */
int GetCellIdx(const Point2f& point)
{
	for (int i = 0; i < g_GridSize; i++)
	{
		if (IsPointInRect(point, pCells[i])) return i;
	}
	return 0;
}

/* Function to draw the grid based on rects in array */
void DrawGrid()
{
	SetColor(.2f, .2f, .2f, 1);
	for (int i{ 0 }; i < g_GridSize; i++) {
		DrawRect(pCells[i]);
	}
}

/* Function to draw the snake (current: 1 rect big) */
void DrawSnakeHead()
{
	Rectf srcRect{ 0, 0, 60, 64 }, dstRect{ pCells[g_HeadIdx].left, pCells[g_HeadIdx].bottom, g_CellSize, g_CellSize };

	switch (g_Dir)
	{
	case Direction::left:
		srcRect = Rectf{ 194, 124, 63, 58 };
		break;
	case Direction::right:
		srcRect = Rectf{ 255, 61, 62, 59 };
		break;
	case Direction::up:
		srcRect = Rectf{ 195, 64, 58, 63 };
		break;
	case Direction::down:
	case Direction::none:
		srcRect = Rectf{ 258, 126, 58, 62 };
		break;
	default:
		break;
	}

	DrawTexture(g_SnakeGraphics, dstRect, srcRect);

	DrawSnakeBody();
	DrawSnakeTail();
}

void DrawSnakeBody()
{

}

void DrawSnakeTail()
{
	int g_TailIdx{};
	Rectf srcRect{ 0, 0, 52, 60 };

	Point2D pos{ g_HeadIdx / g_NrCols, g_HeadIdx % g_NrCols };
	switch (g_Dir)
	{
	case  Direction::up:
		srcRect = Rectf{ 198, 187, 52, 59 };
		--pos.row;
		break;
	case Direction::down:
		srcRect = Rectf{ 262, 256, 52, 59 };
		++pos.row;
		break;
	case Direction::left:
		srcRect = Rectf{ 192, 250, 59, 52 };
		++pos.col;
		break;
	case Direction::right:
		srcRect = Rectf{ 261, 185, 59, 52 };
		--pos.col;
		break;
	case Direction::none:
		return;
	}

	g_TailIdx = GetLinearIndexFrom2DIndex(pos.row, pos.col, g_NrCols);
	Rectf dstRect{ pCells[g_TailIdx].left, pCells[g_TailIdx].bottom, g_CellSize, g_CellSize };
	DrawTexture(g_SnakeGraphics, dstRect, srcRect);
}

/* Function to draw the fruit */
void DrawFruit()
{
	if (g_UpdateFruit)	// if true, get a new cell index
	{
		InitFruit();
		g_UpdateFruit = false;
	}

	Rectf srcRect{ 3, 255, 57, 63 }, dstRect{ pCells[g_FruitIdx].left, pCells[g_FruitIdx].bottom, g_CellSize, g_CellSize };
	DrawTexture(g_SnakeGraphics, dstRect, srcRect);
}

/* Function to check if the snake has the fruit or not, returns a bool */
bool DidSnakeGetFruit()
{
	if (g_HeadIdx == g_FruitIdx)
	{
		g_UpdateFruit = true;
		++g_Score;
		return true;
	}
	return false;
}

/* Function to move the snake */
void MoveSnake(float elapsedSec)
{
	if (g_SnakeMoving)
	{
		Point2D pos{ g_HeadIdx / g_NrCols, g_HeadIdx % g_NrCols };

		switch (g_Dir)
		{
		case  Direction::up:
			pos.row++;
			break;
		case Direction::down:
			pos.row--;
			break;
		case Direction::left:
			pos.col--;
			break;
		case Direction::right:
			pos.col++;
			break;
		case Direction::none:
			return;
		}

		//no borders (stops wall warp)
		if (pos.row < 0)
			pos.row = g_NrRows - 1;
		else if (g_NrRows < pos.row)
			pos.row = 0;
		else if (pos.col < 0)
			pos.col = g_NrCols - 1;
		else if (g_NrCols - 1 < pos.col)
			pos.col = 0;

		g_HeadIdx = GetLinearIndexFrom2DIndex(pos.row, pos.col, g_NrCols);
		DidSnakeGetFruit();
	}
}

/* Function to show info about the game, e.g. keybindings */
void ShowInfo()
{
	if (g_ShowInfo)	// boolean to allow for toggleability
	{
		g_SnakeMoving = false;
		DrawInfo();
	}
	else
	{
		g_SnakeMoving = true;

		// info
		Point2f pos{ g_WindowWidth - 3 * g_CellSize, g_WindowHeight - 2 * g_CellSize - 10 };
		Rectf dstRect{pos.x, pos.y, 50, 50};
		DrawTexture(g_InfoBoxTexture, dstRect);

		// score
		SetColor(0, 0, 1, .5f);
		FillRect(g_WindowWidth - 5 * g_CellSize, g_WindowHeight - 6 * g_CellSize, 4 * g_CellSize, 2 * g_CellSize);

		pos = Point2f{ g_WindowWidth - 5 * g_CellSize, g_WindowHeight - 5 * g_CellSize - 10 };
		DrawTexture(g_ScoreTexture, pos);

		pos.x += g_CellSize * 2.5f;
		bool success{ TextureFromString(std::to_string(g_Score), "resources/DIN-light.otf", 20, g_Black, g_ScoreNrTexture) };
		if (!success) std::cout << "Failed loading g_ScoreNrTexture";
		DrawTexture(g_ScoreNrTexture, pos);
		DeleteTexture(g_ScoreNrTexture);
	}
}

/* Function to print the controls to the console */
void PrintInfo()
{
	std::cout << "Controls:\n";
	std::cout << "W or Up key to move snake up\n";
	std::cout << "A or Left key to move snake left\n";
	std::cout << "S or Down key to move snake down\n";
	std::cout << "D or Right key to move snake right\n";
}

/* Function to print controls and credit info in game */
void DrawInfo()
{
	// Black Background Overlay
	SetColor(0, 0, 0, .7f);
	FillRect(g_CellSize, g_CellSize, (g_WindowWidth - 2 * g_CellSize), (g_WindowHeight - 2 * g_CellSize));

	// Info text
	const Point2f infoPos{ g_WindowWidth / 2 - 40, g_WindowHeight - 4 * g_CellSize }, creditPos{ g_WindowWidth / 2 - 70, g_WindowHeight / 2 };
	const int fontSize{ 25 }, nrLines{ 7 };
	Texture textTexture{};
	Point2f posText{};

	std::string pGameInfo[nrLines]
	{
	  "W or Up key to move snake up",
	  "A or Left key to move snake left",
	  "S or Down key to move snake down",
	  "D or Right key to move snake right",
	  "",
	  "Anouchka Thijs, 1DAEGDE13",
	  "Angelica Rings, 1DAEGDE12"
	};

	DrawTexture(g_InfoTexture, infoPos);
	for (int i{ 0 }; i < nrLines; ++i)
	{
		bool successful{ TextureFromString(pGameInfo[i], "Resources/DIN-Light.otf", fontSize, g_White, textTexture) };

		if (i < 4)
		{
			posText = Point2f{ g_WindowWidth / 4, infoPos.y - (textTexture.height * (i + 1)) };
		}
		else if (i == 4)
		{
			DrawTexture(g_Info2Texture, creditPos);
		}
		else if (i > 4)
		{
			posText = Point2f{ g_WindowWidth / 4, infoPos.y - (textTexture.height * (i + 4)) };
		}

		if (successful) DrawTexture(textTexture, posText);

		DeleteTexture(textTexture);
	}
}
#pragma endregion ownDefinitions