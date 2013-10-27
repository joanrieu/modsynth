#pragma once

struct Module {
  virtual ~Module();
  virtual void update(float dt);
};

struct VCO: Module {
  VCO();
  float* cv;
  float* gate;
  float out;
  void update(float dt);
  private:
  float t;
  virtual float wave(float t) = 0;
};

struct SineVCO: VCO {
  private:
  float wave(float t);
};

struct SquareVCO: VCO {
  private:
  float wave(float t);
};

struct SawVCO: VCO {
  private:
  float wave(float t);
};

struct TriangleVCO: VCO {
  private:
  float wave(float t);
};

struct Gain: Module {
  Gain();
  float* in;
  float* base;
  float* amount;
  float out;
  void update(float dt);
};

struct SimpleFilter: Module {
  SimpleFilter();
  float* in;
  float* amount;
  float out_low;
  float out_high;
  void update(float dt);
};
