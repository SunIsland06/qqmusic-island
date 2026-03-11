#include "mpris_watcher.hpp"

#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusReply>

MprisWatcher::MprisWatcher(QObject* parent) : QObject(parent) {

}

void MprisWatcher::start() {

    // 自动寻找包含 "qqmusic" 的 MPRIS 服务名
    auto iface = QDBusConnection::sessionBus().interface();
    if (!iface) {
        qWarning() << "DBus session interface not available";
        return;
    }

    QStringList services = iface->registeredServiceNames();

    for (const auto& s : services) {
        if (s.startsWith("org.mpris.MediaPlayer2.") && s.contains("chromium")) {
            connectToService(s);
            return;
        }
    }

    qWarning() << "No qqmusic MPRIS service found. Running players?";
}

void MprisWatcher::connectToService(const QString& service) {

    m_service = service;
    qDebug() << "Using MPRIS service:" << m_service;

    QDBusConnection::sessionBus().connect(
        m_service,
        "/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onPropertiesChanged(QString, QVariantMap, QStringList))
    );

}

TrackInfo MprisWatcher::parseMeta(const QVariantMap& meta) {

    TrackInfo info;
    info.title = meta.value("xesam:title").toString();

    QVariant artistVar = meta.value("xesam:artist");
    if (artistVar.canConvert<QStringList>()) {
        info.artist = artistVar.toStringList().value(0);
    } else {
        info.artist = artistVar.toString();
    }

    info.album = meta.value("xesam:album").toString();
    info.lengthMs = meta.value("mpris:length").toLongLong() / 1000;
    info.trackId = meta.value("mpris:trackid").toString();

    return info;

}

void MprisWatcher::onPropertiesChanged(const QString& iface,
                                       const QVariantMap& changed,
                                       const QStringList& invalidated) {

    if (iface != "org.mpris.MediaPlayer2.Player") return;

    if (changed.contains("Metadata")) {
        QVariant metaVar = changed.value("Metadata");
        QVariantMap meta;

        if (metaVar.canConvert<QDBusArgument>()) {
            QDBusArgument arg = metaVar.value<QDBusArgument>();
            arg >> meta;
        } else {
            meta = metaVar.toMap();
        }
        auto info = parseMeta(meta);

        // 过滤空信息
        if(info.title.isEmpty() && info.artist.isEmpty()) return;

        emit trackChanged(info);
    }

}

void MprisWatcher::requestMetadataOnce() {
    QDBusInterface props(
        m_service,
        "/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties",
        QDBusConnection::sessionBus()
    );

    QDBusReply<QVariant> reply = props.call(
        "Get", "org.mpris.MediaPlayer2.Player", "Metadata"
    );

    if (!reply.isValid()) return;

    auto meta = reply.value().toMap();
    auto info = parseMeta(meta);

    if (!info.title.isEmpty() || !info.artist.isEmpty()) {
        emit trackChanged(info);
    }
}