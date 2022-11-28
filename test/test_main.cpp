
#include <iostream>
#include <juce_audio.h>
#include <random>
#include <thread>

struct TestCallback : AudioCallback
{
    void process (const float** input, float** output, int numInputChannels, int numOutputChannels, int numSamples) override
    {
        for (int channel = 0; channel < numOutputChannels; ++channel)
        {
            for (int sample = 0; sample < numSamples; ++sample)
            {
                output[channel][sample] = static_cast<float> (distribution (generator)) * 0.2f;
            }
        }
    }

private:
    std::random_device randomDevice;
    std::mt19937 generator { randomDevice() };
    std::uniform_real_distribution<> distribution { 0.0, 1.0 };
};


int main()
{
    TestCallback callback;
    AudioBackend audioBackend{ callback };

    audioBackend.openDefaultIODevice (0, 2);

    std::this_thread::sleep_for (std::chrono::seconds (2));

    audioBackend.closeDevice();

    return 0;
}
