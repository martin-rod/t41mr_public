/* A simple sketch to verify correct audio transfer in both receive and transmit directions.
 *  Transmits a 700Hz and 1000Hz tone on the Left and Right channels.
 *  Received audio is played on the Audio Shield's Left and Right DAC outputs
 */

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <utility/imxrt_hw.h> // needed for SetI2SFreq()
#include <usb_audio.h>

AudioInputUSB            usbIn;
AudioOutputUSB           usbOut;

AudioSynthWaveformSine   sine1;
AudioSynthWaveformSine   sine2;
AudioOutputI2SQuad       i2s_quad;

AudioConnection          patchCord1(usbIn, 0, i2s_quad, 0);
AudioConnection          patchCord2(usbIn, 1, i2s_quad, 1);
AudioConnection          patchCord3(sine1, 0, usbOut, 0);
AudioConnection          patchCord4(sine2, 0, usbOut, 1);

AudioControlSGTL5000     sgtl5000;

void printCPUandMemoryUsage(Stream *s) {
  static unsigned long curTime_millis = 0;
  static unsigned long lastMemUpdate_millis=0;

  //print status information to the Serial port
  curTime_millis = millis();
  if ((curTime_millis - lastMemUpdate_millis) < 0) lastMemUpdate_millis=0;  //handle case where millis wraps around!

  if ((curTime_millis - lastMemUpdate_millis) > 2000) {  // print a summary of the current & maximum usage
    s->print("Usage/Max: ");
    s->print("oscillator CPU = "); s->print(sine1.processorUsage()); s->print("/"); s->print(sine1.processorUsageMax());s->print(", ");
    s->print("all CPU = " ); s->print(AudioProcessorUsage()); s->print("/");  s->print(AudioProcessorUsageMax());s->print(", ");
    s->print("Int16 Mem = ");s->print(AudioMemoryUsage()); s->print("/"); s->print(AudioMemoryUsageMax());s->print(", ");
    s->println();

    lastMemUpdate_millis = curTime_millis;

    SerialUSB1.println("The second USB serial port is also working");
  }
};

int SetI2SFreq(int freq) {  // extracted from T41 source code
  int n1;
  int n2 ;
  int c0;
  int c2;
  int c1;
  double C;

  // PLL between 27*24 = 648MHz und 54*24=1296MHz
  // Fudge to handle 8kHz - El Supremo
  if (freq > 8000) {
    n1 = 4; //SAI prescaler 4 => (n1*n2) = multiple of 4
  } else {
    n1 = 8;
  }
  n2 = 1 + (24000000 * 27) / (freq * 256 * n1);
  C = ((double)freq * 256 * n1 * n2) / 24000000;
  c0 = C;
  c2 = 10000;
  c1 = C * c2 - (c0 * c2);
  set_audioClock(c0, c1, c2, true);
  CCM_CS1CDR = (CCM_CS1CDR & ~(CCM_CS1CDR_SAI1_CLK_PRED_MASK | CCM_CS1CDR_SAI1_CLK_PODF_MASK))
               | CCM_CS1CDR_SAI1_CLK_PRED(n1 - 1) // &0x07
               | CCM_CS1CDR_SAI1_CLK_PODF(n2 - 1); // &0x3f

  CCM_CS2CDR = (CCM_CS2CDR & ~(CCM_CS2CDR_SAI2_CLK_PRED_MASK | CCM_CS2CDR_SAI2_CLK_PODF_MASK))
               | CCM_CS2CDR_SAI2_CLK_PRED(n1 - 1) // &0x07
               | CCM_CS2CDR_SAI2_CLK_PODF(n2 - 1); // &0x3f)
  return freq;
}

#define SAMPLE_RATE 192000

void setup() {
  delay(500);
  Serial.print("usb audio tests at "); Serial.print(SAMPLE_RATE); Serial.println("sps");

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(30);  //allocate Int16 audio data blocks

  // Enable the audio shield, no input, and enable output
  sgtl5000.enable();                   //start the audio board
  sgtl5000.volume(0.5);                //volume can be 0.0 to 1.0.  0.5 seems to be the usual default.

  sine1.frequency(700); //*44100/SAMPLE_RATE
  sine1.amplitude(0.5);

  sine2.frequency(1000); //*44100/SAMPLE_RATE
  sine2.amplitude(0.5);

  SetI2SFreq(SAMPLE_RATE); 
}

void loop() {
  printCPUandMemoryUsage(&Serial);
}

