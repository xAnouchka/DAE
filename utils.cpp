#include "pch.h"
//#define _USE_MATH_DEFINES
//#include <cmath>
#include <algorithm>
#include <iostream>
#include "utils.h"
#include <string>

namespace utils
{
#pragma region OpenGLDrawFunctionality
	void ClearBackground(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void ClearBackground()
	{
		ClearBackground(185.0f / 255.0f, 211.0f / 255.0f, 238.0f / 255.0f);
	}
	void SetColor(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
	}

	void SetColor(const Color4f& color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		}
		glEnd();
	}

	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, lineWidth);
	}

	void DrawRect(float left, float bottom, float width, float height, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}

	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3)
	{
		glBegin(GL_TRIANGLES);
		{
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
			glVertex2f(p3.x, p3.y);
		}
		glEnd();
	}

	void DrawRect(const Point2f& bottomLeft, float width, float height, float lineWidth)
	{
		DrawRect(bottomLeft.x, bottomLeft.y, width, height, lineWidth);
	}

	void DrawRect(const Rectf& rect, float lineWidth)
	{
		DrawRect(rect.left, rect.bottom, rect.width, rect.height, lineWidth);
	}

	void FillRect(float left, float bottom, float width, float height)
	{
		glBegin(GL_POLYGON);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}

	void FillRect(const Point2f& bottomLeft, float width, float height)
	{
		FillRect(bottomLeft.x, bottomLeft.y, width, height);
	}

	void FillRect(const Rectf& rect)
	{
		FillRect(rect.left, rect.bottom, rect.width, rect.height);
	}

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth)
	{
		DrawEllipse(center.x, center.y, radX, radY, lineWidth);
	}

	void DrawEllipse(const Ellipsef& ellipse, float lineWidth)
	{
		DrawEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY, lineWidth);
	}

	void FillEllipse(float centerX, float centerY, float radX, float radY)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void FillEllipse(const Ellipsef& ellipse)
	{
		FillEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY);
	}

	void FillEllipse(const Point2f& center, float radX, float radY)
	{
		FillEllipse(center.x, center.y, radX, radY);
	}

	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_STRIP);
		{
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();

	}

	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		DrawArc(center.x, center.y, radX, radY, fromAngle, tillAngle, lineWidth);
	}

	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			glVertex2f(centerX, centerY);
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();
	}

	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle)
	{
		FillArc(center.x, center.y, radX, radY, fromAngle, tillAngle);
	}

	void DrawPolygon(const std::vector<Point2f>& vertices, bool closed, float lineWidth)
	{
		DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
	}

	void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed, float lineWidth)
	{
		glLineWidth(lineWidth);
		closed ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}

	void FillPolygon(const std::vector<Point2f>& vertices)
	{
		FillPolygon(vertices.data(), vertices.size());
	}

	void FillPolygon(const Point2f* pVertices, size_t nrVertices)
	{
		glBegin(GL_POLYGON);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}

#pragma endregion OpenGLDrawFunctionality


