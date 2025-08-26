#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


struct GameData {
	const SDL_WindowFlags window_flags{ 0u };
	SDL_Window* window_ptr{ nullptr };

	bool game_is_running{ false };

	SDL_Event events;

	SDL_Renderer* renderer_ptr{ nullptr };

	SDL_Texture* assets_texture_ptr{ nullptr };

	unsigned int choice{ 0u };
	unsigned int answer{ 0u };
	unsigned int level{ 0u };
} GameData{};


struct Mouse{
	struct pos {
		float x, y;
	} pos{};
	struct button {
		bool raw, state, up, down;
	} left{}, right{};
} Mouse{};


struct Button_t {
	SDL_FRect area;
	bool hover, pressed, held, released;
};

SDL_FRect get_number( unsigned int number, const unsigned int& place_value = 0u ) {
	number /= (unsigned int)SDL_powf( 10.0f, (float)place_value );
	number %= 10u;
	return SDL_FRect{
		.x = number % 7u * 9.0f,
		.y = SDL_floorf( number / 7.0f ) * 14.0f,
		.w =  9.0f,
		.h = 14.0f,
	};
}


SDL_Texture* load_texture( SDL_Renderer* renderer_ptr, const char* file_path, const SDL_ScaleMode& scale_mode ) {
	char buffer[256u];
	SDL_snprintf( buffer, SDL_arraysize( buffer ), "%s../assets/%s", SDL_GetBasePath(), file_path );
	SDL_Texture* texture_ptr = IMG_LoadTexture( GameData.renderer_ptr, buffer );
	SDL_SetTextureScaleMode( texture_ptr, scale_mode );
	return texture_ptr;
}


