#include "ui.hh"

bool UI::Button::IsClicked(Game game) {
	if (!game.mouseLeftPressed) {
		return false;
	}
	return (
		(game.mousePos.x >= rect.x) &&
		(game.mousePos.x <= rect.x + rect.w) &&
		(game.mousePos.y >= rect.h) &&
		(game.mousePos.y <= rect.y + rect.h)
	);
}

void UI::Button::Render(Game game) {
	SDL_SetRenderDrawColor(game.renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(game.renderer, &rect);
	SDL_SetRenderDrawColor(game.renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game.renderer, &rect);

	SDL_Rect textLocation = game.GetTextSize(text, 2);
	textLocation.x = (rect.w / 2) - (textLocation.w / 2);
	textLocation.y = (rect.h / 2) - (textLocation.h / 2);

	game.RenderText(text, rect.x, rect.y, 2);
}