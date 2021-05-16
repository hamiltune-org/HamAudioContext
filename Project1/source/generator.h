#ifndef generator_h
#define generator_h

#define _USE_MATH_DEFINES
#include <math.h>

class Generator 
{
public:
	// Constructor & Destructor (sort of)
	Generator() { _buffer = nullptr; num_samples = 0; num_channels = 0; }
	void Initialize(int _num_samples, int _num_channels) {

		num_samples = _num_samples;
		num_channels = _num_channels;

		_buffer = new float[num_samples * num_channels];

		// If two channels, first half is left, second half is right
		if (num_channels == 2) {
			is_stereo = true;
			left_buffer = _buffer;
			right_buffer = _buffer + num_samples * sizeof(float);
		}
	}
	~Generator() {
		delete [] _buffer;
		if (is_stereo) {
			left_buffer = nullptr;
			right_buffer = nullptr;
		}
	}
	// Getters
	int numSamples() { return num_samples; }
	int numChannels() { return num_channels; }
	float* buffer() { return _buffer; }
	float* leftBuffer() { return left_buffer; }
	float* rightBuffer() { return right_buffer; }
	bool isStereo() { return is_stereo; }

	// TODO: User defined
	virtual void userInit() = 0;
	virtual void userDelete() = 0;
	virtual int generateSamples() = 0;

protected:
	// Int Replace with GUI ptr
	int GUI = -1;
	int num_samples;
	int num_channels;
	float* _buffer;
	// Used only for stereo
	bool is_stereo = false;
	float* left_buffer = nullptr;
	float* right_buffer = nullptr;
};

#endif // !generator_h