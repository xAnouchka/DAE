#include "pch.h"
#include "Game.h"
#include <iostream>

#pragma region gameFunctions											
void Start()
{
	InitTextures();
	InitGrid();
	InitSnake();
	InitFruit();

	g_UpdateFruit = true;
	g_GameOver = false;
	g_Dir = Direction::right;
}

void Draw()
{
	ClearBackground(.9f, .8f, .6f);
	// DrawGrid();
	DrawSnake();
	DrawFruit();
	ShowInfo();
}

void Update(float elapsedSec)
{
	g_SpeedFactor;
	g_AccumulatedTime += elapsedSec;
	if (1.0f / g_SpeedFactor < g_AccumulatedTime)
	{
		g_AccumulatedTime -= 1.0f / g_SpeedFactor;
		MoveSnake(elapsedSec);
	}
}

void End()
{
	delete[] pCells;
	pCells = nullptr;
	delete[] g_Snake;
	g_Snake = nullptr;
	DeleteTexture(g_SnakeGraphics);
	DeleteTexture(g_InfoTexture);
	DeleteTexture(g_Info2Texture);
	DeleteTexture(g_InfoBoxTexture);
	DeleteTexture(g_ScoreTexture);
	DeleteTexture(g_ScoreNrTexture);
	DeleteTexture(g_DifficultyTexture);
	DeleteTexture(g_GameOverTexture);
	DeleteTexture(g_GameOver2Texture);
	DeleteTexture(g_EndScoreTexture);
	DeleteTexture(g_EndScoreNrTexture);
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
	case SDLK_1:
		g_SpeedFactor = 5.f;
		g_ShowDifficulty = false;
		break;
	case SDLK_2:
		g_SpeedFactor = 9.f;
		g_ShowDifficulty = false;
		break;
	case SDLK_3:
		g_SpeedFactor = 16.f;
		g_ShowDifficulty = false;
		break;
	}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}
#pragma endregion inputHandling



#pragma region initFunctions
void InitTextures()
{
	bool success{ TextureFromFile("Resources/snake-graphics.png", g_SnakeGraphics) };
	if (!success) std::cout << "Failed to load snake texture";
	success = { TextureFromFile("Resources/info.png", g_InfoBoxTexture) };
	if (!success) std::cout << "Failed loading g_InfoBoxTexture";

	success = { TextureFromString("Controls", "Resources/Retro-Gaming.otf", 50, g_White, g_InfoTexture) };
	if (!success) std::cout << "Failed loading g_InfoTexture";
	success = { TextureFromString("Credits", "Resources/Retro-Gaming.otf", 50, g_White, g_Info2Texture) };
	if (!success) std::cout << "Failed loading g_Info2Texture";
	success = { TextureFromString("Score: ", "Resources/Retro-Gaming.otf", 20, g_Black, g_ScoreTexture) };
	if (!success) std::cout << "Failed loading g_ScoreTexture";
	success = { TextureFromString("Difficulty: ", "Resources/Retro-Gaming.otf", 50, g_White, g_DifficultyTexture) };
	if (!success) std::cout << "Failed loading g_DifficultyTexture ";
	success = { TextureFromString("GAME OVER", "Resources/Retro-Gaming.otf", 75, g_Red, g_GameOverTexture) };
	if (!success) std::cout << "Failed loading g_GameOverTexture ";
	success = { TextureFromString("you died, rip", "Resources/Retro-Gaming.otf", 25, g_Red, g_GameOver2Texture) };
	if (!success) std::cout << "Failed loading g_GameOver2Texture ";
	success = { TextureFromString("score: ", "Resources/Retro-Gaming.otf", 25, g_White, g_EndScoreTexture) };
	if (!success) std::cout << "Failed loading g_EndScoreTexture ";
	success = { TextureFromString(std::to_string(g_Score), "Resources/Retro-Gaming.otf", 25, g_White, g_EndScoreNrTexture) };
	if (!success) std::cout << "Failed loading g_EndScoreNrTexture ";
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
	const int initialSize{ 2 };
	g_Snake = new int[g_GridSize];
	Point2D center{ g_NrRows / 2, g_NrCols / 2 };

	//g_Snake[0] = GetLinearIndexFrom2DIndex(center.row, center.col, g_NrCols);

	for (int i{ 0 }; i < initialSize; ++i)
	{
		if (i >= g_GridSize) break;
		g_Snake[i] = GetLinearIndexFrom2DIndex(center.row, center.col, g_NrCols);
		center.col--;
	}

	g_SnakeLength = initialSize;
}

