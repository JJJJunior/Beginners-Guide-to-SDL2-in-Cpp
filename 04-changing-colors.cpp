#include <iostream>
#include <memory>
#include <format>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <random>

void initialize_sdl();
void close_sdl();

class Game
{
public:
    Game();
    void init();
    void run();
    void load_media();

    static constexpr int width{800};
    static constexpr int height{600};

private:
    const std::string title;
    SDL_Event event;
    std::mt19937 gen;
    std::uniform_int_distribution<Uint8> rand_color;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> backgroud;
};

Game::Game() : title{"Changing Colors"}, gen{}, rand_color{0, 255},
               window(nullptr, SDL_DestroyWindow),
               renderer(nullptr, SDL_DestroyRenderer),
               backgroud(nullptr, SDL_DestroyTexture) {}

void Game::init()
{
    this->window.reset(SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, this->width, this->height, 0));
    if (!this->window.get())
    {
        auto error = std::format("Failed to create window: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->renderer.reset(SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!this->renderer.get())
    {
        auto error = std::format("Failed to create renderer: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->gen.seed(std::random_device()());
}

void Game::load_media()
{
    this->backgroud.reset(IMG_LoadTexture(this->renderer.get(), "images/background.png"));
    if (!this->backgroud)
    {
        auto error = std::format("Error loading Texture: {}", IMG_GetError());
        throw std::runtime_error(error);
    }
}

void Game::run()
{
    while (true)
    {
        while (SDL_PollEvent(&this->event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    return;
                    break;
                case SDL_SCANCODE_SPACE:
                    SDL_SetRenderDrawColor(
                        this->renderer.get(), this->rand_color(gen),
                        this->rand_color(gen), this->rand_color(gen), 255);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        SDL_RenderClear(this->renderer.get());

        SDL_RenderCopy(this->renderer.get(), this->backgroud.get(), nullptr, nullptr);

        SDL_RenderPresent(this->renderer.get());
        // 1000/60  milliseconds/seconds 1second 1 frame;
        SDL_Delay(16);
    }
}

/*
BMP JPG PNG TIF

1000
0100
0010
0001

*/

void initialize_sdl()
{
    int sdl_flags = SDL_INIT_EVERYTHING;
    int img_flags = IMG_INIT_PNG;

    if (SDL_Init(sdl_flags))
    {
        auto error = std::format("Failed to initialize SDL: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    if ((IMG_Init(img_flags) & img_flags) != img_flags)
    {
        auto error = std::format("Error initialize SDL_image: {}", IMG_GetError());
        throw std::runtime_error(error);
    }
}

void close_sdl()
{
    IMG_Quit();
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
        game.load_media();
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