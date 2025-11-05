#pragma once
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_events.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#define BUFFER 4096
#define DR_FLAC_IMPLEMENTATION
#define DR_MP3_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION
#include "dr_flac.h"
#include "dr_mp3.h"
#include "dr_wav.h"
#include "filesystem/filesystem_path.hpp"
using namespace CPPN::FileSystem;

namespace CPPN::Audio {

struct DecodedAudio {
    float* samples = nullptr;
    uint64_t frameCount = 0;
    int channels = 0;
    int sampleRate = 0;
};

class Sound;

inline std::vector<Sound*> sounds;
inline int MAX_SOUNDS = 10;
inline SDL_AudioSpec spec{};
inline SDL_AudioDeviceID device;

inline void RegisterSound(Sound* sound) {
    if (std::find(sounds.begin(), sounds.end(), sound) != sounds.end())
        return;
    sounds.emplace_back(sound);
    if (sounds.size() > MAX_SOUNDS)
        sounds.erase(sounds.begin());
}

enum class DecoderType { None, FLAC, MP3, WAV };

struct Decoder {
    DecoderType type = DecoderType::None;
    void* handle = nullptr;
    int channels = 0;
    int sampleRate = 0;

    bool open(const char* path) {
        if (strstr(path, ".flac")) {
            drflac* f = drflac_open_file(path, nullptr);
            if (!f) return false;
            handle = f;
            channels = f->channels;
            sampleRate = f->sampleRate;
            type = DecoderType::FLAC;
        } else if (strstr(path, ".mp3")) {
            drmp3* m = (drmp3*)malloc(sizeof(drmp3));
            if (!drmp3_init_file(m, path, nullptr)) { free(m); return false; }
            handle = m;
            channels = m->channels;
            sampleRate = m->sampleRate;
            type = DecoderType::MP3;
        } else if (strstr(path, ".wav")) {
            drwav* w = (drwav*)malloc(sizeof(drwav));
            if (!drwav_init_file(w, path, nullptr)) { free(w); return false; }
            handle = w;
            channels = w->channels;
            sampleRate = w->sampleRate;
            type = DecoderType::WAV;
        } else {
            return false;
        }
        return true;
    }

    uint64_t read(float* buffer, uint64_t framesToRead) {
        if (!handle) return 0;
        switch (type) {
            case DecoderType::FLAC: return drflac_read_pcm_frames_f32((drflac*)handle, framesToRead, buffer);
            case DecoderType::MP3:  return drmp3_read_pcm_frames_f32((drmp3*)handle, framesToRead, buffer);
            case DecoderType::WAV:  return drwav_read_pcm_frames_f32((drwav*)handle, framesToRead, buffer);
            default: return 0;
        }
    }

    void close() {
        if (!handle) return;
        switch (type) {
            case DecoderType::FLAC: drflac_close((drflac*)handle); break;
            case DecoderType::MP3:  drmp3_uninit((drmp3*)handle); free(handle); break;
            case DecoderType::WAV:  drwav_uninit((drwav*)handle); free(handle); break;
            default: break;
        }
        handle = nullptr;
        type = DecoderType::None;
    }
};

class Sound {
private:
    Path path;
    bool loaded = false;
    int totalFrames = 0;

public:
    Decoder decoder;
    float pcmBuffer[BUFFER * 2];
    explicit Sound(const std::string& pth) : path(pth) {
        decoder.open(path.value.c_str());
    }
    bool playing = false;
    float percent = 0.0f;
    DecodedAudio audio;
    int frame = 0;
    void play() {
        RegisterSound(this);
        playing = true;
    }
    void tick(uint32_t frames) {
        if (!playing) return;
        uint64_t got = decoder.read(pcmBuffer, frames);
        if (got < frames) {
            playing = false;
            decoder.close();
        }
    }
};

void AudioCallback(void* userdata, Uint8* stream, int len) {
    float* out = reinterpret_cast<float*>(stream);
    int totalSamples = len / sizeof(float);
    int channels = 2;
    std::fill(out, out + totalSamples, 0.0f);
    for (auto& s : CPPN::Audio::sounds) {
        if (!s->playing) continue;
        int framesToMix = totalSamples / channels;
        s->tick(framesToMix);
        for (int f = 0; f < framesToMix; ++f) {
            for (int c = 0; c < channels; ++c) {
                int outIndex = f * channels + c;
                if (c < s->decoder.channels)
                    out[outIndex] += s->pcmBuffer[f * s->decoder.channels + c];
            }
        }
    }
    for (int i = 0; i < totalSamples; ++i)
        out[i] = tanhf(out[i]);
}

inline void Init() {
    SDL_zero(spec);
    spec.freq = 48000;
    spec.format = AUDIO_F32;
    spec.channels = 2;
    spec.samples = BUFFER;
    spec.callback = AudioCallback;
    device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (!device)
        printf("SDL_OpenAudioDevice error: %s\n", SDL_GetError());
    SDL_PauseAudioDevice(device, 0);
}

}