#pragma endregion initFunctions



#pragma region DrawFunctions
/* Function to draw the grid based on rects in array */
void DrawGrid()
{
	SetColor(.2f, .2f, .2f, 1);
	for (int i{ 0 }; i < g_GridSize; i++)
	{
		DrawRect(pCells[i]);
	}
}

/* Functions to draw the snake: head, body and tail */
void DrawSnake()
{
	for (int i = 0; i < g_SnakeLength; i++)
	{
		//Point2D pos{ Get2DIndexFromLinearIndex(g_Snake[i], g_NrCols) };
		SetColor(0.0f, 0.7f, 0.0f);
		//FillRect(pCells[g_Snake[i]]);
	}

	DrawSnakeHead();
	DrawSnakeBody();
	DrawSnakeTail();
}

void DrawSnakeHead()
{
	Rectf srcRect{ GetSrcRect(SnakePart::head, g_Dir) }, 
		dstRect{ pCells[g_Snake[0]].left, pCells[g_Snake[0]].bottom, g_CellSize, g_CellSize };
	DrawTexture(g_SnakeGraphics, dstRect, srcRect);
}

void DrawSnakeBody()
{
	Rectf src{};

	for (int i{ 1 }; i < g_SnakeLength - 1; i++)
	{
		SnakePart bodypart = GetSnakePart(g_Snake[i]);

		int current{ g_Snake[i] }, before{ g_Snake[i - 1] }, after{ g_Snake[i + 1] };
		Direction dir{}, dir2{};

		Point2D posCurrent{ Get2DIndexFromLinearIndex(current, g_NrCols) },
			posBefore{ Get2DIndexFromLinearIndex(before, g_NrCols) },
			posAfter{ Get2DIndexFromLinearIndex(after, g_NrCols) };

		if (bodypart == SnakePart::body)
		{
			if ( (posBefore.row < posCurrent.row && posCurrent.row < posAfter.row) || (posBefore.row > posCurrent.row && posCurrent.row > posAfter.row) )
			{
				dir = Direction::down;
			}
			//else if ( (posBefore.col < posCurrent.col && posAfter.col < posAfter.col) || (posBefore.col > posCurrent.col && posCurrent.col > posAfter.col))
			else
			{
				dir = Direction::left;
			}

			src = GetSrcRect(bodypart, dir);
		}
		else if (bodypart == SnakePart::corner)
		{
			if (posBefore.row < posCurrent.row && posCurrent.col < posAfter.col)
			{
				dir = Direction::down;
				dir2 = Direction::right;
			}
			else if (posBefore.row < posCurrent.row && posCurrent.col > posAfter.col)
			{
				dir = Direction::down;
				dir2 = Direction::left;
			}
			else if (posBefore.row > posCurrent.row && posCurrent.col < posAfter.col)
			{
				dir = Direction::up;
				dir2 = Direction::right;
			}
			//else if (posBefore.row > posCurrent.row && posCurrent.col > posAfter.col)
			else
			{
				dir = Direction::up;
				dir2 = Direction::left;
			}

			src = GetSrcRect(bodypart, dir, dir2);
		}

		Rectf dstRect{ pCells[g_Snake[i]].left, pCells[g_Snake[i]].bottom, g_CellSize, g_CellSize };
		DrawTexture(g_SnakeGraphics, dstRect, src);
	}
}

