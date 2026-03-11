#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

#include "ipc_server.hpp"
#include "lyrics_provider.hpp"
#include "mpris_watcher.hpp"
#include "timeline_engine.hpp"

int main(int argc, char** argv) {

    QCoreApplication app(argc, argv);
    qDebug() << "qqmusic-island: app started";

    MprisWatcher watcher;

    QObject::connect(&watcher, &MprisWatcher::trackChanged,
                   [](const TrackInfo& info) {
        qDebug().noquote()
        << "Track:"
        << info.title
        << "-"
        << info.artist
        << "| album:" << info.album
        << "| length(ms):" << info.lengthMs;
    });

    watcher.start();
    
    return app.exec();

}
