#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include "ModSynth.h"

ModSynth::ModSynth() {
  initSynth();
  initJACK();
}

void ModSynth::initSynth() {

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

  lp = new SimpleFilter;
  lp->in = &osc->out;
  lp->amount = new float(0);
  modules.push_back(lp);

}

namespace {
  int process(jack_nframes_t nframes, void* arg) {
    return static_cast<ModSynth*>(arg)->processJACK(nframes);
  }
}

void ModSynth::initJACK() {

  client = jack_client_open("modsynth", JackNullOption, nullptr, nullptr);

  if (!client)
    return;

  port = jack_port_register(client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
  jack_set_process_callback(client, process, this);
  jack_activate(client);

  const char** ports = jack_get_ports(client, nullptr, nullptr, JackPortIsInput | JackPortIsPhysical);
  jack_connect(client, jack_port_name(port), ports[0]);
  jack_connect(client, jack_port_name(port), ports[1]);

}

int ModSynth::processJACK(jack_nframes_t nframes) {

  jack_default_audio_sample_t* buffer = static_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(port, nframes));

  for (jack_nframes_t i = 0; i < nframes; ++i) {

    buffer[i] = lp->out_low;

    for (auto& module: modules)
      module->update(1.f / jack_get_sample_rate(client));

  }

  return 0;

}

qreal ModSynth::filterCutoff() const {
  return *lp->amount;
}

void ModSynth::setFilterCutoff(qreal cutoff) {
  if (*lp->amount != cutoff) {
    *lp->amount = cutoff;
    emit filterCutoffChanged();
  }
}

int main(int argc, char** argv) {

  QGuiApplication app(argc, argv);
  QQuickView view;
  view.setSource(QUrl::fromLocalFile("main.qml"));
  ModSynth modsynth;
  view.rootContext()->setContextProperty("modsynth", &modsynth);
  view.show();
  return app.exec();

}