void DrawSnakeTail()
{
	// direction == where the previous bodyblock is

	int tailIdx{ g_Snake[g_SnakeLength-1] };
	Direction dir{};

	Point2D posTail{ tailIdx / g_NrCols, tailIdx % g_NrCols }
		, posBefore{ Get2DIndexFromLinearIndex(g_Snake[g_SnakeLength -2], g_NrCols) };

	if (posBefore.row < posTail.row) {
		dir = Direction::down;
	}
	else if (posBefore.row > posTail.row) {
		dir = Direction::up;
	}
	else if (posBefore.col < posTail.col)
	{
		dir = Direction::left;
	}
	else if (posBefore.col > posTail.col) 
	{
		dir = Direction::right;
	}

	Rectf srcRect{ GetSrcRect(SnakePart::tail, dir) };
	Rectf dstRect{ pCells[tailIdx].left, pCells[tailIdx].bottom, g_CellSize, g_CellSize };

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

void DrawDifficulty()
{
	if (g_ShowDifficulty)
	{
		SetColor(0, 0, 0, .7f);
		FillRect(g_WindowWidth / 4, g_WindowHeight / 3, g_WindowWidth / 2, g_WindowHeight / 3);
		const Point2f difficultyPos{ g_WindowWidth / 3 - 50, g_WindowHeight * 2 / 3 - g_CellSize*3};
		DrawTexture(g_DifficultyTexture, difficultyPos);

		const int fontSize{ 25 }, nrLines{ 3 };
		Texture difficultyTexture{};
		std::string pDifficulty[nrLines]
		{
		"press 1 for easy",
		"press 2 for medium",
		"press 3 for hard",
		};

		for (int i = 0; i < nrLines; ++i)
		{
			bool successful{ TextureFromString(pDifficulty[i], "Resources/Retro-Gaming.otf", fontSize, g_White, difficultyTexture) };

			DrawTexture(difficultyTexture, Point2f{ g_WindowWidth / 4 + 5.f, difficultyPos.y - (difficultyTexture.height * (i + 1)) });
			DeleteTexture(difficultyTexture);
		}
	}

}

void DrawGameOver()
{
	g_GameOver = true;
	if (g_GameOver)
	{
	SetColor(g_Black);
	FillRect(0.0f, 0.0f, g_WindowWidth, g_WindowHeight);
	Point2f gameoverPos{ g_WindowWidth / 7, g_WindowHeight * 2/ 3 };

	DrawTexture(g_GameOverTexture, gameoverPos);

	Point2f gameover2Pos{ g_WindowWidth / 3 + 25, g_WindowHeight * 2 / 3 - 25 };
	DrawTexture(g_GameOver2Texture, gameover2Pos);

	Point2f scorePos{ g_WindowWidth / 2 - 75, g_WindowHeight/ 3 };
	DrawTexture(g_EndScoreTexture, scorePos);

	scorePos.x += 125;
	DrawTexture(g_EndScoreNrTexture, scorePos);
	}
}

/* Function to move the snake */
void MoveSnake(float elapsedSec)
{
	if (g_SnakeMoving && !g_GameOver)
	{
		Point2D pos{ Get2DIndexFromLinearIndex(g_Snake[0], g_NrCols) };
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
		else if (g_NrRows - 1 < pos.row)
			pos.row = 0;
		else if (pos.col < 0)
			pos.col = g_NrCols - 1;
		else if (g_NrCols - 1 < pos.col)
			pos.col = 0;

		for (int i{ g_SnakeLength - 1 }; 0 < i; --i)
		{
			g_Snake[i] = g_Snake[i - 1];
		}
		g_Snake[0] = GetLinearIndexFrom2DIndex(pos.row, pos.col, g_NrCols);

		DidSnakeGetFruit();
		if (SelfCollision()) DrawGameOver();
	}
}
#pragma endregion DrawFunctions



#pragma region InfoFunctions
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
		FillRect(g_WindowWidth - 6 * g_CellSize, g_WindowHeight - 6 * g_CellSize, 5 * g_CellSize, 2 * g_CellSize);

		pos = Point2f{ g_WindowWidth - 6 * g_CellSize, g_WindowHeight - 5 * g_CellSize - 10 };
		DrawTexture(g_ScoreTexture, pos);

		pos.x += g_CellSize * 3.5f;
		bool success{ TextureFromString(std::to_string(g_Score), "resources/Retro-Gaming.otf", 20, g_Black, g_ScoreNrTexture) };
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
	const Point2f infoPos{ g_WindowWidth / 3, g_WindowHeight - 4 * g_CellSize }, creditPos{ g_WindowWidth/3, g_WindowHeight / 2 };
	const int fontSize{ 25 }, nrLines{ 7 };
	Texture textTexture{};
	Point2f posText{};

	std::string pGameInfo[nrLines]
	{
	  "W or Up key to move snake up",
	  "A or Left key to move snake left",
	  "S or Down key to move snake down",
	  "D or Right key to move snake right",
	  " ",
	  "Anouchka Thijs, 1DAEGDE13",
	  "Angelica Rings, 1DAEGDE12"
	};

	DrawTexture(g_InfoTexture, infoPos);
	for (int i{ 0 }; i < nrLines; ++i)
	{
		bool successful{ TextureFromString(pGameInfo[i], "Resources/Retro-Gaming.otf", fontSize, g_White, textTexture) };

		if (i < 4)
		{
			posText = Point2f{ g_CellSize*3, infoPos.y - (textTexture.height * (i + 1)) };
		}
		else if (i == 4)
		{
			DrawTexture(g_Info2Texture, creditPos);
		}
		else if (i > 4)
		{
			posText = Point2f{ g_CellSize * 3, infoPos.y - (textTexture.height * (i + 4)) };
		}

		if (successful) DrawTexture(textTexture, posText);

		DeleteTexture(textTexture);
	}
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

/* Function to check if the snake has the fruit or not, returns a bool */
bool DidSnakeGetFruit()
{
	if (g_Snake[0] == g_FruitIdx)
	{
		g_UpdateFruit = true;
		++g_Score;
		g_Snake[g_SnakeLength] = g_Snake[1];
		++g_SnakeLength;
		return true;
	}
	return false;
}

/* Function to return the source rect of snake-graphics.png correspondent to the part that needs to be drawn */
Rectf GetSrcRect(const SnakePart& snk, const Direction& dir1, Direction dir2)
{
	float size{64};
	int col{ 5 }, row{ 4 };
	Rectf src{ size, size, size, size };

	switch (snk)
	{
	case SnakePart::head:
		switch (dir1)
		{
		case Direction::left:
			src.bottom *= 2;
			src.left *= 3;
			break;

		case Direction::right:
			src.bottom *= 1;
			src.left *= 4;
			break;

		case Direction::up:
			src.bottom *= 1;
			src.left *= 3;
			break;

		case Direction::down:
			src.bottom *= 2;
			src.left *= 4;
			break;

		default:
			break;
		}
		break;

	case SnakePart::body:
		switch (dir1)
		{
		/*case Direction::left:
		case Direction::right:
			src.bottom *= 1;
			src.left *= 1;
			break;*/
		case Direction::up:
		case Direction::down:
			src.bottom *= 2;
			src.left *= 2;
			break;
		default:
			break;
		}
		break;

	case SnakePart::tail:
		switch (dir1)
		{
		case Direction::left:
			src.bottom *= 4;
			src.left *= 3;
			break;

		case Direction::right:
			src.bottom *= 3;
			src.left *= 4;
			break;

		case Direction::up:
			src.bottom *= 3;
			src.left *= 3;
			break;

		case Direction::down:
			src.bottom *= 4;
			src.left *= 4;
			break;

		default:
			break;
		}
		break;

	case SnakePart::corner:
		if (dir1 == Direction::down || dir2 == Direction::down)
		{
			if (dir1 == Direction::left || dir2 == Direction::left)
			{
				src.bottom *= 1;
				src.left *= 2;
			}
			else if (dir1 == Direction::right || dir2 == Direction::right)
			{
				src.bottom *= 1;
				src.left *= 0;
			}

		}
		else if (dir1 == Direction::up || dir2 == Direction::up)
		{
			if (dir1 == Direction::left || dir2 == Direction::left)
			{
				src.bottom *= 3;
				src.left *= 2;
			}
			else if (dir1 == Direction::right || dir2 == Direction::right)
			{
				src.bottom *= 2;
				src.left *= 0;
			}
		}
		break;

	default:
		break;
	}

	return src;
}

/* Function to detect if the snake head collides with any part of its body */
bool SelfCollision() {
	for (int i{ 1 }; i < g_SnakeLength; i++)
	{
		if (g_Snake[0] == g_Snake[i]) return true;
	}
	return false;
}

/* Function to return which part of the snake body is equal to idx */
SnakePart GetSnakePart(int idx)
{
	if (idx == 0)
	{
		return SnakePart::head;
	}
	else if (idx == g_SnakeLength)
	{
		return SnakePart::tail;
	}
	else
	{
		Point2D pos0{ Get2DIndexFromLinearIndex(idx, g_NrCols) };
		Point2D pos1{ Get2DIndexFromLinearIndex(idx-1, g_NrCols) };
		Point2D pos2{ Get2DIndexFromLinearIndex(idx+1, g_NrCols) };

		if ( (pos0.col == pos1.col && pos1.col == pos2.col) || (pos0.row == pos1.row && pos1.row == pos2.row))
		{
			return SnakePart::body;
		}
		else return SnakePart::corner;
	}
}
#pragma endregion InfoFunctions
