# How to clone
this repo uses JUCE as a submodule, so clone with:
```bash
git clone --recursive https://github.com/w-ensink/juce_audio_backend.git 
```

# How to compile:
```bash
 cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"
 cmake --build build --target juce_audio_backend
```

There will now be a folder called `build` with a file called `libjuce_audio_backend.a` in it.

To use this in a project, copy the path of this file and add `-L<path>` to your build config.

Then copy `juce_audio.h` to your project and `#include` it in your source files.