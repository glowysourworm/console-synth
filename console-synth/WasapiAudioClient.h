#pragma once


#include <AudioSessionTypes.h>
#include <Audioclient.h>
#include <Windows.h>
#include <cassert>
#include <cstdio>
#include <exception>
#include <format>
#include <ksmedia.h>
#include <mmdeviceapi.h>
#include <mmreg.h>
#include <string>

class WasapiAudioClient
{
public:

	WasapiAudioClient();
	~WasapiAudioClient();

	/// <summary>
	/// Enumerates audio devices, and establishes instance of audio renderer.
	/// </summary>
	/// <exception>Initialization failed (TBD for details)</exception>
	void Initialize();

	/// <summary>
	/// Activates the audio client, gets format details, establishes rendering
	/// service, and initializes the stream.
	/// </summary>
	/// <exception>Open failed (TBD for details)</exception>
	void Open();

	/// <summary>
	/// Closes audio client and renderer. Releases instances.
	/// </summary>
	/// <exception>Close failed (TBD for details)</exception>
	void Close();

	/// <summary>
	/// Requests buffer from the audio client. Assumes that there will be success unless it throws an
	/// exception. For this reason, you should surround this call with a try / catch. The buffer will
	/// be written with the TSignal* buffer, which should follow the format for this backend. 
	/// <param name="buffer">Pointer to buffer data</param>
	/// <param name="frameSize">Number of frames requested</param>
	/// </summary>
	/// <exception>Rendering failed (TBD for details)</exception>
	void RenderBuffer(const BYTE* buffer, int frameSize);

	/// <summary>
	/// Returns sampling rate for the device. This is needed to be honored by the 
	/// buffer. There may be ways of requesting other sampling rates from MSFT's backend.
	/// </summary>
	int GetSamplingRate() const;

	/// <summary>
	/// Returns the number of channels for the output device
	/// </summary>
	short GetNumberChannels() const;

	/// <summary>
	/// This shows the format of the sample for the device
	/// </summary>
	short GetBitsPerSample() const;

	/// <summary>
	/// This is the size of a frame, also called "block align".
	/// </summary>
	short GetFrameSizeBytes() const;

    int GetRenderBufferFrameSize() const;
    int GetDevicePeriod() const;

    std::string GetApiInfo() const;
    std::string GetFormatInfo() const;

    double GetTime() const;
    double GetLatency() const;

    /// <summary>
    /// Returns number of current padding frames 
    /// </summary>
    int GetCurrentPadding() const;

private:

    void HandleError(bool success, const std::string& message) const;

private:

	bool _initialized;
	bool _open;

	// MSFT WASAPI Components
	//
	IMMDevice* _renderer;
	WAVEFORMATEX* _format;
    IAudioClock* _audioClock;
	IAudioClient* _renderClient;
	IAudioRenderClient* _renderService;
};

WasapiAudioClient::WasapiAudioClient()
{
    _renderer = NULL;
    _format = NULL;
    _audioClock = NULL;
    _renderClient = NULL;
    _renderService = NULL;

    _initialized = false;
    _open = false;
}

WasapiAudioClient::~WasapiAudioClient()
{
    // It seems like MSFT is taking care of its own memory. So we'll leave this blank
    // until we have more information

    if (_open)
        this->Close();
}

void WasapiAudioClient::HandleError(bool success, const std::string& message) const
{
    if (success)
        return;

    throw new std::exception(message.c_str());
}

