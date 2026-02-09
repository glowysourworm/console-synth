# console-synth

A console synthesizer implementation using FTXUI / RTAudio / Airwindows built in native C++ to
play around with and understand real time audio. 

![screenshot](https://github.com/glowysourworm/console-synth/blob/master/screenshot.png)

# Real Time (RT Audio / Port Audio / Windows APIs)

The learning experience was to: 

1) See how real time audio was processed in a single / multi-threaded environment
2) Understand latency (how many samples can we process in our application while accessing the sound hardware?)
3) Learn a portable library for the sound hardware
4) Build some basic effects, and work with other audio effects plugins

# FTXUI

This is a stack-based, portable, native, pseudo-fluent style API, which has many efficient
features for rendering terminal output. I'd highly recommend trying it out! The UI looks 
great; and it certainly keeps up with our application. 

However, there is need of some documentation on std::move, and std::make_shared. The library
makes heavy use of stack memory; and shared pointers. Also, it uses nested classes to keep
the stack hierarchy of parent-child relationships; and, thus, requires that you tailor your
classes as the library does. (this can cause memory allocation issues)

I'd ask / recommend that we share a discussion about this (if you're interested), and that
we come up with a full example of how to inherit and extend the ftxui::Component properly.

# Latency

The RT Audio, Port Audio, and WASAPI clients were tried (see repo history). The RT Audio
API is very easy to use; but lacks some detail on the sound hardware for the audio buffer.

The hardware has settings for the buffer that it preferrs to use - which was evident from
the WASAPI implementation. 

Windows has several (updated) versions of their IAudioClient interface - which are worth
seeing through if you're willing to put up with the details. I'm assuming Port Audio also
had a good implementation. But, sometimes it's hard to know where crackling audio is 
coming from. 

I believe all three API's are very workable; but there are some details of device specifics
that are left out of some of them. So, I'd be very careful as to your target platform when
starting a project. And, ask people about what's in them! Sometimes you find people that
have experience on a bigger project.

# Effects

The synth currently has:

1) Oscillator (VCO)
2) Input Envelope
3) Envelope Filter (Manual / VLO / Sweep)
4) Compressor
5) Reverb (Airwindows kCathedral)
6) Delay (simple tap delay via a Comb Filter)

There are people who dedicate their LIVES to audio effects! I'd highly recommend visiting
the Airwindows DSP / DAW site and looking through their source code.

# How to Use

Currently, you'll need Visual Studio 2022 (Community Edition), or an equivalent compiler
to build and run this project.

For dependencies, you'll need to build FTXUI, RT Audio, and set up your project to build
and run the terminal synth based on these libs. In the future, there could be a build 
process to help out users; but it depends on how much time I have to finish it. 

For now, you'll have to use what's there; and I'd be happy to answer any questions you
have.

~gsw

