#pragma once
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#define BUFFER 2048
#define DR_FLAC_IMPLEMENTATION
#define DR_MP3_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION
#include "dr_flac.h"
#include "dr_mp3.h"
#include "dr_wav.h"

#include "filesystem/filesystem_path.hpp"
using namespace CPPN::FileSystem;

namespace CPPN::Audio {

    // =====================================
    // 🔊 Decoded Audio Data Struct
    // =====================================
    struct DecodedAudio {
        float* samples = nullptr;
        uint64_t frameCount = 0;
        int channels = 0;
        int sampleRate = 0;
    };

    // Forward declaration for class Sound so globals can reference it
    class Sound;

    // =====================================
    // 🎚️ Global Audio State
    // =====================================
    inline std::vector<Sound*> sounds;
    inline int MAX_SOUNDS = 10;
    inline SDL_AudioSpec spec{};
    SDL_AudioDeviceID device;

    // =====================================
    // ⚙️ Audio Core Functions
    // =====================================
    inline void Init() {
        SDL_zero(spec);
        spec.freq = 48000;
        spec.format = AUDIO_F32;
        spec.channels = 2;
        spec.samples = BUFFER;

        device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
        if (!device)
        {printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());}
        SDL_PauseAudioDevice(device, 0);
    }

    inline void RegisterSound(Sound* sound) {
        if (std::find(sounds.begin(), sounds.end(), sound) != sounds.end())
            return;

        sounds.emplace_back(sound);
        if (sounds.size() > MAX_SOUNDS)
            sounds.erase(sounds.begin());
    }

    // =====================================
    // 🎵 Sound Class
    // =====================================
    class Sound {
    private:
        Path path;
        bool loaded = false;
        int frame = 0;
        int totalFrames = 0;

        DecodedAudio LoadAudio() {
            if (!path.exists)
                throw std::runtime_error("File not found");

            const char* filename = path.value.c_str();
            DecodedAudio audio{};

            if (strstr(filename, ".flac")) {
                drflac* flac = drflac_open_file(filename, NULL);
                if (flac) {
                    audio.frameCount = flac->totalPCMFrameCount;
                    audio.channels = flac->channels;
                    audio.sampleRate = flac->sampleRate;
                    audio.samples = static_cast<float*>(
                        malloc(audio.frameCount * audio.channels * sizeof(float)));
                    drflac_read_pcm_frames_f32(flac, audio.frameCount, audio.samples);
                    drflac_close(flac);
                }
            } else if (strstr(filename, ".mp3")) {
                drmp3 mp3;
                if (drmp3_init_file(&mp3, filename, NULL)) {
                    drmp3_uint64 frameCount = drmp3_get_pcm_frame_count(&mp3);
                    audio.frameCount = frameCount;
                    audio.channels = mp3.channels;
                    audio.sampleRate = mp3.sampleRate;
                    audio.samples = static_cast<float*>(
                        malloc(frameCount * audio.channels * sizeof(float)));
                    drmp3_read_pcm_frames_f32(&mp3, frameCount, audio.samples);
                    drmp3_uninit(&mp3);
                }
            } else if (strstr(filename, ".wav")) {
                drwav wav;
                if (drwav_init_file(&wav, filename, NULL)) {
                    audio.frameCount = wav.totalPCMFrameCount;
                    audio.channels = wav.channels;
                    audio.sampleRate = wav.sampleRate;
                    audio.samples = static_cast<float*>(
                        malloc(audio.frameCount * audio.channels * sizeof(float)));
                    drwav_read_pcm_frames_f32(&wav, audio.frameCount, audio.samples);
                    drwav_uninit(&wav);
                }
            }

            return audio;
        }
        

    public:
        explicit Sound(const std::string& pth) : path(pth) {
            audio = LoadAudio();
        }

        bool playing = false;
        float percent = 0.0f;
        DecodedAudio audio;

        void play() {
            RegisterSound(this);
            playing = true;
            totalFrames = static_cast<int>(audio.frameCount);
        }

        float tick(float* smp) {
            if (!playing) return 0.0f;
            if (frame >= totalFrames / BUFFER) {
                playing = false;
                return 0.0f;
            }
            memcpy(smp, audio.samples + (BUFFER * frame), BUFFER * sizeof(float));
            frame++;
            return 0.0f;
        }
    };
    inline void Tick() {
        if (!device) return;

        float buffer[BUFFER] = {0.0f};
        float temp[BUFFER];

        // 1️⃣ Sum all playing sounds
        for (auto& m : sounds) {
            if (!m->playing) continue;
            m->tick(temp);
            for (int i = 0; i < BUFFER; ++i)
                buffer[i] += temp[i];
        }

        // 2️⃣ Soft-clip (gentle limiter)
        for (int i = 0; i < BUFFER; ++i)
            buffer[i] = buffer[i] / (1.0f + fabsf(buffer[i]));

        // 3️⃣ Send to SDL
        SDL_QueueAudio(device, buffer, BUFFER * sizeof(float));
    }


}
