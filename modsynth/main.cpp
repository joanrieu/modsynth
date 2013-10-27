#include <functional>
#include <vector>
#include <QGuiApplication>
#include <QQuickView>
#include <jack/jack.h>
#include "../libmodsynth/modsynth.h"

int process(jack_nframes_t nframes, void* arg) {
  (*static_cast<std::function<void(jack_nframes_t)>*>(arg))(nframes);
  return 0;
}

int main(int argc, char** argv) {

  // Synth

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

  // JACK

  jack_client_t* client = jack_client_open("modsynth", JackNullOption, nullptr, nullptr);
  jack_port_t* port = jack_port_register(client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  std::function<void(jack_nframes_t)> real_process = [&](jack_nframes_t nframes) {
    jack_default_audio_sample_t* buffer = static_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(port, nframes));
    for (jack_nframes_t i = 0; i < nframes; ++i) {
      buffer[i] = lp->out_low;
      for (auto& module: modules)
        module->update(1 / 44100.f);
    }
  };
  jack_set_process_callback(client, process, &real_process);
  jack_activate(client);
  const char** ports = jack_get_ports(client, nullptr, nullptr, JackPortIsInput | JackPortIsPhysical);
  jack_connect(client, jack_port_name(port), ports[0]);
  jack_connect(client, jack_port_name(port), ports[1]);

  // Qt

  QGuiApplication app(argc, argv);
  QQuickView view;
  view.setSource(QUrl::fromLocalFile("main.qml"));
  view.show();
  return app.exec();

}
