#include "library.h"

namespace image {


	void Image::init(std::string path) {
		SDL_Surface* temp = IMG_Load(path.c_str());

		if (temp == nullptr) {
			std::cout << path << " doesn't exist..." << std::endl;
			return;
		}

		this->width = temp->w;
		this->height = temp->h;
		this->bbp = temp->format->BytesPerPixel;
		
		this->pixels.resize(this->width * this->height * this->bbp);

		memcpy(this->pixels.data(), temp->pixels, this->pixels.size());

		SDL_FreeSurface(temp);
	}

	void Image::release() {
		this->pixels.clear();
	}

}