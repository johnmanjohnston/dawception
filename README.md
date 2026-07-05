# DAWception
DAWception is a mini DAW in a plugin. You can add tracks, drag audio clips, and host other plugins.

DAWception was developed as a workaround to track count limits in "lower tier versions" of DAWs (like Ableton Live Lite's 8 track limit).

> [!WARNING]
> DAWception is not released yet, but hopefully will be, very soon!

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

# Usage
Here's a short, to the point usage guide explaining all main features (there aren't many, this isn't a full fledged DAW), nicely sorted into expandable sections:

## Tracks and groups

<details>
    <summary>Creating tracks and groups</summary>

| Description | Screenshot |
| --- | --- |
| Use "ADD GROUP" and "ADD TRACK" buttons at top of tracklist | <img width="552" height="35" alt="image" src="https://github.com/user-attachments/assets/717ab2a3-cc66-46a5-9369-35e360a3ca23" /> |
| Right click in the tracklist area to show context menu | <img width="232" height="90" alt="image" src="https://github.com/user-attachments/assets/dde110f0-4e93-4e1f-b0f6-cbd19a535ad3" /> |
| Right click on a group for context menu to add tracks/groups | <img height="300" alt="image" src="https://github.com/user-attachments/assets/27d2f28b-4993-4665-a3d5-ad2563c5362e" /> |
</details>

<details>
    <summary>Moving tracks/groups into groups</summary>
    
| Description | Demo |
| --- | --- |
| Drag a track/group and drop inside group  | <img height="400" alt="nesting-nodes-by-dragging" src="https://github.com/user-attachments/assets/28fcc60f-dbe4-42dd-b9e2-ffef3f3ab97e" /> |
| To copy/paste tracks/groups, right click on it to copy, select "Copy", then right click on the group to paste inside, and paste | <img height="400" alt="copy-paste-nodes" src="https://github.com/user-attachments/assets/d05bdc7a-a4de-43a7-9381-5df5eed6bb80" /> |
</details>

## Audio clips
<details>
    <summary>Adding audio clips</summary>
    
| Description | Screenshot/demo |
| --- | --- |
| Drag audio file from your file explorer to timeline | <img width="286" height="76" alt="drag-file" src="https://github.com/user-attachments/assets/e8abbb83-5f15-45b4-8564-591ec4ab8ab8" /> |
| Right click to show context menu, select "Insert audio file" and select file from your file explorer | <img width="197" height="176" alt="image" src="https://github.com/user-attachments/assets/d6c4ad8d-a3e2-4254-8bde-82f675b2e114" /> |
<p>
    Some DAWs (like Ableton Live) might be using the audio file you're trying to add, so it won't let you import it into DAWception. If this happens, <b>locate the audio file in your file explorer, consolidate/flatten the audio clip in your DAW, then import the audio file from the already open file explorer window</b>.</p>
</details>

<details>
    <summary>Clip manipulation</summary>
    
| Description | Screenshot/demo |
| --- | --- |
| **Move clips by dragging them**, trim clips by hovering on the side so that the "trim handles" are visible, then **hold <kbd>Ctrl</kbd> while dragging the trim handles** | <img width="304" height="98" alt="moving-and-trimming" src="https://github.com/user-attachments/assets/a6626d25-7dbf-4193-b1f5-efd091e8f79f" /> |
| Change clips' gain by opening its clip properties window (by double clicking said clip, or by context menu upon right clicking over clip) | <img width="490" height="270" alt="clip-properties-window" src="https://github.com/user-attachments/assets/b6689d81-6037-4d43-827c-b8f15b37823c" /> |
</details>
    <b>By default, moving/trimming clips snaps them to grid. To stop snapping to grid, hold <kbd>Alt</kbd> while dragging/trimming clips.</b>

## Hosting plugins
<details>
    <summary>Scanning plugins</summary>

| Description | Screenshot |
| --- | --- |
| **Click "CONFIG" on the top right**, selecting "Scan plugins" prompts you for your VST3 directory and scans plugins, selecting "Lazy scan for plugins" does a similar scan, and is *much* faster but is less through | <img width="265" height="361" alt="image" src="https://github.com/user-attachments/assets/2b6f3757-9073-44d0-84e7-1163c0ad6454" /> |
</details>

<details>
    <summary>Adding and reordering plugins</summary>
    
| Description | Demo |
| --- | --- |
| To add plugins, **click "FX"** on the track/group you want to add plugins too, **right click in the empty space to open a context menu** and select the plugin you want to add | <img width="500"  alt="adding-plugins" src="https://github.com/user-attachments/assets/f394d852-7580-48ed-948d-a2ef56ec3c12" /> |
| To reorder plugins, drag the plugin to where you want the plugin to be positioned; the plugin will finally be positioned on the **right** of the position indicator line thing | <img width="556" height="164" alt="reordering-plugins" src="https://github.com/user-attachments/assets/09c421d3-b250-4636-8217-0de31dc3f46a" /> |
<p>The slider is the <b>dry/wet mix</b> of the plugin. The <b><i>X</i></b> on the top right of the plugin removes the plugin. </p>
</details>

## Automating plugins
<details>
    <summary>Automating hosted plugins/groups</summary>
    The way automation works in DAWception, is that <b>DAWception has 128 automatable parameters, each parameter stores a value from 0% to 100%. You relay the value from that parameter, to any plugin's parameter</b>. A value of 0 corresponds to the target plugin parameter's minimum value, and 100 corresponds to target plugin parameter's maximum value. <br> <br> You can also map the same input parameter across multiple output parameters, for different plugins.<br> <br>

| Description | Demo |
| --- | --- |
| To automate a plugin, find the plugin and **click "AUTOMATE"**, **right click for context menu to create a relayed parameter**. <br> <br> "IN" is the parameter DAWception should read from, "OUT" is the parameter of the hosted plugin that DAWception should write to. Use your DAW's automation feature to automate DAWception's appropriate "IN" parameter (your DAW should be playing&mdash;not paused&mdash;to reflect the changes in the subhosted plugin's editor) | <img width="500" alt="result" src="https://github.com/user-attachments/assets/8b1630a0-342d-43b8-a268-1b2ed74d251a" /> |
</details>

<details>
    <summary>Your DAW (<i>cough</i> Ableton <i>cough</i>) doesn't show all 128 automatable parameters?</summary>
    Your DAW might refuse to show so many automatable parameters by default, the fix is dependent on the DAW you use. If you're using Ableton, to fix this: <br> <br>
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

# Installation
All releases are provided as VST3 plugins.
### Latest Release (v0.0.1)
Linux: TODO

Windows: TODO

macOS: TODO

### Older Versions
For older builds, from the Releases section, download the appropriate build for your platform.

# Building
### Downloading Source and Dependencies
```bash
# clone this repo and cd into it
git clone https://github.com/johnmanjohnston/dawception/
cd dawception

# clone JUCE
git clone https://github.com/juce-framework/JUCE/
```

<details>
    <summary><b>JUCE dependencies for Linux</b></summary>
<br>
    
You can install dependencies with the `apt` package manager by running:
```bash
sudo apt install libasound2-dev libjack-jackd2-dev \
    libfreetype-dev libfontconfig1-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libglu1-mesa-dev mesa-common-dev
```
You should then be able to compile JUCE without any errors.
<br> <br>
</details>

```bash
# generate build files with CMake
cmake .
```

You should then see the required files to build for your platform (Makefile for Linux/macOS, Visual Studio solution for Windows).

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
#### Linux/macOS (Makefile)
```bash
# run make
make
```

#### Windows (Visual Studio)
- open `dawception.sln`
- Build > Build Solution (or do <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>B</kbd>)

### Running
- If you built a **debug** build, move `dawception_artefacts/Debug/VST3/DAWception.vst3`
- If you built a **release** build, move `dawception_artefacts/Release/VST3/DAWception.vst3`

to your DAW's VST3 plugin folder.
Then, scan and add DAWception to any track in your DAW.

# License
DAWception is licensed under the AGPLv3, see [LICENSE](https://github.com/johnmanjohnston/dawception/blob/main/LICENSE) for details.
