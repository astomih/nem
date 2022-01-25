// system include
#include <cmath>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <unistd.h> //for sleep

// third party
#include "wave_writer.h"
#include <AL/al.h>
#include <AL/alc.h>

static const int ch = 1;
static const int sampling_rate = 8000;
static const int bit_depth = 8;
static const int play_time_sec = 5;
static const int freq = 500;

static const int length = sampling_rate * play_time_sec;

static const double PI_2 = 6.2831853;

int main() {
  ALCdevice *device;
  ALCcontext *context;
  uint8_t data[length];
  ALuint buffer, source;
  int i;

  // open device.
  device = alcOpenDevice(NULL);
  context = alcCreateContext(device, NULL);
  alcMakeContextCurrent(context);
  // generate buffer
  alGenBuffers(1, &buffer);

  // generate signal
  for (i = 0; i < length; ++i) {
    data[i] =
        (uint8_t)((sin(PI_2 * i * freq / sampling_rate) + 1.0) / 2.0 * 255.0);
  }
  // input signal to buffer
  alBufferData(buffer, AL_FORMAT_MONO8, data, sizeof(data), sampling_rate);
  // generate source
  alGenSources(1, &source);
  // attach source to buffer
  alSourcei(source, AL_BUFFER, buffer);
  // play
  alSourcePlay(source);
  sleep(4);

  // delete buffer,source,etc...
  alSourceStop(source);
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);

  // ファイルロード
  auto wave_out = new wave_writer("Sin_500Hz.wav");
  if (!wave_out->has_file)
    exit(1);

  // ヘッダ構築
  wave_out->set_header(length, ch, sampling_rate, bit_depth);
  wave_out->print_header();

  // 書き出し
  wave_out->write_file(data);

  delete wave_out;
  return 0;
}