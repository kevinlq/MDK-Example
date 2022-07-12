#include "mdkplayer.h"
#include <QDebug>
#include <QMimeDatabase>
#include <QDir>
#include <QTime>

static inline QStringList suffixesToMimeTypes(const QStringList &suffixes)
{
    QStringList mimeTypes{};
    const QMimeDatabase db;
    for (auto &&suffix : qAsConst(suffixes)) {
        const QList<QMimeType> typeList = db.mimeTypesForFileName(suffix);
        if (!typeList.isEmpty()) {
            for (auto &&mimeType : qAsConst(typeList)) {
                const QString name = mimeType.name();
                if (!name.isEmpty()) {
                    mimeTypes.append(name);
                }
            }
        }
    }
    if (!mimeTypes.isEmpty()) {
        mimeTypes.removeDuplicates();
    }
    return mimeTypes;
}

static inline QString timeToString(const qint64 ms)
{
    return QTime(0, 0).addMSecs(ms).toString(QStringLiteral("hh:mm:ss"));
}

static inline std::vector<std::string> qStringListToStdStringVector(const QStringList &stringList)
{
    if (stringList.isEmpty())
    {
        return {};
    }
    std::vector<std::string> result = {};
    for (auto &&string : qAsConst(stringList))
    {
        result.push_back(string.toStdString());
    }
    return result;
}

static inline QString urlToString(const QUrl &value, const bool display = false)
{
    if (!value.isValid())
    {
        return {};
    }
    return (value.isLocalFile() ? QDir::toNativeSeparators(value.toLocalFile())
                                : (display ? value.toDisplayString() : value.url()));
}

class VideoRendererInternal : public QQuickFramebufferObject::Renderer
{
public:
    VideoRendererInternal(QmlMDKPlayer *r)
    {
        this->r = r;
    }

