#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <filesystem>

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

    SDL_AudioStream* audio_stream;
    float audio_buffer[65536];

public:

    Display(std::string filename, SDL_Renderer* renderer, SDL_Texture* screen_texture, const SDL_PixelFormatDetails* pixel_format_details, SDL_AudioStream* audio_stream)
    :
    renderer(renderer),
    screen_texture(screen_texture),
    audio_stream(audio_stream) {

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

    bool OnUserUpdate(float delta_time) {
        updateControllerStates();

        if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
            return false;
        } else if (keyboard_state[SDL_SCANCODE_R]) {
            nes->reset();
        }

        int audio_samples_to_generate = 44100 * delta_time;
        if (audio_samples_to_generate <= sizeof(audio_buffer) / sizeof(float)) {
            for (int i = 0; i < audio_samples_to_generate; i++) {
                while (!nes->clockForAudioSample()) {
                    // do nothing - wait for audio sample to be available
                }

                audio_buffer[i] = nes->getAudioSample();
            }

            SDL_PutAudioStreamData(audio_stream, audio_buffer, audio_samples_to_generate * sizeof(float));
        }

        renderDisplay();

        return true;
    }

    void cleanup() {
        delete nes;
        delete cartridge;
    }

    void updateControllerStates() {
        Uint8 controller1State = 0x00;
        controller1State |= (keyboard_state[SDL_SCANCODE_K] ? 0x01 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_J] ? 0x02 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_U] ? 0x04 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_I] ? 0x08 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_W] ? 0x10 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_S] ? 0x20 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_A] ? 0x40 : 0x00);
        controller1State |= (keyboard_state[SDL_SCANCODE_D] ? 0x80 : 0x00);

        // controller 2 is unimplemented for now
        Uint8 controller2State = 0x00;

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
SDL_AudioStream* audio_stream;

// I know that this being global isn't the best decision, but it's such a
// pain to pass it all the way down to the actual SaveRam instance. 🤷🏻‍♂️
std::string sram_file_name;

// --------------------------------------------------------------------------

void setup_sram_file_name(const std::string& rom_file_name) {
    std::filesystem::path path(rom_file_name);

    if (path.has_parent_path()) {
        sram_file_name = path.parent_path();
        sram_file_name += "/";
        sram_file_name += path.stem();
        sram_file_name += ".sram";
    } else {
        sram_file_name = path.stem();
        sram_file_name += ".sram";
    }
}

// --------------------------------------------------------------------------

void cleanup() {
    if (audio_stream != nullptr) {
        SDL_DestroyAudioStream(audio_stream);
    }

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
    setup_sram_file_name(filename);

    // --- sdl3 setup ---

    const int NES_SCREEN_WIDTH = 256;
    const int NES_SCREEN_HEIGHT = 240;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("NES Emulator", NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
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

    SDL_SetRenderVSync(renderer, 1);

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

    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_F32;
    spec.channels = 1;
    spec.freq = 44100;
    audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
    if (audio_stream == nullptr) {
        std::cerr << "[ERROR] SDL_OpenAudioDeviceStream error:" << SDL_GetError() << std::endl;
        cleanup();
        return 1;
    }

    if (!SDL_ResumeAudioStreamDevice(audio_stream)) {
        std::cerr << "[ERROR] SDL_ResumeAudioStreamDevice error:" << SDL_GetError() << std::endl;
        cleanup();
        return 1;
    }

    // --- main loop setup ---

    Display display(filename, renderer, screen_texture, pixel_format_details, audio_stream);

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
