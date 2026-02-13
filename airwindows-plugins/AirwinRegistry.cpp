/*
 * AirwinConsolidated - an adaptation of the airwindows effect suite
 * for various open source clients
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#include "AirwinRegistry.h"

#include "ADClip7.h"
#include "ADClip8.h"
#include "ADT.h"
#include "AQuickVoiceClip.h"
#include "Acceleration.h"
#include "Acceleration2.h"
#include "Air.h"
#include "Air2.h"
#include "Air3.h"
#include "Air4.h"
#include "AngleEQ.h"
#include "AngleFilter.h"
#include "Apicolypse.h"
#include "AtmosphereBuss.h"
#include "AtmosphereChannel.h"
#include "Aura.h"
#include "AutoPan.h"
#include "AverMatrix.h"
#include "Average.h"
#include "Balanced.h"
#include "BassAmp.h"
#include "BassDrive.h"
#include "BassKit.h"
#include "Baxandall.h"
#include "Baxandall2.h"
#include "Baxandall3.h"
#include "Beam.h"
#include "BezEQ.h"
#include "BeziComp.h"
#include "BigAmp.h"
#include "Biquad.h"
#include "Biquad2.h"
#include "BiquadDouble.h"
#include "BiquadHiLo.h"
#include "BiquadNonLin.h"
#include "BiquadOneHalf.h"
#include "BiquadPlus.h"
#include "BiquadStack.h"
#include "BiquadTriple.h"
#include "BitDualPan.h"
#include "BitGlitter.h"
#include "BitShiftGain.h"
#include "BitShiftPan.h"
#include "Bite.h"
#include "BlockParty.h"
#include "BrassRider.h"
#include "BrightAmbience.h"
#include "BrightAmbience2.h"
#include "BrightAmbience3.h"
#include "BuildATPDF.h"
#include "BussColors4.h"
#include "ButterComp.h"
#include "ButterComp2.h"
#include "C5RawBuss.h"
#include "C5RawChannel.h"
#include "CStrip.h"
#include "CStrip2.h"
#include "Cabs.h"
#include "Cabs2.h"
#include "Calibre.h"
#include "Cans.h"
#include "CansAW.h"
#include "Capacitor.h"
#include "Capacitor2.h"
#include "Chamber.h"
#include "Chamber2.h"
#include "Channel4.h"
#include "Channel5.h"
#include "Channel6.h"
#include "Channel7.h"
#include "Channel8.h"
#include "Channel9.h"
#include "ChimeyDeluxe.h"
#include "ChimeyGuitar.h"
#include "ChimeyGuitar2.h"
#include "Chorus.h"
#include "ChorusEnsemble.h"
#include "ChromeOxide.h"
#include "Cider.h"
#include "ClearCoat.h"
#include "ClipOnly2.h"
#include "ClipSoftly.h"
#include "CloudCoat.h"
#include "Coils.h"
#include "Coils2.h"
#include "Cojones.h"
#include "Compresaturator.h"
#include "Console0Buss.h"
#include "Console0Channel.h"
#include "Console4Buss.h"
#include "Console4Channel.h"
#include "Console5Buss.h"
#include "Console5Channel.h"
#include "Console5DarkCh.h"
#include "Console6Buss.h"
#include "Console6Channel.h"
#include "Console7Buss.h"
#include "Console7Cascade.h"
#include "Console7Channel.h"
#include "Console7Crunch.h"
#include "Console8BussHype.h"
#include "Console8BussIn.h"
#include "Console8BussOut.h"
#include "Console8ChannelHype.h"
#include "Console8ChannelIn.h"
#include "Console8ChannelOut.h"
#include "Console8LiteBuss.h"
#include "Console8LiteChannel.h"
#include "Console8SubHype.h"
#include "Console8SubIn.h"
#include "Console8SubOut.h"
#include "Console9Buss.h"
#include "Console9Channel.h"
#include "ConsoleHBuss.h"
#include "ConsoleHChannel.h"
#include "ConsoleHPre.h"
#include "ConsoleLABuss.h"
#include "ConsoleLAChannel.h"
#include "ConsoleMCBuss.h"
#include "ConsoleMCChannel.h"
#include "ConsoleMDBuss.h"
#include "ConsoleMDChannel.h"
#include "ConsoleX2Buss.h"
#include "ConsoleX2Channel.h"
#include "ConsoleX2Pre.h"
#include "ConsoleXBuss.h"
#include "ConsoleXChannel.h"
#include "ConsoleXPre.h"
#include "ContentHideD.h"
#include "CreamCoat.h"
#include "Creature.h"
#include "CrickBass.h"
#include "CrunchCoat.h"
#include "CrunchyGrooveWear.h"
#include "Crystal.h"
#include "DCVoltage.h"
#include "Dark.h"
#include "DarkNoise.h"
#include "DeBess.h"
#include "DeBez.h"
#include "DeCrackle.h"
#include "DeEss.h"
#include "DeHiss.h"
#include "DeNoise.h"
#include "DeRez.h"
#include "DeRez2.h"
#include "DeRez3.h"
#include "Deckwrecka.h"
#include "Density.h"
#include "Density2.h"
#include "Density3.h"
#include "Desk.h"
#include "Desk4.h"
#include "DigitalBlack.h"
#include "Dirt.h"
#include "Discontapeity.h"
#include "Discontinuity.h"
#include "Disintegrate.h"
#include "Distance.h"
#include "Distance2.h"
#include "Distance3.h"
#include "Distortion.h"
#include "DitherFloat.h"
#include "DitherMeDiskers.h"
#include "DitherMeTimbers.h"
#include "Ditherbox.h"
#include "DoublePaul.h"
#include "Doublelay.h"
#include "Drive.h"
#include "DrumSlam.h"
#include "DubCenter.h"
#include "DubPlate.h"
#include "DubPlate2.h"
#include "DubSub.h"
#include "DubSub2.h"
#include "Dubly.h"
#include "Dubly2.h"
#include "Dubly3.h"
#include "DustBunny.h"
#include "Dynamics.h"
#include "Dynamics2.h"
#include "Dynamics3.h"
#include "Dyno.h"
#include "EQ.h"
#include "EdIsDim.h"
#include "Edge.h"
#include "Elation.h"
#include "ElectroHat.h"
#include "Elliptical.h"
#include "Energy.h"
#include "Energy2.h"
#include "Ensemble.h"
#include "EveryConsole.h"
#include "EverySlew.h"
#include "EveryTrim.h"
#include "Exciter.h"
#include "Facet.h"
#include "FatEQ.h"
#include "FathomFive.h"
#include "FinalClip.h"
#include "FireAmp.h"
#include "Flipity.h"
#include "Floor.h"
#include "Flutter.h"
#include "Flutter2.h"
#include "Focus.h"
#include "Fracture.h"
#include "Fracture2.h"
#include "FromTape.h"
#include "Galactic.h"
#include "Galactic2.h"
#include "Galactic3.h"
#include "GalacticVibe.h"
#include "Gatelope.h"
#include "GlitchShifter.h"
#include "GoldenSlew.h"
#include "Golem.h"
#include "GrindAmp.h"
#include "Gringer.h"
#include "GrooveWear.h"
#include "GuitarConditioner.h"
#include "HardVacuum.h"
#include "HermeTrim.h"
#include "Hermepass.h"
#include "HighGlossDither.h"
#include "HighImpact.h"
#include "Highpass.h"
#include "Highpass2.h"
#include "HipCrush.h"
#include "Holt.h"
#include "Holt2.h"
#include "Hombre.h"
#include "Huge.h"
#include "Hull.h"
#include "Hull2.h"
#include "Hype.h"
#include "Hypersoft.h"
#include "HypersonX.h"
#include "Hypersonic.h"
#include "Infinity.h"
#include "Infinity2.h"
#include "Inflamer.h"
#include "Infrasonic.h"
#include "Interstage.h"
#include "IronOxide5.h"
#include "IronOxideClassic.h"
#include "IronOxideClassic2.h"
#include "Isolator.h"
#include "Isolator2.h"
#include "Isolator3.h"
#include "Kalman.h"
#include "LRConvolve.h"
#include "LRConvolve2.h"
#include "LRConvolve3.h"
#include "LRFlipTimer.h"
#include "LeadAmp.h"
#include "LeftoMono.h"
#include "LilAmp.h"
#include "Logical4.h"
#include "Loud.h"
#include "Lowpass.h"
#include "Lowpass2.h"
#include "Luxor.h"
#include "MSFlipTimer.h"
#include "MV.h"
#include "MV2.h"
#include "MackEQ.h"
#include "Mackity.h"
#include "Mastering.h"
#include "Mastering2.h"
#include "MatrixVerb.h"
#include "Melt.h"
#include "MidAmp.h"
#include "MidSide.h"
#include "MoNoam.h"
#include "Mojo.h"
#include "Monitoring.h"
#include "Monitoring2.h"
#include "Monitoring3.h"
#include "MultiBandDistortion.h"
#include "NCSeventeen.h"
#include "NaturalizeDither.h"
#include "Neverland.h"
#include "Nikola.h"
#include "NodeDither.h"
#include "Noise.h"
#include "NonlinearSpace.h"
#include "NotJustAnotherCD.h"
#include "NotJustAnotherDither.h"
#include "OneCornerClip.h"
#include "OrbitKick.h"
#include "Overheads.h"
#include "PDBuss.h"
#include "PDChannel.h"
#include "Pafnuty.h"
#include "Pafnuty2.h"
#include "Parametric.h"
#include "PaulDither.h"
#include "PaulWide.h"
#include "PeaksOnly.h"
#include "Pear.h"
#include "Pear2.h"
#include "PearEQ.h"
#include "PearLiteEQ.h"
#include "PhaseNudge.h"
#include "PitchDelay.h"
#include "PitchNasty.h"
#include "PlatinumSlew.h"
#include "PocketVerbs.h"
#include "Pockey.h"
#include "Pockey2.h"
#include "Podcast.h"
#include "PodcastDeluxe.h"
#include "Point.h"
#include "PointyDeluxe.h"
#include "PointyGuitar.h"
#include "Pop.h"
#include "Pop2.h"
#include "Pop3.h"
#include "PowerSag.h"
#include "PowerSag2.h"
#include "Precious.h"
#include "Preponderant.h"
#include "Pressure4.h"
#include "Pressure5.h"
#include "Pressure6.h"
#include "PrimeFIR.h"
#include "PunchyDeluxe.h"
#include "PunchyGuitar.h"
#include "PurestAir.h"
#include "PurestConsole2Buss.h"
#include "PurestConsole2Channel.h"
#include "PurestConsole3Buss.h"
#include "PurestConsole3Channel.h"
#include "PurestConsoleBuss.h"
#include "PurestConsoleChannel.h"
#include "PurestDrive.h"
#include "PurestDualPan.h"
#include "PurestEcho.h"
#include "PurestFade.h"
#include "PurestGain.h"
#include "PurestSaturation.h"
#include "PurestSquish.h"
#include "PurestWarm.h"
#include "PurestWarm2.h"
#include "Pyewacket.h"
#include "RawGlitters.h"
#include "RawTimbers.h"
#include "Recurve.h"
#include "Remap.h"
#include "ResEQ.h"
#include "ResEQ2.h"
#include "Reverb.h"
#include "Righteous4.h"
#include "RightoMono.h"
#include "RingModulator.h"
#include "SampleDelay.h"
#include "Shape.h"
#include "ShortBuss.h"
#include "SideDull.h"
#include "Sidepass.h"
#include "Silhouette.h"
#include "Silken.h"
#include "Sinew.h"
#include "SingleEndedTriode.h"
#include "Slew.h"
#include "Slew2.h"
#include "Slew3.h"
#include "Slew4.h"
#include "SlewOnly.h"
#include "SlewSonic.h"
#include "Smooth.h"
#include "SmoothEQ.h"
#include "SmoothEQ2.h"
#include "SmoothEQ3.h"
#include "SoftClock.h"
#include "SoftClock2.h"
#include "SoftClock3.h"
#include "SoftGate.h"
#include "SpatializeDither.h"
#include "Spiral.h"
#include "Spiral2.h"
#include "SquareRoot.h"
#include "Srsly.h"
#include "Srsly2.h"
#include "Srsly3.h"
#include "StarChild.h"
#include "StarChild2.h"
#include "StereoChorus.h"
#include "StereoDoubler.h"
#include "StereoEnsemble.h"
#include "StereoFX.h"
#include "StoneFireComp.h"
#include "Stonefire.h"
#include "StudioTan.h"
#include "SubTight.h"
#include "SubsOnly.h"
#include "Surge.h"
#include "SurgeTide.h"
#include "SweetWide.h"
#include "Sweeten.h"
#include "Swell.h"
#include "TPDFDither.h"
#include "TPDFWide.h"
#include "TakeCare.h"
#include "Tape.h"
#include "TapeBias.h"
#include "TapeDelay.h"
#include "TapeDelay2.h"
#include "TapeDither.h"
#include "TapeDust.h"
#include "TapeFat.h"
#include "TapeHack.h"
#include "TapeHack2.h"
#include "Texturize.h"
#include "TexturizeMS.h"
#include "Thunder.h"
#include "ToTape5.h"
#include "ToTape6.h"
#include "ToTape7.h"
#include "ToTape8.h"
#include "ToVinyl4.h"
#include "ToneSlant.h"
#include "TransDesk.h"
#include "TremoSquare.h"
#include "Tremolo.h"
#include "Trianglizer.h"
#include "TripleSpread.h"
#include "Tube.h"
#include "Tube2.h"
#include "TubeDesk.h"
#include "UltrasonX.h"
#include "Ultrasonic.h"
#include "UltrasonicLite.h"
#include "UltrasonicMed.h"
#include "UnBox.h"
#include "VariMu.h"
#include "VerbSixes.h"
#include "VerbThic.h"
#include "VerbTiny.h"
#include "Verbity.h"
#include "Verbity2.h"
#include "Vibrato.h"
#include "VinylDither.h"
#include "VoiceOfTheStarship.h"
#include "VoiceTrick.h"
#include "Weight.h"
#include "Wider.h"
#include "Wolfbot.h"
#include "X2Buss.h"
#include "XBandpass.h"
#include "XHighpass.h"
#include "XLowpass.h"
#include "XNotch.h"
#include "XRegion.h"
#include "YBandpass.h"
#include "YHighpass.h"
#include "YLowpass.h"
#include "YNotBandpass.h"
#include "YNotHighpass.h"
#include "YNotLowpass.h"
#include "YNotNotch.h"
#include "YNotch.h"
#include "ZBandpass.h"
#include "ZBandpass2.h"
#include "ZHighpass.h"
#include "ZHighpass2.h"
#include "ZLowpass.h"
#include "ZLowpass2.h"
#include "ZNotch.h"
#include "ZNotch2.h"
#include "ZOutputStage.h"
#include "ZRegion.h"
#include "ZRegion2.h"
#include "Zoom.h"
#include "curve.h"
#include "kAlienSpaceship.h"
#include "kBeyond.h"
#include "kCathedral.h"
#include "kCathedral2.h"
#include "kCathedral3.h"
#include "kCathedral4.h"
#include "kCathedral5.h"
#include "kChamberAR.h"
#include "kCosmos.h"
#include "kGuitarHall.h"
#include "kGuitarHall2.h"
#include "kPlate140.h"
#include "kPlate240.h"
#include "kPlateA.h"
#include "kPlateB.h"
#include "kPlateC.h"
#include "kPlateD.h"
#include "kStation.h"
#include "kWoodRoom.h"
#include "uLawDecode.h"
#include "uLawEncode.h"


#include "AirwinRegistryEntry.h"
#include <exception>
#include <map>
#include <string>
#include <utility>
#include <vector>

AirwinRegistry::AirwinRegistry()
{
    _registry = new std::map<std::string, AirwinRegistryEntry*>();
    _byCategory = new std::map<std::string, std::vector<AirwinRegistryEntry*>*>();
}

AirwinRegistry::~AirwinRegistry()
{
    // Categories
    for (auto iter = _byCategory->begin(); iter != _byCategory->end(); ++iter)
    {
        delete iter->second;    // std::vector*
    }

    // Registry
    for (auto iter = _registry->begin(); iter != _registry->end(); ++iter)
    {
        delete iter->second;    // AirwinRegistryEntry*
    }

    delete _byCategory;
    delete _registry;
}

bool AirwinRegistry::Load(float samplingRate)
{
    try
    {

        this->RegisterAirwindow({ "ADClip7", "Clipping", 89, true, "ADClip7 is the original Airwindows loudness maximizer/ peak control.", airwinconsolidated::ADClip7::kNumParameters, "2022-11-21", new airwinconsolidated::ADClip7::ADClip7(samplingRate) });
        this->RegisterAirwindow({ "ADClip8", "Clipping", 86, true, "ADClip8 is the ultimate Airwindows loudenator/biggenator.", airwinconsolidated::ADClip8::kNumParameters, "2024-02-17", new airwinconsolidated::ADClip8::ADClip8(samplingRate) });
        this->RegisterAirwindow({ "ADT", "Ambience", 10, true, "ADT is a double short delay tap with saturation.", airwinconsolidated::ADT::kNumParameters, "2022-11-21", new airwinconsolidated::ADT::ADT(samplingRate) });
        this->RegisterAirwindow({ "AQuickVoiceClip", "Clipping", 90, true, "AQuickVoiceClip softens headset mic recordings that have been super hard clipped on capture.", airwinconsolidated::AQuickVoiceClip::kNumParameters, "2022-11-21", new airwinconsolidated::AQuickVoiceClip::AQuickVoiceClip(samplingRate) });
        this->RegisterAirwindow({ "Acceleration", "Brightness", 77, true, "Acceleration is an acceleration limiter that tames edge, leaves brightness.", airwinconsolidated::Acceleration::kNumParameters, "2022-11-21", new airwinconsolidated::Acceleration::Acceleration(samplingRate) });
        this->RegisterAirwindow({ "Acceleration2", "Brightness", 67, true, "Acceleration2 is Acceleration, better! Updated for high sample rate and more extreme effect.", airwinconsolidated::Acceleration2::kNumParameters, "2022-11-21", new airwinconsolidated::Acceleration2::Acceleration2(samplingRate) });
        this->RegisterAirwindow({ "Air", "Brightness", 75, true, "Air is a different bright EQ from any other. Requires 44.1K.", airwinconsolidated::Air::kNumParameters, "2022-11-21", new airwinconsolidated::Air::Air(samplingRate) });
        this->RegisterAirwindow({ "Air2", "Brightness", 74, true, "Air2 is a different bright EQ with silk tone and high sample rate support.", airwinconsolidated::Air2::kNumParameters, "2022-11-21", new airwinconsolidated::Air2::Air2(samplingRate) });
        this->RegisterAirwindow({ "Air3", "Brightness", 61, true, "Air3 creates a new form of air-band EQ based on Kalman filtering.", airwinconsolidated::Air3::kNumParameters, "2024-03-10", new airwinconsolidated::Air3::Air3(samplingRate) });
        this->RegisterAirwindow({ "Air4", "Brightness", 60, true, "Air4 extends Air3 with controllable high frequency limiting.", airwinconsolidated::Air4::kNumParameters, "2025-03-17", new airwinconsolidated::Air4::Air4(samplingRate) });
        this->RegisterAirwindow({ "AngleEQ", "Filter", 242, true, "AngleEQ is a strange and colorful EQ.", airwinconsolidated::AngleEQ::kNumParameters, "2025-03-30", new airwinconsolidated::AngleEQ::AngleEQ(samplingRate) });
        this->RegisterAirwindow({ "AngleFilter", "Filter", 243, true, "AngleFilter is the synth-style extension of AngleEQ.", airwinconsolidated::AngleFilter::kNumParameters, "2025-03-30", new airwinconsolidated::AngleFilter::AngleFilter(samplingRate) });
        this->RegisterAirwindow({ "Apicolypse", "Tone Color", 403, true, "Apicolypse is a re-release of my old API-style color adder, exacly as it was.", airwinconsolidated::Apicolypse::kNumParameters, "2022-11-21", new airwinconsolidated::Apicolypse::Apicolypse(samplingRate) });
        this->RegisterAirwindow({ "AtmosphereBuss", "Consoles", 133, true, "AtmosphereBuss is Console5 processing with powerful new acoustic distance effects.", airwinconsolidated::AtmosphereBuss::kNumParameters, "2022-11-21", new airwinconsolidated::AtmosphereBuss::AtmosphereBuss(samplingRate) });
        this->RegisterAirwindow({ "AtmosphereChannel", "Consoles", 134, true, "AtmosphereChannel is Console5 processing with powerful new acoustic distance effects.", airwinconsolidated::AtmosphereChannel::kNumParameters, "2022-11-21", new airwinconsolidated::AtmosphereChannel::AtmosphereChannel(samplingRate) });
        this->RegisterAirwindow({ "Aura", "Effects", 221, true, "Aura is a new kind of resonant lowpass EQ.", airwinconsolidated::Aura::kNumParameters, "2022-11-21", new airwinconsolidated::Aura::Aura(samplingRate) });
        this->RegisterAirwindow({ "AutoPan", "Stereo", 366, false, "AutoPan is for getting some organic stereo movement into your mix.", airwinconsolidated::AutoPan::kNumParameters, "2022-11-21", new airwinconsolidated::AutoPan::AutoPan(samplingRate) });
        this->RegisterAirwindow({ "AverMatrix", "Filter", 261, true, "AverMatrix turns averaging into a full-featured EQ.", airwinconsolidated::AverMatrix::kNumParameters, "2022-11-21", new airwinconsolidated::AverMatrix::AverMatrix(samplingRate) });
        this->RegisterAirwindow({ "Average", "Filter", 262, true, "Average is a distinctive sort of lowpass filter.", airwinconsolidated::Average::kNumParameters, "2022-11-21", new airwinconsolidated::Average::Average(samplingRate) });
        this->RegisterAirwindow({ "Balanced", "Utility", 445, false, "Balanced lets you run an XLR input into a stereo input, sum it to mono, and use it.", airwinconsolidated::Balanced::kNumParameters, "2022-11-21", new airwinconsolidated::Balanced::Balanced(samplingRate) });
        this->RegisterAirwindow({ "BassAmp", "Amp Sims", 40, true, "BassAmp is an old plugin with new tweaks, that gives some bass guitar tones.", airwinconsolidated::BassAmp::kNumParameters, "2022-11-21", new airwinconsolidated::BassAmp::BassAmp(samplingRate) });
        this->RegisterAirwindow({ "BassDrive", "Amp Sims", 39, true, "BassDrive is an old secret weapon, like a kind of bass amp.", airwinconsolidated::BassDrive::kNumParameters, "2022-11-21", new airwinconsolidated::BassDrive::BassDrive(samplingRate) });
        this->RegisterAirwindow({ "BassKit", "Bass", 45, false, "BassKit is centered bass reinforcement with subs fill. Clean and controllable.", airwinconsolidated::BassKit::kNumParameters, "2022-11-21", new airwinconsolidated::BassKit::BassKit(samplingRate) });
        this->RegisterAirwindow({ "Baxandall", "Filter", 265, true, "Baxandall is a killer general-purpose EQ.", airwinconsolidated::Baxandall::kNumParameters, "2022-11-21", new airwinconsolidated::Baxandall::Baxandall(samplingRate) });
        this->RegisterAirwindow({ "Baxandall2", "Filter", 246, true, "Baxandall2 is Baxandall, extended and made more powerful.", airwinconsolidated::Baxandall2::kNumParameters, "2022-11-21", new airwinconsolidated::Baxandall2::Baxandall2(samplingRate) });
        this->RegisterAirwindow({ "Baxandall3", "Unclassified", -1, false, "", airwinconsolidated::Baxandall3::kNumParameters, "2025-03-30", new airwinconsolidated::Baxandall3::Baxandall3(samplingRate) });
        this->RegisterAirwindow({ "Beam", "Dithers", 158, true, "Beam is a wordlength reducer that tries to heighten sonority.", airwinconsolidated::Beam::kNumParameters, "2022-11-21", new airwinconsolidated::Beam::Beam(samplingRate) });
        this->RegisterAirwindow({ "BezEQ", "Filter", 245, true, "BezEQ is a strange, alien three-band shelving EQ.", airwinconsolidated::BezEQ::kNumParameters, "2025-02-02", new airwinconsolidated::BezEQ::BezEQ(samplingRate) });
        this->RegisterAirwindow({ "BeziComp", "Dynamics", 176, false, "BeziComp is a radical compressor that eliminates aliasing!", airwinconsolidated::BeziComp::kNumParameters, "2025-03-17", new airwinconsolidated::BeziComp::BeziComp(samplingRate) });
        this->RegisterAirwindow({ "BigAmp", "Amp Sims", 37, true, "BigAmp is a very flexible amplike thing with a taste for the bizarre.", airwinconsolidated::BigAmp::kNumParameters, "2022-11-21", new airwinconsolidated::BigAmp::BigAmp(samplingRate) });
        this->RegisterAirwindow({ "Biquad", "Biquads", 55, true, "Biquad is the Airwindows implementation of a biquad filter.", airwinconsolidated::Biquad::kNumParameters, "2022-11-21", new airwinconsolidated::Biquad::Biquad(samplingRate) });
        this->RegisterAirwindow({ "Biquad2", "Biquads", 59, true, "Biquad2 is the Airwindows biquad filter that's more sweepable and synthy.", airwinconsolidated::Biquad2::kNumParameters, "2022-11-21", new airwinconsolidated::Biquad2::Biquad2(samplingRate) });
        this->RegisterAirwindow({ "BiquadDouble", "Biquads", 56, true, "BiquadDouble is a handy Airwindows cascaded biquad filter: steeper roll-off before resonance.", airwinconsolidated::BiquadDouble::kNumParameters, "2022-11-21", new airwinconsolidated::BiquadDouble::BiquadDouble(samplingRate) });
        this->RegisterAirwindow({ "BiquadHiLo", "Biquads", 53, true, "BiquadHiLo is the highpass and lowpass filter in ConsoleX.", airwinconsolidated::BiquadHiLo::kNumParameters, "2024-05-10", new airwinconsolidated::BiquadHiLo::BiquadHiLo(samplingRate) });
        this->RegisterAirwindow({ "BiquadNonLin", "Biquads", 52, true, "BiquadNonLin is Capacitor2, but for biquad filtering.", airwinconsolidated::BiquadNonLin::kNumParameters, "2024-03-10", new airwinconsolidated::BiquadNonLin::BiquadNonLin(samplingRate) });
        this->RegisterAirwindow({ "BiquadOneHalf", "Biquads", 57, true, "BiquadOneHalf is an interleaved biquad filter like Biquad.", airwinconsolidated::BiquadOneHalf::kNumParameters, "2022-11-21", new airwinconsolidated::BiquadOneHalf::BiquadOneHalf(samplingRate) });
        this->RegisterAirwindow({ "BiquadPlus", "Biquads", 54, true, "BiquadPlus is Biquad plus zipper noise suppression! For twiddling the controls.", airwinconsolidated::BiquadPlus::kNumParameters, "2022-11-21", new airwinconsolidated::BiquadPlus::BiquadPlus(samplingRate) });
        this->RegisterAirwindow({ "BiquadStack", "Biquads", 51, true, "BiquadStack is a way of making a parametric EQ out of stacked biquad filters.", airwinconsolidated::BiquadStack::kNumParameters, "2024-05-10", new airwinconsolidated::BiquadStack::BiquadStack(samplingRate) });
        this->RegisterAirwindow({ "BiquadTriple", "Biquads", 58, true, "BiquadTriple is a handy Airwindows cascaded biquad filter: steeper roll-off before resonance.", airwinconsolidated::BiquadTriple::kNumParameters, "2022-11-21", new airwinconsolidated::BiquadTriple::BiquadTriple(samplingRate) });
        this->RegisterAirwindow({ "BitDualPan", "Unclassified", -1, false, "", airwinconsolidated::BitDualPan::kNumParameters, "2025-11-28", new airwinconsolidated::BitDualPan::BitDualPan(samplingRate) });
        this->RegisterAirwindow({ "BitGlitter", "Lo-Fi", 288, true, "BitGlitter is an old-sampler style digital lo-fi plugin.", airwinconsolidated::BitGlitter::kNumParameters, "2022-11-21", new airwinconsolidated::BitGlitter::BitGlitter(samplingRate) });
        this->RegisterAirwindow({ "BitShiftGain", "Utility", 419, true, "BitShiftGain is the ‘One Weird Trick’ perfect boost/pad, but in 6db increments ONLY.", airwinconsolidated::BitShiftGain::kNumParameters, "2022-11-21", new airwinconsolidated::BitShiftGain::BitShiftGain(samplingRate) });
        this->RegisterAirwindow({ "BitShiftPan", "Utility", 418, false, "BitShiftPan gives you a gain and a pan control that are ONLY done using bit shifts.", airwinconsolidated::BitShiftPan::kNumParameters, "2023-05-13", new airwinconsolidated::BitShiftPan::BitShiftPan(samplingRate) });
        this->RegisterAirwindow({ "Bite", "Lo-Fi", 294, true, "Bite is an unusual edge-maker.", airwinconsolidated::Bite::kNumParameters, "2022-11-21", new airwinconsolidated::Bite::Bite(samplingRate) });
        this->RegisterAirwindow({ "BlockParty", "Dynamics", 191, true, "BlockParty is like a moderately saturated analog limiter.", airwinconsolidated::BlockParty::kNumParameters, "2022-11-21", new airwinconsolidated::BlockParty::BlockParty(samplingRate) });
        this->RegisterAirwindow({ "BrassRider", "Dynamics", 197, false, "BrassRider is One Weird Trick for drum overheads in metal!", airwinconsolidated::BrassRider::kNumParameters, "2022-11-21", new airwinconsolidated::BrassRider::BrassRider(samplingRate) });
        this->RegisterAirwindow({ "BrightAmbience", "Ambience", 19, true, "BrightAmbience is a plugin for very bright gated reverbs.", airwinconsolidated::BrightAmbience::kNumParameters, "2022-11-21", new airwinconsolidated::BrightAmbience::BrightAmbience(samplingRate) });
        this->RegisterAirwindow({ "BrightAmbience2", "Ambience", 18, false, "BrightAmbience2 is more BrightAmbience with better tone and more slapbacky effects.", airwinconsolidated::BrightAmbience2::kNumParameters, "2022-11-21", new airwinconsolidated::BrightAmbience2::BrightAmbience2(samplingRate) });
        this->RegisterAirwindow({ "BrightAmbience3", "Ambience", 5, false, "BrightAmbience3 adds undersampling for high sample rates, and better feedback.", airwinconsolidated::BrightAmbience3::kNumParameters, "2022-11-21", new airwinconsolidated::BrightAmbience3::BrightAmbience3(samplingRate) });
        this->RegisterAirwindow({ "BuildATPDF", "Dithers", 164, true, "BuildATPDF is a dither-making toolkit.", airwinconsolidated::BuildATPDF::kNumParameters, "2022-11-21", new airwinconsolidated::BuildATPDF::BuildATPDF(samplingRate) });
        this->RegisterAirwindow({ "BussColors4", "Tone Color", 401, true, "BussColors4 is the Airwindows console emulations, now working up to 192K correctly.", airwinconsolidated::BussColors4::kNumParameters, "2022-11-21", new airwinconsolidated::BussColors4::BussColors4(samplingRate) });
        this->RegisterAirwindow({ "ButterComp", "Dynamics", 196, true, "ButterComp is my softest, smoothest compressor.", airwinconsolidated::ButterComp::kNumParameters, "2022-11-21", new airwinconsolidated::ButterComp::ButterComp(samplingRate) });
        this->RegisterAirwindow({ "ButterComp2", "Dynamics", 186, true, "ButterComp2 is improved ButterComp with an output control and sound upgrades.", airwinconsolidated::ButterComp2::kNumParameters, "2022-11-21", new airwinconsolidated::ButterComp2::ButterComp2(samplingRate) });
        this->RegisterAirwindow({ "C5RawBuss", "Consoles", 131, true, "C5RawBuss is the original Console5 algorithm, with optional very gentle DC suppression.", airwinconsolidated::C5RawBuss::kNumParameters, "2022-11-21", new airwinconsolidated::C5RawBuss::C5RawBuss(samplingRate) });
        this->RegisterAirwindow({ "C5RawChannel", "Consoles", 132, true, "C5RawChannel is the original Console5 algorithm, with optional very gentle DC suppression.", airwinconsolidated::C5RawChannel::kNumParameters, "2022-11-21", new airwinconsolidated::C5RawChannel::C5RawChannel(samplingRate) });
        this->RegisterAirwindow({ "CStrip", "Unclassified", -1, true, "CStrip is an Airwindows channel strip.", airwinconsolidated::CStrip::kNumParameters, "2023-04-12", new airwinconsolidated::CStrip::CStrip(samplingRate) });
        this->RegisterAirwindow({ "CStrip2", "Filter", 253, true, "CStrip2 refines and optimizes CStrip, by request!", airwinconsolidated::CStrip2::kNumParameters, "2023-03-11", new airwinconsolidated::CStrip2::CStrip2(samplingRate) });
        this->RegisterAirwindow({ "Cabs", "Amp Sims", 38, true, "Cabs is an Airwindows guitar speaker cabinet simulator.", airwinconsolidated::Cabs::kNumParameters, "2022-11-21", new airwinconsolidated::Cabs::Cabs(samplingRate) });
        this->RegisterAirwindow({ "Cabs2", "Amp Sims", 26, true, "Cabs2 uses really phasey filters to be a speaker cabinet simulator.", airwinconsolidated::Cabs2::kNumParameters, "2025-09-20", new airwinconsolidated::Cabs2::Cabs2(samplingRate) });
        this->RegisterAirwindow({ "Calibre", "Tone Color", 406, true, "Calibre is a re-release of another old Character plugin.", airwinconsolidated::Calibre::kNumParameters, "2022-11-21", new airwinconsolidated::Calibre::Calibre(samplingRate) });
        this->RegisterAirwindow({ "Cans", "Utility", 431, false, "Cans is a room simulator for headphone mixers.", airwinconsolidated::Cans::kNumParameters, "2024-08-18", new airwinconsolidated::Cans::Cans(samplingRate) });
        this->RegisterAirwindow({ "CansAW", "Utility", 430, false, "CansAW is Chris's Airwindows room, in a plugin, for headphone mixers.", airwinconsolidated::CansAW::kNumParameters, "2025-02-09", new airwinconsolidated::CansAW::CansAW(samplingRate) });
        this->RegisterAirwindow({ "Capacitor", "Filter", 268, true, "Capacitor is a lowpass/highpass filter of a new type.", airwinconsolidated::Capacitor::kNumParameters, "2022-11-21", new airwinconsolidated::Capacitor::Capacitor(samplingRate) });
        this->RegisterAirwindow({ "Capacitor2", "Filter", 248, true, "Capacitor2 is Capacitor with extra analog modeling and mojo.", airwinconsolidated::Capacitor2::kNumParameters, "2022-11-21", new airwinconsolidated::Capacitor2::Capacitor2(samplingRate) });
        this->RegisterAirwindow({ "Chamber", "Reverb", 334, true, "Chamber is a feedforward reverb based on the golden ratio.", airwinconsolidated::Chamber::kNumParameters, "2022-11-21", new airwinconsolidated::Chamber::Chamber(samplingRate) });
        this->RegisterAirwindow({ "Chamber2", "Reverb", 333, false, "Chamber2 is a feedforward reverb, a blur delay, and a glitch topping!", airwinconsolidated::Chamber2::kNumParameters, "2023-02-04", new airwinconsolidated::Chamber2::Chamber2(samplingRate) });
        this->RegisterAirwindow({ "Channel4", "Tone Color", 415, true, "Channel4 is a tone coloring plugin that gives more of an analog feel.", airwinconsolidated::Channel4::kNumParameters, "2022-11-21", new airwinconsolidated::Channel4::Channel4(samplingRate) });
        this->RegisterAirwindow({ "Channel5", "Tone Color", 414, true, "Channel5 is Channel for 2018, with new subtleties.", airwinconsolidated::Channel5::kNumParameters, "2022-11-21", new airwinconsolidated::Channel5::Channel5(samplingRate) });
        this->RegisterAirwindow({ "Channel6", "Tone Color", 413, true, "Channel6 uses the Spiral algorithm.", airwinconsolidated::Channel6::kNumParameters, "2022-11-21", new airwinconsolidated::Channel6::Channel6(samplingRate) });
        this->RegisterAirwindow({ "Channel7", "Tone Color", 412, true, "Channel7 improves slew clipping and lets you blend Channel 6 and 5.", airwinconsolidated::Channel7::kNumParameters, "2022-11-21", new airwinconsolidated::Channel7::Channel7(samplingRate) });
        this->RegisterAirwindow({ "Channel8", "Tone Color", 411, true, "Channel8 is Channel7 with updates from Capacitor2 and Slew3. More and better of what Channel is.", airwinconsolidated::Channel8::kNumParameters, "2022-11-21", new airwinconsolidated::Channel8::Channel8(samplingRate) });
        this->RegisterAirwindow({ "Channel9", "Tone Color", 402, true, "Channel9 is Channel8 with ultrasonic filtering, and new Teac and Mackie 'low end' settings.", airwinconsolidated::Channel9::kNumParameters, "2022-11-21", new airwinconsolidated::Channel9::Channel9(samplingRate) });
        this->RegisterAirwindow({ "ChimeyDeluxe", "Amp Sims", 25, true, "ChimeyDeluxe is a very flexible compressed DI conditioner.", airwinconsolidated::ChimeyDeluxe::kNumParameters, "2025-11-07", new airwinconsolidated::ChimeyDeluxe::ChimeyDeluxe(samplingRate) });
        this->RegisterAirwindow({ "ChimeyGuitar", "Amp Sims", 29, true, "ChimeyGuitar is a supremely compressible instrument amp.", airwinconsolidated::ChimeyGuitar::kNumParameters, "2025-05-10", new airwinconsolidated::ChimeyGuitar::ChimeyGuitar(samplingRate) });
        this->RegisterAirwindow({ "ChimeyGuitar2", "Unclassified", -1, true, "", airwinconsolidated::ChimeyGuitar2::kNumParameters, "2025-07-27", new airwinconsolidated::ChimeyGuitar2::ChimeyGuitar2(samplingRate) });
        this->RegisterAirwindow({ "Chorus", "Ambience", 22, true, "Chorus is a mono chorus, also works as a vibrato.", airwinconsolidated::Chorus::kNumParameters, "2022-11-21", new airwinconsolidated::Chorus::Chorus(samplingRate) });
        this->RegisterAirwindow({ "ChorusEnsemble", "Ambience", 23, true, "ChorusEnsemble is a more complex, multi-tap mono chorus.", airwinconsolidated::ChorusEnsemble::kNumParameters, "2022-11-21", new airwinconsolidated::ChorusEnsemble::ChorusEnsemble(samplingRate) });
        this->RegisterAirwindow({ "ChromeOxide", "Lo-Fi", 291, true, "ChromeOxide is an alternate path to vibey old tape sonics.", airwinconsolidated::ChromeOxide::kNumParameters, "2022-11-21", new airwinconsolidated::ChromeOxide::ChromeOxide(samplingRate) });
        this->RegisterAirwindow({ "Cider", "Tone Color", 407, true, "Cider is a re-release of another old Character plugin.", airwinconsolidated::Cider::kNumParameters, "2022-11-21", new airwinconsolidated::Cider::Cider(samplingRate) });
        this->RegisterAirwindow({ "ClearCoat", "Ambience", 0, false, "ClearCoat is an array of bright ambience reverbs.", airwinconsolidated::ClearCoat::kNumParameters, "2023-11-06", new airwinconsolidated::ClearCoat::ClearCoat(samplingRate) });
        this->RegisterAirwindow({ "ClipOnly2", "Clipping", 85, true, "ClipOnly2 suppresses the brightness of digital clipping without affecting unclipped samples, at any sample rate.", airwinconsolidated::ClipOnly2::kNumParameters, "2022-11-21", new airwinconsolidated::ClipOnly2::ClipOnly2(samplingRate) });
        this->RegisterAirwindow({ "ClipSoftly", "Clipping", 87, true, "ClipSoftly is ClipOnly2 but as a softclipper: a very handy building block.", airwinconsolidated::ClipSoftly::kNumParameters, "2022-11-21", new airwinconsolidated::ClipSoftly::ClipSoftly(samplingRate) });
        this->RegisterAirwindow({ "CloudCoat", "Effects", 212, false, "CloudCoat is an array of blur effects with a taste for evil.", airwinconsolidated::CloudCoat::kNumParameters, "2024-02-03", new airwinconsolidated::CloudCoat::CloudCoat(samplingRate) });
        this->RegisterAirwindow({ "Coils", "Subtlety", 386, true, "Coils models the types of distortion you'll find in transformers.", airwinconsolidated::Coils::kNumParameters, "2022-11-21", new airwinconsolidated::Coils::Coils(samplingRate) });
        this->RegisterAirwindow({ "Coils2", "Subtlety", 381, true, "Coils2 is a transformer overdrive emulator.", airwinconsolidated::Coils2::kNumParameters, "2022-11-21", new airwinconsolidated::Coils2::Coils2(samplingRate) });
        this->RegisterAirwindow({ "Cojones", "Lo-Fi", 292, true, "Cojones is a new kind of distorty.", airwinconsolidated::Cojones::kNumParameters, "2022-11-21", new airwinconsolidated::Cojones::Cojones(samplingRate) });
        this->RegisterAirwindow({ "Compresaturator", "Dynamics", 194, true, "Compresaturator fades between compressing and soft clipping.", airwinconsolidated::Compresaturator::kNumParameters, "2022-11-21", new airwinconsolidated::Compresaturator::Compresaturator(samplingRate) });
        this->RegisterAirwindow({ "Console0Buss", "Consoles", 113, false, "Console0Buss is a radical, minimalist Console system.", airwinconsolidated::Console0Buss::kNumParameters, "2023-05-13", new airwinconsolidated::Console0Buss::Console0Buss(samplingRate) });
        this->RegisterAirwindow({ "Console0Channel", "Consoles", 112, false, "Console0Channel is a radical, minimalist Console system.", airwinconsolidated::Console0Channel::kNumParameters, "2023-05-13", new airwinconsolidated::Console0Channel::Console0Channel(samplingRate) });
        this->RegisterAirwindow({ "Console4Buss", "Consoles", 135, true, "Console4Buss is the original Airwindows ITB mixing system, a two-plugin mix buss replacement for DAWs.", airwinconsolidated::Console4Buss::kNumParameters, "2022-11-21", new airwinconsolidated::Console4Buss::Console4Buss(samplingRate) });
        this->RegisterAirwindow({ "Console4Channel", "Consoles", 136, true, "Console4Channel is the original Airwindows ITB mixing system, a two-plugin mix buss replacement for DAWs.", airwinconsolidated::Console4Channel::kNumParameters, "2022-11-21", new airwinconsolidated::Console4Channel::Console4Channel(samplingRate) });
        this->RegisterAirwindow({ "Console5Buss", "Consoles", 128, true, "Console5Buss is a richer, warmer Console system.", airwinconsolidated::Console5Buss::kNumParameters, "2022-11-21", new airwinconsolidated::Console5Buss::Console5Buss(samplingRate) });
        this->RegisterAirwindow({ "Console5Channel", "Consoles", 129, true, "Console5Channel is a richer, warmer Console system.", airwinconsolidated::Console5Channel::kNumParameters, "2022-11-21", new airwinconsolidated::Console5Channel::Console5Channel(samplingRate) });
        this->RegisterAirwindow({ "Console5DarkCh", "Consoles", 130, true, "Console5DarkCh is a richer, warmer Console system.", airwinconsolidated::Console5DarkCh::kNumParameters, "2022-11-21", new airwinconsolidated::Console5DarkCh::Console5DarkCh(samplingRate) });
        this->RegisterAirwindow({ "Console6Buss", "Consoles", 124, true, "Console6Buss is a new more intense Console that peaks at lower dB.", airwinconsolidated::Console6Buss::kNumParameters, "2024-03-30", new airwinconsolidated::Console6Buss::Console6Buss(samplingRate) });
        this->RegisterAirwindow({ "Console6Channel", "Consoles", 125, true, "Console6Channel is a new more intense Console that peaks at lower dB.", airwinconsolidated::Console6Channel::kNumParameters, "2024-03-30", new airwinconsolidated::Console6Channel::Console6Channel(samplingRate) });
        this->RegisterAirwindow({ "Console7Buss", "Consoles", 118, true, "Console7Buss adds anti-alias filtering and special saturation curves.", airwinconsolidated::Console7Buss::kNumParameters, "2022-11-21", new airwinconsolidated::Console7Buss::Console7Buss(samplingRate) });
        this->RegisterAirwindow({ "Console7Cascade", "Consoles", 120, true, "Console7Cascade is a drop-in replacement for Console7Channel that allows for MUCH higher gain.", airwinconsolidated::Console7Cascade::kNumParameters, "2022-11-21", new airwinconsolidated::Console7Cascade::Console7Cascade(samplingRate) });
        this->RegisterAirwindow({ "Console7Channel", "Consoles", 119, true, "Console7Channel adds anti-alias filtering and special saturation curves.", airwinconsolidated::Console7Channel::kNumParameters, "2022-11-21", new airwinconsolidated::Console7Channel::Console7Channel(samplingRate) });
        this->RegisterAirwindow({ "Console7Crunch", "Consoles", 121, true, "Console7Crunch shows you Cascade but with its EQ weighted for more edge and grit.", airwinconsolidated::Console7Crunch::kNumParameters, "2022-11-21", new airwinconsolidated::Console7Crunch::Console7Crunch(samplingRate) });
        this->RegisterAirwindow({ "Console8BussHype", "Consoles", 103, true, "Console8BussHype swaps out just the In section of Console8BussIn, for a brighter sparklier circuit.", airwinconsolidated::Console8BussHype::kNumParameters, "2022-11-21", new airwinconsolidated::Console8BussHype::Console8BussHype(samplingRate) });
        this->RegisterAirwindow({ "Console8BussIn", "Consoles", 104, true, "Console8BussIn moves to a channel/submix/buss topology and adds analog simulation.", airwinconsolidated::Console8BussIn::kNumParameters, "2022-11-21", new airwinconsolidated::Console8BussIn::Console8BussIn(samplingRate) });
        this->RegisterAirwindow({ "Console8BussOut", "Consoles", 105, true, "Console8BussOut moves to a channel/submix/buss topology and adds analog simulation.", airwinconsolidated::Console8BussOut::kNumParameters, "2022-11-21", new airwinconsolidated::Console8BussOut::Console8BussOut(samplingRate) });
        this->RegisterAirwindow({ "Console8ChannelHype", "Consoles", 106, true, "Console8ChannelHype swaps out just the In section of Console8ChannelIn, for a brighter sparklier circuit.", airwinconsolidated::Console8ChannelHype::kNumParameters, "2022-11-21", new airwinconsolidated::Console8ChannelHype::Console8ChannelHype(samplingRate) });
        this->RegisterAirwindow({ "Console8ChannelIn", "Consoles", 107, true, "Console8ChannelIn moves to a channel/submix/buss topology and adds analog simulation.", airwinconsolidated::Console8ChannelIn::kNumParameters, "2022-11-21", new airwinconsolidated::Console8ChannelIn::Console8ChannelIn(samplingRate) });
        this->RegisterAirwindow({ "Console8ChannelOut", "Consoles", 108, true, "Console8ChannelOut moves to a channel/submix/buss topology and adds analog simulation.", airwinconsolidated::Console8ChannelOut::kNumParameters, "2022-11-21", new airwinconsolidated::Console8ChannelOut::Console8ChannelOut(samplingRate) });
        this->RegisterAirwindow({ "Console8LiteBuss", "Consoles", 102, true, "Console8LiteBuss is simplified Console8, working with just a single mix buss.", airwinconsolidated::Console8LiteBuss::kNumParameters, "2023-02-10", new airwinconsolidated::Console8LiteBuss::Console8LiteBuss(samplingRate) });
        this->RegisterAirwindow({ "Console8LiteChannel", "Consoles", 101, true, "Console8LiteChannel is simplified Console8, working with just a single mix buss.", airwinconsolidated::Console8LiteChannel::kNumParameters, "2023-02-10", new airwinconsolidated::Console8LiteChannel::Console8LiteChannel(samplingRate) });
        this->RegisterAirwindow({ "Console8SubHype", "Consoles", 109, true, "Console8SubHype swaps out just the In section of Console8SubIn, for a brighter sparklier circuit.", airwinconsolidated::Console8SubHype::kNumParameters, "2022-11-21", new airwinconsolidated::Console8SubHype::Console8SubHype(samplingRate) });
        this->RegisterAirwindow({ "Console8SubIn", "Consoles", 110, true, "Console8SubIn moves to a channel/submix/buss topology and adds analog simulation.", airwinconsolidated::Console8SubIn::kNumParameters, "2022-11-21", new airwinconsolidated::Console8SubIn::Console8SubIn(samplingRate) });
        this->RegisterAirwindow({ "Console8SubOut", "Consoles", 111, true, "Console8SubOut moves to a channel/submix/buss topology and adds analog simulation.", airwinconsolidated::Console8SubOut::kNumParameters, "2022-11-21", new airwinconsolidated::Console8SubOut::Console8SubOut(samplingRate) });
        this->RegisterAirwindow({ "Console9Buss", "Consoles", 93, false, "Console9Buss is just the summing from ConsoleX.", airwinconsolidated::Console9Buss::kNumParameters, "2024-03-30", new airwinconsolidated::Console9Buss::Console9Buss(samplingRate) });
        this->RegisterAirwindow({ "Console9Channel", "Consoles", 94, false, "Console9Channel is just the summing from ConsoleX.", airwinconsolidated::Console9Channel::kNumParameters, "2024-05-19", new airwinconsolidated::Console9Channel::Console9Channel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleHBuss", "Unclassified", -1, false, "", airwinconsolidated::ConsoleHBuss::kNumParameters, "2025-12-20", new airwinconsolidated::ConsoleHBuss::ConsoleHBuss(samplingRate) });
        this->RegisterAirwindow({ "ConsoleHChannel", "Unclassified", -1, false, "", airwinconsolidated::ConsoleHChannel::kNumParameters, "2026-01-18", new airwinconsolidated::ConsoleHChannel::ConsoleHChannel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleHPre", "Unclassified", -1, true, "", airwinconsolidated::ConsoleHPre::kNumParameters, "2026-01-18", new airwinconsolidated::ConsoleHPre::ConsoleHPre(samplingRate) });
        this->RegisterAirwindow({ "ConsoleLABuss", "Consoles", 95, false, "ConsoleLABuss is the Airwindows take on the Quad Eight console.", airwinconsolidated::ConsoleLABuss::kNumParameters, "2025-03-17", new airwinconsolidated::ConsoleLABuss::ConsoleLABuss(samplingRate) });
        this->RegisterAirwindow({ "ConsoleLAChannel", "Consoles", 96, false, "ConsoleLAChannel is the Airwindows take on the Quad Eight console.", airwinconsolidated::ConsoleLAChannel::kNumParameters, "2023-12-16", new airwinconsolidated::ConsoleLAChannel::ConsoleLAChannel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleMCBuss", "Consoles", 97, false, "ConsoleMCBuss is the initial, bright take on the MCI console.", airwinconsolidated::ConsoleMCBuss::kNumParameters, "2023-11-26", new airwinconsolidated::ConsoleMCBuss::ConsoleMCBuss(samplingRate) });
        this->RegisterAirwindow({ "ConsoleMCChannel", "Consoles", 98, false, "ConsoleMCChannel is the initial, bright take on the MCI console.", airwinconsolidated::ConsoleMCChannel::kNumParameters, "2023-11-26", new airwinconsolidated::ConsoleMCChannel::ConsoleMCChannel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleMDBuss", "Consoles", 99, false, "ConsoleMDBuss goes for the tone shaping of the classic MCI console!", airwinconsolidated::ConsoleMDBuss::kNumParameters, "2023-10-30", new airwinconsolidated::ConsoleMDBuss::ConsoleMDBuss(samplingRate) });
        this->RegisterAirwindow({ "ConsoleMDChannel", "Consoles", 100, false, "ConsoleMDChannel goes for the tone shaping of the classic MCI console!", airwinconsolidated::ConsoleMDChannel::kNumParameters, "2023-10-30", new airwinconsolidated::ConsoleMDChannel::ConsoleMDChannel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleX2Buss", "Unclassified", -1, false, "", airwinconsolidated::ConsoleX2Buss::kNumParameters, "2025-12-13", new airwinconsolidated::ConsoleX2Buss::ConsoleX2Buss(samplingRate) });
        this->RegisterAirwindow({ "ConsoleX2Channel", "Unclassified", -1, false, "", airwinconsolidated::ConsoleX2Channel::kNumParameters, "2026-01-18", new airwinconsolidated::ConsoleX2Channel::ConsoleX2Channel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleX2Pre", "Unclassified", -1, true, "", airwinconsolidated::ConsoleX2Pre::kNumParameters, "2026-01-18", new airwinconsolidated::ConsoleX2Pre::ConsoleX2Pre(samplingRate) });
        this->RegisterAirwindow({ "ConsoleXBuss", "Unclassified", -1, false, "", airwinconsolidated::ConsoleXBuss::kNumParameters, "2024-05-25", new airwinconsolidated::ConsoleXBuss::ConsoleXBuss(samplingRate) });
        this->RegisterAirwindow({ "ConsoleXChannel", "Unclassified", -1, false, "", airwinconsolidated::ConsoleXChannel::kNumParameters, "2024-05-25", new airwinconsolidated::ConsoleXChannel::ConsoleXChannel(samplingRate) });
        this->RegisterAirwindow({ "ConsoleXPre", "Unclassified", -1, false, "", airwinconsolidated::ConsoleXPre::kNumParameters, "2024-09-22", new airwinconsolidated::ConsoleXPre::ConsoleXPre(samplingRate) });
        this->RegisterAirwindow({ "ContentHideD", "Utility", 450, false, "ContentHideD mutes audio except for brief glimpses.", airwinconsolidated::ContentHideD::kNumParameters, "2024-03-30", new airwinconsolidated::ContentHideD::ContentHideD(samplingRate) });
        this->RegisterAirwindow({ "CreamCoat", "Reverb", 321, false, "CreamCoat is a swiss army knife reverb with soft undersampling.", airwinconsolidated::CreamCoat::kNumParameters, "2024-06-23", new airwinconsolidated::CreamCoat::CreamCoat(samplingRate) });
        this->RegisterAirwindow({ "Creature", "Saturation", 345, true, "Creature is a soft slew saturator, a new class of noisechanger.", airwinconsolidated::Creature::kNumParameters, "2023-08-19", new airwinconsolidated::Creature::Creature(samplingRate) });
        this->RegisterAirwindow({ "CrickBass", "Amp Sims", 33, false, "CrickBass is a flexible but aggressive bass tone for dual pickups.", airwinconsolidated::CrickBass::kNumParameters, "2023-09-17", new airwinconsolidated::CrickBass::CrickBass(samplingRate) });
        this->RegisterAirwindow({ "CrunchCoat", "Reverb", 327, false, "CrunchCoat is a cursed retro digital reverb!", airwinconsolidated::CrunchCoat::kNumParameters, "2024-05-25", new airwinconsolidated::CrunchCoat::CrunchCoat(samplingRate) });
        this->RegisterAirwindow({ "CrunchyGrooveWear", "Lo-Fi", 283, true, "CrunchyGrooveWear is a version of GrooveWear for more edge and distortion.", airwinconsolidated::CrunchyGrooveWear::kNumParameters, "2022-11-21", new airwinconsolidated::CrunchyGrooveWear::CrunchyGrooveWear(samplingRate) });
        this->RegisterAirwindow({ "Crystal", "Tone Color", 408, true, "Crystal is a tone shaper and buss soft-clipper.", airwinconsolidated::Crystal::kNumParameters, "2022-11-21", new airwinconsolidated::Crystal::Crystal(samplingRate) });
        this->RegisterAirwindow({ "DCVoltage", "Utility", 436, true, "DCVoltage is literally a DC control voltage, in a plugin.", airwinconsolidated::DCVoltage::kNumParameters, "2022-11-21", new airwinconsolidated::DCVoltage::DCVoltage(samplingRate) });
        this->RegisterAirwindow({ "Dark", "Dithers", 151, true, "Dark is a wordlength reducer that gives your music a blacker backdrop.", airwinconsolidated::Dark::kNumParameters, "2022-11-21", new airwinconsolidated::Dark::Dark(samplingRate) });
        this->RegisterAirwindow({ "DarkNoise", "Noise", 302, true, "DarkNoise is an alternative method for producing bassier noise directly. Sound design stuff.", airwinconsolidated::DarkNoise::kNumParameters, "2022-11-21", new airwinconsolidated::DarkNoise::DarkNoise(samplingRate) });
        this->RegisterAirwindow({ "DeBess", "Brightness", 63, true, "DeBess is an improved DeEss, with perfect rejection of non-ess audio.", airwinconsolidated::DeBess::kNumParameters, "2022-11-21", new airwinconsolidated::DeBess::DeBess(samplingRate) });
        this->RegisterAirwindow({ "DeBez", "Lo-Fi", 278, true, "DeBez gives you retro sampley textures!", airwinconsolidated::DeBez::kNumParameters, "2025-08-09", new airwinconsolidated::DeBez::DeBez(samplingRate) });
        this->RegisterAirwindow({ "DeCrackle", "Utility", 449, false, "DeCrackle isolates clicks and vinyl crackles to remove them.", airwinconsolidated::DeCrackle::kNumParameters, "2025-03-30", new airwinconsolidated::DeCrackle::DeCrackle(samplingRate) });
        this->RegisterAirwindow({ "DeEss", "Brightness", 68, true, "DeEss is the best de-essing solution there is. A go-to utility plugin.", airwinconsolidated::DeEss::kNumParameters, "2022-11-21", new airwinconsolidated::DeEss::DeEss(samplingRate) });
        this->RegisterAirwindow({ "DeHiss", "Brightness", 78, true, "DeHiss tries to suppress background hiss, like a hiss gate.", airwinconsolidated::DeHiss::kNumParameters, "2022-11-21", new airwinconsolidated::DeHiss::DeHiss(samplingRate) });
        this->RegisterAirwindow({ "DeNoise", "Noise", 297, true, "DeNoise can work as a multiband gate and as a wild effect.", airwinconsolidated::DeNoise::kNumParameters, "2025-10-12", new airwinconsolidated::DeNoise::DeNoise(samplingRate) });
        this->RegisterAirwindow({ "DeRez", "Lo-Fi", 289, true, "DeRez is an analog-style bit and sample rate crusher with continuous adjustments.", airwinconsolidated::DeRez::kNumParameters, "2022-11-21", new airwinconsolidated::DeRez::DeRez(samplingRate) });
        this->RegisterAirwindow({ "DeRez2", "Lo-Fi", 287, true, "DeRez2 brings more authentic retro-digital hardware tones.", airwinconsolidated::DeRez2::kNumParameters, "2022-11-21", new airwinconsolidated::DeRez2::DeRez2(samplingRate) });
        this->RegisterAirwindow({ "DeRez3", "Lo-Fi", 281, true, "DeRez3 reinvents retro digital tones.", airwinconsolidated::DeRez3::kNumParameters, "2024-06-23", new airwinconsolidated::DeRez3::DeRez3(samplingRate) });
        this->RegisterAirwindow({ "Deckwrecka", "Lo-Fi", 295, true, "Deckwrecka fattens and dirties up beats.", airwinconsolidated::Deckwrecka::kNumParameters, "2022-11-21", new airwinconsolidated::Deckwrecka::Deckwrecka(samplingRate) });
        this->RegisterAirwindow({ "Density", "Distortion", 145, true, "Density is a Swiss Army Knife of saturation/antisaturation.", airwinconsolidated::Density::kNumParameters, "2022-11-21", new airwinconsolidated::Density::Density(samplingRate) });
        this->RegisterAirwindow({ "Density2", "Distortion", 143, true, "Density2 is a different color for Density, some old code I had that people wanted.", airwinconsolidated::Density2::kNumParameters, "2022-11-21", new airwinconsolidated::Density2::Density2(samplingRate) });
        this->RegisterAirwindow({ "Density3", "Unclassified", -1, true, "", airwinconsolidated::Density3::kNumParameters, "2025-10-24", new airwinconsolidated::Density3::Density3(samplingRate) });
        this->RegisterAirwindow({ "Desk", "Subtlety", 387, true, "Desk is classic Airwindows subtle analog modeling.", airwinconsolidated::Desk::kNumParameters, "2022-11-21", new airwinconsolidated::Desk::Desk(samplingRate) });
        this->RegisterAirwindow({ "Desk4", "Saturation", 357, true, "Desk4 is distinctive analog coloration (a tuneable version of the control-less Desk plugins)", airwinconsolidated::Desk4::kNumParameters, "2022-11-21", new airwinconsolidated::Desk4::Desk4(samplingRate) });
        this->RegisterAirwindow({ "DigitalBlack", "Dynamics", 183, true, "DigitalBlack is a quick, staccato gate.", airwinconsolidated::DigitalBlack::kNumParameters, "2022-11-21", new airwinconsolidated::DigitalBlack::DigitalBlack(samplingRate) });
        this->RegisterAirwindow({ "Dirt", "Distortion", 140, true, "Dirt is a soft-clip distortion in the spirit of Edge.", airwinconsolidated::Dirt::kNumParameters, "2022-11-21", new airwinconsolidated::Dirt::Dirt(samplingRate) });
        this->RegisterAirwindow({ "Discontapeity", "Subtlety", 372, true, "Discontapeity combines air modeling with tape-style overdrive.", airwinconsolidated::Discontapeity::kNumParameters, "2025-09-14", new airwinconsolidated::Discontapeity::Discontapeity(samplingRate) });
        this->RegisterAirwindow({ "Discontinuity", "Subtlety", 374, true, "Discontinuity models air under intense loudness.", airwinconsolidated::Discontinuity::kNumParameters, "2024-03-10", new airwinconsolidated::Discontinuity::Discontinuity(samplingRate) });
        this->RegisterAirwindow({ "Disintegrate", "Effects", 213, true, "Disintegrate is Discontinuity on steroids.", airwinconsolidated::Disintegrate::kNumParameters, "2025-01-26", new airwinconsolidated::Disintegrate::Disintegrate(samplingRate) });
        this->RegisterAirwindow({ "Distance", "Filter", 274, true, "Distance is a sound design or reverb far-away-izer.", airwinconsolidated::Distance::kNumParameters, "2022-11-21", new airwinconsolidated::Distance::Distance(samplingRate) });
        this->RegisterAirwindow({ "Distance2", "Filter", 275, true, "Distance2 is a versatile space shaper for creating depth.", airwinconsolidated::Distance2::kNumParameters, "2022-11-21", new airwinconsolidated::Distance2::Distance2(samplingRate) });
        this->RegisterAirwindow({ "Distance3", "Filter", 249, true, "Distance3 combines the best parts of Distance and Discontinuity.", airwinconsolidated::Distance3::kNumParameters, "2024-05-10", new airwinconsolidated::Distance3::Distance3(samplingRate) });
        this->RegisterAirwindow({ "Distortion", "Distortion", 138, true, "Distortion is a slightly dark analog-style distortion with several presets, like Focus.", airwinconsolidated::Distortion::kNumParameters, "2022-11-21", new airwinconsolidated::Distortion::Distortion(samplingRate) });
        this->RegisterAirwindow({ "DitherFloat", "Dithers", 173, true, "DitherFloat is a demo of floating point truncation: can also dither to 32 bit float output.", airwinconsolidated::DitherFloat::kNumParameters, "2022-11-21", new airwinconsolidated::DitherFloat::DitherFloat(samplingRate) });
        this->RegisterAirwindow({ "DitherMeDiskers", "Dithers", 168, true, "DitherMeDiskers is dark deep 'dither' in a 16 bit version.", airwinconsolidated::DitherMeDiskers::kNumParameters, "2022-11-21", new airwinconsolidated::DitherMeDiskers::DitherMeDiskers(samplingRate) });
        this->RegisterAirwindow({ "DitherMeTimbers", "Dithers", 167, true, "DitherMeTimbers is dark deep 'dither' in a 24 bit version.", airwinconsolidated::DitherMeTimbers::kNumParameters, "2022-11-21", new airwinconsolidated::DitherMeTimbers::DitherMeTimbers(samplingRate) });
        this->RegisterAirwindow({ "Ditherbox", "Dithers", 163, true, "Ditherbox is a switchable selection of dithers in 16 and 24 bit, plus monitoring tools.", airwinconsolidated::Ditherbox::kNumParameters, "2023-04-12", new airwinconsolidated::Ditherbox::Ditherbox(samplingRate) });
        this->RegisterAirwindow({ "DoublePaul", "Dithers", 162, true, "DoublePaul is like PaulDither but more so.", airwinconsolidated::DoublePaul::kNumParameters, "2022-11-21", new airwinconsolidated::DoublePaul::DoublePaul(samplingRate) });
        this->RegisterAirwindow({ "Doublelay", "Ambience", 2, false, "Doublelay is StereoDoubler with independent delays and feedback.", airwinconsolidated::Doublelay::kNumParameters, "2022-11-21", new airwinconsolidated::Doublelay::Doublelay(samplingRate) });
        this->RegisterAirwindow({ "Drive", "Distortion", 146, true, "Drive is the angry distortion!", airwinconsolidated::Drive::kNumParameters, "2022-11-21", new airwinconsolidated::Drive::Drive(samplingRate) });
        this->RegisterAirwindow({ "DrumSlam", "Dynamics", 195, true, "DrumSlam is a heavy-processing tape modeler.", airwinconsolidated::DrumSlam::kNumParameters, "2022-11-21", new airwinconsolidated::DrumSlam::DrumSlam(samplingRate) });
        this->RegisterAirwindow({ "DubCenter", "Bass", 46, false, "DubCenter is a version of DubSub where the bass reinforcement is purely mono.", airwinconsolidated::DubCenter::kNumParameters, "2022-11-21", new airwinconsolidated::DubCenter::DubCenter(samplingRate) });
        this->RegisterAirwindow({ "DubPlate", "Utility", 429, false, "DubPlate is ruthless dubplate mastering for electronic music.", airwinconsolidated::DubPlate::kNumParameters, "2025-03-17", new airwinconsolidated::DubPlate::DubPlate(samplingRate) });
        this->RegisterAirwindow({ "DubPlate2", "Utility", 428, false, "DubPlate2 is like an ITB mastering house for electronic music.", airwinconsolidated::DubPlate2::kNumParameters, "2025-03-30", new airwinconsolidated::DubPlate2::DubPlate2(samplingRate) });
        this->RegisterAirwindow({ "DubSub", "Bass", 47, true, "DubSub is a fully featured bass doctor.", airwinconsolidated::DubSub::kNumParameters, "2022-11-21", new airwinconsolidated::DubSub::DubSub(samplingRate) });
        this->RegisterAirwindow({ "DubSub2", "Bass", 41, false, "DubSub2 is the essence of the Airwindows head bump.", airwinconsolidated::DubSub2::kNumParameters, "2024-08-04", new airwinconsolidated::DubSub2::DubSub2(samplingRate) });
        this->RegisterAirwindow({ "Dubly", "Effects", 215, true, "Dubly is a retro vibe and atmosphere maker.", airwinconsolidated::Dubly::kNumParameters, "2022-11-21", new airwinconsolidated::Dubly::Dubly(samplingRate) });
        this->RegisterAirwindow({ "Dubly2", "Effects", 210, true, "Dubly2 is a key part of seventies sonics!", airwinconsolidated::Dubly2::kNumParameters, "2024-07-28", new airwinconsolidated::Dubly2::Dubly2(samplingRate) });
        this->RegisterAirwindow({ "Dubly3", "Effects", 209, true, "Dubly3 refines and transforms the Dubly sound.", airwinconsolidated::Dubly3::kNumParameters, "2024-09-22", new airwinconsolidated::Dubly3::Dubly3(samplingRate) });
        this->RegisterAirwindow({ "DustBunny", "Lo-Fi", 296, true, "DustBunny is unpredictable distorty noise, like dust wedged under your turntable needle.", airwinconsolidated::DustBunny::kNumParameters, "2022-11-21", new airwinconsolidated::DustBunny::DustBunny(samplingRate) });
        this->RegisterAirwindow({ "Dynamics", "Dynamics", 181, false, "Dynamics is a combination compressor gate.", airwinconsolidated::Dynamics::kNumParameters, "2022-11-21", new airwinconsolidated::Dynamics::Dynamics(samplingRate) });
        this->RegisterAirwindow({ "Dynamics2", "Dynamics", 174, false, "Dynamics2 is the compressor gate for new Console plugins!", airwinconsolidated::Dynamics2::kNumParameters, "2025-10-04", new airwinconsolidated::Dynamics2::Dynamics2(samplingRate) });
        this->RegisterAirwindow({ "Dynamics3", "Unclassified", -1, false, "", airwinconsolidated::Dynamics3::kNumParameters, "2025-12-13", new airwinconsolidated::Dynamics3::Dynamics3(samplingRate) });
        this->RegisterAirwindow({ "Dyno", "Saturation", 354, true, "Dyno is a lot like Mojo, but for intensity instead of loudness.", airwinconsolidated::Dyno::kNumParameters, "2022-11-21", new airwinconsolidated::Dyno::Dyno(samplingRate) });
        this->RegisterAirwindow({ "EQ", "Filter", 267, true, "EQ is just the EQ parts of CStrip.", airwinconsolidated::EQ::kNumParameters, "2022-11-21", new airwinconsolidated::EQ::EQ(samplingRate) });
        this->RegisterAirwindow({ "EdIsDim", "Utility", 439, false, "EdIsDim is mid/side conversion utility plugins.", airwinconsolidated::EdIsDim::kNumParameters, "2022-11-21", new airwinconsolidated::EdIsDim::EdIsDim(samplingRate) });
        this->RegisterAirwindow({ "Edge", "Distortion", 139, true, "Edge is a seven-stage distortion with Hypersonic filtering and tone shaping controls.", airwinconsolidated::Edge::kNumParameters, "2022-11-21", new airwinconsolidated::Edge::Edge(samplingRate) });
        this->RegisterAirwindow({ "Elation", "Tone Color", 405, true, "Elation is a re-release of another old Character plugin.", airwinconsolidated::Elation::kNumParameters, "2022-11-21", new airwinconsolidated::Elation::Elation(samplingRate) });
        this->RegisterAirwindow({ "ElectroHat", "Noise", 303, true, "ElectroHat is a hi-hat tone generator that uses the original sound as a control voltage.", airwinconsolidated::ElectroHat::kNumParameters, "2022-11-21", new airwinconsolidated::ElectroHat::ElectroHat(samplingRate) });
        this->RegisterAirwindow({ "Elliptical", "Bass", 43, false, "Elliptical highpasses the side channel.", airwinconsolidated::Elliptical::kNumParameters, "2025-02-16", new airwinconsolidated::Elliptical::Elliptical(samplingRate) });
        this->RegisterAirwindow({ "Energy", "Effects", 228, true, "Energy is electrifying fixed-frequency treble boosts.", airwinconsolidated::Energy::kNumParameters, "2022-11-21", new airwinconsolidated::Energy::Energy(samplingRate) });
        this->RegisterAirwindow({ "Energy2", "Effects", 227, true, "Energy2 is electrifying fixed-frequency treble boosts for high sample rate.", airwinconsolidated::Energy2::kNumParameters, "2022-11-21", new airwinconsolidated::Energy2::Energy2(samplingRate) });
        this->RegisterAirwindow({ "Ensemble", "Ambience", 24, true, "Ensemble is a weird flangey little modulation effect.", airwinconsolidated::Ensemble::kNumParameters, "2022-11-21", new airwinconsolidated::Ensemble::Ensemble(samplingRate) });
        this->RegisterAirwindow({ "EveryConsole", "Consoles", 137, true, "EveryConsole is mix-and-match Airwindows Console.", airwinconsolidated::EveryConsole::kNumParameters, "2023-07-29", new airwinconsolidated::EveryConsole::EveryConsole(samplingRate) });
        this->RegisterAirwindow({ "EverySlew", "Brightness", 70, true, "EverySlew is a wide variety of unique sound processing tools.", airwinconsolidated::EverySlew::kNumParameters, "2023-07-22", new airwinconsolidated::EverySlew::EverySlew(samplingRate) });
        this->RegisterAirwindow({ "EveryTrim", "Utility", 426, false, "EveryTrim is Left/Right, Mid/Side, and Master in one plugin.", airwinconsolidated::EveryTrim::kNumParameters, "2022-11-21", new airwinconsolidated::EveryTrim::EveryTrim(samplingRate) });
        this->RegisterAirwindow({ "Exciter", "Effects", 226, true, "Exciter is an Aural Exciter plugin that can be both subtle and extreme.", airwinconsolidated::Exciter::kNumParameters, "2022-11-21", new airwinconsolidated::Exciter::Exciter(samplingRate) });
        this->RegisterAirwindow({ "Facet", "Effects", 229, true, "Facet is a new kind of hard clip that adds two sharp corners into the wave.", airwinconsolidated::Facet::kNumParameters, "2022-11-21", new airwinconsolidated::Facet::Facet(samplingRate) });
        this->RegisterAirwindow({ "FatEQ", "Unclassified", -1, true, "", airwinconsolidated::FatEQ::kNumParameters, "2025-10-12", new airwinconsolidated::FatEQ::FatEQ(samplingRate) });
        this->RegisterAirwindow({ "FathomFive", "Bass", 50, true, "FathomFive is a way of supplementing extreme bass that’s not just EQ.", airwinconsolidated::FathomFive::kNumParameters, "2022-11-21", new airwinconsolidated::FathomFive::FathomFive(samplingRate) });
        this->RegisterAirwindow({ "FinalClip", "Clipping", 92, true, "FinalClip is one stage of ADClip8 set up to clip +6dB for Final Cut Pro.", airwinconsolidated::FinalClip::kNumParameters, "2024-02-10", new airwinconsolidated::FinalClip::FinalClip(samplingRate) });
        this->RegisterAirwindow({ "FireAmp", "Amp Sims", 31, true, "FireAmp is a bright, loud, tubey amp sim for leads and dirt guitar.", airwinconsolidated::FireAmp::kNumParameters, "2022-11-21", new airwinconsolidated::FireAmp::FireAmp(samplingRate) });
        this->RegisterAirwindow({ "Flipity", "Utility", 446, false, "Flipity is a channel phase flipper/swapper utility.", airwinconsolidated::Flipity::kNumParameters, "2022-11-21", new airwinconsolidated::Flipity::Flipity(samplingRate) });
        this->RegisterAirwindow({ "Floor", "Bass", 48, true, "Floor is fake bottom octave for fun and profit!", airwinconsolidated::Floor::kNumParameters, "2022-11-21", new airwinconsolidated::Floor::Floor(samplingRate) });
        this->RegisterAirwindow({ "Flutter", "Lo-Fi", 286, false, "Flutter is the flutter from ToTape6, standalone.", airwinconsolidated::Flutter::kNumParameters, "2022-11-21", new airwinconsolidated::Flutter::Flutter(samplingRate) });
        this->RegisterAirwindow({ "Flutter2", "Lo-Fi", 280, true, "Flutter2 is the flutter from ToTape7, standalone.", airwinconsolidated::Flutter2::kNumParameters, "2024-07-21", new airwinconsolidated::Flutter2::Flutter2(samplingRate) });
        this->RegisterAirwindow({ "Focus", "Saturation", 352, true, "Focus brings out clarity by distorting. Aggressive, subtle, flexible.", airwinconsolidated::Focus::kNumParameters, "2022-11-21", new airwinconsolidated::Focus::Focus(samplingRate) });
        this->RegisterAirwindow({ "Fracture", "Effects", 230, true, "Fracture is a frequency multiplier/waveshaper with a soft disruption to the sound.", airwinconsolidated::Fracture::kNumParameters, "2022-11-21", new airwinconsolidated::Fracture::Fracture(samplingRate) });
        this->RegisterAirwindow({ "Fracture2", "Effects", 214, true, "Fracture2 is a wavefolder that allows more extreme disruption.", airwinconsolidated::Fracture2::kNumParameters, "2023-06-25", new airwinconsolidated::Fracture2::Fracture2(samplingRate) });
        this->RegisterAirwindow({ "FromTape", "Tape", 395, true, "FromTape is a minimalist, cleaner analog tape emulation.", airwinconsolidated::FromTape::kNumParameters, "2022-11-21", new airwinconsolidated::FromTape::FromTape(samplingRate) });
        this->RegisterAirwindow({ "Galactic", "Reverb", 330, false, "Galactic is a super-reverb designed specially for pads and space ambient.", airwinconsolidated::Galactic::kNumParameters, "2022-11-21", new airwinconsolidated::Galactic::Galactic(samplingRate) });
        this->RegisterAirwindow({ "Galactic2", "Reverb", 331, false, "Galactic2 is a super-reverb REdesigned specially for pads and space ambient.", airwinconsolidated::Galactic2::kNumParameters, "2023-04-23", new airwinconsolidated::Galactic2::Galactic2(samplingRate) });
        this->RegisterAirwindow({ "Galactic3", "Reverb", 317, false, "Galactic3 is exactly Galactic again, but with better undersampling.", airwinconsolidated::Galactic3::kNumParameters, "2024-10-13", new airwinconsolidated::Galactic3::Galactic3(samplingRate) });
        this->RegisterAirwindow({ "GalacticVibe", "Effects", 211, false, "GalacticVibe is the stereo vibrato from the original Galactic reverb.", airwinconsolidated::GalacticVibe::kNumParameters, "2023-04-23", new airwinconsolidated::GalacticVibe::GalacticVibe(samplingRate) });
        this->RegisterAirwindow({ "Gatelope", "Dynamics", 199, true, "Gatelope is a special gate that applies filters.", airwinconsolidated::Gatelope::kNumParameters, "2022-11-21", new airwinconsolidated::Gatelope::Gatelope(samplingRate) });
        this->RegisterAirwindow({ "GlitchShifter", "Effects", 224, true, "GlitchShifter is a really gnarly, raw-sounding pitch shifter with a dose of insanity!", airwinconsolidated::GlitchShifter::kNumParameters, "2022-11-21", new airwinconsolidated::GlitchShifter::GlitchShifter(samplingRate) });
        this->RegisterAirwindow({ "GoldenSlew", "Brightness", 64, true, "GoldenSlew is a multistage slew clipper based on the Golden Ratio.", airwinconsolidated::GoldenSlew::kNumParameters, "2023-07-02", new airwinconsolidated::GoldenSlew::GoldenSlew(samplingRate) });
        this->RegisterAirwindow({ "Golem", "Utility", 435, false, "Golem lets you blend a stereo track of two mics on an amp.", airwinconsolidated::Golem::kNumParameters, "2022-11-21", new airwinconsolidated::Golem::Golem(samplingRate) });
        this->RegisterAirwindow({ "GrindAmp", "Amp Sims", 30, true, "GrindAmp is a heavier amp sim for many purposes.", airwinconsolidated::GrindAmp::kNumParameters, "2022-11-21", new airwinconsolidated::GrindAmp::GrindAmp(samplingRate) });
        this->RegisterAirwindow({ "Gringer", "Effects", 225, true, "Gringer is a full-wave rectifier plugin, like a Green Ringer guitar effect.", airwinconsolidated::Gringer::kNumParameters, "2022-11-21", new airwinconsolidated::Gringer::Gringer(samplingRate) });
        this->RegisterAirwindow({ "GrooveWear", "Lo-Fi", 284, true, "GrooveWear is for scrubbing highs off mechanically like a stylus would.", airwinconsolidated::GrooveWear::kNumParameters, "2022-11-21", new airwinconsolidated::GrooveWear::GrooveWear(samplingRate) });
        this->RegisterAirwindow({ "GuitarConditioner", "Effects", 220, true, "GuitarConditioner is like a Tube Screamer voicing without the squishiness and indistinctness.", airwinconsolidated::GuitarConditioner::kNumParameters, "2022-11-21", new airwinconsolidated::GuitarConditioner::GuitarConditioner(samplingRate) });
        this->RegisterAirwindow({ "HardVacuum", "Distortion", 148, true, "HardVacuum is tube style saturation effects.", airwinconsolidated::HardVacuum::kNumParameters, "2022-11-21", new airwinconsolidated::HardVacuum::HardVacuum(samplingRate) });
        this->RegisterAirwindow({ "HermeTrim", "Utility", 427, false, "HermeTrim is Very Fine Adjustments, otherwise just like EveryTrim.", airwinconsolidated::HermeTrim::kNumParameters, "2022-11-21", new airwinconsolidated::HermeTrim::HermeTrim(samplingRate) });
        this->RegisterAirwindow({ "Hermepass", "Bass", 44, true, "Hermepass is a mastering highpass to set by ear only.", airwinconsolidated::Hermepass::kNumParameters, "2022-11-21", new airwinconsolidated::Hermepass::Hermepass(samplingRate) });
        this->RegisterAirwindow({ "HighGlossDither", "Dithers", 172, true, "HighGlossDither is a hybrid between shiny dither and truncation! Unnatural science experiment.", airwinconsolidated::HighGlossDither::kNumParameters, "2022-11-21", new airwinconsolidated::HighGlossDither::HighGlossDither(samplingRate) });
        this->RegisterAirwindow({ "HighImpact", "Distortion", 149, true, "HighImpact is distorted grit and punch without fatness.", airwinconsolidated::HighImpact::kNumParameters, "2022-11-21", new airwinconsolidated::HighImpact::HighImpact(samplingRate) });
        this->RegisterAirwindow({ "Highpass", "Filter", 277, true, "Highpass is a time warp, for retro midrangey sounds.", airwinconsolidated::Highpass::kNumParameters, "2022-11-21", new airwinconsolidated::Highpass::Highpass(samplingRate) });
        this->RegisterAirwindow({ "Highpass2", "Filter", 273, true, "Highpass2 is an unusual-sounding variable-slope highpass filter.", airwinconsolidated::Highpass2::kNumParameters, "2022-11-21", new airwinconsolidated::Highpass2::Highpass2(samplingRate) });
        this->RegisterAirwindow({ "HipCrush", "Lo-Fi", 279, true, "HipCrush is the de-rez section for ConsoleH.", airwinconsolidated::HipCrush::kNumParameters, "2025-10-24", new airwinconsolidated::HipCrush::HipCrush(samplingRate) });
        this->RegisterAirwindow({ "Holt", "Filter", 259, true, "Holt is a synth-like resonant lowpass filter focussed on bass frequencies.", airwinconsolidated::Holt::kNumParameters, "2022-11-21", new airwinconsolidated::Holt::Holt(samplingRate) });
        this->RegisterAirwindow({ "Holt2", "Filter", 258, true, "Holt2 is a bass-cab-like resonant lowpass filter.", airwinconsolidated::Holt2::kNumParameters, "2022-11-21", new airwinconsolidated::Holt2::Holt2(samplingRate) });
        this->RegisterAirwindow({ "Hombre", "Ambience", 17, true, "Hombre is atmosphere and texture (through very short delays).", airwinconsolidated::Hombre::kNumParameters, "2022-11-21", new airwinconsolidated::Hombre::Hombre(samplingRate) });
        this->RegisterAirwindow({ "Huge", "Saturation", 346, true, "Huge is a dirty, dirty loudenator.", airwinconsolidated::Huge::kNumParameters, "2023-03-31", new airwinconsolidated::Huge::Huge(samplingRate) });
        this->RegisterAirwindow({ "Hull", "Filter", 266, true, "Hull is an alternate form of highpass/lowpass filter.", airwinconsolidated::Hull::kNumParameters, "2022-11-21", new airwinconsolidated::Hull::Hull(samplingRate) });
        this->RegisterAirwindow({ "Hull2", "Filter", 264, true, "Hull2 is a very clear three-band EQ.", airwinconsolidated::Hull2::kNumParameters, "2023-12-09", new airwinconsolidated::Hull2::Hull2(samplingRate) });
        this->RegisterAirwindow({ "Hype", "Subtlety", 375, true, "Hype is a treble softening experiment gone strangely awry :)", airwinconsolidated::Hype::kNumParameters, "2022-11-21", new airwinconsolidated::Hype::Hype(samplingRate) });
        this->RegisterAirwindow({ "Hypersoft", "Saturation", 344, true, "Hypersoft is a more extreme form of soft-clipper.", airwinconsolidated::Hypersoft::kNumParameters, "2024-11-24", new airwinconsolidated::Hypersoft::Hypersoft(samplingRate) });
        this->RegisterAirwindow({ "HypersonX", "Brightness", 80, true, "HypersonX is like UltrasonX but with seven stages instead of five.", airwinconsolidated::HypersonX::kNumParameters, "2022-11-21", new airwinconsolidated::HypersonX::HypersonX(samplingRate) });
        this->RegisterAirwindow({ "Hypersonic", "Brightness", 79, true, "Hypersonic is Ultrasonic, but steeper and higher cutoff.", airwinconsolidated::Hypersonic::kNumParameters, "2022-11-21", new airwinconsolidated::Hypersonic::Hypersonic(samplingRate) });
        this->RegisterAirwindow({ "Infinity", "Reverb", 338, true, "Infinity is a MatrixVerb specifically designed to sustain and layer sounds forever.", airwinconsolidated::Infinity::kNumParameters, "2022-11-21", new airwinconsolidated::Infinity::Infinity(samplingRate) });
        this->RegisterAirwindow({ "Infinity2", "Reverb", 335, true, "Infinity2 expands Infinity with more tone shaping and a much-requested kill-switch!", airwinconsolidated::Infinity2::kNumParameters, "2022-11-21", new airwinconsolidated::Infinity2::Infinity2(samplingRate) });
        this->RegisterAirwindow({ "Inflamer", "Subtlety", 377, true, "Inflamer is an Airwindowsized take on the waveshapers in Oxford Inflator.", airwinconsolidated::Inflamer::kNumParameters, "2023-08-12", new airwinconsolidated::Inflamer::Inflamer(samplingRate) });
        this->RegisterAirwindow({ "Infrasonic", "Bass", 49, true, "Infrasonic is a very steep subsonic filter, built like Ultrasonic.", airwinconsolidated::Infrasonic::kNumParameters, "2022-11-21", new airwinconsolidated::Infrasonic::Infrasonic(samplingRate) });
        this->RegisterAirwindow({ "Interstage", "Subtlety", 382, true, "Interstage is a subtle and sophisticated analogifier.", airwinconsolidated::Interstage::kNumParameters, "2022-11-21", new airwinconsolidated::Interstage::Interstage(samplingRate) });
        this->RegisterAirwindow({ "IronOxide5", "Tape", 398, true, "IronOxide5 is the old school, heavily colored Airwindows tape emulation.", airwinconsolidated::IronOxide5::kNumParameters, "2022-11-21", new airwinconsolidated::IronOxide5::IronOxide5(samplingRate) });
        this->RegisterAirwindow({ "IronOxideClassic", "Tape", 400, true, "IronOxideClassic is the purer, simpler, early form of Iron Oxide before all the features.", airwinconsolidated::IronOxideClassic::kNumParameters, "2022-11-21", new airwinconsolidated::IronOxideClassic::IronOxideClassic(samplingRate) });
        this->RegisterAirwindow({ "IronOxideClassic2", "Tape", 397, true, "IronOxideClassic2 is my bandpassy tape sim, updated for high sample rate and aliasing control.", airwinconsolidated::IronOxideClassic2::kNumParameters, "2022-11-21", new airwinconsolidated::IronOxideClassic2::IronOxideClassic2(samplingRate) });
        this->RegisterAirwindow({ "Isolator", "Filter", 269, true, "Isolator is a steep highpass or lowpass filter, like you might find in a speaker crossover.", airwinconsolidated::Isolator::kNumParameters, "2022-11-21", new airwinconsolidated::Isolator::Isolator(samplingRate) });
        this->RegisterAirwindow({ "Isolator2", "Filter", 255, true, "Isolator2 is Isolator, but with control smoothing and a new resonance booster.", airwinconsolidated::Isolator2::kNumParameters, "2022-11-21", new airwinconsolidated::Isolator2::Isolator2(samplingRate) });
        this->RegisterAirwindow({ "Isolator3", "Filter", 244, true, "Isolator3 is Isolator2, but on one slider, with a band-narrower control.", airwinconsolidated::Isolator3::kNumParameters, "2024-08-18", new airwinconsolidated::Isolator3::Isolator3(samplingRate) });
        this->RegisterAirwindow({ "Kalman", "Filter", 256, true, "Kalman is not a real filter!", airwinconsolidated::Kalman::kNumParameters, "2024-03-10", new airwinconsolidated::Kalman::Kalman(samplingRate) });
        this->RegisterAirwindow({ "LRConvolve", "Utility", 438, false, "LRConvolve multiplies each channel by the other!", airwinconsolidated::LRConvolve::kNumParameters, "2025-01-26", new airwinconsolidated::LRConvolve::LRConvolve(samplingRate) });
        this->RegisterAirwindow({ "LRConvolve2", "Utility", 437, false, "LRConvolve2 multiplies each channel by the other, plus Soar!", airwinconsolidated::LRConvolve2::kNumParameters, "2025-02-16", new airwinconsolidated::LRConvolve2::LRConvolve2(samplingRate) });
        this->RegisterAirwindow({ "LRConvolve3", "Unclassified", -1, false, "", airwinconsolidated::LRConvolve3::kNumParameters, "2025-02-16", new airwinconsolidated::LRConvolve3::LRConvolve3(samplingRate) });
        this->RegisterAirwindow({ "LRFlipTimer", "Stereo", 367, false, "LRFlipTimer is a utility that swaps Left with Right every few (1-10) minutes.", airwinconsolidated::LRFlipTimer::kNumParameters, "2022-11-21", new airwinconsolidated::LRFlipTimer::LRFlipTimer(samplingRate) });
        this->RegisterAirwindow({ "LeadAmp", "Amp Sims", 32, true, "LeadAmp is an amp sim with a clear, penetrating, vocal tone.", airwinconsolidated::LeadAmp::kNumParameters, "2022-11-21", new airwinconsolidated::LeadAmp::LeadAmp(samplingRate) });
        this->RegisterAirwindow({ "LeftoMono", "Utility", 444, false, "LeftoMono copies one channel to both, losslessly.", airwinconsolidated::LeftoMono::kNumParameters, "2022-11-21", new airwinconsolidated::LeftoMono::LeftoMono(samplingRate) });
        this->RegisterAirwindow({ "LilAmp", "Amp Sims", 35, true, "LilAmp is a tiny amp sim, like a little bitty practice amp without that much gain.", airwinconsolidated::LilAmp::kNumParameters, "2022-11-21", new airwinconsolidated::LilAmp::LilAmp(samplingRate) });
        this->RegisterAirwindow({ "Logical4", "Dynamics", 184, false, "Logical4 is a classic 2-buss compressor.", airwinconsolidated::Logical4::kNumParameters, "2022-11-21", new airwinconsolidated::Logical4::Logical4(samplingRate) });
        this->RegisterAirwindow({ "Loud", "Distortion", 147, true, "Loud is distortion and demolition of air molecules, modeled.", airwinconsolidated::Loud::kNumParameters, "2022-11-21", new airwinconsolidated::Loud::Loud(samplingRate) });
        this->RegisterAirwindow({ "Lowpass", "Filter", 276, true, "Lowpass deepens the tone, leaves a gloss and textural modifications.", airwinconsolidated::Lowpass::kNumParameters, "2022-11-21", new airwinconsolidated::Lowpass::Lowpass(samplingRate) });
        this->RegisterAirwindow({ "Lowpass2", "Filter", 272, true, "Lowpass2 is an unusual-sounding variable-slope lowpass filter.", airwinconsolidated::Lowpass2::kNumParameters, "2022-11-21", new airwinconsolidated::Lowpass2::Lowpass2(samplingRate) });
        this->RegisterAirwindow({ "Luxor", "Tone Color", 410, true, "Luxor is a re-release of another old Character plugin.", airwinconsolidated::Luxor::kNumParameters, "2022-11-21", new airwinconsolidated::Luxor::Luxor(samplingRate) });
        this->RegisterAirwindow({ "MSFlipTimer", "Stereo", 368, false, "MSFlipTimer is a utility that swaps stereo with mono every few (1-10) minutes.", airwinconsolidated::MSFlipTimer::kNumParameters, "2023-12-09", new airwinconsolidated::MSFlipTimer::MSFlipTimer(samplingRate) });
        this->RegisterAirwindow({ "MV", "Ambience", 9, true, "MV is a dual-mono reverb based on BitShiftGain and the old Midiverbs.", airwinconsolidated::MV::kNumParameters, "2022-11-21", new airwinconsolidated::MV::MV(samplingRate) });
        this->RegisterAirwindow({ "MV2", "Ambience", 8, true, "MV2 is a dual-mono reverb based on BitShiftGain and the old Midiverbs, adapted to high sample rates.", airwinconsolidated::MV2::kNumParameters, "2023-04-23", new airwinconsolidated::MV2::MV2(samplingRate) });
        this->RegisterAirwindow({ "MackEQ", "Filter", 263, true, "MackEQ is the Mackie distortion but with treble and bass controls added.", airwinconsolidated::MackEQ::kNumParameters, "2022-11-21", new airwinconsolidated::MackEQ::MackEQ(samplingRate) });
        this->RegisterAirwindow({ "Mackity", "Distortion", 141, true, "Mackity is an emulation of the input stage of a vintage Mackie 1202!", airwinconsolidated::Mackity::kNumParameters, "2022-11-21", new airwinconsolidated::Mackity::Mackity(samplingRate) });
        this->RegisterAirwindow({ "Mastering", "Subtlety", 373, false, "Mastering is Airwindows style, and can do things nothing else can!", airwinconsolidated::Mastering::kNumParameters, "2025-01-05", new airwinconsolidated::Mastering::Mastering(samplingRate) });
        this->RegisterAirwindow({ "Mastering2", "Subtlety", 371, false, "Mastering2 is Airwindows style, and can do elliptical EQ now!", airwinconsolidated::Mastering2::kNumParameters, "2025-03-17", new airwinconsolidated::Mastering2::Mastering2(samplingRate) });
        this->RegisterAirwindow({ "MatrixVerb", "Reverb", 339, true, "MatrixVerb is a wildly adjustable, strangely colorful reverb for deep and flexible spaces.", airwinconsolidated::MatrixVerb::kNumParameters, "2022-11-21", new airwinconsolidated::MatrixVerb::MatrixVerb(samplingRate) });
        this->RegisterAirwindow({ "Melt", "Ambience", 7, true, "Melt is a wobbly chorusy weird diffuse effect.", airwinconsolidated::Melt::kNumParameters, "2022-11-21", new airwinconsolidated::Melt::Melt(samplingRate) });
        this->RegisterAirwindow({ "MidAmp", "Amp Sims", 36, true, "MidAmp is a clean amp sim meant to work like a loud Twin or something of that nature.", airwinconsolidated::MidAmp::kNumParameters, "2022-11-21", new airwinconsolidated::MidAmp::MidAmp(samplingRate) });
        this->RegisterAirwindow({ "MidSide", "Utility", 440, false, "MidSide is the other side of the EdIsDim mid/side conversion utility plugin.", airwinconsolidated::MidSide::kNumParameters, "2025-06-23", new airwinconsolidated::MidSide::MidSide(samplingRate) });
        this->RegisterAirwindow({ "MoNoam", "Utility", 447, false, "MoNoam gives you variations on mono, mid, and side in different channels.", airwinconsolidated::MoNoam::kNumParameters, "2022-11-21", new airwinconsolidated::MoNoam::MoNoam(samplingRate) });
        this->RegisterAirwindow({ "Mojo", "Saturation", 353, true, "Mojo is a biggenator that also works as a loudenator.", airwinconsolidated::Mojo::kNumParameters, "2022-11-21", new airwinconsolidated::Mojo::Mojo(samplingRate) });
        this->RegisterAirwindow({ "Monitoring", "Utility", 425, false, "Monitoring is your one-stop shop for final 2-buss work!", airwinconsolidated::Monitoring::kNumParameters, "2022-11-21", new airwinconsolidated::Monitoring::Monitoring(samplingRate) });
        this->RegisterAirwindow({ "Monitoring2", "Utility", 424, false, "Monitoring2 is Monitoring, but with Dark as the wordlength reducer.", airwinconsolidated::Monitoring2::kNumParameters, "2022-11-21", new airwinconsolidated::Monitoring2::Monitoring2(samplingRate) });
        this->RegisterAirwindow({ "Monitoring3", "Utility", 423, false, "Monitoring3 is Monitoring, but with Ten Nines into Dark as the wordlength reducer.", airwinconsolidated::Monitoring3::kNumParameters, "2022-11-21", new airwinconsolidated::Monitoring3::Monitoring3(samplingRate) });
        this->RegisterAirwindow({ "MultiBandDistortion", "Distortion", 150, true, "MultiBandDistortion is an old weird gnarly sound wrecker :)", airwinconsolidated::MultiBandDistortion::kNumParameters, "2022-11-21", new airwinconsolidated::MultiBandDistortion::MultiBandDistortion(samplingRate) });
        this->RegisterAirwindow({ "NCSeventeen", "Saturation", 347, true, "NCSeventeen is Dirty Loud!", airwinconsolidated::NCSeventeen::kNumParameters, "2022-11-21", new airwinconsolidated::NCSeventeen::NCSeventeen(samplingRate) });
        this->RegisterAirwindow({ "NaturalizeDither", "Dithers", 171, true, "NaturalizeDither is deterministic dither that uses Benford Realness calculations for each sample.", airwinconsolidated::NaturalizeDither::kNumParameters, "2023-04-12", new airwinconsolidated::NaturalizeDither::NaturalizeDither(samplingRate) });
        this->RegisterAirwindow({ "Neverland", "Tone Color", 404, true, "Neverland is a re-release of my old Neve-style color adder, exacly as it was.", airwinconsolidated::Neverland::kNumParameters, "2022-11-21", new airwinconsolidated::Neverland::Neverland(samplingRate) });
        this->RegisterAirwindow({ "Nikola", "Effects", 234, true, "Nikola is an Audio Tesla Coil plugin! Note: audio tesla coils don't sound nice.", airwinconsolidated::Nikola::kNumParameters, "2022-11-21", new airwinconsolidated::Nikola::Nikola(samplingRate) });
        this->RegisterAirwindow({ "NodeDither", "Dithers", 165, true, "NodeDither is adjusta-TPDF-dither, like a dither flanger. Can do Paul and Tape settings.", airwinconsolidated::NodeDither::kNumParameters, "2022-11-21", new airwinconsolidated::NodeDither::NodeDither(samplingRate) });
        this->RegisterAirwindow({ "Noise", "Noise", 298, true, "Noise is the Airwindows deep noise oscillator, as a sound reinforcer.", airwinconsolidated::Noise::kNumParameters, "2022-11-21", new airwinconsolidated::Noise::Noise(samplingRate) });
        this->RegisterAirwindow({ "NonlinearSpace", "Reverb", 336, false, "NonlinearSpace is a flexible reverb plugin.", airwinconsolidated::NonlinearSpace::kNumParameters, "2022-11-21", new airwinconsolidated::NonlinearSpace::NonlinearSpace(samplingRate) });
        this->RegisterAirwindow({ "NotJustAnotherCD", "Dithers", 157, true, "NotJustAnotherCD is a next-gen wordlength reducers for 16 bit with DeRez.", airwinconsolidated::NotJustAnotherCD::kNumParameters, "2023-04-12", new airwinconsolidated::NotJustAnotherCD::NotJustAnotherCD(samplingRate) });
        this->RegisterAirwindow({ "NotJustAnotherDither", "Dithers", 156, true, "NotJustAnotherDither is a next-gen wordlength reducers for 24 bit with DeRez.", airwinconsolidated::NotJustAnotherDither::kNumParameters, "2022-11-21", new airwinconsolidated::NotJustAnotherDither::NotJustAnotherDither(samplingRate) });
        this->RegisterAirwindow({ "OneCornerClip", "Clipping", 88, true, "OneCornerClip is an ultimate full-bandwidth clipper.", airwinconsolidated::OneCornerClip::kNumParameters, "2022-11-21", new airwinconsolidated::OneCornerClip::OneCornerClip(samplingRate) });
        this->RegisterAirwindow({ "OrbitKick", "Bass", 42, false, "OrbitKick is a bass generator and kick drum reinforcer.", airwinconsolidated::OrbitKick::kNumParameters, "2023-03-11", new airwinconsolidated::OrbitKick::OrbitKick(samplingRate) });
        this->RegisterAirwindow({ "Overheads", "Dynamics", 188, false, "Overheads is for compressing only part of the sound, strangely!", airwinconsolidated::Overheads::kNumParameters, "2023-12-16", new airwinconsolidated::Overheads::Overheads(samplingRate) });
        this->RegisterAirwindow({ "PDBuss", "Consoles", 126, true, "PDBuss is Console5 and PurestDrive, sittin’ in a tree… (i.e. both at once, on channels and buss)", airwinconsolidated::PDBuss::kNumParameters, "2022-11-21", new airwinconsolidated::PDBuss::PDBuss(samplingRate) });
        this->RegisterAirwindow({ "PDChannel", "Consoles", 127, true, "PDChannel is Console5 and PurestDrive, sittin’ in a tree… (i.e. both at once, on channels and buss)", airwinconsolidated::PDChannel::kNumParameters, "2022-11-21", new airwinconsolidated::PDChannel::PDChannel(samplingRate) });
        this->RegisterAirwindow({ "Pafnuty", "Unclassified", -1, true, "Pafnuty is a Chebyshev filter, that adds harmonics.", airwinconsolidated::Pafnuty::kNumParameters, "2022-11-21", new airwinconsolidated::Pafnuty::Pafnuty(samplingRate) });
        this->RegisterAirwindow({ "Pafnuty2", "Effects", 216, true, "Pafnuty2 is a Chebyshev filter, that adds harmonics, and fits in the VCV Rack port.", airwinconsolidated::Pafnuty2::kNumParameters, "2023-03-11", new airwinconsolidated::Pafnuty2::Pafnuty2(samplingRate) });
        this->RegisterAirwindow({ "Parametric", "Filter", 239, true, "Parametric is three bands of ConsoleX EQ in advance.", airwinconsolidated::Parametric::kNumParameters, "2024-05-10", new airwinconsolidated::Parametric::Parametric(samplingRate) });
        this->RegisterAirwindow({ "PaulDither", "Dithers", 153, true, "PaulDither is a highpassed TPDF dither. (quieter, airier)", airwinconsolidated::PaulDither::kNumParameters, "2022-11-21", new airwinconsolidated::PaulDither::PaulDither(samplingRate) });
        this->RegisterAirwindow({ "PaulWide", "Dithers", 152, false, "PaulWide is a highpassed TPDF wide dither. (quieter, airier AND wider)", airwinconsolidated::PaulWide::kNumParameters, "2022-11-21", new airwinconsolidated::PaulWide::PaulWide(samplingRate) });
        this->RegisterAirwindow({ "PeaksOnly", "Utility", 434, true, "PeaksOnly is a transformative mix check tool.", airwinconsolidated::PeaksOnly::kNumParameters, "2022-11-21", new airwinconsolidated::PeaksOnly::PeaksOnly(samplingRate) });
        this->RegisterAirwindow({ "Pear", "Filter", 250, false, "Pear is the testbed for a new filter, implemented as a highpass/lowpass shelf.", airwinconsolidated::Pear::kNumParameters, "2023-06-25", new airwinconsolidated::Pear::Pear(samplingRate) });
        this->RegisterAirwindow({ "Pear2", "Filter", 247, false, "Pear2 is my Pear filter plus nonlinearity.", airwinconsolidated::Pear2::kNumParameters, "2024-03-10", new airwinconsolidated::Pear2::Pear2(samplingRate) });
        this->RegisterAirwindow({ "PearEQ", "Filter", 236, true, "PearEQ is a six-band Pear-based graphic EQ.", airwinconsolidated::PearEQ::kNumParameters, "2025-10-04", new airwinconsolidated::PearEQ::PearEQ(samplingRate) });
        this->RegisterAirwindow({ "PearLiteEQ", "Unclassified", -1, true, "", airwinconsolidated::PearLiteEQ::kNumParameters, "2025-10-04", new airwinconsolidated::PearLiteEQ::PearLiteEQ(samplingRate) });
        this->RegisterAirwindow({ "PhaseNudge", "Subtlety", 383, true, "PhaseNudge is a phase rotator/allpass filter.", airwinconsolidated::PhaseNudge::kNumParameters, "2022-11-21", new airwinconsolidated::PhaseNudge::PhaseNudge(samplingRate) });
        this->RegisterAirwindow({ "PitchDelay", "Ambience", 3, true, "PitchDelay is TapeDelay2 but with pitch shift instead of flutter.", airwinconsolidated::PitchDelay::kNumParameters, "2022-11-21", new airwinconsolidated::PitchDelay::PitchDelay(samplingRate) });
        this->RegisterAirwindow({ "PitchNasty", "Effects", 217, true, "PitchNasty is a primitive pitch shifter that also has primitive time-stretch artifacts.", airwinconsolidated::PitchNasty::kNumParameters, "2022-11-21", new airwinconsolidated::PitchNasty::PitchNasty(samplingRate) });
        this->RegisterAirwindow({ "PlatinumSlew", "Brightness", 62, true, "PlatinumSlew is the best easy-to-use slew clipper.", airwinconsolidated::PlatinumSlew::kNumParameters, "2023-07-22", new airwinconsolidated::PlatinumSlew::PlatinumSlew(samplingRate) });
        this->RegisterAirwindow({ "PocketVerbs", "Reverb", 340, true, "PocketVerbs is my popular old special effects reverbs plugin!", airwinconsolidated::PocketVerbs::kNumParameters, "2022-11-21", new airwinconsolidated::PocketVerbs::PocketVerbs(samplingRate) });
        this->RegisterAirwindow({ "Pockey", "Lo-Fi", 285, true, "Pockey is 12 bit (and under) lo-fi hiphop in a plugin.", airwinconsolidated::Pockey::kNumParameters, "2022-11-21", new airwinconsolidated::Pockey::Pockey(samplingRate) });
        this->RegisterAirwindow({ "Pockey2", "Lo-Fi", 282, true, "Pockey2 is more efficient, more intense lo-fi hiphop in a plugin.", airwinconsolidated::Pockey2::kNumParameters, "2022-11-21", new airwinconsolidated::Pockey2::Pockey2(samplingRate) });
        this->RegisterAirwindow({ "Podcast", "Dynamics", 201, true, "Podcast is a simpler pile of curve-style compressors with hard clipping.", airwinconsolidated::Podcast::kNumParameters, "2022-11-21", new airwinconsolidated::Podcast::Podcast(samplingRate) });
        this->RegisterAirwindow({ "PodcastDeluxe", "Dynamics", 200, true, "PodcastDeluxe is a pile of compressors (curve style) and phase rotators.", airwinconsolidated::PodcastDeluxe::kNumParameters, "2022-11-21", new airwinconsolidated::PodcastDeluxe::PodcastDeluxe(samplingRate) });
        this->RegisterAirwindow({ "Point", "Dynamics", 198, true, "Point is an explosive transient designer.", airwinconsolidated::Point::kNumParameters, "2022-11-21", new airwinconsolidated::Point::Point(samplingRate) });
        this->RegisterAirwindow({ "PointyDeluxe", "Amp Sims", 28, true, "PointyDeluxe devours all mix space.", airwinconsolidated::PointyDeluxe::kNumParameters, "2025-06-21", new airwinconsolidated::PointyDeluxe::PointyDeluxe(samplingRate) });
        this->RegisterAirwindow({ "PointyGuitar", "Amp Sims", 27, true, "PointyGuitar is a supremely adaptable instrument amp.", airwinconsolidated::PointyGuitar::kNumParameters, "2025-04-19", new airwinconsolidated::PointyGuitar::PointyGuitar(samplingRate) });
        this->RegisterAirwindow({ "Pop", "Dynamics", 182, true, "Pop is a crazy overcompressor with a very exaggerated sound.", airwinconsolidated::Pop::kNumParameters, "2022-11-21", new airwinconsolidated::Pop::Pop(samplingRate) });
        this->RegisterAirwindow({ "Pop2", "Dynamics", 178, true, "Pop2 adds control and punch to Pop.", airwinconsolidated::Pop2::kNumParameters, "2022-11-21", new airwinconsolidated::Pop2::Pop2(samplingRate) });
        this->RegisterAirwindow({ "Pop3", "Dynamics", 177, false, "Pop3 is the dynamics from ConsoleX.", airwinconsolidated::Pop3::kNumParameters, "2024-05-25", new airwinconsolidated::Pop3::Pop3(samplingRate) });
        this->RegisterAirwindow({ "PowerSag", "Effects", 232, true, "PowerSag is for emulating power supply limitations in analog modeling.", airwinconsolidated::PowerSag::kNumParameters, "2022-11-21", new airwinconsolidated::PowerSag::PowerSag(samplingRate) });
        this->RegisterAirwindow({ "PowerSag2", "Effects", 231, true, "PowerSag2 is my improved circuit-starve plugin, now with inverse effect!", airwinconsolidated::PowerSag2::kNumParameters, "2022-11-21", new airwinconsolidated::PowerSag2::PowerSag2(samplingRate) });
        this->RegisterAirwindow({ "Precious", "Tone Color", 409, true, "Precious is a re-release of another old Character plugin.", airwinconsolidated::Precious::kNumParameters, "2022-11-21", new airwinconsolidated::Precious::Precious(samplingRate) });
        this->RegisterAirwindow({ "Preponderant", "Effects", 233, true, "Preponderant is Airwindows Anti-Soothe, a strange tone intensifier and balancer.", airwinconsolidated::Preponderant::kNumParameters, "2022-11-21", new airwinconsolidated::Preponderant::Preponderant(samplingRate) });
        this->RegisterAirwindow({ "Pressure4", "Dynamics", 204, false, "Pressure4 is a compressor adjustable between vari-mu and ‘new york’ peak-retaining behaviors.", airwinconsolidated::Pressure4::kNumParameters, "2022-11-21", new airwinconsolidated::Pressure4::Pressure4(samplingRate) });
        this->RegisterAirwindow({ "Pressure5", "Dynamics", 179, false, "Pressure5 expands Pressure4 with after-boost and built-in ClipOnly2.", airwinconsolidated::Pressure5::kNumParameters, "2022-11-21", new airwinconsolidated::Pressure5::Pressure5(samplingRate) });
        this->RegisterAirwindow({ "Pressure6", "Dynamics", 175, false, "Pressure6 refines the Pressure compressor and how it moves.", airwinconsolidated::Pressure6::kNumParameters, "2025-07-13", new airwinconsolidated::Pressure6::Pressure6(samplingRate) });
        this->RegisterAirwindow({ "PrimeFIR", "Filter", 257, true, "PrimeFIR is a mostly linear-phase brickwall with a taste for the bizarre!", airwinconsolidated::PrimeFIR::kNumParameters, "2025-04-12", new airwinconsolidated::PrimeFIR::PrimeFIR(samplingRate) });
        this->RegisterAirwindow({ "PunchyDeluxe", "Unclassified", -1, true, "", airwinconsolidated::PunchyDeluxe::kNumParameters, "2025-12-13", new airwinconsolidated::PunchyDeluxe::PunchyDeluxe(samplingRate) });
        this->RegisterAirwindow({ "PunchyGuitar", "Unclassified", -1, true, "", airwinconsolidated::PunchyGuitar::kNumParameters, "2025-12-13", new airwinconsolidated::PunchyGuitar::PunchyGuitar(samplingRate) });
        this->RegisterAirwindow({ "PurestAir", "Brightness", 76, true, "PurestAir is a brightness experiment.", airwinconsolidated::PurestAir::kNumParameters, "2022-11-21", new airwinconsolidated::PurestAir::PurestAir(samplingRate) });
        this->RegisterAirwindow({ "PurestConsole2Buss", "Consoles", 116, true, "PurestConsole2Buss is the distributed filter version of PurestConsole.", airwinconsolidated::PurestConsole2Buss::kNumParameters, "2022-11-21", new airwinconsolidated::PurestConsole2Buss::PurestConsole2Buss(samplingRate) });
        this->RegisterAirwindow({ "PurestConsole2Channel", "Consoles", 117, true, "PurestConsole2Channel is the distributed filter version of PurestConsole.", airwinconsolidated::PurestConsole2Channel::kNumParameters, "2022-11-21", new airwinconsolidated::PurestConsole2Channel::PurestConsole2Channel(samplingRate) });
        this->RegisterAirwindow({ "PurestConsole3Buss", "Consoles", 114, true, "PurestConsole3Buss is an alternate algorithm for minimalist Console.", airwinconsolidated::PurestConsole3Buss::kNumParameters, "2023-06-25", new airwinconsolidated::PurestConsole3Buss::PurestConsole3Buss(samplingRate) });
        this->RegisterAirwindow({ "PurestConsole3Channel", "Consoles", 115, true, "PurestConsole3Channel is an alternate algorithm for minimalist Console.", airwinconsolidated::PurestConsole3Channel::kNumParameters, "2023-06-25", new airwinconsolidated::PurestConsole3Channel::PurestConsole3Channel(samplingRate) });
        this->RegisterAirwindow({ "PurestConsoleBuss", "Consoles", 122, true, "PurestConsoleBuss is the most free from coloration Console system.", airwinconsolidated::PurestConsoleBuss::kNumParameters, "2022-11-21", new airwinconsolidated::PurestConsoleBuss::PurestConsoleBuss(samplingRate) });
        this->RegisterAirwindow({ "PurestConsoleChannel", "Consoles", 123, true, "PurestConsoleChannel is the most free from coloration Console system.", airwinconsolidated::PurestConsoleChannel::kNumParameters, "2022-11-21", new airwinconsolidated::PurestConsoleChannel::PurestConsoleChannel(samplingRate) });
        this->RegisterAirwindow({ "PurestDrive", "Saturation", 351, true, "PurestDrive is the magic saturation plugin of subtlety and French House tone.", airwinconsolidated::PurestDrive::kNumParameters, "2022-11-21", new airwinconsolidated::PurestDrive::PurestDrive(samplingRate) });
        this->RegisterAirwindow({ "PurestDualPan", "Utility", 421, false, "PurestDualPan is an updated PurestGain but as a dual pan.", airwinconsolidated::PurestDualPan::kNumParameters, "2025-11-28", new airwinconsolidated::PurestDualPan::PurestDualPan(samplingRate) });
        this->RegisterAirwindow({ "PurestEcho", "Ambience", 13, true, "PurestEcho is optimized Airwindows echo with exactly four evenly spaced taps on tap.", airwinconsolidated::PurestEcho::kNumParameters, "2022-11-21", new airwinconsolidated::PurestEcho::PurestEcho(samplingRate) });
        this->RegisterAirwindow({ "PurestFade", "Utility", 422, true, "PurestFade is just like PurestGain, but for LONG fades.", airwinconsolidated::PurestFade::kNumParameters, "2022-11-21", new airwinconsolidated::PurestFade::PurestFade(samplingRate) });
        this->RegisterAirwindow({ "PurestGain", "Utility", 420, true, "PurestGain is a high-res noise shaped gain, with smoothed fader.", airwinconsolidated::PurestGain::kNumParameters, "2022-11-21", new airwinconsolidated::PurestGain::PurestGain(samplingRate) });
        this->RegisterAirwindow({ "PurestSaturation", "Saturation", 342, true, "PurestSaturation is an experiment in softclipping.", airwinconsolidated::PurestSaturation::kNumParameters, "2025-11-28", new airwinconsolidated::PurestSaturation::PurestSaturation(samplingRate) });
        this->RegisterAirwindow({ "PurestSquish", "Dynamics", 203, true, "PurestSquish is an open-sounding compressor with bass bloom.", airwinconsolidated::PurestSquish::kNumParameters, "2022-11-21", new airwinconsolidated::PurestSquish::PurestSquish(samplingRate) });
        this->RegisterAirwindow({ "PurestWarm", "Subtlety", 380, true, "PurestWarm is a subtle tone shaper and warmth adder.", airwinconsolidated::PurestWarm::kNumParameters, "2022-11-21", new airwinconsolidated::PurestWarm::PurestWarm(samplingRate) });
        this->RegisterAirwindow({ "PurestWarm2", "Subtlety", 379, true, "PurestWarm2 adds dual dry/wet controls for each side of the waveform.", airwinconsolidated::PurestWarm2::kNumParameters, "2022-11-21", new airwinconsolidated::PurestWarm2::PurestWarm2(samplingRate) });
        this->RegisterAirwindow({ "Pyewacket", "Dynamics", 190, false, "Pyewacket is an old school compressor for high definition transients. Adds no fatness, just energy.", airwinconsolidated::Pyewacket::kNumParameters, "2022-11-21", new airwinconsolidated::Pyewacket::Pyewacket(samplingRate) });
        this->RegisterAirwindow({ "RawGlitters", "Dithers", 170, true, "RawGlitters is just the quantization from Dither Me Timbers… and the opposite, as a brightener.", airwinconsolidated::RawGlitters::kNumParameters, "2022-11-21", new airwinconsolidated::RawGlitters::RawGlitters(samplingRate) });
        this->RegisterAirwindow({ "RawTimbers", "Dithers", 169, true, "RawTimbers is just the quantization from Dither Me Timbers… and the opposite, as a brightener.", airwinconsolidated::RawTimbers::kNumParameters, "2022-11-21", new airwinconsolidated::RawTimbers::RawTimbers(samplingRate) });
        this->RegisterAirwindow({ "Recurve", "Dynamics", 189, false, "Recurve is a special buss compressor with no threshold point.", airwinconsolidated::Recurve::kNumParameters, "2022-11-21", new airwinconsolidated::Recurve::Recurve(samplingRate) });
        this->RegisterAirwindow({ "Remap", "Subtlety", 384, true, "Remap puts the guts back into overloudenated audio!", airwinconsolidated::Remap::kNumParameters, "2022-11-21", new airwinconsolidated::Remap::Remap(samplingRate) });
        this->RegisterAirwindow({ "ResEQ", "Filter", 271, true, "ResEQ is a bank of mostly midrange resonances.", airwinconsolidated::ResEQ::kNumParameters, "2022-11-21", new airwinconsolidated::ResEQ::ResEQ(samplingRate) });
        this->RegisterAirwindow({ "ResEQ2", "Filter", 251, true, "ResEQ2 is a single, sharp, sonorous mid peak.", airwinconsolidated::ResEQ2::kNumParameters, "2023-09-17", new airwinconsolidated::ResEQ2::ResEQ2(samplingRate) });
        this->RegisterAirwindow({ "Reverb", "Reverb", 341, true, "Reverb is simplified and tuned MatrixVerb, adjusting everything for each optimum verb size.", airwinconsolidated::Reverb::kNumParameters, "2022-11-21", new airwinconsolidated::Reverb::Reverb(samplingRate) });
        this->RegisterAirwindow({ "Righteous4", "Saturation", 358, true, "Righteous4 is a final output stage for targeting dynamic range.", airwinconsolidated::Righteous4::kNumParameters, "2023-04-12", new airwinconsolidated::Righteous4::Righteous4(samplingRate) });
        this->RegisterAirwindow({ "RightoMono", "Utility", 443, false, "RightoMono copies one channel to both, losslessly.", airwinconsolidated::RightoMono::kNumParameters, "2022-11-21", new airwinconsolidated::RightoMono::RightoMono(samplingRate) });
        this->RegisterAirwindow({ "RingModulator", "Effects", 208, false, "RingModulator repitches sounds mathematically, not harmonically.", airwinconsolidated::RingModulator::kNumParameters, "2025-02-16", new airwinconsolidated::RingModulator::RingModulator(samplingRate) });
        this->RegisterAirwindow({ "SampleDelay", "Ambience", 4, true, "SampleDelay is three delays combined: millisecond, sample and subsample.", airwinconsolidated::SampleDelay::kNumParameters, "2023-03-20", new airwinconsolidated::SampleDelay::SampleDelay(samplingRate) });
        this->RegisterAirwindow({ "Shape", "Subtlety", 376, true, "Shape is an asymmetrical waveshaper for peak manipulating.", airwinconsolidated::Shape::kNumParameters, "2022-11-21", new airwinconsolidated::Shape::Shape(samplingRate) });
        this->RegisterAirwindow({ "ShortBuss", "Effects", 219, true, "ShortBuss chases second harmonic, to add or remove it.", airwinconsolidated::ShortBuss::kNumParameters, "2024-01-28", new airwinconsolidated::ShortBuss::ShortBuss(samplingRate) });
        this->RegisterAirwindow({ "SideDull", "Stereo", 370, false, "SideDull is like Sidepass, but a lowpass on the side channel. Treble-centerer.", airwinconsolidated::SideDull::kNumParameters, "2022-11-21", new airwinconsolidated::SideDull::SideDull(samplingRate) });
        this->RegisterAirwindow({ "Sidepass", "Stereo", 369, false, "Sidepass is a simple utility plugin, a highpass on the side channel. Mono-maker.", airwinconsolidated::Sidepass::kNumParameters, "2022-11-21", new airwinconsolidated::Sidepass::Sidepass(samplingRate) });
        this->RegisterAirwindow({ "Silhouette", "Noise", 304, true, "Silhouette replaces the sound with raw noise sculpted to match its dynamics.", airwinconsolidated::Silhouette::kNumParameters, "2022-11-21", new airwinconsolidated::Silhouette::Silhouette(samplingRate) });
        this->RegisterAirwindow({ "Silken", "Filter", 240, true, "Silken is a high frequency boost that gives ambience and texture.", airwinconsolidated::Silken::kNumParameters, "2025-08-03", new airwinconsolidated::Silken::Silken(samplingRate) });
        this->RegisterAirwindow({ "Sinew", "Brightness", 65, true, "Sinew combines sines and slew clipping for a tape bias effect!", airwinconsolidated::Sinew::kNumParameters, "2025-03-17", new airwinconsolidated::Sinew::Sinew(samplingRate) });
        this->RegisterAirwindow({ "SingleEndedTriode", "Subtlety", 385, true, "SingleEndedTriode is unusual analog modeling effects.", airwinconsolidated::SingleEndedTriode::kNumParameters, "2022-11-21", new airwinconsolidated::SingleEndedTriode::SingleEndedTriode(samplingRate) });
        this->RegisterAirwindow({ "Slew", "Brightness", 73, true, "Slew is a slew clipper, which darkens treble in an unusual way.", airwinconsolidated::Slew::kNumParameters, "2022-11-21", new airwinconsolidated::Slew::Slew(samplingRate) });
        this->RegisterAirwindow({ "Slew2", "Brightness", 72, true, "Slew2 works like a de-esser or acceleration limiter: controls extreme highs.", airwinconsolidated::Slew2::kNumParameters, "2022-11-21", new airwinconsolidated::Slew2::Slew2(samplingRate) });
        this->RegisterAirwindow({ "Slew3", "Brightness", 71, true, "Slew3 is a new approach to slew clipping meant for a more analog-like darkening effect.", airwinconsolidated::Slew3::kNumParameters, "2022-11-21", new airwinconsolidated::Slew3::Slew3(samplingRate) });
        this->RegisterAirwindow({ "Slew4", "Unclassified", -1, true, "", airwinconsolidated::Slew4::kNumParameters, "2025-10-04", new airwinconsolidated::Slew4::Slew4(samplingRate) });
        this->RegisterAirwindow({ "SlewOnly", "Utility", 432, true, "SlewOnly is a mix check plugin that shows you only the extreme highs.", airwinconsolidated::SlewOnly::kNumParameters, "2022-11-21", new airwinconsolidated::SlewOnly::SlewOnly(samplingRate) });
        this->RegisterAirwindow({ "SlewSonic", "Brightness", 66, true, "SlewSonic combines SlewOnly with ultrasonic filtering to solo brightness.", airwinconsolidated::SlewSonic::kNumParameters, "2023-02-27", new airwinconsolidated::SlewSonic::SlewSonic(samplingRate) });
        this->RegisterAirwindow({ "Smooth", "Brightness", 69, true, "Smooth can tame pointy sounds or make drums explode.", airwinconsolidated::Smooth::kNumParameters, "2022-11-21", new airwinconsolidated::Smooth::Smooth(samplingRate) });
        this->RegisterAirwindow({ "SmoothEQ", "Filter", 238, true, "SmoothEQ is a sharp, accurate, transparent three-band filter.", airwinconsolidated::SmoothEQ::kNumParameters, "2025-04-12", new airwinconsolidated::SmoothEQ::SmoothEQ(samplingRate) });
        this->RegisterAirwindow({ "SmoothEQ2", "Filter", 237, true, "SmoothEQ2 is a wildly intuitive vibe EQ with clarity and intensity.", airwinconsolidated::SmoothEQ2::kNumParameters, "2025-08-23", new airwinconsolidated::SmoothEQ2::SmoothEQ2(samplingRate) });
        this->RegisterAirwindow({ "SmoothEQ3", "Filter", 235, true, "SmoothEQ3 is the most approachable EQ.", airwinconsolidated::SmoothEQ3::kNumParameters, "2025-08-23", new airwinconsolidated::SmoothEQ3::SmoothEQ3(samplingRate) });
        this->RegisterAirwindow({ "SoftClock", "Utility", 417, false, "SoftClock is a groove-oriented time reference.", airwinconsolidated::SoftClock::kNumParameters, "2025-03-17", new airwinconsolidated::SoftClock::SoftClock(samplingRate) });
        this->RegisterAirwindow({ "SoftClock2", "Utility", 416, false, "SoftClock2 is a groove-oriented time reference.", airwinconsolidated::SoftClock2::kNumParameters, "2025-10-24", new airwinconsolidated::SoftClock2::SoftClock2(samplingRate) });
        this->RegisterAirwindow({ "SoftClock3", "Unclassified", -1, false, "", airwinconsolidated::SoftClock3::kNumParameters, "2026-01-18", new airwinconsolidated::SoftClock3::SoftClock3(samplingRate) });
        this->RegisterAirwindow({ "SoftGate", "Dynamics", 192, false, "SoftGate is a gate that can mute hiss and smooth sample tails.", airwinconsolidated::SoftGate::kNumParameters, "2022-11-21", new airwinconsolidated::SoftGate::SoftGate(samplingRate) });
        this->RegisterAirwindow({ "SpatializeDither", "Dithers", 160, true, "SpatializeDither is a high-performance clarity and accuracy dither.", airwinconsolidated::SpatializeDither::kNumParameters, "2022-11-21", new airwinconsolidated::SpatializeDither::SpatializeDither(samplingRate) });
        this->RegisterAirwindow({ "Spiral", "Saturation", 355, true, "Spiral is the new best smoothest distortion algorithm.", airwinconsolidated::Spiral::kNumParameters, "2022-11-21", new airwinconsolidated::Spiral::Spiral(samplingRate) });
        this->RegisterAirwindow({ "Spiral2", "Saturation", 350, true, "Spiral2 is Spiral with controls including Presence.", airwinconsolidated::Spiral2::kNumParameters, "2022-11-21", new airwinconsolidated::Spiral2::Spiral2(samplingRate) });
        this->RegisterAirwindow({ "SquareRoot", "Saturation", 343, true, "SquareRoot has new discoveries in soft saturation!", airwinconsolidated::SquareRoot::kNumParameters, "2025-02-16", new airwinconsolidated::SquareRoot::SquareRoot(samplingRate) });
        this->RegisterAirwindow({ "Srsly", "Stereo", 361, false, "Srsly is a psychoacoustic stereo processor.", airwinconsolidated::Srsly::kNumParameters, "2022-11-21", new airwinconsolidated::Srsly::Srsly(samplingRate) });
        this->RegisterAirwindow({ "Srsly2", "Stereo", 360, false, "Srsly2 is a revisit of Srsly, to make the stereo widening more extreme.", airwinconsolidated::Srsly2::kNumParameters, "2022-11-21", new airwinconsolidated::Srsly2::Srsly2(samplingRate) });
        this->RegisterAirwindow({ "Srsly3", "Stereo", 359, false, "Srsly3 is Srsly2, with a Nonlin control to analogify the filters.", airwinconsolidated::Srsly3::kNumParameters, "2024-05-19", new airwinconsolidated::Srsly3::Srsly3(samplingRate) });
        this->RegisterAirwindow({ "StarChild", "Ambience", 16, false, "StarChild is a weird digital ambience/echo plugin.", airwinconsolidated::StarChild::kNumParameters, "2022-11-21", new airwinconsolidated::StarChild::StarChild(samplingRate) });
        this->RegisterAirwindow({ "StarChild2", "Ambience", 15, false, "StarChild2 is a weird digital ambience/echo plugin adapted to high sample rates.", airwinconsolidated::StarChild2::kNumParameters, "2023-04-23", new airwinconsolidated::StarChild2::StarChild2(samplingRate) });
        this->RegisterAirwindow({ "StereoChorus", "Ambience", 12, false, "StereoChorus is a nice basic stereo chorus.", airwinconsolidated::StereoChorus::kNumParameters, "2022-11-21", new airwinconsolidated::StereoChorus::StereoChorus(samplingRate) });
        this->RegisterAirwindow({ "StereoDoubler", "Ambience", 21, false, "StereoDoubler is like GlitchShifter optimized for pitch shift doubling and tripling.", airwinconsolidated::StereoDoubler::kNumParameters, "2022-11-21", new airwinconsolidated::StereoDoubler::StereoDoubler(samplingRate) });
        this->RegisterAirwindow({ "StereoEnsemble", "Ambience", 20, false, "StereoEnsemble is a sort of hyperchorus blast from the past.", airwinconsolidated::StereoEnsemble::kNumParameters, "2022-11-21", new airwinconsolidated::StereoEnsemble::StereoEnsemble(samplingRate) });
        this->RegisterAirwindow({ "StereoFX", "Stereo", 364, false, "StereoFX is an aggressive stereo widener.", airwinconsolidated::StereoFX::kNumParameters, "2022-11-21", new airwinconsolidated::StereoFX::StereoFX(samplingRate) });
        this->RegisterAirwindow({ "StoneFireComp", "Dynamics", 180, false, "StoneFireComp is some of the dynamics for ConsoleX.", airwinconsolidated::StoneFireComp::kNumParameters, "2024-05-25", new airwinconsolidated::StoneFireComp::StoneFireComp(samplingRate) });
        this->RegisterAirwindow({ "Stonefire", "Filter", 241, true, "Stonefire is the non-EQ EQ designed for ConsoleX.", airwinconsolidated::Stonefire::kNumParameters, "2024-04-06", new airwinconsolidated::Stonefire::Stonefire(samplingRate) });
        this->RegisterAirwindow({ "StudioTan", "Dithers", 166, true, "StudioTan is all the 'non-dither' dithers, up to date and convenient.", airwinconsolidated::StudioTan::kNumParameters, "2022-11-21", new airwinconsolidated::StudioTan::StudioTan(samplingRate) });
        this->RegisterAirwindow({ "SubTight", "Filter", 252, true, "SubTight uses a variation on the Creature algorithm to tighten sub-lows.", airwinconsolidated::SubTight::kNumParameters, "2023-11-26", new airwinconsolidated::SubTight::SubTight(samplingRate) });
        this->RegisterAirwindow({ "SubsOnly", "Utility", 433, true, "SubsOnly is a mix check plugin that shows you only the extreme lows.", airwinconsolidated::SubsOnly::kNumParameters, "2022-11-21", new airwinconsolidated::SubsOnly::SubsOnly(samplingRate) });
        this->RegisterAirwindow({ "Surge", "Dynamics", 205, false, "Surge is a compressor for accentuating beats and pulses.", airwinconsolidated::Surge::kNumParameters, "2022-11-21", new airwinconsolidated::Surge::Surge(samplingRate) });
        this->RegisterAirwindow({ "SurgeTide", "Dynamics", 206, false, "SurgeTide is a surge and flow dynamics plugin.", airwinconsolidated::SurgeTide::kNumParameters, "2022-11-21", new airwinconsolidated::SurgeTide::SurgeTide(samplingRate) });
        this->RegisterAirwindow({ "SweetWide", "Stereo", 362, false, "SweetWide is a strange grungy stereo widener.", airwinconsolidated::SweetWide::kNumParameters, "2025-02-16", new airwinconsolidated::SweetWide::SweetWide(samplingRate) });
        this->RegisterAirwindow({ "Sweeten", "Subtlety", 378, true, "Sweeten is where you can find super-clean second harmonic.", airwinconsolidated::Sweeten::kNumParameters, "2023-08-19", new airwinconsolidated::Sweeten::Sweeten(samplingRate) });
        this->RegisterAirwindow({ "Swell", "Dynamics", 202, true, "Swell is Dial-an-attack, like sidechaining.", airwinconsolidated::Swell::kNumParameters, "2022-11-21", new airwinconsolidated::Swell::Swell(samplingRate) });
        this->RegisterAirwindow({ "TPDFDither", "Dithers", 155, true, "TPDFDither is TPDF dither. With 16/24 bit output and a DeRez control.", airwinconsolidated::TPDFDither::kNumParameters, "2022-11-21", new airwinconsolidated::TPDFDither::TPDFDither(samplingRate) });
        this->RegisterAirwindow({ "TPDFWide", "Dithers", 154, false, "TPDFWide is still TPDF dither but better and wider. With 16/24 bit output and a DeRez control.", airwinconsolidated::TPDFWide::kNumParameters, "2022-11-21", new airwinconsolidated::TPDFWide::TPDFWide(samplingRate) });
        this->RegisterAirwindow({ "TakeCare", "Effects", 207, false, "TakeCare is a lush chorus ensemble universe.", airwinconsolidated::TakeCare::kNumParameters, "2025-08-09", new airwinconsolidated::TakeCare::TakeCare(samplingRate) });
        this->RegisterAirwindow({ "Tape", "Tape", 396, true, "Tape is simplified, all-purpose tape mojo: my personal jam.", airwinconsolidated::Tape::kNumParameters, "2022-11-21", new airwinconsolidated::Tape::Tape(samplingRate) });
        this->RegisterAirwindow({ "TapeBias", "Lo-Fi", 290, true, "TapeBias is a new piece of ToTape allowing you to underbias or overbias.", airwinconsolidated::TapeBias::kNumParameters, "2024-07-28", new airwinconsolidated::TapeBias::TapeBias(samplingRate) });
        this->RegisterAirwindow({ "TapeDelay", "Ambience", 14, true, "TapeDelay is an old school tape echo with pitch swerve effects.", airwinconsolidated::TapeDelay::kNumParameters, "2022-11-21", new airwinconsolidated::TapeDelay::TapeDelay(samplingRate) });
        this->RegisterAirwindow({ "TapeDelay2", "Ambience", 1, true, "TapeDelay2 is a new implementation of a flexible tape echo.", airwinconsolidated::TapeDelay2::kNumParameters, "2022-11-21", new airwinconsolidated::TapeDelay2::TapeDelay2(samplingRate) });
        this->RegisterAirwindow({ "TapeDither", "Dithers", 159, true, "TapeDither is TPDF dither with noise like reel-to-reel tape.", airwinconsolidated::TapeDither::kNumParameters, "2022-11-21", new airwinconsolidated::TapeDither::TapeDither(samplingRate) });
        this->RegisterAirwindow({ "TapeDust", "Noise", 305, true, "TapeDust is just a special treble-erode noise, a ‘slew noise’ plugin.", airwinconsolidated::TapeDust::kNumParameters, "2022-11-21", new airwinconsolidated::TapeDust::TapeDust(samplingRate) });
        this->RegisterAirwindow({ "TapeFat", "Filter", 270, true, "TapeFat is the tone control from TapeDelay.", airwinconsolidated::TapeFat::kNumParameters, "2022-11-21", new airwinconsolidated::TapeFat::TapeFat(samplingRate) });
        this->RegisterAirwindow({ "TapeHack", "Tape", 391, true, "TapeHack is a new dimension in tape realism, abstracted to software.", airwinconsolidated::TapeHack::kNumParameters, "2025-08-16", new airwinconsolidated::TapeHack::TapeHack(samplingRate) });
        this->RegisterAirwindow({ "TapeHack2", "Tape", 390, true, "TapeHack2 brings Airwindows tape to a new level.", airwinconsolidated::TapeHack2::kNumParameters, "2026-01-18", new airwinconsolidated::TapeHack2::TapeHack2(samplingRate) });
        this->RegisterAirwindow({ "Texturize", "Noise", 299, true, "Texturize is a hidden-noise plugin for adding sonic texture to things.", airwinconsolidated::Texturize::kNumParameters, "2022-11-21", new airwinconsolidated::Texturize::Texturize(samplingRate) });
        this->RegisterAirwindow({ "TexturizeMS", "Noise", 300, false, "TexturizeMS is a hidden-noise plugin for adding mid-side sonic texture to things.", airwinconsolidated::TexturizeMS::kNumParameters, "2022-11-21", new airwinconsolidated::TexturizeMS::TexturizeMS(samplingRate) });
        this->RegisterAirwindow({ "Thunder", "Dynamics", 193, false, "Thunder is a compressor that retains or exaggerates subsonic bass when you push it.", airwinconsolidated::Thunder::kNumParameters, "2022-11-21", new airwinconsolidated::Thunder::Thunder(samplingRate) });
        this->RegisterAirwindow({ "ToTape5", "Tape", 399, true, "ToTape5 is Airwindows analog tape emulation.", airwinconsolidated::ToTape5::kNumParameters, "2022-11-21", new airwinconsolidated::ToTape5::ToTape5(samplingRate) });
        this->RegisterAirwindow({ "ToTape6", "Tape", 394, true, "ToTape6 is Airwindows tape emulation for 2020!", airwinconsolidated::ToTape6::kNumParameters, "2022-11-21", new airwinconsolidated::ToTape6::ToTape6(samplingRate) });
        this->RegisterAirwindow({ "ToTape7", "Tape", 393, false, "ToTape7 is Airwindows tape emulation with bias and Dubly.", airwinconsolidated::ToTape7::kNumParameters, "2024-08-18", new airwinconsolidated::ToTape7::ToTape7(samplingRate) });
        this->RegisterAirwindow({ "ToTape8", "Tape", 392, true, "ToTape8 is Airwindows tape emulation with gain staging, bias and optimized Dubly!", airwinconsolidated::ToTape8::kNumParameters, "2024-09-22", new airwinconsolidated::ToTape8::ToTape8(samplingRate) });
        this->RegisterAirwindow({ "ToVinyl4", "Stereo", 365, false, "ToVinyl4 is a vinyl-mastering simulator bringing several vinyl-type colors.", airwinconsolidated::ToVinyl4::kNumParameters, "2022-11-21", new airwinconsolidated::ToVinyl4::ToVinyl4(samplingRate) });
        this->RegisterAirwindow({ "ToneSlant", "Filter", 260, true, "ToneSlant is a super-transparent ‘tilt EQ’ with very low Q.", airwinconsolidated::ToneSlant::kNumParameters, "2022-11-21", new airwinconsolidated::ToneSlant::ToneSlant(samplingRate) });
        this->RegisterAirwindow({ "TransDesk", "Subtlety", 388, true, "TransDesk is more of a transistory, rock desk analog modeling.", airwinconsolidated::TransDesk::kNumParameters, "2022-11-21", new airwinconsolidated::TransDesk::TransDesk(samplingRate) });
        this->RegisterAirwindow({ "TremoSquare", "Effects", 222, true, "TremoSquare is a squarewave tremolo effect that only switches on zero crossings.", airwinconsolidated::TremoSquare::kNumParameters, "2022-11-21", new airwinconsolidated::TremoSquare::TremoSquare(samplingRate) });
        this->RegisterAirwindow({ "Tremolo", "Effects", 223, true, "Tremolo is fluctuating saturation curves for a tubey tremolo.", airwinconsolidated::Tremolo::kNumParameters, "2022-11-21", new airwinconsolidated::Tremolo::Tremolo(samplingRate) });
        this->RegisterAirwindow({ "Trianglizer", "Effects", 218, true, "Trianglizer is a special waveshaper that can transform sines into triangles.", airwinconsolidated::Trianglizer::kNumParameters, "2024-03-10", new airwinconsolidated::Trianglizer::Trianglizer(samplingRate) });
        this->RegisterAirwindow({ "TripleSpread", "Ambience", 6, false, "TripleSpread is a stereo tripler with extra wideness and GlitchShifter processing.", airwinconsolidated::TripleSpread::kNumParameters, "2022-11-21", new airwinconsolidated::TripleSpread::TripleSpread(samplingRate) });
        this->RegisterAirwindow({ "Tube", "Saturation", 349, true, "Tube is a tube style SoundBetterIzer using a new algorithm for analog modeling!", airwinconsolidated::Tube::kNumParameters, "2022-11-21", new airwinconsolidated::Tube::Tube(samplingRate) });
        this->RegisterAirwindow({ "Tube2", "Saturation", 348, true, "Tube2 extends Tube, with more sophisticated processing and input trim.", airwinconsolidated::Tube2::kNumParameters, "2022-11-21", new airwinconsolidated::Tube2::Tube2(samplingRate) });
        this->RegisterAirwindow({ "TubeDesk", "Subtlety", 389, true, "TubeDesk is a tube recording console type tone coloring.", airwinconsolidated::TubeDesk::kNumParameters, "2022-11-21", new airwinconsolidated::TubeDesk::TubeDesk(samplingRate) });
        this->RegisterAirwindow({ "UltrasonX", "Brightness", 84, true, "UltrasonX is a method for rolling your own Console-type systems with total control over your ultrasonic filtering.", airwinconsolidated::UltrasonX::kNumParameters, "2022-11-21", new airwinconsolidated::UltrasonX::UltrasonX(samplingRate) });
        this->RegisterAirwindow({ "Ultrasonic", "Brightness", 81, true, "Ultrasonic is a very clean, plain, high quality supersonic filter, for using inside digital mixes.", airwinconsolidated::Ultrasonic::kNumParameters, "2022-11-21", new airwinconsolidated::Ultrasonic::Ultrasonic(samplingRate) });
        this->RegisterAirwindow({ "UltrasonicLite", "Brightness", 82, true, "UltrasonicLite is just one stage of supersonic filter, for using inside digital mixes.", airwinconsolidated::UltrasonicLite::kNumParameters, "2022-11-21", new airwinconsolidated::UltrasonicLite::UltrasonicLite(samplingRate) });
        this->RegisterAirwindow({ "UltrasonicMed", "Brightness", 83, true, "UltrasonicMed is two stages of supersonic filter, for using inside digital mixes.", airwinconsolidated::UltrasonicMed::kNumParameters, "2022-11-21", new airwinconsolidated::UltrasonicMed::UltrasonicMed(samplingRate) });
        this->RegisterAirwindow({ "UnBox", "Saturation", 356, true, "UnBox is a distortion where only the harmonics that don't alias are allowed to distort.", airwinconsolidated::UnBox::kNumParameters, "2022-11-21", new airwinconsolidated::UnBox::UnBox(samplingRate) });
        this->RegisterAirwindow({ "VariMu", "Dynamics", 185, true, "VariMu is a more organic variation on Pressure (a compressor)", airwinconsolidated::VariMu::kNumParameters, "2022-11-21", new airwinconsolidated::VariMu::VariMu(samplingRate) });
        this->RegisterAirwindow({ "VerbSixes", "Reverb", 315, false, "VerbSixes is a calibrated reference reverb plugin for Householder matrices.", airwinconsolidated::VerbSixes::kNumParameters, "2025-06-14", new airwinconsolidated::VerbSixes::VerbSixes(samplingRate) });
        this->RegisterAirwindow({ "VerbThic", "Reverb", 311, false, "VerbThic is a classic artificial reverb for being opaque and textured.", airwinconsolidated::VerbThic::kNumParameters, "2025-11-28", new airwinconsolidated::VerbThic::VerbThic(samplingRate) });
        this->RegisterAirwindow({ "VerbTiny", "Reverb", 310, false, "VerbTiny is a classic artificial reverb that expands reverb shape.", airwinconsolidated::VerbTiny::kNumParameters, "2025-11-28", new airwinconsolidated::VerbTiny::VerbTiny(samplingRate) });
        this->RegisterAirwindow({ "Verbity", "Reverb", 332, true, "Verbity is a dual-mono reverb, which uses feedforward reverb topology.", airwinconsolidated::Verbity::kNumParameters, "2022-11-21", new airwinconsolidated::Verbity::Verbity(samplingRate) });
        this->RegisterAirwindow({ "Verbity2", "Reverb", 329, false, "Verbity2 adds stereo crossmodulation and expands Verbity's feedforward reverb topology.", airwinconsolidated::Verbity2::kNumParameters, "2023-02-05", new airwinconsolidated::Verbity2::Verbity2(samplingRate) });
        this->RegisterAirwindow({ "Vibrato", "Lo-Fi", 293, true, "Vibrato lets you vibrato, chorus, flange, and make odd FM noises.", airwinconsolidated::Vibrato::kNumParameters, "2022-11-21", new airwinconsolidated::Vibrato::Vibrato(samplingRate) });
        this->RegisterAirwindow({ "VinylDither", "Dithers", 161, true, "VinylDither is a high-performance dither that converts digital noise to ‘groove noise’.", airwinconsolidated::VinylDither::kNumParameters, "2022-11-21", new airwinconsolidated::VinylDither::VinylDither(samplingRate) });
        this->RegisterAirwindow({ "VoiceOfTheStarship", "Noise", 301, true, "VoiceOfTheStarship is a deep noise tone source.", airwinconsolidated::VoiceOfTheStarship::kNumParameters, "2022-11-21", new airwinconsolidated::VoiceOfTheStarship::VoiceOfTheStarship(samplingRate) });
        this->RegisterAirwindow({ "VoiceTrick", "Utility", 448, false, "VoiceTrick lets you record vocals while monitoring over speakers.", airwinconsolidated::VoiceTrick::kNumParameters, "2022-11-21", new airwinconsolidated::VoiceTrick::VoiceTrick(samplingRate) });
        this->RegisterAirwindow({ "Weight", "Filter", 254, true, "Weight is a very accurate sub-bass boost based on Holt.", airwinconsolidated::Weight::kNumParameters, "2023-02-10", new airwinconsolidated::Weight::Weight(samplingRate) });
        this->RegisterAirwindow({ "Wider", "Stereo", 363, false, "Wider is Airwindows stereo space shaping.", airwinconsolidated::Wider::kNumParameters, "2022-11-21", new airwinconsolidated::Wider::Wider(samplingRate) });
        this->RegisterAirwindow({ "Wolfbot", "Amp Sims", 34, true, "Wolfbot is an aggressive Kalman bandpass with evil in its heart.", airwinconsolidated::Wolfbot::kNumParameters, "2024-05-25", new airwinconsolidated::Wolfbot::Wolfbot(samplingRate) });
        this->RegisterAirwindow({ "X2Buss", "Unclassified", -1, false, "", airwinconsolidated::X2Buss::kNumParameters, "2025-11-02", new airwinconsolidated::X2Buss::X2Buss(samplingRate) });
        this->RegisterAirwindow({ "XBandpass", "XYZ Filters", 469, true, "XBandpass is a distorted digital EQ, inspired by retro sampler DSP.", airwinconsolidated::XBandpass::kNumParameters, "2022-11-21", new airwinconsolidated::XBandpass::XBandpass(samplingRate) });
        this->RegisterAirwindow({ "XHighpass", "XYZ Filters", 470, true, "XHighpass is a distorted digital EQ, inspired by retro sampler DSP.", airwinconsolidated::XHighpass::kNumParameters, "2022-11-21", new airwinconsolidated::XHighpass::XHighpass(samplingRate) });
        this->RegisterAirwindow({ "XLowpass", "XYZ Filters", 471, true, "XLowpass is a distorted digital EQ, inspired by retro sampler DSP.", airwinconsolidated::XLowpass::kNumParameters, "2022-11-21", new airwinconsolidated::XLowpass::XLowpass(samplingRate) });
        this->RegisterAirwindow({ "XNotch", "XYZ Filters", 472, true, "XNotch is a distorted digital EQ, not as glitchy as the others.", airwinconsolidated::XNotch::kNumParameters, "2022-11-21", new airwinconsolidated::XNotch::XNotch(samplingRate) });
        this->RegisterAirwindow({ "XRegion", "XYZ Filters", 473, true, "XRegion is distorted staggered bandpasses, for extreme soundmangling.", airwinconsolidated::XRegion::kNumParameters, "2022-11-21", new airwinconsolidated::XRegion::XRegion(samplingRate) });
        this->RegisterAirwindow({ "YBandpass", "XYZ Filters", 461, true, "YBandpass is soft and smooth to nasty, edgy texture-varying filtering.", airwinconsolidated::YBandpass::kNumParameters, "2022-11-21", new airwinconsolidated::YBandpass::YBandpass(samplingRate) });
        this->RegisterAirwindow({ "YHighpass", "XYZ Filters", 463, true, "YHighpass is soft and smooth to nasty, edgy texture-varying filtering.", airwinconsolidated::YHighpass::kNumParameters, "2022-11-21", new airwinconsolidated::YHighpass::YHighpass(samplingRate) });
        this->RegisterAirwindow({ "YLowpass", "XYZ Filters", 465, true, "YLowpass is soft and smooth to nasty, edgy texture-varying filtering.", airwinconsolidated::YLowpass::kNumParameters, "2022-11-21", new airwinconsolidated::YLowpass::YLowpass(samplingRate) });
        this->RegisterAirwindow({ "YNotBandpass", "XYZ Filters", 462, true, "YNotBandpass is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwinconsolidated::YNotBandpass::kNumParameters, "2023-10-01", new airwinconsolidated::YNotBandpass::YNotBandpass(samplingRate) });
        this->RegisterAirwindow({ "YNotHighpass", "XYZ Filters", 464, true, "YNotHighpass is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwinconsolidated::YNotHighpass::kNumParameters, "2023-10-01", new airwinconsolidated::YNotHighpass::YNotHighpass(samplingRate) });
        this->RegisterAirwindow({ "YNotLowpass", "XYZ Filters", 466, true, "YNotLowpass is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwinconsolidated::YNotLowpass::kNumParameters, "2023-10-01", new airwinconsolidated::YNotLowpass::YNotLowpass(samplingRate) });
        this->RegisterAirwindow({ "YNotNotch", "XYZ Filters", 468, true, "YNotNotch is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwinconsolidated::YNotNotch::kNumParameters, "2023-10-01", new airwinconsolidated::YNotNotch::YNotNotch(samplingRate) });
        this->RegisterAirwindow({ "YNotch", "XYZ Filters", 467, true, "YNotch is soft and smooth to nasty, edgy texture-varying filtering.", airwinconsolidated::YNotch::kNumParameters, "2022-11-21", new airwinconsolidated::YNotch::YNotch(samplingRate) });
        this->RegisterAirwindow({ "ZBandpass", "XYZ Filters", 456, true, "ZBandpass is a bandpass made to sound and act like the Emu e6400 Ultra bandpass.", airwinconsolidated::ZBandpass::kNumParameters, "2022-11-21", new airwinconsolidated::ZBandpass::ZBandpass(samplingRate) });
        this->RegisterAirwindow({ "ZBandpass2", "XYZ Filters", 451, true, "ZBandpass2 acts more like the Emu e6400 Ultra bandpass in motion, with control smoothing.", airwinconsolidated::ZBandpass2::kNumParameters, "2022-11-21", new airwinconsolidated::ZBandpass2::ZBandpass2(samplingRate) });
        this->RegisterAirwindow({ "ZHighpass", "XYZ Filters", 457, true, "ZHighpass is a highpass made to sound and act like the Emu e6400 Ultra highpass.", airwinconsolidated::ZHighpass::kNumParameters, "2022-11-21", new airwinconsolidated::ZHighpass::ZHighpass(samplingRate) });
        this->RegisterAirwindow({ "ZHighpass2", "XYZ Filters", 452, true, "ZHighpass2 acts more like the Emu e6400 Ultra highpass in motion, with control smoothing.", airwinconsolidated::ZHighpass2::kNumParameters, "2022-11-21", new airwinconsolidated::ZHighpass2::ZHighpass2(samplingRate) });
        this->RegisterAirwindow({ "ZLowpass", "XYZ Filters", 458, true, "ZLowpass is a lowpass made to sound and act like the Emu e6400 Ultra lowpass.", airwinconsolidated::ZLowpass::kNumParameters, "2022-11-21", new airwinconsolidated::ZLowpass::ZLowpass(samplingRate) });
        this->RegisterAirwindow({ "ZLowpass2", "XYZ Filters", 453, true, "ZLowpass2 acts more like the Emu e6400 Ultra lowpass in motion, with control smoothing.", airwinconsolidated::ZLowpass2::kNumParameters, "2022-11-21", new airwinconsolidated::ZLowpass2::ZLowpass2(samplingRate) });
        this->RegisterAirwindow({ "ZNotch", "XYZ Filters", 459, true, "ZNotch is a notch filter made to sound and act like the Emu e6400 Phaser.", airwinconsolidated::ZNotch::kNumParameters, "2022-11-21", new airwinconsolidated::ZNotch::ZNotch(samplingRate) });
        this->RegisterAirwindow({ "ZNotch2", "XYZ Filters", 454, true, "ZNotch2 acts more like the Emu e6400 Ultra phaser in motion, with control smoothing.", airwinconsolidated::ZNotch2::kNumParameters, "2022-11-21", new airwinconsolidated::ZNotch2::ZNotch2(samplingRate) });
        this->RegisterAirwindow({ "ZOutputStage", "Distortion", 142, true, "ZOutputStage is the output clipping from the Emu e6400 style Z filters.", airwinconsolidated::ZOutputStage::kNumParameters, "2023-12-21", new airwinconsolidated::ZOutputStage::ZOutputStage(samplingRate) });
        this->RegisterAirwindow({ "ZRegion", "XYZ Filters", 460, true, "ZRegion is an Emu e6400 style Airwindows Region filter.", airwinconsolidated::ZRegion::kNumParameters, "2022-11-21", new airwinconsolidated::ZRegion::ZRegion(samplingRate) });
        this->RegisterAirwindow({ "ZRegion2", "XYZ Filters", 455, true, "ZRegion2 is an Emu e6400 style Airwindows Region filter, with control smoothing.", airwinconsolidated::ZRegion2::kNumParameters, "2022-11-21", new airwinconsolidated::ZRegion2::ZRegion2(samplingRate) });
        this->RegisterAirwindow({ "Zoom", "Distortion", 144, true, "Zoom is a bank of Density-like waveshapers for subtly modulating peak energy.", airwinconsolidated::Zoom::kNumParameters, "2024-10-13", new airwinconsolidated::Zoom::Zoom(samplingRate) });
        this->RegisterAirwindow({ "curve", "Dynamics", 187, false, "curve is the simplest, purest form of Recurve with no extra boosts.", airwinconsolidated::curve::kNumParameters, "2022-11-21", new airwinconsolidated::curve::curve(samplingRate) });
        this->RegisterAirwindow({ "kAlienSpaceship", "Reverb", 312, false, "kAlienSpaceship is an unreal realistic reverb.", airwinconsolidated::kAlienSpaceship::kNumParameters, "2025-08-09", new airwinconsolidated::kAlienSpaceship::kAlienSpaceship(samplingRate) });
        this->RegisterAirwindow({ "kBeyond", "Reverb", 314, false, "kBeyond is a recital hall.", airwinconsolidated::kBeyond::kNumParameters, "2025-06-21", new airwinconsolidated::kBeyond::kBeyond(samplingRate) });
        this->RegisterAirwindow({ "kCathedral", "Reverb", 337, false, "kCathedral is a giant reverby space.", airwinconsolidated::kCathedral::kNumParameters, "2023-10-28", new airwinconsolidated::kCathedral::kCathedral(samplingRate) });
        this->RegisterAirwindow({ "kCathedral2", "Reverb", 328, false, "kCathedral2 is a giant reverby space modeled after the Bricasti Cathedral.", airwinconsolidated::kCathedral2::kNumParameters, "2024-02-10", new airwinconsolidated::kCathedral2::kCathedral2(samplingRate) });
        this->RegisterAirwindow({ "kCathedral3", "Reverb", 318, false, "kCathedral3 is a giant cathedral-like space using Bezier undersampling.", airwinconsolidated::kCathedral3::kNumParameters, "2024-06-23", new airwinconsolidated::kCathedral3::kCathedral3(samplingRate) });
        this->RegisterAirwindow({ "kCathedral4", "Reverb", 313, false, "kCathedral4 is the Cathedral sound expressed as distant space.", airwinconsolidated::kCathedral4::kNumParameters, "2025-07-19", new airwinconsolidated::kCathedral4::kCathedral4(samplingRate) });
        this->RegisterAirwindow({ "kCathedral5", "Reverb", 309, false, "kCathedral5 lets you place yourself anywhere you like in the space.", airwinconsolidated::kCathedral5::kNumParameters, "2025-08-09", new airwinconsolidated::kCathedral5::kCathedral5(samplingRate) });
        this->RegisterAirwindow({ "kChamberAR", "Ambience", 11, false, "kChamberAR is a take on tape echo into chamber echo.", airwinconsolidated::kChamberAR::kNumParameters, "2023-05-13", new airwinconsolidated::kChamberAR::kChamberAR(samplingRate) });
        this->RegisterAirwindow({ "kCosmos", "Reverb", 316, false, "kCosmos is infinite space ambient, or titanic hall.", airwinconsolidated::kCosmos::kNumParameters, "2025-05-10", new airwinconsolidated::kCosmos::kCosmos(samplingRate) });
        this->RegisterAirwindow({ "kGuitarHall", "Reverb", 322, false, "kGuitarHall accentuates loud midrange skronk!", airwinconsolidated::kGuitarHall::kNumParameters, "2024-06-23", new airwinconsolidated::kGuitarHall::kGuitarHall(samplingRate) });
        this->RegisterAirwindow({ "kGuitarHall2", "Reverb", 308, false, "kGuitarHall2 puts things in a Loud Room.", airwinconsolidated::kGuitarHall2::kNumParameters, "2025-08-09", new airwinconsolidated::kGuitarHall2::kGuitarHall2(samplingRate) });
        this->RegisterAirwindow({ "kPlate140", "Reverb", 319, false, "kPlate140 is a next-generation Airwindows plate reverb.", airwinconsolidated::kPlate140::kNumParameters, "2024-06-23", new airwinconsolidated::kPlate140::kPlate140(samplingRate) });
        this->RegisterAirwindow({ "kPlate240", "Reverb", 320, false, "kPlate240 is for the texture of smaller, gold foil reverb.", airwinconsolidated::kPlate240::kNumParameters, "2024-06-23", new airwinconsolidated::kPlate240::kPlate240(samplingRate) });
        this->RegisterAirwindow({ "kPlateA", "Reverb", 325, false, "kPlateA is a plate reverb, not unlike its namesake atop Abbey Road.", airwinconsolidated::kPlateA::kNumParameters, "2023-03-31", new airwinconsolidated::kPlateA::kPlateA(samplingRate) });
        this->RegisterAirwindow({ "kPlateB", "Reverb", 324, false, "kPlateB is a plate reverb, not unlike its namesake atop Abbey Road.", airwinconsolidated::kPlateB::kNumParameters, "2023-04-09", new airwinconsolidated::kPlateB::kPlateB(samplingRate) });
        this->RegisterAirwindow({ "kPlateC", "Reverb", 326, false, "kPlateC is a plate reverb, not unlike its namesake atop Abbey Road.", airwinconsolidated::kPlateC::kNumParameters, "2023-04-09", new airwinconsolidated::kPlateC::kPlateC(samplingRate) });
        this->RegisterAirwindow({ "kPlateD", "Reverb", 323, false, "kPlateD is a plate reverb, not unlike its namesake atop Abbey Road.", airwinconsolidated::kPlateD::kNumParameters, "2023-04-09", new airwinconsolidated::kPlateD::kPlateD(samplingRate) });
        this->RegisterAirwindow({ "kStation", "Reverb", 307, false, "kStation is a realistic small room modeled after David Bowie's vocal reverb.", airwinconsolidated::kStation::kNumParameters, "2025-09-20", new airwinconsolidated::kStation::kStation(samplingRate) });
        this->RegisterAirwindow({ "kWoodRoom", "Reverb", 306, false, "kWoodRoom is a small wooden performing space.", airwinconsolidated::kWoodRoom::kNumParameters, "2025-11-28", new airwinconsolidated::kWoodRoom::kWoodRoom(samplingRate) });
        this->RegisterAirwindow({ "uLawDecode", "Utility", 442, true, "uLawDecode is a Console-like encode/decode pair, but much more extreme.", airwinconsolidated::uLawDecode::kNumParameters, "2022-11-21", new airwinconsolidated::uLawDecode::uLawDecode(samplingRate) });
        this->RegisterAirwindow({ "uLawEncode", "Utility", 441, true, "uLawEncode is a Console-like encode/decode pair, but much more extreme.", airwinconsolidated::uLawEncode::kNumParameters, "2022-11-21", new airwinconsolidated::uLawEncode::uLawEncode(samplingRate) });
    }
    catch (std::exception ex)   // This probably won't catch low level assertions
    {
        return false;
    }

    return true;
}

AirwinRegistryEntry* AirwinRegistry::GetPlugin(const std::string& airwinPluginName) const
{
    if (!_registry->contains(airwinPluginName))
        throw new std::exception("Airwin plugin not found in registry");

    return _registry->at(airwinPluginName);
}

void AirwinRegistry::GetPlugins(std::vector<std::string>& destination)
{
    for (auto iter = _registry->begin(); iter != _registry->end(); ++iter)
    {
        destination.push_back(iter->first);
    }
}

void AirwinRegistry::GetCategories(std::vector<std::string>& destination)
{
    for (auto iter = _byCategory->begin(); iter != _byCategory->end(); ++iter)
    {
        destination.push_back(iter->first);
    }
}

void AirwinRegistry::GetCategoryList(std::vector<std::string>& destination, const std::string& category)
{
    if (!_byCategory->contains(category))
        throw new std::exception("Category not found in AirwinRegistry. Please use GetCategories() to return a full list of categories");

    for (auto iter = _byCategory->at(category)->begin(); iter != _byCategory->at(category)->end(); ++iter)
    {
        destination.push_back((*iter)->GetName());
    }
}

void AirwinRegistry::RegisterAirwindow(const AirwinRegistryEntry& entry)
{
    if (_registry->contains(entry.GetName()))
        throw new std::exception("Duplicate Airwindow Effect");

    // Airwindow Audio Effect:  These pointers are created above, are sittiing on the
    //                          heap, and are passed in the copy constructor. The 
    //                          destructor for this registry will delete them.

    AirwinRegistryEntry* entryHeap = new AirwinRegistryEntry(entry);

    // Primary Registry
    _registry->insert(std::make_pair(entry.GetName(), entryHeap));

    // Category
    if (!_byCategory->contains(entry.GetCategory()))
        _byCategory->insert(std::make_pair(entry.GetCategory(), new std::vector<AirwinRegistryEntry*>()));

    _byCategory->at(entry.GetCategory())->push_back(entryHeap);
}