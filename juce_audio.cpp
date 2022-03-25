
#include "juce_audio.h"
#include <juce_audio_devices/juce_audio_devices.h>

struct AudioBackend::Pimpl : juce::AudioIODeviceCallback
{
    void openDefaultIODevice (int numInputChannels, int numOutputChannels)
    {
        audioDeviceManager.addAudioCallback (this);
        audioDeviceManager.initialiseWithDefaultDevices (numInputChannels, numOutputChannels);
    }

    void setCallback (AudioCallback* audioCallback)
    {
        callback = audioCallback;

        if (callback != nullptr)
            callback->prepareToPlay (sampleRate, blockSize);
    }

    void closeDevice()
    {
        audioDeviceManager.closeAudioDevice();
    }

private:
    AudioCallback* callback = nullptr;
    juce::AudioDeviceManager audioDeviceManager;
    double sampleRate = 0.0;
    int blockSize = 0;

    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) override
    {
        if (callback != nullptr)
            callback->process (inputChannelData, outputChannelData, numInputChannels, numOutputChannels, numSamples);
    }

    void audioDeviceAboutToStart (juce::AudioIODevice* device) override
    {
        sampleRate = device->getCurrentSampleRate();
        blockSize = device->getCurrentBufferSizeSamples();

        if (callback != nullptr)
            callback->prepareToPlay (sampleRate, blockSize);
    }

    void audioDeviceStopped() override
    {
        if (callback != nullptr)
            callback->releaseResources();
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


AudioBackend::AudioBackend() : pimpl (std::unique_ptr<Pimpl, PimplDeleter> { new Pimpl() }) {}

void AudioBackend::openDefaultIODevice (int numInputChannels, int numOutputChannels)
{
    pimpl->openDefaultIODevice (numInputChannels, numOutputChannels);
}

void AudioBackend::registerCallback (AudioCallback* callback)
{
    pimpl->setCallback (callback);
}

void AudioBackend::closeDevice()
{
    pimpl->closeDevice();
}
