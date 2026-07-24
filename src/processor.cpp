#include "processor.h"
#include "daw/automation_relay.h"
#include "daw/defs.h"
#include "daw/track.h"
#include "daw/utility.h"
#include "editor.h"

AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      juce::ChangeBroadcaster() {
    addParameter(masterGain = new juce::AudioParameterFloat("master", "Master",
                                                            0.f, 4.f, 1.f));

    DBG("track v" << VERSION_STRING);

    updateLatencyAfterDelay();

    juce::AudioParameterIntAttributes johnAttr =
        juce::AudioParameterIntAttributes().withAutomatable(false);
    addParameter(johnInt = new juce::AudioParameterInt("john", "john", 0, 1, 0,
                                                       johnAttr));

    for (int i = 0; i < 128; ++i) {
        juce::String paramID = "param_" + juce::String(i);
        juce::AudioParameterFloat *param =
            new juce::AudioParameterFloat(paramID, paramID, 0.f, 100.f, 0.f);

        addParameter(param);
    }

    for (int i = 0; i < getParameters().size(); ++i) {
        juce::AudioProcessorParameter *param = getParameters()[i];
        juce::AudioProcessorParameterWithID *paramWithID =
            (juce::AudioProcessorParameterWithID *)param;

        if (paramWithID->getName(16) == "param_0") {
            automatableParametersIndexOffset = i;
            break;
        }
    }
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

const juce::String AudioPluginAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginAudioProcessor::getNumPrograms() { return 1; }

int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String &newName) {
    juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {

    DBG("prepareToPlay() called: ");
    DBG("sampleRate = " << sampleRate);
    DBG("track::SAMPLE_RATE = " << track::SAMPLE_RATE);

    if (!juce::approximatelyEqual(sampleRate, track::SAMPLE_RATE) &&
        track::SAMPLE_RATE > 0.0) {
        this->faultySampleRate = track::SAMPLE_RATE;
        this->dispatchGUIInstruction(
            UI_INSTRUCTION_HOST_PROCESSOR_SAMPLE_RATE_MISMATCH);
    }

    track::SAMPLE_RATE = sampleRate;
    track::SAMPLES_PER_BLOCK = samplesPerBlock;

    this->maxSamplesPerBlock = samplesPerBlock;

    updateLatency();

    for (track::audioNode &t : tracks) {
        t.processor = this;
        t.preparePlugins();
    }

    if (prepared)
        return;

    if (afm.findFormatForFileExtension("wav") == nullptr) {
        afm.registerBasicFormats();
    }

    bool createDummyTracks = false;
    if (createDummyTracks) {
        auto &g1 = tracks.emplace_back();
        g1.isTrack = false;
        g1.trackName = "g1";
        g1.processor = this;

        for (int i = 0; i < 5; ++i) {
            auto &subtrack = g1.childNodes.emplace_back();
            subtrack.trackName = "child track " + juce::String(i);
            subtrack.processor = this;
        }

        auto &sg = g1.childNodes.emplace_back();
        sg.isTrack = false;
        sg.trackName = "subgroup1";
        sg.processor = this;

        for (int i = 0; i < 3; ++i) {
            auto &subtrack = sg.childNodes.emplace_back();
            subtrack.trackName = "subgroup child " + juce::String(i);
            subtrack.processor = this;
        }

        auto &g2 = tracks.emplace_back();
        g2.trackName = "group 2";
        g2.isTrack = false;
        g2.processor = this;
        auto &x = g2.childNodes.emplace_back();
        x.trackName = "2nd parent's child whcih is a group";
        x.isTrack = false;
        x.processor = this;

        auto &gilbert = x.childNodes.emplace_back();
        gilbert.trackName = "gilbert :)";
        gilbert.isTrack = false;
        gilbert.processor = this;
        for (int i = 0; i < 3; ++i) {
            auto &sub = gilbert.childNodes.emplace_back();
            sub.trackName = juce::String(i);
            sub.isTrack = false;
            sub.processor = this;

            for (int j = 0; j < 4; ++j) {
                auto &y = sub.childNodes.emplace_back();
                y.trackName = juce::String(j);
                y.processor = this;
            }
        }
    }

    /*
    DBG("prepareToPlay() called with sample rate " << sampleRate);
    DBG("total outputs: " << getTotalNumOutputChannels());
    DBG("total inputs: " << getTotalNumInputChannels());
    */

    prepared = true;
}

void AudioPluginAudioProcessor::releaseResources() {}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                             juce::MidiBuffer &midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::AudioPlayHead *playhead = getPlayHead();
    bool playheadExists = playhead != nullptr;

    bool temporaryThing = false;
    if (temporaryThing) {
        // process tracks; tracks populate their internal buffer
        for (size_t i = 0; i < tracks.size(); ++i) {
            tracks[i].process(buffer.getNumSamples(), 44100);
        }
    }

    if (playheadExists) {
        if (playhead->getPosition()->getIsPlaying() == true) {

            int currentSample = *playhead->getPosition()->getTimeInSamples();
            --currentSample;

            // process tracks; tracks populate their internal buffer
            for (size_t i = 0; i < tracks.size(); ++i) {
                tracks[i].process(buffer.getNumSamples(), currentSample);
            }

            // sum track buffers
            for (size_t i = 0; i < tracks.size(); ++i) {
                for (int channel = 0; channel < totalNumOutputChannels;
                     ++channel) {

                    buffer.addFrom(channel, 0, tracks[i].buffer, channel, 0,
                                   buffer.getNumSamples());
                }
            }
        }
    }

    buffer.applyGain(*masterGain);
}

