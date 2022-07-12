#ifndef VIDEORENDERER_H
#define VIDEORENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <QDebug>
#include "mdk/Player.h"

using namespace MDK_NS;

class QmlMDKPlayer : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit QmlMDKPlayer(QQuickItem *parent = nullptr);
    virtual ~QmlMDKPlayer();
    Renderer *createRenderer() const override;

    Q_INVOKABLE QString source() { return m_source; }
    Q_INVOKABLE void setSource(const QString & s);

    Q_INVOKABLE void play();
    Q_INVOKABLE void setPlaybackRate(float rate);
    Q_INVOKABLE void setVideoSurfaceSize(int width, int height);

    Q_INVOKABLE void setVideoDecoders(const QStringList &value);


    static inline QStringList videoSuffixes();
    static inline QStringList audioSuffixes();
    static inline QStringList subtitleSuffixes();
    static QStringList videoMimeType();
    static QStringList audioMimeType();
    static inline QStringList getDefaultVideoDecoders();

    void renderVideo();

signals:
    void sourceChanged();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    QString m_source;
    Player *internal_player;

    QStringList m_videoDecoders = {};
    QStringList m_audioDecoders = {};

    qreal m_volume = 1.0;

    bool m_mute = false;
    bool m_hasVideo = false;
    bool m_hasAudio = false;
    bool m_hasSubtitle = false;
    bool m_hasChapters = false;
    bool m_hardwareDecoding = false;
    bool m_autoStart = true;
    bool m_livePreview = false;
    bool m_loop = false;
};

#endif // VIDEORENDERER_H
