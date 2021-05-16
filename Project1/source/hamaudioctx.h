#ifndef hamaudioctx_h
#define hamaudioctx_h

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <vector>

#include <windows.h>
#include <combaseapi.h>
#include <xaudio2.h>
#include <thread>

#include "generator.h"
#include "voicecallback.h"
#include <wrl\client.h>
// class
using Microsoft::WRL::ComPtr;

class HamAudioCtx
{
public:
    HamAudioCtx(int num_channels, int sample_rate, int buffer_size);
    ~HamAudioCtx();

    void feedData();
    void Initialize();
    int Generate();
    void Iterate();
    void Terminate();
    bool AddGenerator(Generator* generator);

private:

    ComPtr<IXAudio2> XAudio2;
    IXAudio2MasteringVoice* MasterVoice;
    IXAudio2SourceVoice* SourceVoice;
    WAVEFORMATEXTENSIBLE wfx;
    BYTE* audioData = nullptr;
    XAUDIO2_BUFFER buffer;
    VoiceCallback voiceCallback;
    HRESULT hr;

    int numChannels = 1;
    int sampleRate = 44100;
    int bufferSize = 512;
    bool active = false;

    float* floatBuffer;
    std::vector<Generator*> generators;
};

void InitThread(HamAudioCtx* ctx);

#endif // !hamaudioctx_h