    void render() override
    {
        qDebug() << "#qi->>> render begin...";
        r->renderVideo();
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override
    {
        qDebug() << "#qi->>> createFramebufferObject begin...";

        r->setVideoSurfaceSize(size.width(), size.height());
        return new QOpenGLFramebufferObject(size);
    }

    QmlMDKPlayer *r;
};


QmlMDKPlayer::QmlMDKPlayer(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    internal_player(new Player)
{
    setMirrorVertically(true);

    internal_player->setRenderCallback([=](void *){
        QMetaObject::invokeMethod(this, "update");
    });

    SetGlobalOption("logLevel", LogLevel::All);

    setVideoDecoders(getDefaultVideoDecoders());
}

QmlMDKPlayer::~QmlMDKPlayer()
{
    delete internal_player;
    internal_player = nullptr;
}

QQuickFramebufferObject::Renderer *QmlMDKPlayer::createRenderer() const
{
    qDebug() << "#qi->>> createRenderer begin...";

    return new VideoRendererInternal(const_cast<QmlMDKPlayer*>(this));
}

void QmlMDKPlayer::setSource(const QString &s)
{
    QString sourceUrl = s;
    if (sourceUrl.startsWith("file:///"))
    {
        sourceUrl.remove("file:///");
    }

    internal_player->setMedia(sourceUrl.toUtf8().data());
    m_source = sourceUrl;
    emit sourceChanged();
    play();
}

void QmlMDKPlayer::play()
{
    internal_player->set(PlaybackState::Playing);

    //startTimer(50);
}

void QmlMDKPlayer::setPlaybackRate(float rate)
{
    internal_player->setPlaybackRate(rate);
}

void QmlMDKPlayer::setVideoSurfaceSize(int width, int height)
{
    internal_player->setVideoSurfaceSize(width, height);
}

void QmlMDKPlayer::setVideoDecoders(const QStringList &value)
{
    if (m_videoDecoders != value)
    {
        m_videoDecoders = value.isEmpty() ? QStringList{QStringLiteral("FFmpeg")} : value;
        internal_player->setDecoders(MediaType::Video, qStringListToStdStringVector(m_videoDecoders));
    }
}

QStringList QmlMDKPlayer::videoSuffixes()
{
    static const QStringList list =
    {
        QStringLiteral("*.3g2"),   QStringLiteral("*.3ga"),
        QStringLiteral("*.3gp"),   QStringLiteral("*.3gp2"),
        QStringLiteral("*.3gpp"),  QStringLiteral("*.amv"),
        QStringLiteral("*.asf"),   QStringLiteral("*.asx"),
        QStringLiteral("*.avf"),   QStringLiteral("*.avi"),
        QStringLiteral("*.bdm"),   QStringLiteral("*.bdmv"),
        QStringLiteral("*.bik"),   QStringLiteral("*.clpi"),
        QStringLiteral("*.cpi"),   QStringLiteral("*.dat"),
        QStringLiteral("*.divx"),  QStringLiteral("*.drc"),
        QStringLiteral("*.dv"),    QStringLiteral("*.dvr-ms"),
        QStringLiteral("*.f4v"),   QStringLiteral("*.flv"),
        QStringLiteral("*.gvi"),   QStringLiteral("*.gxf"),
        QStringLiteral("*.hdmov"), QStringLiteral("*.hlv"),
        QStringLiteral("*.iso"),   QStringLiteral("*.letv"),
        QStringLiteral("*.lrv"),   QStringLiteral("*.m1v"),
        QStringLiteral("*.m2p"),   QStringLiteral("*.m2t"),
        QStringLiteral("*.m2ts"),  QStringLiteral("*.m2v"),
        QStringLiteral("*.m3u"),   QStringLiteral("*.m3u8"),
        QStringLiteral("*.m4v"),   QStringLiteral("*.mkv"),
        QStringLiteral("*.moov"),  QStringLiteral("*.mov"),
        QStringLiteral("*.mp2"),   QStringLiteral("*.mp2v"),
        QStringLiteral("*.mp4"),   QStringLiteral("*.mp4v"),
        QStringLiteral("*.mpe"),   QStringLiteral("*.mpeg"),
        QStringLiteral("*.mpeg1"), QStringLiteral("*.mpeg2"),
        QStringLiteral("*.mpeg4"), QStringLiteral("*.mpg"),
        QStringLiteral("*.mpl"),   QStringLiteral("*.mpls"),
        QStringLiteral("*.mpv"),   QStringLiteral("*.mpv2"),
        QStringLiteral("*.mqv"),   QStringLiteral("*.mts"),
        QStringLiteral("*.mtv"),   QStringLiteral("*.mxf"),
        QStringLiteral("*.mxg"),   QStringLiteral("*.nsv"),
        QStringLiteral("*.nuv"),   QStringLiteral("*.ogm"),
        QStringLiteral("*.ogv"),   QStringLiteral("*.ogx"),
        QStringLiteral("*.ps"),    QStringLiteral("*.qt"),
        QStringLiteral("*.qtvr"),  QStringLiteral("*.ram"),
        QStringLiteral("*.rec"),   QStringLiteral("*.rm"),
        QStringLiteral("*.rmj"),   QStringLiteral("*.rmm"),
        QStringLiteral("*.rms"),   QStringLiteral("*.rmvb"),
        QStringLiteral("*.rmx"),   QStringLiteral("*.rp"),
        QStringLiteral("*.rpl"),   QStringLiteral("*.rv"),
        QStringLiteral("*.rvx"),   QStringLiteral("*.thp"),
        QStringLiteral("*.tod"),   QStringLiteral("*.tp"),
        QStringLiteral("*.trp"),   QStringLiteral("*.ts"),
        QStringLiteral("*.tts"),   QStringLiteral("*.txd"),
        QStringLiteral("*.vcd"),   QStringLiteral("*.vdr"),
        QStringLiteral("*.vob"),   QStringLiteral("*.vp8"),
        QStringLiteral("*.vro"),   QStringLiteral("*.webm"),
        QStringLiteral("*.wm"),    QStringLiteral("*.wmv"),
        QStringLiteral("*.wtv"),   QStringLiteral("*.xesc"),
        QStringLiteral("*.xspf")
    };
    return list;
}

QStringList QmlMDKPlayer::audioSuffixes()
{
    static const QStringList list =
    {
        QStringLiteral("*.mp3"),
        QStringLiteral("*.aac"),
        QStringLiteral("*.mka"),
        QStringLiteral("*.dts"),
        QStringLiteral("*.flac"),
        QStringLiteral("*.ogg"),
        QStringLiteral("*.m4a"),
        QStringLiteral("*.ac3"),
        QStringLiteral("*.opus"),
        QStringLiteral("*.wav"),
        QStringLiteral("*.wv")
    };
    return list;
}

QStringList QmlMDKPlayer::subtitleSuffixes()
{
    static const QStringList list =
    {
        QStringLiteral("*.utf"),
        QStringLiteral("*.utf8"),
        QStringLiteral("*.utf-8"),
        QStringLiteral("*.idx"),
        QStringLiteral("*.sub"),
        QStringLiteral("*.srt"),
        QStringLiteral("*.rt"),
        QStringLiteral("*.ssa"),
        QStringLiteral("*.ass"),
        QStringLiteral("*.mks"),
        QStringLiteral("*.vtt"),
        QStringLiteral("*.sup"),
        QStringLiteral("*.scc"),
        QStringLiteral("*.smi")
    };
    return list;
}

QStringList QmlMDKPlayer::videoMimeType()
{
    return suffixesToMimeTypes(videoSuffixes());
}

QStringList QmlMDKPlayer::audioMimeType()
{
    return suffixesToMimeTypes(audioSuffixes());
}

QStringList QmlMDKPlayer::getDefaultVideoDecoders()
{
#ifdef Q_OS_WIN32
        static const QStringList list =
        {
            QStringLiteral("MFT:d3d=11"),
            QStringLiteral("MFT:d3d=9"),
            QStringLiteral("MFT"),
            QStringLiteral("D3D11"),
            QStringLiteral("DXVA"),
            QStringLiteral("CUDA"),
            QStringLiteral("NVDEC"),
            QStringLiteral("FFmpeg")
        };
#elif defined(Q_OS_LINUX)
        static const QStringList list =
        {
            QStringLiteral("VAAPI"),
            QStringLiteral("VDPAU"),
            QStringLiteral("CUDA"),
            QStringLiteral("NVDEC"),
            QStringLiteral("FFmpeg")
        };
#elif defined(Q_OS_DARWIN)
        static const QStringList list =
        {
            QStringLiteral("VT"),
            QStringLiteral("VideoToolbox"),
            QStringLiteral("FFmpeg")
        };
#elif defined(Q_OS_ANDROID)
        static const QStringList list =
        {
            QStringLiteral("AMediaCodec"),
            QStringLiteral("FFmpeg")
        };
#else
#error "Unsupported platform!"
#endif
        return list;
}

void QmlMDKPlayer::renderVideo()
{
    qDebug() << "###renderVideo:";
    internal_player->renderVideo();
}

void QmlMDKPlayer::timerEvent(QTimerEvent *event)
{
    QQuickItem::timerEvent(event);

    renderVideo();
}
