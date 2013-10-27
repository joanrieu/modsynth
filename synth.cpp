#include <cmath>
#include "synth.h"

Module::~Module() {}
void Module::update(float dt) {}

VCO::VCO(): cv(nullptr), out(0), t(0) {}
void VCO::update(float dt) {
  t -= (int)t;
  out = wave(t);
  t += dt * *cv * *gate;
}

float SineVCO::wave(float t) {
  return std::sin(2 * M_PI * t);
}

float SquareVCO::wave(float t) {
  return t < .5 ? -1 : 1;
}

float SawVCO::wave(float t) {
  return 2 * t - 1;
}

float TriangleVCO::wave(float t) {
  if (t < .5)
    return 4 * t - 1;
  else
    return wave(1 - t);
}

Gain::Gain(): in(nullptr), base(nullptr), amount(nullptr), out(0) {}
void Gain::update(float dt) {
  out = *base + *amount * *in;
}

SimpleFilter::SimpleFilter(): in(nullptr), amount(nullptr), out_low(0), out_high(0) {}
void SimpleFilter::update(float dt) {
  const float a = *amount;
  out_low = a * *in + (1 - a) * out_low;
  out_high = *in - out_low;
}
