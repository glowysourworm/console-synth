#pragma once

#include "PlaybackParameters.h"
#include "SynthConfiguration.h"

/// <summary>
/// Simple base class to enforce Initialize / Dispose pattern on the controllers. The Synth, SynthPlaybackDevice, and
/// the signal chain all depend on real time updates. These two pointers have to be shared appropriately in order to 
/// avoid context switching between threads.
/// </summary>
class BaseController
{
public:

	/// <summary>
	/// Initialization function for the synth backend. This must be called before starting the player!
	/// </summary>
	virtual bool Initialize(const SynthConfiguration* configuration, const PlaybackParameters* parameters) = 0;

	/// <summary>
	/// Disposes of backend, and controller resources
	/// </summary>
	virtual bool Dispose() = 0;
};