void WasapiAudioClient::Initialize()
{
    if (_initialized)
        throw new std::exception("WASAPI client already initialized");

    if (_open)
        throw new std::exception("WASAPI client already initialized and open");

    // WASAPI:  A full duplex example was tailored from this source (see link). The result
    //          should open and render sound in shared client mode.
    //
    // https://medium.com/@shahidahmadkhan86/sound-in-windows-the-wasapi-in-c-23024cdac7c6
    //

    HRESULT hr;
    IMMDeviceEnumerator* enumerator = NULL;

    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    HandleError(SUCCEEDED(hr), "WASAPI:  CoInitializeEx Failed");

    // LINK ERROR!  https://stackoverflow.com/questions/12844431/linking-wasapi-in-vs-2010
    // 
    //              Added __uuidof wrapper to the two enumerators. 
    //              Also changed target CLSCTX_ALL to "server". 
    //              Not sure.
    // 
    // CoCreateInstance -> CoCreateInstanceEx
    // 
    // https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstanceex
    //
    // Still problems, and couldn't find an example of a WASAPI usage of this function
    // 
    // CoCreateInstanceEx(
    //    CLSID_MMDeviceEnumerator,
    //    NULL,
    //    CLSCTX_ALL,
    //    NULL,                           // Local Computer (NULL)
    //    //IID_IMMDeviceEnumerator,      // CoCreateInstance() (interface object to create the enumerator)
    //    1,                              // Instance count to create
    //    (void**)&enumerator             // [out] target (this didn't compile, anyway)
    // );
    //
    // Older Method: https://hero.handmade.network/forums/code-discussion/t/8433-correct_implementation_of_wasapi
    //
    // Looks like IID_PPV_ARGS isn't up to date. So, I found the top link error, which may work.
    //
    // (code) CoCreateInstance(__uuidof(MMDeviceEnumerator), 
    //                         NULL, 
    //                         CLSCTX_INPROC_SERVER, 
    //                         IID_PPV_ARGS(&mmDeviceEnumerator));
    //

    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        NULL,
        CLSCTX_INPROC_SERVER,
        __uuidof(IMMDeviceEnumerator),
        (void**)&enumerator
    );
    HandleError(SUCCEEDED(hr), "WASAPI:  CoCreateInstance Failed");

    hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_renderer);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetDefaultAudioEndpoint Failed");

    hr = enumerator->Release();
    HandleError(SUCCEEDED(hr), "WASAPI:  Release Failed");

    _initialized = true;
}

void WasapiAudioClient::Open()
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (_open)
        throw new std::exception("WASAPI client already initialized and open");

    HRESULT hr;

    hr = _renderer->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, NULL, (void**)&_renderClient);
    HandleError(SUCCEEDED(hr), "WASAPI:  Activate Failed");

    hr = _renderClient->GetMixFormat(&_format);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetMixFormat Failed");

    REFERENCE_TIME defaultPeriod, minimumPeriod;

    hr = _renderClient->GetDevicePeriod(&defaultPeriod, &minimumPeriod);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetDevicePeriod Failed");

    hr = _renderClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, defaultPeriod, 0, _format, NULL);
    HandleError(SUCCEEDED(hr), "WASAPI:  Initialize Failed");

    hr = _renderClient->GetService(__uuidof(IAudioRenderClient), (void**)&_renderService);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetService Failed");

    hr = _renderClient->Start();
    HandleError(SUCCEEDED(hr), "WASAPI:  Start Failed");

    // Additional IAudioClock Service Request
    hr = _renderClient->GetService(__uuidof(IAudioClock), (void**)&_audioClock);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetService Failed");

    _open = true;
}

void WasapiAudioClient::Close()
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not open");

    _open = false;

    _renderClient->Stop();
    _renderService->Release();
    _renderClient->Release();
    _renderer->Release();
    _audioClock->Release();

    CoUninitialize();

    _renderClient = NULL;
    _renderService = NULL;
    _renderer = NULL;
    _audioClock = NULL;

    delete _format;

    _format = NULL;
}

