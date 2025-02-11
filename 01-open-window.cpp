#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <format>

void initialize_sdl();
void close_sdl();

class Game
{
public:
    Game();
    void init();
    void run();

    static constexpr int with{800};
    static constexpr int height{600};

private:
    const std::string title;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};

Game::Game() : title{"Open Window"}, window{nullptr, SDL_DestroyWindow}, renderer{nullptr, SDL_DestroyRenderer} {}

void Game::init()
{
    this->window.reset(SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, this->with, this->height, 0));
    if (!this->window)
    {
        auto error = std::format("Failed to create window: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->renderer.reset(SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED));

    if (!this->renderer)
    {
        auto error = std::format("Failed to create renderer: {}", SDL_GetError());
        throw std::runtime_error(error);
    }
}

void Game::run()
{
    SDL_RenderClear(this->renderer.get());
    SDL_RenderPresent(this->renderer.get());
    SDL_Delay(2000);
}

void initialize_sdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        auto error = std::format("Error initialize SDL2: {}", SDL_GetError());
        throw std::runtime_error(error);
    }
}

void close_sdl()
{
    SDL_Quit();
}

int main(int arg, char **args)
{
    int exit_val = EXIT_SUCCESS;

    try
    {
        initialize_sdl();
        Game game;
        game.init();
        game.run();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        exit_val = EXIT_FAILURE;
    }

    close_sdl();

    return 0;
}