bool AudioPluginAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *AudioPluginAudioProcessor::createEditor() {
    return new AudioPluginAudioProcessorEditor(*this);
}

juce::XmlElement *
AudioPluginAudioProcessor::serializeNode(track::audioNode *node) {
    juce::XmlElement *nodeElement = new juce::XmlElement("node");
    nodeElement->setAttribute("istrack", node->isTrack);
    nodeElement->setAttribute("name", node->trackName);
    nodeElement->setAttribute("gain", node->gain);
    nodeElement->setAttribute("pan", node->pan);
    nodeElement->setAttribute("solo", node->explicitSolo);
    nodeElement->setAttribute("mute", node->m);

    for (size_t i = 0; i < node->plugins.size(); ++i) {
        auto &pluginInstance = node->plugins[i];
        juce::XmlElement *pluginElement = new juce::XmlElement("plugin");

        juce::String identifier =
            pluginInstance->plugin->getPluginDescription()
                .fileOrIdentifier.upToLastOccurrenceOf(".vst3", true, true);
        pluginElement->setAttribute("identifier", identifier);

        juce::MemoryBlock pluginData;
        pluginInstance->plugin->getStateInformation(pluginData);
        pluginElement->setAttribute("data", pluginData.toBase64Encoding());
        pluginElement->setAttribute("bypass", pluginInstance->bypassed);
        pluginElement->setAttribute("drywetmix", pluginInstance->dryWetMix);

        for (size_t j = 0; j < pluginInstance->relayParams.size(); ++j) {
            juce::XmlElement *relayParamElement =
                new juce::XmlElement("relayparam");

            relayParamElement->setAttribute(
                "pluginparamindex",
                pluginInstance->relayParams[j].pluginParamIndex);

            relayParamElement->setAttribute(
                "relayindex", pluginInstance->relayParams[j].outputParamID);

            pluginElement->addChildElement(relayParamElement);
        }

        nodeElement->addChildElement(pluginElement);
    }

    if (node->isTrack) {
        for (size_t i = 0; i < node->clips.size(); ++i) {
            juce::XmlElement *clipElement = new juce::XmlElement("clip");
            track::clip *c = &node->clips[i];

            clipElement->setAttribute("active", c->active);

            clipElement->setAttribute("path", c->path);
            clipElement->setAttribute("start", c->startPositionSample);
            clipElement->setAttribute("name", c->name);
            clipElement->setAttribute("gain", c->gain);

            clipElement->setAttribute("trimleft", c->trimLeft);
            clipElement->setAttribute("trimright", c->trimRight);

            nodeElement->addChildElement(clipElement);
        }
    } else {
        for (track::audioNode &child : node->childNodes) {
            nodeElement->addChildElement(serializeNode(&child));
        }
    }

    return nodeElement;
}

