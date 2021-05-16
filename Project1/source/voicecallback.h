#ifndef voicecallback_h
#define voicecallback_h

#include "hamaudioctx.h"

// Callback for XAudio

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
    HANDLE hBufferEndEvent;
    VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
    ~VoiceCallback() { CloseHandle(hBufferEndEvent); }

    //Called when the voice has just finished playing a contiguous audio stream.
    //void OnStreamEnd() { SetEvent(hBufferEndEvent); }
    /*void STDMETHODCALLTYPE OnStreamEnd() {
        SetEvent(hBufferEndEvent);
        printf("YOLO\n");
    }*/
    void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) {
        //printf("YOLO\n");
        SetEvent(hBufferEndEvent);
    }

    //Unused methods are stubs
    void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() { }
    void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
    //void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) { }
    void STDMETHODCALLTYPE OnStreamEnd() { }
    void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {    }
    void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {    }
    void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) { }
};

#endif // !voicecallback_h
