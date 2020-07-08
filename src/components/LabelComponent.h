#ifndef LABELCOMPONENT_H
#define LABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../EntityManager.cpp"
#include "../Game.cpp"
#include "../AssetManager.cpp"

class LabelComponent: public Component {
private:
    SDL_Rect position;
    std::string text;
    std::string fontFamily;
    SDL_Color color;
    SDL_Texture* texture;

public:
    LabelComponent();

    LabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color color) {
        this->position.x = x;
        this->position.y = y;
        this->text = text;
        this->fontFamily = fontFamily;
        this->color = color;
        this->SetLabelText(text, fontFamily);
    }

    void SetLabelText(std::string text, std::string fontFamily) {
        SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text.c_str(), color);
        texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
        SDL_FreeSurface(surface);
        SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
    }

    void Render() override {
        FontManager::Draw(texture, position);
    }
};

#endif /* LABELCOMPONENT_H */