void WasapiAudioClient::RenderBuffer(const BYTE* buffer, int frameSize)
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    HRESULT hr;
    BYTE* renderBuffer;
    
    // See above link to MSFT example
    // 
    hr = _renderService->GetBuffer(frameSize, &renderBuffer);
    //HandleError(SUCCEEDED(hr), "WASAPI:  GetBuffer Failed");

    if (!SUCCEEDED(hr))
    {
        switch (hr)
        {
        case AUDCLNT_E_BUFFER_TOO_LARGE:
            HandleError(false, "WASAPI GetBuffer:  Buffer too large"); break;
        case AUDCLNT_E_BUFFER_SIZE_ERROR:
            HandleError(false, "WASAPI GetBuffer:  Buffer size error"); break;
        case AUDCLNT_E_OUT_OF_ORDER:
            HandleError(false, "WASAPI GetBuffer:  Out of order"); break;
        case AUDCLNT_E_DEVICE_INVALIDATED:
            HandleError(false, "WASAPI GetBuffer:  Device invalidated"); break;
        case AUDCLNT_E_BUFFER_OPERATION_PENDING:
            HandleError(false, "WASAPI GetBuffer:  Operation pending"); break;
        case AUDCLNT_E_SERVICE_NOT_RUNNING:
            HandleError(false, "WASAPI GetBuffer:  Service not running"); break;
        case E_POINTER:
            HandleError(false, "WASAPI GetBuffer:  E_Pointer Parameter ppData is NULL"); break;
        default:
            HandleError(false, "Unknown Error:  WASAPI GetBuffer");
            break;
        }
    }

    memcpy(renderBuffer, buffer, _format->nBlockAlign * frameSize);

    hr = _renderService->ReleaseBuffer(frameSize, 0);
    HandleError(SUCCEEDED(hr), "WASAPI:  ReleaseBuffer Failed");
}

int WasapiAudioClient::GetSamplingRate() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    return _format->nSamplesPerSec;
}

short WasapiAudioClient::GetNumberChannels() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    return _format->nChannels;
}

short WasapiAudioClient::GetBitsPerSample() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    return _format->wBitsPerSample;
}

short WasapiAudioClient::GetFrameSizeBytes() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    return _format->nBlockAlign;
}

std::string WasapiAudioClient::GetApiInfo() const
{
    return "WASAPI";
}

std::string WasapiAudioClient::GetFormatInfo() const
{
    std::string format = "Unknown";
    std::string bitsPerSample = std::to_string(_format->wBitsPerSample);
    std::string samplingRate = std::to_string(_format->nSamplesPerSec);
    std::string frameSize = std::to_string(_format->nBlockAlign * 8);
    std::string numberChannels = std::to_string(_format->nChannels);

    REFERENCE_TIME defaultPeriod = 0, minimumPeriod = 0, latency = 0;

    _renderClient->GetDevicePeriod(&defaultPeriod, &minimumPeriod);
    _renderClient->GetStreamLatency(&latency);

    switch (_format->wFormatTag)
    {
    case WAVE_FORMAT_PCM: format = "PCM"; break;
    case WAVE_FORMAT_EXTENSIBLE: format = "Extensible"; break;
    case WAVE_INVALIDFORMAT: format = "Invalid"; break;
    default:
        break;
    }

    return std::format("Format: {} Frame: {} * {}/{} Sample Rate: {} Period (default / min): {}/{} Latency: {}", 
            format, bitsPerSample, numberChannels, frameSize, samplingRate, 
            std::to_string(defaultPeriod), 
            std::to_string(minimumPeriod), 
            std::to_string(latency));
}

int WasapiAudioClient::GetRenderBufferFrameSize() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    HRESULT hr;
    UINT32 numberFrames;

    hr = _renderClient->GetBufferSize(&numberFrames);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetBufferSize Failed");

    return numberFrames;
}

double WasapiAudioClient::GetTime() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    HRESULT hr;

    // The second variable marks the time when the device received the get position call
    UINT64 position, frequency, performancePosition;

    // Stream Time:  https://learn.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclock-getposition
    // 
    // ...Thus, the stream-relative offset in seconds can always be calculated as p/f. 
    //
    hr = _audioClock->GetPosition(&position, &performancePosition);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetPosition Failed");

    hr = _audioClock->GetFrequency(&frequency);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetFrequency Failed");

    return position / (double)frequency;
}

double WasapiAudioClient::GetLatency() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    HRESULT hr;
    REFERENCE_TIME latency = 0;
    
    hr = _renderClient->GetStreamLatency(&latency);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetStreamLatency Failed");

    return latency;
}

int WasapiAudioClient::GetCurrentPadding() const
{
    if (!_initialized)
        throw new std::exception("WASAPI client not yet initialized");

    if (!_open)
        throw new std::exception("WASAPI client not yet open");

    HRESULT hr;
    UINT32 padding = 0;

    hr = _renderClient->GetCurrentPadding(&padding);
    HandleError(SUCCEEDED(hr), "WASAPI:  GetCurrentPadding Failed");

    return padding;
}