void AudioPluginAudioProcessor::deserializeNode(juce::XmlElement *nodeElement,
                                                track::audioNode *node) {
    node->isTrack = nodeElement->getBoolAttribute("istrack", true);
    node->trackName = nodeElement->getStringAttribute("name");
    node->gain = (float)nodeElement->getDoubleAttribute("gain", 1.0);
    node->pan = (float)nodeElement->getDoubleAttribute("pan", 0.0);
    node->explicitSolo = nodeElement->getBoolAttribute("solo");
    node->m = nodeElement->getBoolAttribute("mute");
    node->processor = this;

    juce::XmlElement *pluginElement = nodeElement->getChildByName("plugin");
    for (size_t i = 0; pluginElement != nullptr; ++i) {
        juce::String identifier =
            pluginElement->getStringAttribute("identifier");
        bool success = node->addPlugin(identifier);

        if (success) {
            // get plugin
            auto &pluginInstance = *node->plugins.back();

            // get and assign data from base64
            juce::String encodedPluginData =
                pluginElement->getStringAttribute("data");
            juce::MemoryBlock pluginData;

            pluginData.fromBase64Encoding(encodedPluginData);
            pluginInstance.plugin->setStateInformation(pluginData.getData(),
                                                       pluginData.getSize());
            bool bypassed = pluginElement->getBoolAttribute("bypass", false);
            node->plugins[i]->bypassed = bypassed;

            float dryWetMix =
                pluginElement->getDoubleAttribute("drywetmix", 1.f);
            node->plugins[i]->dryWetMix = dryWetMix;

            juce::XmlElement *relayParamElement =
                pluginElement->getChildByName("relayparam");

            while (relayParamElement != nullptr) {
                node->plugins[i]->relayParams.emplace_back();
                track::relayParam *relayParam =
                    &node->plugins[i]->relayParams.back();

                relayParam->pluginParamIndex =
                    relayParamElement->getIntAttribute("pluginparamindex", -1);
                relayParam->outputParamID =
                    relayParamElement->getIntAttribute("relayindex", -1);

                relayParamElement =
                    relayParamElement->getNextElementWithTagName("relayparam");
            }
        } else {
            failedDeserializationErrors.emplace_back(
                "could not load plugin with path: " + identifier);
        }

        pluginElement = pluginElement->getNextElementWithTagName("plugin");
    }

    if (node->isTrack) {
        juce::XmlElement *clipElement = nodeElement->getChildByName("clip");

        while (clipElement != nullptr) {
            // get clip data
            juce::String path = clipElement->getStringAttribute("path");
            juce::String clipName = clipElement->getStringAttribute("name");
            int start = clipElement->getIntAttribute("start");
            bool active = clipElement->getIntAttribute("active");

            int trimLeft = clipElement->getIntAttribute("trimleft");
            int trimRight = clipElement->getIntAttribute("trimright");

            float clipGain = clipElement->getDoubleAttribute("gain", 1.f);

            // create clip instance
            node->clips.emplace_back();
            track::clip *c = &node->clips.back();
            c->active = active;
            c->path = path;
            c->name = clipName;
            c->startPositionSample = start;
            c->trimLeft = trimLeft;
            c->trimRight = trimRight;
            c->gain = clipGain;
            bool success = c->updateBuffer();

            if (!success) {
                failedDeserializationErrors.emplace_back(
                    "could not find audio file for '" + clipName +
                    "' missing path is " + path);
            }

            clipElement = clipElement->getNextElementWithTagName("clip");
        }
    } else {
        juce::XmlElement *childElement = nodeElement->getChildByName("node");
        while (childElement != nullptr) {
            track::audioNode *child = &node->childNodes.emplace_back();
            deserializeNode(childElement, child);

            childElement = childElement->getNextElementWithTagName("node");
        }
    }
}

