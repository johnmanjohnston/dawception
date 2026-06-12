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

### Limitations
This project is a **workaround** to track count limits in your DAW. DAWception is not a full replacement for all the features you get by buying a better edition of your DAW.
DAWception does not have:
- MIDI support (yet)
- sidechaining
- pitch shifting
- quantization
- any other fancy features that upgraded editions of your DAW might have

If you can afford to upgrade your DAW, that is most certainly a better option than using DAWception. If you can't, DAWception exists as a workaround.

## Usage
<details>
    <summary>Creating tracks and groups</summary>

| Description | Screenshot |
| --- | --- |
| Use "ADD GROUP" and "ADD TRACK" buttons at top of tracklist | <img width="464" height="46" alt="image" src="https://github.com/user-attachments/assets/3d0456e4-6af6-4922-a3f8-c7b017744e0b" /> |
| Right click in the tracklist area to show context menu | <img width="232" height="90" alt="image" src="https://github.com/user-attachments/assets/dde110f0-4e93-4e1f-b0f6-cbd19a535ad3" /> |
| Right click on a group to add child tracks/groups | <img width="328" height="349" alt="image" src="https://github.com/user-attachments/assets/27d2f28b-4993-4665-a3d5-ad2563c5362e" /> |
</details>

<details>
    <summary>Adding audio clips</summary>
    Audio clips can only be added to tracks. If you try to add an audio clip to a group, it will be rejected. <br>
    Some DAWs (like Ableton Live) might be using the audio file you're trying to add, so it won't let you import it into DAWception. If this happens, locate the audio file in your file explorer, consolidate/flatten the audio clip in your DAW, then import the audio file from the already open file explorer window. <br> <br>
    The easiest way to add audio clips is by dragging the audio file from your file explorer into the timeline. <br>
    You can also add audio clips by right clicking on the timeline, and selecting "Insert audio file". <br>
</details>

<details>
    <summary>Clip manipulation</summary>
    You can drag clips to move them, so they start at a different point in time. <br>
    You can drag clips between audio tracks. <br>
    Clips automatically snap to grid. To stop snapping to grid, hold <kbd>Alt</kbd> while dragging.
    To trim clips, hover your mouse towards the side you want to trim from, and you'll see the trim handles appear. Hold <kbd>Ctrl</kbd> to trim. To stop snapping while trimming, hold <kbd>Ctrl</kbd> + <kbd>Alt</kbd>
</details>

<details>
    <summary>Scanning plugins</summary>
    Click "CONFIG" on the top right. <br>
    Selecting "Scan plugins" scans your VST3 folder and tries to identify all installed plugins. <br>
    Selecting "Lazy scan" performs a much faster scan, but might be less accurate because it skips plugin validation.
</details>

<details>
    <summary>Adding/hosting plugins inside tracks/groups</summary>
    Click the "FX" button next to the track/group you want to add a plugin to. <br>
    Right click anywhere in the empty space in the plugin chain menu, and hover over "Add plugin" and select the plugin you want to add. <br>
    You can reorder plugins by dragging them left or right.
</details>

<details>
    <summary>Automating hosted plugins/groups</summary>
    The way automation works in DAWception, is that DAWception has 128 automatable parameters, each parameter stores a value from 0% to 100%. You can relay the value from that parameter, to any plugin's parameter. A value of 0 corresponds to the target plugin parameter's minimum value, and 100 corresponds to target plugin parameter's maximum value. <br> <br>
    Click the "FX" button on the track/group with the plugin you want to automate. Find the plugin you want to automate in the plugin chain menu. <br>
    Click "AUTOMATE" to open the relay parameters menu. <br>
    Right click anywhere in the blank space to add a relayed parameter. "IN" is the parameter DAWception should read from, "OUT" is the parameter of the hosted plugin, that DAWception should write to.
    Use your DAW's automation feature to automate the appropriate "IN" parameter. <br> <br>
    <details>
        <summary>Your DAW (<i>cough</i> Ableton <i>cough</i>) doesn't show all 128 automatable parameters?</summary>
        Ableton Live refuses to show all of DAWception's 128 automatable parameters by default. To fix this: <br> <br>
        <figure>
            <p>In Ableton, click "Configure"</p>
            <img width="251" height="204" alt="image" src="https://github.com/user-attachments/assets/329084c1-165d-4f40-9943-8357d0a05453" />
        </figure> <br>
        <figure>
            <p>In DAWception, on the top right, click "CONFIG" then "Open relay params inspector"</p>
            <img width="292" height="409" alt="image" src="https://github.com/user-attachments/assets/f22dea52-917f-4e75-8f3a-fc68462d49b3" />
        </figure> <br>
        <figure>
            <p>Move the slider for whatever input parameter you want to automate</p>
            <img width="317" height="66" alt="image" src="https://github.com/user-attachments/assets/49de3706-a8f1-457b-a8a8-1887da4d5c97" />
        </figure> <br>
        <figure>
            <p>It will then show up inside Ableton</p>
            <img width="252" height="199" alt="image" src="https://github.com/user-attachments/assets/3d4a1f48-20ff-4ba6-b5d1-28ce83af3c8f" />
        </figure>
    </details>

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
- Build > Build Solution (or do <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>B</kbd>)

### Running
If you built a **debug** build:
    `dawception_artefacts/Debug/VST3/DAWception.vst3`
    
If you built a **release** build:
    `dawception_artefacts/Release/VST3/DAWception.vst3`

Move the file to your DAW's VST3 plugin folder.
Then, scan and add DAWception to any track in your DAW.

## License
DAWception is licensed under the AGPLv3, see [LICENSE](https://github.com/johnmanjohnston/dawception/blob/main/LICENSE) for details.
