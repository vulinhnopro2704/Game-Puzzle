#include "LTexture.h"

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path, bool isSetColorKey)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		if (isSetColorKey)
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, int FontSize)
{
	//Get rid of preexisting texture
	free();
	gFont = TTF_OpenFont("Font//Bungee-Regular.ttf", FontSize);
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			//Create texture from surface pixels
			mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
			if (mTexture == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Get image dimensions
				mWidth = textSurface->w;
				mHeight = textSurface->h;
			}

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}
		//Return success
		return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Set Modulate Color picture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::SetBlendMode(SDL_BlendMode blending)
{
	//Set Blending Function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::SetAlpha(Uint8 alpha) {
	//Modulate Texture Alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::Resize(int newWidth, int newHeight)
{
	// Kiểm tra xem tấm ảnh cơ sở đã được tạo chưa
	if (mTexture)
	{
		// Tạo một tấm ảnh mới với kích thước mới
		SDL_Texture* newTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, newWidth, newHeight);

		// Kiểm tra xem việc tạo tấm ảnh mới có thành công không
		if (newTexture != NULL)
		{
			// Tạo một tấm ảnh tạm thời để chứa nội dung cũ
			SDL_Texture* tempTexture = NULL;

			// Đặt tấm ảnh tạm thời làm mục tiêu vẽ
			SDL_SetRenderTarget(gRenderer, tempTexture);

			// Xóa nền bằng màu trong suốt
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
			SDL_RenderClear(gRenderer);

			// Đặt mTexture làm mục tiêu vẽ
			SDL_SetRenderTarget(gRenderer, newTexture);

			// Vẽ tấm ảnh cũ vào tấm ảnh tạm thời và thay đổi kích thước
			SDL_RenderCopy(gRenderer, mTexture, NULL, NULL);

			// Đặt lại mTexture làm mục tiêu vẽ
			SDL_SetRenderTarget(gRenderer, NULL);

			// Xóa tấm ảnh cũ
			SDL_DestroyTexture(mTexture);

			// Gán tấm ảnh tạm thời vào mTexture và cập nhật kích thước
			mTexture = newTexture;
			mWidth = newWidth;
			mHeight = newHeight;
		}
		else
		{
			printf("Unable to create new texture! SDL Error: %s\n", SDL_GetError());
		}
	}
}



void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	
	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}


int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

