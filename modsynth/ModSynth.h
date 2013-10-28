#pragma once

#include <vector>
#include <QObject>
#include <jack/jack.h>
#include "../libmodsynth/modsynth.h"

class ModSynth: public QObject {
  Q_OBJECT
  Q_PROPERTY(qreal filterCutoff READ filterCutoff WRITE setFilterCutoff);
  public:
  ModSynth();
  int processJACK(jack_nframes_t nframes);
  qreal filterCutoff() const;
  void setFilterCutoff(qreal cutoff);
  private:
  void createSynth();
  std::vector<Module*> modules;
  SimpleFilter* lp;
  void createJACK();
  jack_client_t* client;
  jack_port_t* port;
};
