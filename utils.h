#pragma once
#include "structs.h"
#include <vector>

namespace utils
{

	const float g_Pi{ 3.1415926535f };

#pragma region OpenGLDrawFunctionality
	void ClearBackground(float r, float g, float b);
	void ClearBackground();
	// Choose a floating point value for each color component between 0.0f and 1.0f. The a (alpha) is optional.
	// Usage: SetColor(1.0f, 0.0f, 0.0f); set the drawing color to red
	void SetColor(float r, float g, float b, float a = 1);
	// Pass a Color4f object to the function
	void SetColor(const Color4f& color);

	// Draws a line by connecting the two points specified by (x1,y1) and (x2,y2)
	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth = 1.0f);
	// Draws a line by connecting the two points specified by p1 and p1
	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth = 1.0f);

	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth = 1);
	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3);
	void DrawRect(float left, float bottom, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Point2f& bottomLeft, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Rectf& rect, float lineWidth = 1.0f);
	void FillRect(float left, float bottom, float width, float height);
	void FillRect(const Point2f& bottomLeft, float width, float height);
	void FillRect(const Rectf& rect);

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Ellipsef& ellipse, float lineWidth = 1.0f);
	void FillEllipse(float centerX, float centerY, float radX, float radY);
	void FillEllipse(const Ellipsef& ellipse);
	void FillEllipse(const Point2f& center, float radX, float radY);

	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle);

	void DrawPolygon(const std::vector<Point2f>& vertices, bool closed = true, float lineWidth = 1.0f);
	void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f);
	void FillPolygon(const std::vector<Point2f>& vertices);
	void FillPolygon(const Point2f* pVertices, size_t nrVertices);
	

#pragma endregion OpenGLDrawFunctionality

#pragma region TextureFunctionality

	struct Texture
	{
		GLuint id;
		float width;
		float height;
	};
	bool TextureFromFile(const std::string& path, Texture& texture);
	bool TextureFromString(const std::string& text, TTF_Font* pFont, const Color4f& textColor, Texture& texture);
	bool TextureFromString(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, Texture& texture);
	void TextureFromSurface(const SDL_Surface* pSurface, Texture& textureData);
	void DrawTexture(const Texture& texture, const Point2f& dstBottomLeft, const Rectf& srcRect = {});
	void DrawTexture(const Texture& texture, const Rectf& dstRect, const Rectf& srcRect = {});
	void DeleteTexture(Texture& texture);
#pragma endregion TextureFunctionality

#pragma region CollisionFunctionality
	float GetDistance(const Point2f& start, const Point2f& end);
	bool IsPointInCircle(const Point2f& point, const Circlef& circle);
	bool IsPointInRect(const Point2f& point, const Rectf& rect);
	bool IsOverlapping(const Circlef& circle1, const Circlef& circle2);
	bool IsOverlapping(const Rectf& rect1, const Rectf& rect2);
#pragma endregion CollisionFunctionality

#pragma region Vector
	void DrawVector(const Vector2f& vector, Point2f origin = Point2f(0, 0));
	Vector2f Add(const Vector2f& vector1, const Vector2f& vector2);
	Vector2f Subtract(const Vector2f& vector1, const Vector2f& vector2);
	float DotProduct(const Vector2f& vector1, const Vector2f& vector2);
	float CrossProduct(const Vector2f& vector1, const Vector2f& vector2);
	std::string ToString(const Vector2f& vector);
	Vector2f Scale(const Vector2f& vector, float scalar);
	float Length(const Vector2f& vector);
	Vector2f Normalize(const Vector2f& vector);
	float AngleBetween(const Vector2f& vector1, const Vector2f& vector2);
	bool AreEqual(const Vector2f& vector1, const Vector2f& vector2);
	Vector2f PointToVector(const Point2f& point);
	Point2f VectorToPoint(const Vector2f& vector);
#pragma endregion Vector

#pragma region MyUtils
	void DrawPentagram(const Point2f& center, float radius);
	void DrawPentagram(const Point2f& center, float radius, float angle);
	void DrawConcentricSquares(const Point2f& position, float size, float amount);
	void DrawLinearGradient(const Rectf& rect, const Color4f& left, const Color4f& right);
	
	int GetRand(int min, int max);
	float GetRand(float min, float max);
	void Swap(int* pInt, int idx1, int idx2);
	void Swap(Point2f* pPoints, int idx1, int idx2);
	void Swap(Texture* pText, int idx1, int idx2);
	void Shuffle(int* pInt, int size, int amount);
	void Shuffle(Texture* pText, int size, int amount);
	void BubbleSort(int* pInt, int size);
	int GetLinearIndexFrom2DIndex(int rowIndex, int columnIndex, int nrOfColumns);
	Point2D Get2DIndexFromLinearIndex(int linearIndex, int columns);
#pragma endregion MyUtils
}