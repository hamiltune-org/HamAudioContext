#include "hamaudioctx.h"

void HamAudioCtx::feedData() {
    memcpy(audioData, &floatBuffer[0], buffer.AudioBytes);
    buffer.pAudioData = audioData;

    if (FAILED(hr = SourceVoice->SubmitSourceBuffer(&buffer)))
        printf("Failed to laod data: %d\n", hr);
}
int HamAudioCtx::Generate() {
    if (generators.size() == 0) return 1;

    // Clear memory
    ZeroMemory(floatBuffer, bufferSize * sizeof(float));

    // Colelct generated samples
    Generator* gen;
    for (int i = 0; i < generators.size(); i++) {
        gen = generators.at(i);
        int res = gen->generateSamples();
        float* buf = gen->buffer();
        for (int i = 0; i < bufferSize; i++) floatBuffer[i] += buf[i];
        if (res < 0) return -1;
    }
    return 0;
}
HamAudioCtx::HamAudioCtx(int num_channels, int sample_rate, int buffer_size) {

    this->numChannels = num_channels;
    this->sampleRate = sample_rate;
    this->bufferSize = buffer_size;

    // Init
    floatBuffer = new float[numChannels * bufferSize];

    ZeroMemory(&wfx, sizeof(WAVEFORMATEXTENSIBLE));
    ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));

    // Detach for async audio
    std::thread (InitThread, this).detach();
}
void HamAudioCtx::Initialize() {

    HRESULT hr;
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) printf("Failed to initialize : %d\n", hr);

    if (FAILED(hr = XAudio2Create(XAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR)))
        printf("Failed to create xaudio2 : %d\n", hr);

    if (FAILED(hr = XAudio2->CreateMasteringVoice(&MasterVoice)))
        printf("Failed to create a master voice: %d\n", hr);

    ZeroMemory(&wfx, sizeof(WAVEFORMATEXTENSIBLE));
    wfx.Format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT; // Was WAVE_FORMAT_PCM
    wfx.Format.nChannels = this->numChannels;
    wfx.Format.wBitsPerSample = 32;
    wfx.Format.nSamplesPerSec = this->sampleRate;
    wfx.Format.nBlockAlign = wfx.Format.nChannels * wfx.Format.wBitsPerSample / 8;
    wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;

    buffer.AudioBytes = bufferSize * numChannels * sizeof(float);
    audioData = new byte[buffer.AudioBytes];
    
    //if (FAILED(hr = XAudio2->CreateSourceVoice(&SourceVoice, (WAVEFORMATEX*)&wfx)))
        //printf("Failed to create a source voice : %d\n", hr);
    if (FAILED(hr = XAudio2->CreateSourceVoice(&SourceVoice, (WAVEFORMATEX*)&wfx,
        0, XAUDIO2_DEFAULT_FREQ_RATIO, &voiceCallback, NULL, NULL))) 
        printf("Failed to create a source voice : %d\n", hr);

    if (FAILED(hr = SourceVoice->Start(0)))
        printf("Failed to start the source voice : %d\n", hr);

    active = true;
    this->Iterate();
}
void HamAudioCtx::Iterate() {
    this->Generate();
    this->feedData();
    while (active) {
        //printf("%d\n", this->Generate());
        this->Generate();
        this->feedData();
        WaitForSingleObjectEx(voiceCallback.hBufferEndEvent, INFINITE, TRUE);
    }
}

bool HamAudioCtx::AddGenerator(Generator* generator) {
    printf("%d\n", generators.size());
    generator->Initialize(bufferSize, numChannels);
    generator->userInit();
    generators.push_back(generator);
    printf("%d\n", generators.size());
    // Not sure but just in case
    return true;
}
void HamAudioCtx::Terminate() {
    this->active = false;
}
HamAudioCtx::~HamAudioCtx() {

    Generator* g;
    for (int i = 0; i < generators.size(); i++) {
        g = generators.at(i);
        g->userDelete();
        delete this->generators.at(i);
    }

    generators.clear();
    generators.shrink_to_fit();
    delete [] this->floatBuffer;
    delete [] this->audioData;
    MasterVoice->DestroyVoice();
    SourceVoice->Stop();
    SourceVoice->DestroyVoice();
    XAudio2.Reset();
    CoUninitialize();
}

void InitThread(HamAudioCtx* ctx) {
    ctx->Initialize();
}