void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
    std::unique_ptr<juce::XmlElement> xml =
        std::make_unique<juce::XmlElement>("track");

    xml->deleteAllChildElementsWithTagName("node");
    xml->deleteAllChildElementsWithTagName("projectsettings");
    xml->deleteAllChildElementsWithTagName("knownplugins");

    juce::XmlElement *projectSettings = new juce::XmlElement("projectsettings");
    // relayed params
    for (int i = 0; i < 128; ++i) {
        int index = i + automatableParametersIndexOffset;
        projectSettings->setAttribute("param_" + juce::String(i),
                                      getParameters()[index]->getValue());
    }

    projectSettings->setAttribute("samplerate", getSampleRate());
    projectSettings->setAttribute("samplesperblock", track::SAMPLES_PER_BLOCK);
    projectSettings->setAttribute("mastergain", *this->masterGain);
    projectSettings->setAttribute("autogrid", track::AUTO_GRID);
    projectSettings->setAttribute("snapdivision",
                                  track::SNAP_DIVISIONS_PER_QUARTER_NOTE);
    projectSettings->setAttribute("scale", track::SCALE);

    juce::XmlElement *knownPlugins = new juce::XmlElement("knownplugins");
    for (auto &p : knownPluginList.getTypes()) {
        // DBG(p.name << " by " << p.manufacturerName);
        // juce::XmlElement *knownPluginElement = p.createXml().get();
        // knownPlugins->addChildElement(knownPluginElement);

        // ripped from PluginDescription.cpp's PluginDescription::createXml()
        // auto e = std::make_unique<XmlElement>("PLUGIN");

        juce::XmlElement *e =
            new juce::XmlElement("PLUGIN"); // has to be called this otherwise
                                            // loadFromXml() won't work nicely

        e->setAttribute("name", p.name);
        if (p.descriptiveName != p.name)
            e->setAttribute("descriptiveName", p.descriptiveName);

        e->setAttribute("format", p.pluginFormatName);
        e->setAttribute("category", p.category);
        e->setAttribute("manufacturer", p.manufacturerName);
        e->setAttribute("version", p.version);
        e->setAttribute("file", p.fileOrIdentifier);
        e->setAttribute("uniqueId", String::toHexString(p.uniqueId));
        e->setAttribute("isInstrument", p.isInstrument);
        e->setAttribute("fileTime", String::toHexString(
                                        p.lastFileModTime.toMilliseconds()));
        e->setAttribute(
            "infoUpdateTime",
            String::toHexString(p.lastInfoUpdateTime.toMilliseconds()));
        e->setAttribute("numInputs", p.numInputChannels);
        e->setAttribute("numOutputs", p.numOutputChannels);
        e->setAttribute("isShell", p.hasSharedContainer);
        e->setAttribute("hasARAExtension", p.hasARAExtension);
        e->setAttribute("uid", String::toHexString(p.deprecatedUid));

        knownPlugins->addChildElement(e);
    }

    // DBG(knownPlugins->createDocument(""));

    xml->addChildElement(projectSettings);
    xml->addChildElement(knownPlugins);

    for (size_t i = 0; i < tracks.size(); ++i) {
        xml->addChildElement(serializeNode(&tracks[i]));
    }

    copyXmlToBinary(*xml, destData);

    // DBG(xml->createDocument(""));
}

void AudioPluginAudioProcessor::setStateInformation(const void *data,
                                                    int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(
        getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        tracks.clear();

        juce::XmlElement *projectSettings =
            xmlState->getChildByName("projectsettings");

        // deserialize relay params first because we need their indexes and
        // doing it this way is just easier
        for (int i = 0; i < 128; ++i) {
            int index = i + automatableParametersIndexOffset;
            float val = projectSettings->getAttributeValue(i).getFloatValue();
            getParameters()[index]->setValue(val);
        }

        track::SAMPLE_RATE = projectSettings->getDoubleAttribute("samplerate");
        track::SAMPLES_PER_BLOCK =
            projectSettings->getIntAttribute("samplesperblock");
        *this->masterGain =
            (float)projectSettings->getDoubleAttribute("mastergain", 1.0);

        track::AUTO_GRID = projectSettings->getBoolAttribute("autogrid", true);
        track::SNAP_DIVISIONS_PER_QUARTER_NOTE =
            projectSettings->getDoubleAttribute("snapdivision", 1);

        track::SCALE = projectSettings->getDoubleAttribute("scale", 1.0);

        DBG("sample rate on deserialization: " << getSampleRate());
        if (!juce::approximatelyEqual(
                getSampleRate(),
                projectSettings->getDoubleAttribute("samplerate"))) {

            double oldSampleRate =
                projectSettings->getDoubleAttribute("samplerate");
            this->faultySampleRate = oldSampleRate;
            this->deserializationSampleRateMismatch = true;
        }
    }

    juce::XmlElement *knownPlugins = xmlState->getChildByName("knownplugins");
    juce::XmlElement *curPluginElement = knownPlugins->getChildByName("PLUGIN");

    while (curPluginElement != nullptr) {
        juce::PluginDescription pd;
        pd.loadFromXml(*curPluginElement);
        knownPluginList.addType(pd);

        curPluginElement =
            curPluginElement->getNextElementWithTagName("PLUGIN");
    }

    juce::XmlElement *nodeElement = xmlState->getChildByName("node");

    while (nodeElement != nullptr) {
        track::audioNode *node = &tracks.emplace_back();
        // DBG("root deserialization call for " << node->trackName);
        deserializeNode(nodeElement, node);
        nodeElement = nodeElement->getNextElementWithTagName("node");
    }

    if (failedDeserializationErrors.size() > 0) {
        DBG("copying faulty xml...");
        faultyState = xmlState->createDocument("");
    }

    // DBG(xmlState->createDocument(""));
}

