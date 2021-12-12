#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Snake - Thijs, Anouchka - Rings, Angelica - 1DAE13GDE" };

// Change the window dimensions here
float g_WindowWidth{ 700 };
float g_WindowHeight{ 700 };
#pragma endregion gameInformation



#pragma region ownDeclarations
const Color4f g_White{ 1, 1, 1, 1 }, g_Black{ 0, 0, 0, 1 };
const float g_CellSize{ 25 };
const int g_NrCols{ int(g_WindowWidth / g_CellSize) }, g_NrRows{ int(g_WindowHeight / g_CellSize) };
const int g_GridSize{ g_NrCols * g_NrRows };

/* Functional elements of the snake */
Rectf* pCells{ new Rectf[g_GridSize] {} };
int g_FruitIdx{}, g_NrFrames{}, g_Score{}, g_SnakeLength{};
float g_AccumulatedTime{}; g_SpeedFactor{};
int* g_Snake{};

bool g_UpdateFruit{}, g_ShowInfo{}, g_SnakeMoving{}, g_GameOver{}; g_ShowDifficulty{};

enum class Direction { left, right, up, down, none };
Direction g_Dir{};

/* Graphics of the snake */
const Point2f g_InfoPos{ g_WindowWidth / 2 - 40,g_WindowHeight - 4 * g_CellSize };
Texture g_SnakeGraphics{}, g_InfoTexture{}, g_Info2Texture{}, g_InfoBoxTexture{}, g_ScoreTexture{}, g_ScoreNrTexture{}; g_DifficultyTexture{};
Texture g_GameOverTexture{};
Texture g_GameOver2Texture{};
Texture g_EndScoreTexture{};
Texture g_EndScoreNrTexture{};
enum class SnakePart{ head, tail, body, corner};

// Declare your own functions here
void InitTextures();
void InitGrid();
void InitSnake();
void InitFruit();

void DrawGrid();
void DrawSnake();
void DrawSnakeHead();
void DrawSnakeBody();
void DrawSnakeTail();
void DrawFruit();
void DrawDifficulty();
void DrawGameOver();

void MoveSnake(float elapsedSec);

bool DidSnakeGetFruit();
bool SelfCollision();
int GetCellIdx(const Point2f& point);
SnakePart GetSnakePart(int idx);
Rectf GetSrcRect(const SnakePart& snk, const Direction& dir1, Direction dir2 = Direction::none);

void ShowInfo();
void PrintInfo();
void DrawInfo();
#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
