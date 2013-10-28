#pragma once

#include <vector>
#include <QObject>
#include <jack/jack.h>
#include "../libmodsynth/modsynth.h"

class ModSynth: public QObject {

  Q_OBJECT
  Q_PROPERTY(qreal filterCutoff READ filterCutoff WRITE setFilterCutoff NOTIFY filterCutoffChanged);

  public:

  ModSynth();

  int processJACK(jack_nframes_t nframes);

  qreal filterCutoff() const;
  void setFilterCutoff(qreal cutoff);

  signals:

  void filterCutoffChanged();

  private:

  void initSynth();
  std::vector<Module*> modules;
  SimpleFilter* lp;

  void initJACK();
  jack_client_t* client;
  jack_port_t* port;

};
