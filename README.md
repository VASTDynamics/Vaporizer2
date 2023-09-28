# Vaporizer2

![logo](Artwork/vaporizer2.png)
 
"PROBABLY THE MOST VERSATILE WAVETABLE SYNTHESIZER"  
Vaporizer2 is a hybrid wavetable additive / subtractive VST / AU / AAX synthesizer / sampler workstation.  

The heart of this plugin is the alias-free versatile high-performance wavetable sound engine with low system CPU resource usage.  

Vaporizer2 comes with a groundbreaking wavetable editor with a vast number of editing possibilities including frequency shift, smooth, clean, bend and bloat for single-cycles, parts of single-cycles or even whole wavetables.
Featuring an easy-to use wavetable draw mode with smooth Bezier curves and snap to grid function.

Available formats: VST2 / VST3 / AU / AAX plugins and a standalone version for both macOS and Windows.

* Vaporizer2 was developed by VAST Dynamics GbR between 2017 and 2023 and is now free software under GPL3 license.  
* Vaporizer2 is based on the [JUCE framework](https://github.com/juce-framework/JUCE) under GPL3 license.  
* Vaporizer2 ueses [muFFT](https://github.com/Themaister/muFFT/tree/master) under GPL2 license.
* You need a (free) developer license from [Steinberg](https://www.steinberg.net/de/developers/) to use the VST2 SDK (to build a .vst instead of .vst3).  In that case put the SDK in the VST_SDK folder.
* You need a (free) ASIO SDK 2.3 installation from [Steinberg](https://www.steinberg.net/de/developers/). Place it in the asiosdk folder.  
* You need a (free) developer license from [Avid Technology](http://developer.avid.com/aax/) to use the AAX SDK (to build an AAX plugin for ProTools). In that case put the SDK in the AAX_SDK folder. The compiled AAXLibrary_x64.lib is expected here C:\Vaporizer2\AAX_SDK\AAX_SDK_2p3p2\AAX_SDK_2p3p2\Libs\Release\AAXLibrary_x64.lib. You can change the folder in the global path settings of the Projucer.
* Code signing is required to build an AAX or an AU on MacOS. The process is not described here.
* To the best of our knowledge the true type fonts in [VASTvaporizer/Resources/Fonts/](VASTvaporizer/Resources/Fonts/) are freeware and can be distributed. 

## Donation to support the project 
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=5H8QH4AU6UWWC)
   
[![QR-Code](Artwork/QR-Code.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=5H8QH4AU6UWWC)

## Preparation
### 1. Windows
  - The project expects to find the repository structure in c:\Vaporizer2\
    Use
    `mklink /h /J "C:\Vaporizer2" [lokal repository path]`
    to symlink if needed.   
  - Run Projucer.exe as admininistrator.
    
### 2. macOS
  - Build Projucer from JUCE/extras/Projucer/Builds/MacOSX/Projucer.xcodeproj
  - Run Projucer
  - Set Global Paths from Menu Projucer>Global Paths.
  - Set path to JUCE, JUCE modules, VST SDK, AAX SDK

## Screenshot

![my screenshot](Artwork/vapo2box_neu_2_large.png)

Find more about the plug-in [here](https://www.vast-dynamics.com/?q=Vaporizer2).

## Download installers

You can download installers for Windows and macOS with the built and packaged plug-ins [here](https://vast-dynamics.com/?q=products).

## Plug-in features

- Includes 780+ wavetables and single cycles and 410+ factory presets.
- Vaporizer2 features a comprehensive preset management system with tags, free text search, file system folder management and ratings.

### MANY MORE HIGHLIGHTS

- State-of the art wavetable engine with four independent alias-free wavetable oscillator banks with up to 24 oscillator unison
- Low system CPU resource usage - even with more than 1.000 oscillators playing
- Groundbreaking wavetable editor with a vast number of editing possibilities including frequency shift, smooth, clean, bend and bloat for single-cycles, parts of single-cycles or even whole wavetables
- Easy-to use wavetable draw mode with smooth Bezier curves and snap to grid function
- Mix-in, amplitude modulate, interpolate, convolute, time>frequency, frequency>time or normalize wavetable cycles
- Generate FM, PWM, harmonic morphed / reduced, low pass / high pass / band pass / comb / formant filtering and even hard sync wavetables out of an arbitrary single cycle waveform 
- Morphing and phase shifting of wavetable cycles
- Import and change standard (Serum, Icarus) format wavetables - also simply via drag and drop
- Record single cycle changes into a new wavetable
- Oscillator pitch modulation and phase modulation with very fast modulations (up to ~ 2kHz)
- 30 real-time wavetable effects per oscillator including innovative algorithms for hard sync, sincify, brew and bend / bloat
- Additive synthesis section that allows to freely edit all harmonics and phases in real-time
- FM synthesis up to a chain of 4 oscillators
- Sampler section (wav / aiff / ogg / mp3) with loop points, loop start modulation, zero crossing detection and key tracking
- Granular synthesis mode in the sampler section for stunning ambient effects
- Resample samples to wavetables with pitch detection
- Intelligent polyphonic portamento / glissando (you need to hear it!)
- Mono legato mode or polyphonic 4 / 16 voice modes
- Three effects busses including 4 x oversampling and mono cutoff to preserve a clean low end
- 11+ highly parameterizable effects including reverb, delay, chorus, flanger, bitcrush, eq, compressor, limiter, flanger, phaser, comb filter, waveshaper and more
- All effect times and durations can be synced to DAW
- State of the art filter section (30+ different types) with 4 x oversampled LPF (biquad, state variable and diode ladder implementations) plus high pass, all pass, band pass, notch, shelf, comb and scream filters - all with resonance
- Very steep filter curves for punchy basses
- Real-time filter response display
- Comprehensive modulation system with powerful modulation matrix including graphical display of modulation curves
- 25 modulation sources including key track, random drift and four custom macro knobs
- 220(!) modulation destinations (modulatable parameters) for almost unlimited modulation combinations
- Innovative audio routing system for all sound generators, filters and effect busses
- Five freely editable MSEG envelopes with loop function and DAW timecode sync
- Five LFOs (per voice - means up to 80 LFOs in total) with smooth generation optimized for lowest frequencies with ramp and phase that can be set per voice or global - can be synced to the DAW
- Built-in arpeggiator that is freely editable and supports polyphonic modes, hold and midi file drag and drop
- Three step sequencers that can be synced to the DAW
- All parameter knobs / sliders automatable and support manual editing for exact parameter values
- Support for MIDI Polyphonic Expression (MPE): receive MPE MIDI dimensions (X/Y/Z) pitchbend, aftertouch (freely assignable), release velocity / lift (controlled via MSEG) and brightness / timbre (freely assignable) form supported controllers for highly expressive control options
- State of the art preset management system with tags, free text search, file system folder management and ratings
- General setting of wavetable frequency characteristics: sharp (ideal filtering), soft (butterworth filtering), dull (linear) 
- Comes with a daily growing number of presets – free regular updates on the website
- Includes 780+ wavetables and single cycles and 410+ factory presets
- Easy to use and intuitive user interface that is freely scalable and resizable even up to 4k and more with many drag and drop features and many graphical components, e.g. audio-out oscilloscope, filter response, MSEG / LFO / ARP
- Polyphonic real-time visualization of parameter modulations
- Four different UI themes to choose from
- Hotkeys for most important editor functions
- Plus: the sound is stunning!
- Available formats: VST2 / VST3 / AU / AAX plugins and a standalone version for both macOS and Windows.

### SYSTEM REQUIREMENTS

- Any CPU that supports AVX or SSE2 (Intel or AMD)
- 1GB RAM
- Windows 8/10 (32/64bit)
- macOS 10.10 or above
- VSTi / AU / AAX compatible Host software (32bit and 64bit).
