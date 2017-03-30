#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#define WND_HEIGHT 600
#define WND_WIDTH 600
#define WND_BPP 32 // bits per pixel

int main(int argc, char *argv[]) {
    // Firstly, init everything

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        std::cerr << "SDL_Init() failed" << std::endl;

        SDL_Quit();
        return 1;
    }

    // Create our game screen
    SDL_Window *screen = SDL_CreateWindow("Fluid simulation", 100, 100, WND_WIDTH, WND_HEIGHT, SDL_WINDOW_SHOWN);

    // Create our renderer for the fluid sim
    // Third param requests GPU acceleration and vsync
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Get the surface our texture will be drawn on
    SDL_Surface *surface = SDL_CreateRGBSurface(0, WND_WIDTH, WND_HEIGHT, WND_BPP, 0, 0, 0, 0);

    // Create our texture we will draw the pixels on
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);

    // debugging code
    const unsigned red = SDL_MapRGBA(surface->format, 0x1E, 0x1E, 0x1E, 0xFF);
    const unsigned gray = SDL_MapRGBA(surface->format, 0xFA, 0x32, 0x32, 0xFF);

    // Main drawing loop
    bool running = true;
    SDL_Event event;
    while (running) {
        const unsigned long start = SDL_GetPerformanceCounter();

        // Poll for events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                running = false;
                break;
            }
        }

        // put actual rendering code here
        // START DEBUG
        unsigned *pixels = (unsigned*) surface->pixels;
        for (int i = 0; i < WND_HEIGHT; i++) {
            for (int j = 0; j < WND_WIDTH; j++) {
                pixels[i * WND_WIDTH + j] = j % 3 ? gray : red;
            }
        }

        SDL_UpdateTexture(tex, NULL, surface->pixels, surface->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Clear pointers and destroy SDL resources
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(tex);
    surface = nullptr;
    screen = nullptr;
    renderer = nullptr;
    tex = nullptr;

    SDL_Quit();
    return 0;
}