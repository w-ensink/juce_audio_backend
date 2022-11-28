
#include "juce_audio.h"
#include <juce_audio_devices/juce_audio_devices.h>

struct AudioBackend::Pimpl : juce::AudioIODeviceCallback
{
    explicit Pimpl (AudioCallback& callback) : callback (callback) {}

    void openDefaultIODevice (int numInputChannels, int numOutputChannels)
    {
        audioDeviceManager.addAudioCallback (this);
        audioDeviceManager.initialiseWithDefaultDevices (numInputChannels, numOutputChannels);
    }

    void closeDevice()
    {
        audioDeviceManager.closeAudioDevice();
    }

private:
    AudioCallback& callback;
    juce::AudioDeviceManager audioDeviceManager;
    double sampleRate = 0.0;
    int blockSize = 0;

    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) override
    {
        callback.process (inputChannelData, outputChannelData, numInputChannels, numOutputChannels, numSamples);
    }

    void audioDeviceAboutToStart (juce::AudioIODevice* device) override
    {
        sampleRate = device->getCurrentSampleRate();
        blockSize = device->getCurrentBufferSizeSamples();

        callback.prepareToPlay (sampleRate, blockSize);
    }

    void audioDeviceStopped() override
    {
        callback.releaseResources();
    }

    void audioDeviceError (const juce::String& errorMessage) override
    {
        std::cerr << "audio device error: " << errorMessage << "\n";
    }
};


void AudioBackend::PimplDeleter::operator() (Pimpl* toDelete)
{
    delete toDelete;
}


AudioBackend::AudioBackend (AudioCallback& callback) : pimpl (std::unique_ptr<Pimpl, PimplDeleter> { new Pimpl (callback) }) {}

void AudioBackend::openDefaultIODevice (int numInputChannels, int numOutputChannels)
{
    pimpl->openDefaultIODevice (numInputChannels, numOutputChannels);
}

void AudioBackend::closeDevice()
{
    pimpl->closeDevice();
}