void init() {
	SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO );
	GameData.window_ptr = SDL_CreateWindow( "Number Dungeon", 640u, 360u, GameData.window_flags );
	SDL_zero( GameData.events );
	GameData.game_is_running = true;
	GameData.renderer_ptr = SDL_CreateRenderer( GameData.window_ptr, nullptr );
	SDL_SetRenderDrawBlendMode( GameData.renderer_ptr, SDL_BLENDMODE_BLEND );
	GameData.assets_texture_ptr = load_texture( GameData.renderer_ptr, "./assets.png", SDL_SCALEMODE_PIXELART );
}
void exit() {
	SDL_DestroyTexture( GameData.assets_texture_ptr );
	SDL_DestroyRenderer( GameData.renderer_ptr );
	SDL_DestroyWindow( GameData.window_ptr );
}
void events( SDL_Event& event ) {
	while( SDL_PollEvent( &event ) ) {
		if( event.type == SDL_EVENT_QUIT ) {
			GameData.game_is_running = false;
			break;
		}
		if( event.type == SDL_EVENT_MOUSE_BUTTON_UP ) {
			if( event.button.button == SDL_BUTTON_LEFT )
				Mouse.left.raw = false;
			if( event.button.button == SDL_BUTTON_RIGHT )
				Mouse.right.raw = false;
		}
		if( event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ) {
			if( event.button.button == SDL_BUTTON_LEFT )
				Mouse.left.raw = true;
			if( event.button.button == SDL_BUTTON_RIGHT )
				Mouse.right.raw = true;
		}
	}
	SDL_GetMouseState( &Mouse.pos.x, &Mouse.pos.y );
	const bool& l_raw   = Mouse.left.  raw;
	const bool& l_state = Mouse.left.state;
	Mouse.left.down  =  l_raw && !l_state;
	Mouse.left.up    = !l_raw &&  l_state;
	Mouse.left.state =  l_raw;
	const bool& r_raw   = Mouse.right.  raw;
	const bool& r_state = Mouse.right.state;
	Mouse.right.down  =  r_raw && !r_state;
	Mouse.right.up    = !r_raw &&  r_state;
	Mouse.right.state =  r_raw;
}
void enter_choice( const unsigned int& choice ) {
	if( choice == GameData.answer ) {
		GameData.level += 1u;
		GameData.answer = (unsigned int)SDL_rand( SDL_min( GameData.level, 9u ) );
	} else {
		GameData.level = 0u;
		GameData.answer = 0u;
	}
}
void update() {
	if( Mouse.right.up )
		GameData.choice = GameData.choice + 1u % 10u;
	if( Mouse.left.up ) {
		enter_choice( GameData.choice );
		GameData.choice = 0u;
	}
}
void render_level_number( const int& level_number ) {
	SDL_FRect src_rect{
		.x =  0.0f,
		.y = 42.0f,
		.w = 56.0f,
		.h = 14.0f,
	}, dst_rect{
		.x = 0.0f,
		.y = 0.0f,
		.w = src_rect.w * 2.0f,
		.h = src_rect.h * 2.0f,
	};
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
	dst_rect.x += dst_rect.w;
	const unsigned int place_count{ (unsigned int)SDL_log10f( level_number ) + 1u };
	for( unsigned int i{ 0u }; i < place_count; i++ ) {
		src_rect = get_number( level_number, place_count - 1u - i );
		dst_rect.w = src_rect.w * 2.0f;
		dst_rect.h = src_rect.h * 2.0f;
		SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
		dst_rect.x += dst_rect.w;
	}
}
void render_question( const unsigned int& a, const unsigned int& b ) {
	SDL_FRect src_rect{
		.x =  0.0f,
		.y = 29.0f,
		.w = 54.0f, 
		.h = 13.0f,
	}, dst_rect{
		.x = 280.0f,
		.y = 120.0f,
		.w = src_rect.w * 3.0f,
		.h = src_rect.h * 3.0f,
	};
	dst_rect.x -= dst_rect.w * 0.5f;
	dst_rect.y -= dst_rect.h * 0.5f;
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
	src_rect = {
		.x = 36.0f,
		.y = 15.0f,
		.w = 18.0f, 
		.h = 13.0f,
	};
	dst_rect.x += dst_rect.w + 10.0f;
	dst_rect.w = src_rect.w * 3.0f;
	dst_rect.h = src_rect.h * 3.0f;
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
	src_rect = {
		.x = 27.0f, 
		.y = 15.0f,
		.w = 10.0f,
		.h = 13.0f,
	};
	dst_rect = {
		.x = 320.0f,
		.y = 165.0f,
		.w = src_rect.w * 3.0f,
		.h = src_rect.h * 3.0f,
	};
	dst_rect.x -= dst_rect.w * 0.5f;
	dst_rect.y -= dst_rect.h * 0.5f;
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
	src_rect = get_number( a );
	dst_rect.x -= dst_rect.w;
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
	src_rect = get_number( b );
	dst_rect.x += dst_rect.w * 2.0f - 3.0f;
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
}
void render_choice( const unsigned int& choice ) {
	SDL_FRect src_rect = get_number( choice ), dst_rect{
		.x = 320.0f,
		.y = 300.0f,
		.w = src_rect.w * 3.0f,
		.h = src_rect.h * 3.0f,
	};
	dst_rect.x -= dst_rect.w * 0.5f;
	dst_rect.y -= dst_rect.h * 0.5f;
	SDL_RenderTexture( GameData.renderer_ptr, GameData.assets_texture_ptr, &src_rect, &dst_rect );
}
void render() {
	SDL_SetRenderDrawColor( GameData.renderer_ptr, 30u, 30u, 45u, 255u );
	SDL_RenderClear( GameData.renderer_ptr );
	render_level_number( GameData.level );
	render_question( 0u, SDL_min( GameData.level, 9u ) );
	render_choice( GameData.choice );
	SDL_RenderPresent( GameData.renderer_ptr );
}
void loop() {
	while( GameData.game_is_running ) {
		events( GameData.events );
		update();
		render();
	}
}


int main( int argc, char** argv );
int WinMain( int argc, char** argv ) { return main( argc, argv ); }

int main( int argc, char** argv ) {
	init();
	loop();
	exit();
	return 0;
}