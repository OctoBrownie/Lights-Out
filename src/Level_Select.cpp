#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <cmath> 

#include "general_SDL_funcs.h"
#include "Level_Select.h"

Level_Select::Level_Select(SDL_Renderer* ren, std::vector<std::string> menu_options, int screen_height,
			int screen_width) : Menu(ren, "Level Select", menu_options, 20, 20, "res\\Courier font.ttf",
			true, screen_height, screen_width) {
	// reinit the option font and menu_font
	option_font_size = 30;
	menu_font = load_TTF_font(renderer, font_file, option_font_size);
	
	int option_w, option_h;
	// getting the width of the options (assumed of the same character length)
	{
		SDL_Texture* tex;
		try {
			SDL_Color color = {0, 0, 0, 255};
			tex = font_to_tex(ren, menu_font, menu_options[0], color);
			SDL_QueryTexture(tex, NULL, NULL, &option_w, &option_h);
			cleanup(tex);
		}
		catch (...) {
			cleanup(tex);
			throw "Level_Select::Level_Select() ERROR: Constructor could not be executed.\n";
		}
	}
	
	int max_width = screen_width - 4*pad_x;
	int max_height = screen_height - 6*pad_y - 2*title_font_size;
	
	// come up with the optimal number of rows and columns
	if (menu_options.size() <= 0) {
		throw "Level_Select::Level_Select() ERROR: No levels in menu array.\n";
	}
	int curr_rows = max_width / (2*pad_x + option_w); 
	int curr_cols = ceil(array_size*1.0 / curr_rows);
	int max_cols = max_height / (pad_y + option_h);

	if (max_cols < curr_cols) {
		throw "Level_Select::Level_Select() ERROR: Window is too small to display all levels.\n";
	}

	rows = curr_rows;
	cols = curr_cols;
	while (max_cols >= curr_cols && curr_rows*curr_cols >= menu_options.size()) {	// everything fits
		// fits the array size better, rows/cols are more equal, and array is horizontal
		if (curr_rows*curr_cols - menu_options.size() <= rows*cols - menu_options.size() 
			&& curr_rows - curr_cols <= rows - cols && curr_rows >= curr_cols) {
			rows = curr_rows;
			cols = curr_cols;
		}
		curr_rows--;
		curr_cols = ceil(menu_options.size()*1.0 / curr_rows);
	}
	
	// init level array rectangle
	level_array_rect.x = 2*pad_x;
	level_array_rect.y = 3*pad_y + title_font_size;
	
	// the # of elements per row = the # of columns, conversely, the # of elements per column = the # of rows
	level_array_rect.w = cols*(2*pad_x + option_w);		// TODO: check that the padding totals to 2*pad_x
	level_array_rect.h = rows*(2*pad_y + option_h);		// TODO: check that the padding totals to 2*pad_y
}

int Level_Select::render_menu() {
	Uint8 r, g, b, a;       // original draw color
    SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

    // menu background
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &menu_rect);

    SDL_Color title_color = {0, 0, 0, 255};

    // draw title
    SDL_Texture* title_tex = font_to_tex(ren, title_font, title, title_color);
    SDL_Rect title_dest_rect;
    SDL_QueryTexture(title_tex, NULL, NULL, &title_dest_rect.w, &title_dest_rect.h);

    title_dest_rect.y = menu_rect.y + pad_y;
	title_dest_rect.x = menu_rect.w / 2 - title_dest_rect.w / 2;
    SDL_RenderCopy(ren, title_tex, NULL, &title_dest_rect);
    cleanup(title_tex);

    // grid of levels, not including the "Back" option
    for (unsigned int i = 0; i < menu_options.size() - 1; i++) {
        int curr_row = i % rows;
		int curr_col = i / rows;

        SDL_Color option_color = title_color;

        // TODO: Define text_box
        SDL_Rect option_box;
        // option_box.x = level_array_rect.x + pad_x + curr_row*(option_width + padding between options + padding within option box);
		// option_box.y = level_array_rect.y + pad_y + curr_col*(option_font_size + padding between options + padding within option box);
		// option_box.w = padding within option box + option_width

		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

        if (i == active_menu_option) {
            // level with highlighted background (b/c is active)
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			option_color.r = 255;
			option_color.g = 255;
			option_color.b = 255;
        }
        else {
			// level with normal background
			SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        }
		SDL_RenderFillRect(ren, &option_box);
		SDL_SetRenderDrawColor(ren, r, g, b, a);

		// box outline
		SDL_RenderDrawRect(ren, &option_box);

		// actual text
		SDL_Texture* option_tex = font_to_tex(ren, menu_font, menu_options[i], option_color);

        // TODO: Change text_box (use option_box values)
        SDL_Rect text_box;
        SDL_QueryTexture(option_tex, NULL, NULL, &text_box.w, &text_box.h);
		text_box.x = level_array_rect.x + pad_x;
        text_box.y = level_array_rect.y + title_font_size + 3*pad_y + (option_font_size + pad_y)*curr_col;

        SDL_RenderCopy(ren, option_tex, NULL, &option_dest_rect);
    }
    // TODO: render the "Back" option
	// anchored to the bottom of the level_array_rect
    // use title_color

    SDL_SetRenderDrawColor(ren, r, g, b, a);
    return 0;
}

// TODO: Level_Select::keyboard_input_down
void keyboard_input_down(const SDL_Event* event) {}

// TODO: Level_Select::get_option_at_x_y
int get_option_at_x_y(const int x, const int y) {}
