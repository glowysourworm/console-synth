#ifndef MIXER_H
#define MIXER_H

#include <map>

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
	Mixer();
	~Mixer();

	void SetChannel(int channelKey, float sample, float relativeMix = 1.0);
	void ClearChannel(int channelKey);

	// Returns mix vor current sample frame
	float Get();

private:

	std::map<int, Mixer::MixerChannel>* _channels;
};

#endif

