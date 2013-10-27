#include <cmath>
#include <iostream>
#include <vector>

struct Module {
  virtual ~Module() {}
  virtual void update(float dt) {}
};

struct VCO: Module {
  VCO(): cv(nullptr), out(0), t(0) {}
  float* cv;
  float* gate;
  float out;
  void update(float dt) {
    t -= (int)t;
    out = wave(t);
    t += dt * *cv * *gate;
  }
  private:
  float t;
  virtual float wave(float t) = 0;
};

struct SineVCO: VCO {
  private:
  float wave(float t) {
    return std::sin(2 * M_PI * t);
  }
};

struct SquareVCO: VCO {
  private:
  float wave(float t) {
    return t < .5 ? -1 : 1;
  }
};

struct SawVCO: VCO {
  private:
  float wave(float t) {
    return 2 * t - 1;
  }
};

struct TriangleVCO: VCO {
  private:
  float wave(float t) {
    if (t < .5)
      return 4 * t - 1;
    else
      return wave(1 - t);
  }
};

struct Gain: Module {
  Gain(): in(nullptr), base(nullptr), amount(nullptr), out(0) {}
  float* in;
  float* base;
  float* amount;
  float out;
  void update(float dt) {
    out = *base + *amount * *in;
  }
};

struct SimpleFilter: Module {
  SimpleFilter(): in(nullptr), amount(nullptr), out_low(0), out_high(0) {}
  float* in;
  float* amount;
  float out_low;
  float out_high;
  void update(float dt) {
    const float a = *amount;
    out_low = a * *in + (1 - a) * out_low;
    out_high = *in - out_low;
  }
};

void output(float* value) {
  char* raw = reinterpret_cast<char*>(value);
  for (int i = 0; i < sizeof(float); ++i)
    std::cout << raw[i];
}

int main() {

  std::vector<Module*> modules;

  VCO* gate_speed_lfo = new SineVCO;
  gate_speed_lfo->cv = new float(.1);
  gate_speed_lfo->gate = new float(1);
  modules.push_back(gate_speed_lfo);

  Gain* gate_speed_lfo_scale = new Gain;
  gate_speed_lfo_scale->in = &gate_speed_lfo->out;
  gate_speed_lfo_scale->base = new float(8);
  gate_speed_lfo_scale->amount = new float(4);
  modules.push_back(gate_speed_lfo_scale);

  VCO* gate_lfo = new SquareVCO;
  gate_lfo->cv = &gate_speed_lfo_scale->out;
  gate_lfo->gate = new float(1);
  modules.push_back(gate_lfo);

  Gain* gate_lfo_scale = new Gain;
  gate_lfo_scale->in = &gate_lfo->out;
  gate_lfo_scale->base = gate_lfo_scale->amount = new float(.5);
  modules.push_back(gate_lfo_scale);

  VCO* osc = new TriangleVCO;
  osc->cv = new float(55);
  osc->gate = &gate_lfo_scale->out;
  modules.push_back(osc);

  VCO* filter_lfo = new SineVCO;
  filter_lfo->cv = new float(.3);
  filter_lfo->gate = new float(1);
  modules.push_back(filter_lfo);

  Gain* filter_lfo_scale = new Gain;
  filter_lfo_scale->in = &filter_lfo->out;
  filter_lfo_scale->base = filter_lfo_scale->amount = new float (.5);
  modules.push_back(filter_lfo_scale);

  SimpleFilter* lp = new SimpleFilter;
  lp->in = &osc->out;
  lp->amount = &filter_lfo_scale->out;
  modules.push_back(lp);

  while (true) {
    output(&lp->out_low);
    output(&lp->out_low);
    for (auto& module: modules)
      module->update(1 / 44100.f);
  }
}
