#pragma once

#include <QObject>
#include <QVariantMap>

struct TrackInfo {

    QString title;
    QString artist;
    QString album;
    qint64 lengthMs = 0;

    QString trackId;

};

class MprisWatcher : public QObject {

    Q_OBJECT

    public:

        explicit MprisWatcher(QObject* parent=nullptr);
        
        void start();

    signals:

        void trackChanged(const TrackInfo& info);

    private slots:
        void onPropertiesChanged(const QString& iface,
                                const QVariantMap& changed,
                                const QStringList& invalidated);


    private:
    
        void connectToService(const QString& service);
        
        void requestMetadataOnce();
        
        TrackInfo parseMeta(const QVariantMap& meta);

        QString m_service;

};