#include <SDL3/SDL.h>


struct GameData {
	const SDL_WindowFlags window_flags{ 0u };
	SDL_Window* window_ptr{ nullptr };

	bool game_is_running{ false };

	SDL_Event events;

	SDL_Renderer* renderer_ptr{ nullptr };
} GameData{};


void init() {
	SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO );
	GameData.window_ptr = SDL_CreateWindow( "Number Dungeon", 640u, 360u, GameData.window_flags );
	SDL_zero( GameData.events );
	GameData.game_is_running = true;
	GameData.renderer_ptr = SDL_CreateRenderer( GameData.window_ptr, nullptr );
}
void exit() {
	SDL_DestroyRenderer( GameData.renderer_ptr );
	SDL_DestroyWindow( GameData.window_ptr );
}
void events( SDL_Event& event ) {
	while( SDL_PollEvent( &event ) )
		if( event.type == SDL_EVENT_QUIT ) {
			GameData.game_is_running = false;
			break;
		}
}
void update() {
}
void render() {
	SDL_SetRenderDrawColor( GameData.renderer_ptr, 30u, 30u, 45u, 255u );
	SDL_RenderClear( GameData.renderer_ptr );
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