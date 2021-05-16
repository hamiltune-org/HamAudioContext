#include "generator.h"
#include <stdio.h>

class TestSine : public Generator 
{
	bool init = false;
	int freq = 100;
	float j = 0;
	float* myBuf;
	void userInit() {
		if (!init) {
			printf("%d\n", numSamples());
			init = true;
		}
		int l = numSamples();
		myBuf = new float[l];
		for (int i = 0; i < l; i++) {
			myBuf[i] = (float)sin((float)(i) / l * M_PI * 2);
		}
	}
	int generateSamples() {
		float* buf = buffer();
		float t;
		for (int i = 0; i < numSamples(); i++) {

			t = smoothRead();
			buf[i] = t;
			if (numChannels() == 2) rightBuffer()[i] = t;

		}
		return 0;
	}
	float smoothRead() {
		int readInt = (int)floor(j);
		float offset = j - readInt;

		int readInt2 = readInt + 1;
		if (readInt2 == numSamples()) readInt2 = 0;

		float t = myBuf[readInt] + (myBuf[readInt2] - myBuf[readInt]) * offset;

		j += 10;
		if (j >= numSamples()) j = (int)floor(j) % numSamples() + offset;
		return t;
	}
	void userDelete() {
		delete[] myBuf;
	}
};