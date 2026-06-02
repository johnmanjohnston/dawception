# DAWception
A mini DAW in a plugin. You can add tracks, drag audio clips, and host other plugins.

An audio plugin developed as a workaround to track count limits in "lower tier versions" of DAWs (like Ableton Live Lite's 8 track limit).

<figure>
    <img width="1920" height="1024" alt="image" src="https://github.com/user-attachments/assets/5f31f22c-6bf8-4a7d-a0dc-e1ca157066d7" />
    <p align="center">DAWception running within Ableton Live Lite</p>
</figure>

## Features
- **unlimited stereo audio tracks**, which can be organized using groups
- **drag and drop audio clips** (with basic clip manipulation features like splitting and trimming)
- **hosting plugins** for individual tracks or groups, with **latency compensation**
- **automation passthrough** from host DAW to plugins hosted inside DAWception
- **dry/wet control added to every hosted plugin** (even if the hosted plugin doesn't provide it by default)

## Usage
After installing the plugin, scan and add the plugin to any track in your DAW.
Open the plugin editor.

<details>
    <summary>Creating tracks and groups</summary>
    Click "ADD TRACK" to create a track. Click "ADD GROUP" to create a group. <br>
    You can also right click on blank space in the tracklist area to create a track or group. <br>
    You can also right click on a group to add a child track or group. <br>
</details>

<details>
    <summary>Adding audio clips</summary>
    Audio clips can only be added to tracks. If you try to add an audio clip to a group, it will be rejected. <br>
    The easiest way to add audio clips is by dragging the audio clip from your file explorer into the timeline. <br>
    You can also add audio clips by right clicking on the timeline, and selecting "Insert audio file". <br>
</details>

<details>
    <summary>Scanning plugins</summary>
    Click "CONFIG" on the top right. <br>
    Selecting "Scan plugins" scans your VST3 folder and tries to identify all installed plugins.
    Selecting "Lazy scan" performs a much faster scan, but might be less accurate.
</details>

## Installation
All releases are provided as VST3 plugins.
### Latest Release (v0.0.1)
Linux: TODO

Windows: TODO

macOS: TODO

### Older Versions
For older builds, from the Releases section, download the appropriate build for your platform.

## Building
### Downloading Source and Dependencies
```bash
# clone this repo and cd into it
git clone https://github.com/johnmanjohnston/dawception/
cd dawception

# clone JUCE
git clone https://github.com/juce-framework/JUCE/
```

```bash
# generate build files with CMake
cmake .
```

You should then see the required files to build for your platform (Makefile for Linux, Visual Studio solution for Windows).

<details>
    <summary>
        <b>Linux-specific patch for plugin editors to function properly inside DAWception</b>
    </summary>

<br>

On Linux, plugin editors hosted inside of DAWception cannot be dragged around; this JUCE patch fixes that.

Inside `JUCE/modules/juce_audio_processors/format_types/juce_VST3PluginFormat.cpp`, add the following code anywhere inside the `VST3PluginWindow` struct:

```cpp
#if JUCE_LINUX
    void handleCommandMessage(int commandId) override {
        if (commandId == 420) {
            embeddedComponent.updateEmbeddedBounds();
        }
    }
#endif

```
</details>

### Compiling
#### Linux
```bash
# run make
make
```

#### Windows (Visual Studio)
- open `dawception.sln`
- Build > Build Solution (or do `Ctrl+Shift+B`)

### Running
If you built a **debug** build:
    `dawception_artefacts/Debug/VST3/DAWception.vst3`
    
If you built a **release** build:
    `dawception_artefacts/Release/VST3/DAWception.vst3`

Move the file to your DAW's VST3 plugin folder.
Then, scan and add DAWception to any track in your DAW.

## License
DAWception is licensed under the AGPLv3, see [LICENSE](https://github.com/johnmanjohnston/dawception/blob/main/LICENSE) for details.
