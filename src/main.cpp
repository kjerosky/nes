#include <string>
#include <sstream>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <iostream>

#include "TextUtils.h"
#include "Nes.h"
#include "Cartridge.h"

class Display {

private:

    SDL_Renderer* renderer;
    Nes* nes;
    Cartridge* cartridge;

    const bool* keyboard_state;

    SDL_Texture* screen_texture;
    SDL_FRect target_texture_rect;

public:

    Display(std::string filename, SDL_Renderer* renderer, SDL_Texture* screen_texture, const SDL_PixelFormatDetails* pixel_format_details)
    :
    renderer(renderer),
    screen_texture(screen_texture) {

        cartridge = new Cartridge(filename);
        if (!cartridge->isValid()) {
            exit(EXIT_FAILURE);
        }

        nes = new Nes(cartridge, screen_texture, pixel_format_details);

        int audioSampleFrequency = 44100;
        nes->setAudioSampleRate(audioSampleFrequency);

        keyboard_state = SDL_GetKeyboardState(nullptr);

        target_texture_rect = { 0, 0, static_cast<float>(screen_texture->w), static_cast<float>(screen_texture->h) };
    }

    void fit_inner_rect_within_outer_rect(const SDL_FRect& inner_rect, const SDL_FRect& outer_rect, SDL_FRect& result) {
        float inner_aspect_ratio = inner_rect.w / inner_rect.h;

        float vertical_size_after_horizontal_expansion = outer_rect.w / inner_aspect_ratio;
        bool expanding_inner_rect_horizontally_fits_vertically = vertical_size_after_horizontal_expansion <= outer_rect.h;

        if (expanding_inner_rect_horizontally_fits_vertically) {
            result.w = outer_rect.w;
            result.h = vertical_size_after_horizontal_expansion;
            result.x = 0.0f;
            result.y = (outer_rect.h - result.h) / 2.0f;
        } else {
            float horizontal_size_after_vertical_expansion = outer_rect.h * inner_aspect_ratio;

            result.w = horizontal_size_after_vertical_expansion;
            result.h = outer_rect.h;
            result.x = (outer_rect.w - result.w) / 2.0f;
            result.y = 0.0f;
        }
    }

    float createAudioSample(int channel, float globalTime, float timestep) {
        if (channel != 0) {
            return 0;
        }

        while(!nes->clockForAudioSample()) {
            // do nothing - just wait for the audio sample to be available
        }

        return nes->getAudioSample();
    }

    bool OnUserUpdate(float fElapsedTime) {
        updateControllerStates();

        if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
            return false;
        } else if (keyboard_state[SDL_SCANCODE_R]) {
            nes->reset();
        }

        //TODO THIS WILL RUN THE NES FOR NOW, BUT IT'S JUST TEMPORARY!!!
        nes->processTimeElapsed(fElapsedTime);

        renderDisplay();

        return true;
    }

    void cleanup() {
        delete nes;
        delete cartridge;
    }

    void updateControllerStates() {
        nesByte controller1State = 0x00;
        controller1State |= (keyboard_state[SDL_SCANCODE_K] ? 0x01 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_J] ? 0x02 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_L] ? 0x04 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_O] ? 0x08 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_W] ? 0x10 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_S] ? 0x20 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_A] ? 0x40 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_D] ? 0x80 : 0x00);

        // controller 2 is unimplemented for now
        nesByte controller2State = 0x00;

        nes->updateControllerStates(controller1State, controller2State);
    }

    void renderDisplay() {
        int render_width, render_height;
        SDL_GetRenderOutputSize(renderer, &render_width, &render_height);

        SDL_FRect texture_fit_to_window_rect;
        SDL_FRect render_rect = { 0.0f, 0.0f, static_cast<float>(render_width), static_cast<float>(render_height) };
        fit_inner_rect_within_outer_rect(target_texture_rect, render_rect, texture_fit_to_window_rect);

        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, screen_texture, nullptr, &texture_fit_to_window_rect);
        SDL_RenderPresent(renderer);
    }
};

// --------------------------------------------------------------------------

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* screen_texture = nullptr;

// --------------------------------------------------------------------------

void cleanup() {
    if (screen_texture != nullptr) {
        SDL_DestroyTexture(screen_texture);
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }

    SDL_Quit();
}

// --------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    // --- arguments parsing and validation ---

    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    std::string filename = argv[1];

    // --- sdl3 setup ---

    const int NES_SCREEN_WIDTH = 256;
    const int NES_SCREEN_HEIGHT = 240;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("Test", NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "[ERROR] SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        cleanup();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cerr << "[ERROR] SDL_CreateRenderer error:" << SDL_GetError() << std::endl;
        cleanup();
        return 1;
    }

    screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT);
    if (screen_texture == nullptr) {
        std::cerr << "[ERROR] SDL_CreateTexture error:" << SDL_GetError() << std::endl;
        cleanup();
        return 1;
    }

    SDL_SetTextureScaleMode(screen_texture, SDL_SCALEMODE_NEAREST);

    const SDL_PixelFormatDetails* pixel_format_details = SDL_GetPixelFormatDetails(screen_texture->format);
    if (pixel_format_details == nullptr) {
        std::cerr << "[ERROR] SDL_GetPixelFormatDetails error:" << SDL_GetError() << std::endl;
        cleanup();
        return 1;
    }

    // --- main loop setup ---

    Display display(filename, renderer, screen_texture, pixel_format_details);

    Uint64 previous_time_ms = SDL_GetTicks();

    // --- main loop ---

    bool is_running = true;
    SDL_Event event;
    while (is_running) {
        Uint64 current_time_ms = SDL_GetTicks();
        float delta_time = (current_time_ms - previous_time_ms) / 1000.0f;
        previous_time_ms = current_time_ms;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }

        if (is_running) {
            is_running = display.OnUserUpdate(delta_time);
        }
    }

    // --- cleanup and quit ---

    display.cleanup();

    cleanup();

    return 0;
}
