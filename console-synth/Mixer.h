#ifndef MIXER_H
#define MIXER_H

#include <vector>

class Mixer
{
public:

	struct MixerChannel
	{
		float sample;
		float mixLevel;

		MixerChannel()
		{
			this->sample = 0;
			this->mixLevel = 0;
		}

		MixerChannel(float sample, float mixLevel)
		{
			this->sample = sample;
			this->mixLevel = mixLevel;
		}

		void Set(float sample, float mixLevel)
		{
			this->sample = sample;
			this->mixLevel = mixLevel;
		}
	};

	// Creates a mixer with the specified number of channels
	Mixer(int channels);
	~Mixer();

	void SetChannel(int index, float sample, float relativeMix = 1.0);

	// Returns mix vor current sample frame
	float Get();

private:

	std::vector<Mixer::MixerChannel>* _channels;
};

#endif

