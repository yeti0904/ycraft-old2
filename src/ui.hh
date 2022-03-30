#include "_components.hh"
#include "game.hh"

class Game;

#ifndef HEADER_UI_CLASS
#define HEADER_UI_CLASS
namespace UI {
	class Button {
		public:
			// variables
			SDL_Rect  rect;
			string    text;
			TTF_Font* font;
			
			// functions
			bool IsClicked(Game game);
			void Render(Game game);
	};
}
#endif