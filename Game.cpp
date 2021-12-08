#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
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

	DrawGrid();
	DrawSnake();
	DrawFruit();
	ShowInfo();
}

void Update(float elapsedSec)
{
	float speedFactor{ 9.f };
	g_AccumulatedTime += elapsedSec;
	if (1.0f / speedFactor < g_AccumulatedTime)
	{
		g_AccumulatedTime -= 1.0f / speedFactor;
		MoveSnake(elapsedSec);
	}

	if (g_ShowInfo)
	{

	}
}

void End()
{
	delete[] pCells;
	pCells = nullptr;
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
		break;
	case SDLK_a:
	case SDLK_LEFT:
		changeDir(Direction::left);
		break;
	case SDLK_d:
	case SDLK_RIGHT:
		changeDir(Direction::right);
		break;
	case SDLK_w:
	case SDLK_UP:
		changeDir(Direction::up);
		break;
	case SDLK_s:
	case SDLK_DOWN:
		changeDir(Direction::down);
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
	g_TailIdx = GetLinearIndexFrom2DIndex(g_NrRows / 2, g_NrCols / 2, g_NrCols);
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
void DrawSnake()
{
	SetColor(1, 0, 0, 1);
	FillRect(pCells[g_TailIdx]);
}

/* Function to draw the fruit */
void DrawFruit()
{
	if (g_UpdateFruit)	// if true, get a new cell index
	{
		InitFruit();
		g_UpdateFruit = false;
	}

	SetColor(1, 1, 0, 1);
	float radius{ g_CellSize / 2 };
	FillEllipse(Point2f{ pCells[g_FruitIdx].left + radius, pCells[g_FruitIdx].bottom + radius }, radius, radius);
}

/* Function to check if the snake has the fruit or not, returns a bool */
bool DidSnakeGetFruit()
{
	if (g_TailIdx == g_FruitIdx)
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
		Point pos{ g_TailIdx / g_NrCols, g_TailIdx % g_NrCols };

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

		g_TailIdx = GetLinearIndexFrom2DIndex(pos.row, pos.col, g_NrCols);
		DidSnakeGetFruit();
	}
}

void changeDir(Direction new_direction)
{
	g_Dir = new_direction;
}

/* Function to show info about the game, e.g. keybindings */
void ShowInfo()
{
	// THE GAME SHOULD PAUSE SOMEHOW -> when g_ShowInfo is on, stop moving the snake -> bool for moving the snake? or reuse g_ShowInfo
	
	if (g_ShowInfo)	// boolean to allow for toggleability
	{
		g_SnakeMoving = false;

		SetColor(0, 0, 0, .7f);
		FillRect(g_CellSize, g_CellSize, (g_WindowWidth - 2 * g_CellSize), (g_WindowHeight - 2 * g_CellSize));
		
		// key bindings
		// authors of game
		bool success{ TextureFromString("info", "resources/DIN-light.otf", 50, g_White, g_InfoTexture) };

		Point2f pos{ g_WindowWidth / 2 - 40,g_WindowHeight - 4 * g_CellSize };
		DrawTexture(g_InfoTexture, pos);

		success = TextureFromString("controls: ", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.x = g_CellSize * 2 + 5;
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("w or arrow up - up", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("a or arrow left - left", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("s or arrow down - down", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("d or arrow right - right", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("i - info", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("x - quit game", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("credits: ", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 3;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("Anouchka Thijs, 1DAEGDE13", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);

		success = TextureFromString("Angelica Rings, 1DAEGDE12", "resources/DIN-light.otf", 25, g_White, g_Info2Texture);
		pos.y -= g_CellSize * 2;
		DrawTexture(g_Info2Texture, pos);
	}

	if (!g_ShowInfo)
	{
		g_SnakeMoving = true;
		SetColor(1, 0, 0, .5f);
		FillRect(g_WindowWidth - 5 * g_CellSize, g_WindowHeight - 3 * g_CellSize, 4 * g_CellSize, 2 * g_CellSize);

		Point2f pos{ g_WindowWidth - 5 * g_CellSize, g_WindowHeight - 2 * g_CellSize - 10 };
		bool success{ TextureFromString("press i for info", "resources/DIN-light.otf", 15, Color4f{ 0,0,0,1 }, g_InfoBoxTexture) };
		DrawTexture(g_InfoBoxTexture, pos);
	}

	// maybe a score too?
}

void SnakeTail(float elapsedSec)
{
	DidSnakeGetFruit();
	if (true)
	{

	}

}
#pragma endregion ownDefinitions