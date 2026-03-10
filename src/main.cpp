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

    // 1 秒后自动退出，确保事件循环正常工作
    QTimer::singleShot(1000, &app, &QCoreApplication::quit);

    return app.exec();

}