/*
Play audio from T41 to PC sound card:

arecord -D hw:CARD=Audio2xSerial,DEV=0  -v -f S16_LE -c2 -r192000  - | aplay

Record audio from T41 to WAV file:

arecord -D hw:CARD=Audio2xSerial,DEV=0  -v -f S16_LE -c2 -r192000  -d 10 -t wav 192k_test.wav

Play audio from PC to T41 Teensy Audio Adapter (sgtl5000):

aplay -D hw:CARD=Audio2xSerial,DEV=0 -v -f S16_LE -c2 -r192000 192k_test.wav


example:

rod@t440-d12:~/wrk_ham$ arecord -L
null
    Discard all samples (playback) or generate zero samples (capture)
lavrate
    Rate Converter Plugin Using Libav/FFmpeg Library
samplerate
    Rate Converter Plugin Using Samplerate Library
speexrate
    Rate Converter Plugin Using Speex Resampler
jack
    JACK Audio Connection Kit
oss
    Open Sound System
pipewire
    PipeWire Sound Server
pulse
    PulseAudio Sound Server
speex
    Plugin using Speex DSP (resample, agc, denoise, echo, dereverb)
upmix
    Plugin for channel upmix (4,6,8)
vdownmix
    Plugin for channel downmix (stereo) with a simple spacialization
default
    Default ALSA Output (currently PipeWire Media Server)
usbstream:CARD=HDMI
    HDA Intel HDMI
    USB Stream Output
hw:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Direct hardware device without any conversions
plughw:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Hardware device with all software conversions
sysdefault:CARD=PCH
    HDA Intel PCH, ALC3232 Analog
    Default Audio Device
front:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Front output / input
dsnoop:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Direct sample snooping device
usbstream:CARD=PCH
    HDA Intel PCH
    USB Stream Output
hw:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Direct hardware device without any conversions
hw:CARD=Loopback,DEV=1
    Loopback, Loopback PCM
    Direct hardware device without any conversions
plughw:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Hardware device with all software conversions
plughw:CARD=Loopback,DEV=1
    Loopback, Loopback PCM
    Hardware device with all software conversions
sysdefault:CARD=Loopback
    Loopback, Loopback PCM
    Default Audio Device
front:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Front output / input
dsnoop:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Direct sample snooping device
dsnoop:CARD=Loopback,DEV=1
    Loopback, Loopback PCM
    Direct sample snooping device
usbstream:CARD=Loopback
    Loopback
    USB Stream Output
hw:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Direct hardware device without any conversions
plughw:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Hardware device with all software conversions
sysdefault:CARD=Audio2xSerial
    192k Audio/2xSerial, USB Audio
    Default Audio Device
front:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Front output / input
dsnoop:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Direct sample snooping device
usbstream:CARD=Audio2xSerial
    192k Audio/2xSerial
    USB Stream Output
rod@t440-d12:~/wrk_ham$ arecord -D hw:CARD=Audio2xSerial,DEV=0  -v -f S16_LE -c2 -r192000  - | aplay
Recording WAVE '-' : Signed 16 bit Little Endian, Rate 44100 Hz, Stereo
Warning: rate is not accurate (requested = 44100Hz, got = 192000Hz)
         please, try the plug plugin
Hardware PCM card 3 '192k Audio/2xSerial' device 0 subdevice 0
Its setup is:
  stream       : CAPTURE
  access       : RW_INTERLEAVED
  format       : S16_LE
  subformat    : STD
  channels     : 2
  rate         : 192000
  exact rate   : 192000 (192000/1)
  msbits       : 16
  buffer_size  : 96000
  period_size  : 24000
  period_time  : 125000
  tstamp_mode  : NONE
  tstamp_type  : MONOTONIC
  period_step  : 1
  avail_min    : 24000
  period_event : 0
  start_threshold  : 1
  stop_threshold   : 96000
  silence_threshold: 0
  silence_size : 0
  boundary     : 6755399441055744000
  appl_ptr     : 0
  hw_ptr       : 0
Playing WAVE 'stdin' : Signed 16 bit Little Endian, Rate 192000 Hz, Stereo
^CAborted by signal Interrupt...
Aborted by signal Interrupt...
aplay: pcm_write:2127: write error: Interrupted system call

---

rod@t440-d12:~/wrk_ham$ aplay -L
null
    Discard all samples (playback) or generate zero samples (capture)
lavrate
    Rate Converter Plugin Using Libav/FFmpeg Library
samplerate
    Rate Converter Plugin Using Samplerate Library
speexrate
    Rate Converter Plugin Using Speex Resampler
jack
    JACK Audio Connection Kit
oss
    Open Sound System
pipewire
    PipeWire Sound Server
pulse
    PulseAudio Sound Server
speex
    Plugin using Speex DSP (resample, agc, denoise, echo, dereverb)
upmix
    Plugin for channel upmix (4,6,8)
vdownmix
    Plugin for channel downmix (stereo) with a simple spacialization
default
    Default ALSA Output (currently PipeWire Media Server)
hw:CARD=HDMI,DEV=3
    HDA Intel HDMI, B24W-6 LED
    Direct hardware device without any conversions
hw:CARD=HDMI,DEV=7
    HDA Intel HDMI, HDMI 1
    Direct hardware device without any conversions
hw:CARD=HDMI,DEV=8
    HDA Intel HDMI, HDMI 2
    Direct hardware device without any conversions
plughw:CARD=HDMI,DEV=3
    HDA Intel HDMI, B24W-6 LED
    Hardware device with all software conversions
plughw:CARD=HDMI,DEV=7
    HDA Intel HDMI, HDMI 1
    Hardware device with all software conversions
plughw:CARD=HDMI,DEV=8
    HDA Intel HDMI, HDMI 2
    Hardware device with all software conversions
hdmi:CARD=HDMI,DEV=0
    HDA Intel HDMI, B24W-6 LED
    HDMI Audio Output
hdmi:CARD=HDMI,DEV=1
    HDA Intel HDMI, HDMI 1
    HDMI Audio Output
hdmi:CARD=HDMI,DEV=2
    HDA Intel HDMI, HDMI 2
    HDMI Audio Output
dmix:CARD=HDMI,DEV=3
    HDA Intel HDMI, B24W-6 LED
    Direct sample mixing device
dmix:CARD=HDMI,DEV=7
    HDA Intel HDMI, HDMI 1
    Direct sample mixing device
dmix:CARD=HDMI,DEV=8
    HDA Intel HDMI, HDMI 2
    Direct sample mixing device
usbstream:CARD=HDMI
    HDA Intel HDMI
    USB Stream Output
hw:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Direct hardware device without any conversions
plughw:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Hardware device with all software conversions
sysdefault:CARD=PCH
    HDA Intel PCH, ALC3232 Analog
    Default Audio Device
front:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Front output / input
surround21:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    2.1 Surround output to Front and Subwoofer speakers
surround40:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    4.0 Surround output to Front and Rear speakers
surround41:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    4.1 Surround output to Front, Rear and Subwoofer speakers
surround50:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    5.0 Surround output to Front, Center and Rear speakers
surround51:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    5.1 Surround output to Front, Center, Rear and Subwoofer speakers
surround71:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    7.1 Surround output to Front, Center, Side, Rear and Woofer speakers
dmix:CARD=PCH,DEV=0
    HDA Intel PCH, ALC3232 Analog
    Direct sample mixing device
usbstream:CARD=PCH
    HDA Intel PCH
    USB Stream Output
hw:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Direct hardware device without any conversions
hw:CARD=Loopback,DEV=1
    Loopback, Loopback PCM
    Direct hardware device without any conversions
plughw:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Hardware device with all software conversions
plughw:CARD=Loopback,DEV=1
    Loopback, Loopback PCM
    Hardware device with all software conversions
sysdefault:CARD=Loopback
    Loopback, Loopback PCM
    Default Audio Device
front:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Front output / input
surround21:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    2.1 Surround output to Front and Subwoofer speakers
surround40:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    4.0 Surround output to Front and Rear speakers
surround41:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    4.1 Surround output to Front, Rear and Subwoofer speakers
surround50:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    5.0 Surround output to Front, Center and Rear speakers
surround51:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    5.1 Surround output to Front, Center, Rear and Subwoofer speakers
surround71:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    7.1 Surround output to Front, Center, Side, Rear and Woofer speakers
dmix:CARD=Loopback,DEV=0
    Loopback, Loopback PCM
    Direct sample mixing device
dmix:CARD=Loopback,DEV=1
    Loopback, Loopback PCM
    Direct sample mixing device
usbstream:CARD=Loopback
    Loopback
    USB Stream Output
hw:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Direct hardware device without any conversions
plughw:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Hardware device with all software conversions
sysdefault:CARD=Audio2xSerial
    192k Audio/2xSerial, USB Audio
    Default Audio Device
front:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Front output / input
surround21:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    2.1 Surround output to Front and Subwoofer speakers
surround40:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    4.0 Surround output to Front and Rear speakers
surround41:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    4.1 Surround output to Front, Rear and Subwoofer speakers
surround50:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    5.0 Surround output to Front, Center and Rear speakers
surround51:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    5.1 Surround output to Front, Center, Rear and Subwoofer speakers
surround71:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    7.1 Surround output to Front, Center, Side, Rear and Woofer speakers
iec958:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    IEC958 (S/PDIF) Digital Audio Output
dmix:CARD=Audio2xSerial,DEV=0
    192k Audio/2xSerial, USB Audio
    Direct sample mixing device
usbstream:CARD=Audio2xSerial
    192k Audio/2xSerial
    USB Stream Output
rod@t440-d12:~/wrk_ham$ $ aplay -D hw:CARD=Audio2xSerial,DEV=0 -v -f S16_LE -c2 -r192000 192k_test.wav
Playing WAVE '192k_test.wav' : Signed 16 bit Little Endian, Rate 192000 Hz, Stereo
Hardware PCM card 3 '192k Audio/2xSerial' device 0 subdevice 0
Its setup is:
  stream       : PLAYBACK
  access       : RW_INTERLEAVED
  format       : S16_LE
  subformat    : STD
  channels     : 2
  rate         : 192000
  exact rate   : 192000 (192000/1)
  msbits       : 16
  buffer_size  : 96000
  period_size  : 24000
  period_time  : 125000
  tstamp_mode  : NONE
  tstamp_type  : MONOTONIC
  period_step  : 1
  avail_min    : 24000
  period_event : 0
  start_threshold  : 96000
  stop_threshold   : 96000
  silence_threshold: 0
  silence_size : 0
  boundary     : 6755399441055744000
  appl_ptr     : 0
  hw_ptr       : 0

*/