void AudioPluginAudioProcessor::updateLatency() {
    int totalLatency = 0;
    track::MAX_LATENT_SAMPLES = -1;

    for (track::audioNode &node : this->tracks) {
        totalLatency += node.getTotalLatencySamples();
    }

    setLatencySamples(totalLatency);

    dispatchGUIInstruction(UI_INSTRUCTION_UPDATE_DISPLAYED_LATENCY);
}

void AudioPluginAudioProcessor::updateLatencyAfterDelay() {
    juce::Timer::callAfterDelay(1000, [this] { updateLatency(); });
}

void AudioPluginAudioProcessor::dispatchGUIInstruction(
    int commandID, void *data, std::vector<int> routeData) {
    if (commandID != -1) {
        this->GUIInstruction.command = commandID;
    }

    this->GUIInstruction.metadata = data;
    this->GUIInstruction.r = routeData;

    sendSynchronousChangeMessage();
}

void AudioPluginAudioProcessor::requireSaving() {
    johnInt->setValueNotifyingHost(*johnInt == 0 ? 1 : 0);
}

void AudioPluginAudioProcessor::reset() {}

void AudioPluginAudioProcessor::updateImpliedSolos() {
    // if node is marked solo, all nodes leading up to it are marked solo

    // reset implied nodes status
    std::vector<track::audioNode *> nodes =
        track::utility::getFlattenedNodes(this);

    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i]->impliedSolo = false;
    }

    if (!soloMode)
        return;

    // find nodes that should be implied solo
    DBG("updateImpliedSolos() called");
    for (size_t i = 0; i < tracks.size(); ++i) {
        std::vector<int> tmpRoute;
        tmpRoute.emplace_back(i);

        if (!tracks[i].isTrack) {
            std::vector<track::audioNode *> flattenedGroup;
            track::utility::traverseAndFlattenNodes(&flattenedGroup, &tracks[i],
                                                    this);
            bool foundCompetition = false;

            for (size_t j = 0; j < flattenedGroup.size(); ++j) {
                if (flattenedGroup[j]->explicitSolo) {
                    foundCompetition = true;
                    break;
                }
            }

            findImpliedSolos(tmpRoute, &tracks[i],
                             tracks[i].explicitSolo && !foundCompetition);
        }
    }
}

// this whole system is held together by gulab jamun oil
void AudioPluginAudioProcessor::findImpliedSolos(std::vector<int> route,
                                                 track::audioNode *parent,
                                                 bool ancestorIsSolo) {
    // DBG("findImpliedSolos() called for "
    //     << track::utility::prettyVector(route));

    for (size_t i = 0; i < parent->childNodes.size(); ++i) {
        route.emplace_back(i);

        track::audioNode *childNode = &parent->childNodes[i];

        if (ancestorIsSolo) {
            DBG("ancestor is solo, forcing implied solo for "
                << childNode->trackName);
            childNode->impliedSolo = true;
        }

        if (childNode->explicitSolo) {
            // DBG("route is " << track::utility::prettyVector(route));

            std::vector<int> confirmedRoute = route;
            for (int j = 0; j < (int)route.size(); j++) {
                track::utility::getNodeFromRoute(confirmedRoute, this)
                    ->impliedSolo = true;
                confirmedRoute.pop_back();
            }

            if (!childNode->isTrack) {
                DBG("explicit solo on a GROUP!");
            }
        }

        // search for competition
        std::vector<track::audioNode *> flattenedGroup;
        track::utility::traverseAndFlattenNodes(&flattenedGroup, childNode,
                                                this);
        bool foundCompetition = false;

        for (size_t j = 0; j < flattenedGroup.size(); ++j) {
            if (flattenedGroup[j]->explicitSolo) {
                foundCompetition = true;
                break;
            }
        }

        if (!childNode->isTrack)
            findImpliedSolos(route, &parent->childNodes[i],
                             !foundCompetition &&
                                 (childNode->explicitSolo || ancestorIsSolo));

        if (childNode->explicitSolo && !childNode->isTrack &&
            !foundCompetition) {
            std::vector<track::audioNode *> nodes;
            track::utility::traverseAndFlattenNodes(&nodes, childNode, this);
            for (size_t j = 0; j < nodes.size(); ++j) {
                nodes[j]->impliedSolo = true;
            }
        }

        route.pop_back();
    }
}
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new AudioPluginAudioProcessor();
}