#pragma region textureImplementations

	bool TextureFromFile(const std::string& path, Texture& texture)
	{
		//Load file for use as an image in a new surface.
		SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
		if (pLoadedSurface == nullptr)
		{
			std::cerr << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError() << std::endl;
			return false;
		}

		TextureFromSurface(pLoadedSurface, texture);

		//Free loaded surface
		SDL_FreeSurface(pLoadedSurface);

		return true;
	}

	bool TextureFromString(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, Texture& texture)
	{
		// Create font
		TTF_Font* pFont{};
		pFont = TTF_OpenFont(fontPath.c_str(), ptSize);
		if (pFont == nullptr)
		{
			std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError();
			std::cin.get();
			return false;
		}

		// Create texture using this fontand close font afterwards
		bool textureOk = TextureFromString(text, pFont, textColor, texture);
		TTF_CloseFont(pFont);

		return textureOk;
	}

	bool TextureFromString(const std::string& text, TTF_Font* pFont, const Color4f& color, Texture& texture)
	{
		//Render text surface
		SDL_Color textColor{};
		textColor.r = Uint8(color.r * 255);
		textColor.g = Uint8(color.g * 255);
		textColor.b = Uint8(color.b * 255);
		textColor.a = Uint8(color.a * 255);

		SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
		//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
		if (pLoadedSurface == nullptr)
		{
			std::cerr << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
			return false;
		}

		// copy to video memory
		TextureFromSurface(pLoadedSurface, texture);

		//Free loaded surface
		SDL_FreeSurface(pLoadedSurface);

		return true;
	}

	void TextureFromSurface(const SDL_Surface* pSurface, Texture& texture)
	{
		//Get image dimensions
		texture.width = float(pSurface->w);
		texture.height = float(pSurface->h);

		// Get pixel format information and translate to OpenGl format
		GLenum pixelFormat{ GL_RGB };
		switch (pSurface->format->BytesPerPixel)
		{
		case 3:
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGB;
			}
			else
			{
				pixelFormat = GL_BGR;
			}
			break;
		case 4:
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGBA;
			}
			else
			{
				pixelFormat = GL_BGRA;
			}
			break;
		default:
			std::cerr << "TextureFromSurface error: Unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";;
			texture.width = 0;
			texture.height = 0;
			return;
		}

		//Generate an array of textures.  We only want one texture (one element array), so trick
		//it by treating "texture" as array of length one.
		glGenTextures(1, &texture.id);

		//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
		//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
		glBindTexture(GL_TEXTURE_2D, texture.id);

		// check for errors.
		GLenum e = glGetError();
		if (e != GL_NO_ERROR)
		{
			std::cerr << "TextureFromSurface, error binding textures, Error id = " << e << '\n';
			texture.width = 0;
			texture.height = 0;
			return;
		}

		//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
		//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
		//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
		//                         not cached smaller copies)
		//         GL_RGBA:    Specifies the number of color components in the texture.
		//                     This is how OpenGL will store the texture internally (kinda)--
		//                     It's essentially the texture's type.
		//      surface->w:    The width of the texture
		//      surface->h:    The height of the texture
		//               0:    The border.  Don't worry about this if you're just starting.
		//     pixelFormat:    The format that the *data* is in--NOT the texture! 
		//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
		//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
		//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
		//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
		// surface->pixels:    The actual data.  As above, SDL's array of bytes.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

		//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
		//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
		//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
		//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
		//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void DeleteTexture(Texture& texture)
	{
		glDeleteTextures(1, &texture.id);
	}

	void DrawTexture(const Texture& texture, const Point2f& dstBottomLeft, const Rectf& srcRect)
	{
		Rectf dstRect{ dstBottomLeft.x, dstBottomLeft.y, srcRect.width, srcRect.height };
		DrawTexture(texture, dstRect, srcRect);
	}


	void DrawTexture(const Texture& texture, const Rectf& dstRect, const Rectf& srcRect)
	{
		// Determine texture coordinates using srcRect and default destination width and height
		float textLeft{};
		float textRight{};
		float textTop{};
		float textBottom{};

		float defaultDstWidth{};
		float defaultDstHeight{};
		if (!(srcRect.width > 0.0f && srcRect.height > 0.0f)) // No srcRect specified
		{
			// Use complete texture
			textLeft = 0.0f;
			textRight = 1.0f;
			textTop = 0.0f;
			textBottom = 1.0f;

			defaultDstHeight = texture.height;
			defaultDstWidth = texture.width;
		}
		else // srcRect specified
		{
			// Convert to the range [0.0, 1.0]
			textLeft = srcRect.left / texture.width;
			textRight = (srcRect.left + srcRect.width) / texture.width;
			textTop = (srcRect.bottom - srcRect.height) / texture.height;
			textBottom = srcRect.bottom / texture.height;

			defaultDstHeight = srcRect.height;
			defaultDstWidth = srcRect.width;
		}

		// Determine vertex coordinates
		float vertexLeft{ dstRect.left };
		float vertexBottom{ dstRect.bottom };
		float vertexRight{};
		float vertexTop{};
		if (!(dstRect.width > 0.0f && dstRect.height > 0.0f)) // If no size specified use default size
		{
			vertexRight = vertexLeft + defaultDstWidth;
			vertexTop = vertexBottom + defaultDstHeight;
		}
		else
		{
			vertexRight = vertexLeft + dstRect.width;
			vertexTop = vertexBottom + dstRect.height;
		}

		// Tell opengl which texture we will use
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		// set the texture mode https://open.gl/textures
		float color[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		// Draw
		glEnable(GL_TEXTURE_2D);
		{
			glBegin(GL_QUADS);
			{
				glTexCoord2f(textLeft, textBottom);
				glVertex2f(vertexLeft, vertexBottom);

				glTexCoord2f(textLeft, textTop);
				glVertex2f(vertexLeft, vertexTop);

				glTexCoord2f(textRight, textTop);
				glVertex2f(vertexRight, vertexTop);

				glTexCoord2f(textRight, textBottom);
				glVertex2f(vertexRight, vertexBottom);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

	}
#pragma endregion textureImplementations


#pragma region CollisionFunctionality
	float GetDistance(const Point2f& start, const Point2f& end)
	{
		float result{};
		result = sqrtf(powf((end.x - start.x), 2) + powf((end.y - start.y), 2));
		return result;
	}

	bool IsPointInCircle(const Point2f& point, const Circlef& circle)
	{
		if (GetDistance(point, circle.center) < circle.radius)
		{
			return true;
		}
		return false;
	}


	bool IsPointInRect(const Point2f& point, const Rectf& rect)
	{
		if (point.x >= rect.left && point.x <= (rect.left + rect.width)
			&& point.y >= rect.bottom && point.y <= (rect.bottom + rect.width))
		{
			return true;
		}
		return false;
	}

	bool IsOverlapping(const Circlef& circle1, const Circlef& circle2)
	{
		float distSq{}, radsumSq{};
		distSq = powf((circle1.center.x - circle2.center.x), 2) + powf((circle1.center.y - circle2.center.y), 2);
		radsumSq = powf((circle1.radius + circle2.radius), 2);

		if (distSq == radsumSq)
		{
			return false;
		}
		else if (distSq > radsumSq)
		{
			return false;
		}
		else
		{
			return true;
		}

		return false;
	}

	bool IsOverlapping(const Rectf& rect1, const Rectf& rect2)
	{
		Point2f l1{ rect1.left, rect1.bottom + rect1.height }, r1{ rect1.left + rect1.width, rect1.bottom };
		Point2f l2{ rect2.left, rect2.bottom + rect2.height }, r2{ rect2.left + rect2.width, rect2.bottom };
		// To check if either rectangle is actually a line
		// For example :  l1 ={-1,0}  r1={1,1}  l2={0,-1}
		// r2={0,1}

		if (l1.x == r1.x || l1.y == r1.y || l2.x == r2.x
			|| l2.y == r2.y)
		{
			// the line cannot have positive overlap
			return false;
		}

		// If one rectangle is on left side of other
		if (l1.x >= r2.x || l2.x >= r1.x) return false;

		// If one rectangle is above other
		if (r1.y >= l2.y || r2.y >= l1.y) return false;

		return true;
	}
#pragma endregion CollisionFunctionality

#pragma region Vector

	void DrawVector(const Vector2f& vector, Point2f start)
	{
		//DrawLine(start, Point2f(vector.x + start.x, vector.y + start.y));
		//float angle{ atan2(vector.y, vector.x) }, arrowAngle{g_Pi / 6};

		//FillTriangle(Point2f(vector.x + start.x, vector.y + start.y), 
		//			Point2f(vector.x + start.x + 10 * cosf(1.5f * g_Pi - angle -arrowAngle), vector.y + start.y - 10 * sinf(1.5f * g_Pi - angle - arrowAngle)),
		//			Point2f(vector.x + start.x + 10 * cosf(1.5f * g_Pi / 6 - angle + arrowAngle), vector.y +start.y - 10 * sinf(1.5f * g_Pi / 6 - angle + arrowAngle)));
		const Point2f vectorPoint{ start.x + vector.x, start.y + vector.y };
		const float vectorAngle{ atan2(vector.x, vector.y) };
		const float arrowAngle{ g_Pi / 6 };
		const float arrowLength{ 10 };
		const Point2f p1{ vectorPoint.x + arrowLength * cosf(1.5f * g_Pi - vectorAngle - arrowAngle), vectorPoint.y + arrowLength * sinf(1.5f * g_Pi - vectorAngle - arrowAngle) };
		const Point2f p2{ vectorPoint.x + arrowLength * cosf(1.5f * g_Pi - vectorAngle + arrowAngle), vectorPoint.y + arrowLength * sinf(1.5f * g_Pi - vectorAngle + arrowAngle) };
		DrawLine(start, vectorPoint);
		FillTriangle(vectorPoint, p1, p2);
	}

	std::string ToString(const Vector2f& vector)
	{
		return "Vector [" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + "]";
	}

	Vector2f Add(const Vector2f& vector1, const Vector2f& vector2)
	{
		return Vector2f{ vector1.x + vector2.x, vector1.y + vector2.y };
	}

	Vector2f Subtract(const Vector2f& vector1, const Vector2f& vector2)
	{
		return Vector2f{ vector1.x - vector2.x, vector1.y - vector2.y };
	}

	float DotProduct(const Vector2f& vector1, const Vector2f& vector2)
	{
		return ((vector1.x * vector2.x) + (vector1.y * vector2.y));
	}

	float CrossProduct(const Vector2f& vector1, const Vector2f& vector2)
	{
		return (vector1.x * vector2.y) - (vector1.y * vector2.x);
	}

	Vector2f Scale(const Vector2f& vector, float scalar)
	{
		return Vector2f{ scalar * vector.x, scalar * vector.y };
	}

	float Length(const Vector2f& vector)
	{
		return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
	}

	Vector2f Normalize(const Vector2f& vector)
	{
		float length{ Length(vector) };
		return Vector2f{ vector.x / length, vector.y / length };
	}

	float AngleBetween(const Vector2f& vector1, const Vector2f& vector2)
	{
		return (atan2(CrossProduct(vector1, vector2), DotProduct(vector1, vector2)));
	}

	bool AreEqual(const Vector2f& vector1, const Vector2f& vector2)
	{
		const float margin{ 0.001f };

		float length1{ Length(vector1) }, length2{ Length(vector2) };
		float angle1{ tanf(vector1.y / vector1.x) }, angle2{ tanf(vector2.y / vector2.x) };

		if ((vector1.x <= vector2.x - margin) && (vector1.x >= vector2.x + margin))
		{
			if ((vector1.y <= vector2.y - margin) && (vector1.y >= vector2.y + margin))
			{
				return true;
			}
		}
		return false;
	}

	Vector2f PointToVector(const Point2f& point)
	{
		return Vector2f{ point.x, point.y };
	}

	Point2f VectorToPoint(const Vector2f& vector)
	{
		return Point2f{ vector.x, vector.y };
	}

#pragma endregion Vector

#pragma region MyUtils
	void DrawPentagram(const Point2f& center, float radius)
	{
		float angle{ (2 * g_Pi) / 5 };

		const Point2f p1{ center.x + (cosf(angle) * radius), center.y + (sinf(angle) * radius) };
		angle += 0.4f * g_Pi;
		const Point2f p2{ center.x + (cosf(angle) * radius), center.y + (sinf(angle) * radius) };
		angle += 0.4f * g_Pi;
		const Point2f p3{ center.x + (cosf(angle) * radius), center.y + (sinf(angle) * radius) };
		angle += 0.4f * g_Pi;
		const Point2f p4{ center.x + (cosf(angle) * radius), center.y + (sinf(angle) * radius) };
		angle += 0.4f * g_Pi;
		const Point2f p5{ center.x + (cosf(angle) * radius), center.y + (sinf(angle) * radius) };

		DrawLine(p1, p3);
		DrawLine(p1, p4);
		DrawLine(p2, p4);
		DrawLine(p2, p5);
		DrawLine(p3, p5);
	}

	void DrawPentagram(const Point2f& center, float radius, float angle)
	{
		float angleIncremented{ (2 * g_Pi) / 5 + angle };

		const Point2f p1{ center.x + (cosf(angleIncremented) * radius), center.y + (sinf(angleIncremented) * radius) };
		angleIncremented += 0.4f * g_Pi;
		const Point2f p2{ center.x + (cosf(angleIncremented) * radius), center.y + (sinf(angleIncremented) * radius) };
		angleIncremented += 0.4f * g_Pi;
		const Point2f p3{ center.x + (cosf(angleIncremented) * radius), center.y + (sinf(angleIncremented) * radius) };
		angleIncremented += 0.4f * g_Pi;
		const Point2f p4{ center.x + (cosf(angleIncremented) * radius), center.y + (sinf(angleIncremented) * radius) };
		angleIncremented += 0.4f * g_Pi;
		const Point2f p5{ center.x + (cosf(angleIncremented) * radius), center.y + (sinf(angleIncremented) * radius) };

		DrawLine(p1, p3);
		DrawLine(p1, p4);
		DrawLine(p2, p4);
		DrawLine(p2, p5);
		DrawLine(p3, p5);
	}

	void DrawConcentricSquares(const Point2f& position, float size, float amount)
	{
		float distance{ size / amount };

		for (float i = 1; i <= amount; i++)
		{
			DrawRect(position.x + i * distance / 2, position.y + i * distance / 2, size, size);
			size -= distance;
		}
	}

	void DrawLinearGradient(const Rectf& rect, const Color4f& left, const Color4f& right)
	{
		Color4f copyLeft{ left };
		float rectLeftValue{ rect.left }, steps{ rect.width - rect.left };
		float differenceR{ (right.r - left.r) / steps },
			differenceG{ (right.g - left.g) / steps },
			differenceB{ (right.b - left.b) / steps };

		for (float i = 0; i <= steps; i++)
		{
			SetColor(copyLeft);
			utils::FillRect(Rectf(rectLeftValue, rect.bottom, 1, rect.height));

			copyLeft.r += differenceR;
			copyLeft.g += differenceG;
			copyLeft.b += differenceB;
			++rectLeftValue;
		}
	}

	int GetRand(int min, int max)
	{
		return rand() % (max - min + 1) + min;
	}

	float GetRand(float min, float max)
	{
		int iMin{ int(min * 100) };
		int iMax{ int(max * 100) };
		float randNr{ (rand() % (iMax - iMin + 1) + iMin) / 100.f };
		return randNr;
	}

	void Swap(int* pInt, int idx1, int idx2)
	{
		int temp{ pInt[idx1] };
		pInt[idx1] = pInt[idx2];
		pInt[idx2] = temp;
	}

	void Swap(Point2f* pPoints, int idx1, int idx2)
	{
		Point2f temp{ pPoints[idx1] };
		pPoints[idx1] = pPoints[idx2];
		pPoints[idx2] = temp;
	}

	void Swap(Texture* pText, int idx1, int idx2)
	{
		Texture temp{ pText[idx1] };
		pText[idx1] = pText[idx2];
		pText[idx2] = temp;
	}

	void Shuffle(int* pInt, int size, int amount)
	{
		int idx1{}, idx2{};
		for (int i{ 0 }; i < amount; i++)
		{
			idx1 = std::rand() % size;
			idx2 = std::rand() % size;
			Swap(pInt, idx1, idx2);
		}
	}

	void Shuffle(Texture* pText, int size, int amount)
	{
		int idx1{}, idx2{};
		for (int i{ 0 }; i < amount; i++)
		{
			idx1 = std::rand() % size;
			idx2 = std::rand() % size;
			Swap(pText, idx1, idx2);
		}
	}

	void BubbleSort(int* pInt, int size)
	{
		if (size == 1) return;

		for (int i{ 0 }; i < size - 1; i++)
		{
			if (pInt[i] > pInt[i + 1])
			{
				Swap(pInt, i, i + 1);
			}
		}
		BubbleSort(pInt, size - 1);
	}

	int GetLinearIndexFrom2DIndex(int rowIndex, int columnIndex, int nrOfColumns)
	{
		return rowIndex * nrOfColumns + columnIndex;
	}

	Point2D Get2DIndexFromLinearIndex(int linearIndex, int columns)
	{
		int row{ linearIndex / columns };
		int column{ linearIndex % columns };
		return Point2D{ row, column };
	}
#pragma endregion MyUtils
}