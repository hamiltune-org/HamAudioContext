#include "hamaudioctx.h"
#include "testsine.cpp"

void doStuff(HamAudioCtx* ctx);

int main() {
    
    // sample size must be power of 2
    Sleep(3000);
	HamAudioCtx* ctx = new HamAudioCtx(1, 44100, 1024);

    TestSine* sine = new TestSine();
    ctx->AddGenerator(sine);
    printf("Sin added\n");

    // Do some stuff and shutdown
    std::thread t(doStuff, ctx);
    t.join();

    while (true);
    return 0;
}

void doStuff(HamAudioCtx* ctx) {
    Sleep(5000);

    ctx->Terminate();
    delete ctx;

    Sleep(5000);
}