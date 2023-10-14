#include "CommandLine.h"

#include <SessionLockQt/command.h>

#include <QGuiApplication>
#include <QTimer>

#include <unistd.h>

static PassWordInfo *PASSWORDINFO_INSTANCE = nullptr;

PassWordInfo::PassWordInfo(QObject *parent)
  : QObject(parent)
  , m_password(QString())
{
}

PassWordInfo *
PassWordInfo::instance()
{
    if (!PASSWORDINFO_INSTANCE) {
        PASSWORDINFO_INSTANCE = new PassWordInfo;
    }
    return PASSWORDINFO_INSTANCE;
}

void
PassWordInfo::setPassword(const QString &password)
{
    m_password = password;
}

static int
handle_conversation(int num_msg,
                    const struct pam_message **msg,
                    struct pam_response **resp,
                    void *data)
{
    /* PAM expects an array of responses, one for each message */
    struct pam_response *pam_reply =
      static_cast<struct pam_response *>(calloc(num_msg, sizeof(struct pam_response)));
    if (pam_reply == NULL) {
        return PAM_ABORT;
    }
    *resp = pam_reply;
    for (int i = 0; i < num_msg; ++i) {
        switch (msg[i]->msg_style) {
        case PAM_PROMPT_ECHO_OFF:
        case PAM_PROMPT_ECHO_ON:
            pam_reply[i].resp =
              strdup(PassWordInfo::instance()->password().toLocal8Bit().data()); // PAM clears and
                                                                                 // frees this
            if (pam_reply[i].resp == NULL) {
                return PAM_ABORT;
            }
            break;
        case PAM_ERROR_MSG:
        case PAM_TEXT_INFO:
            break;
        }
    }
    return PAM_SUCCESS;
}

CommandLine::CommandLine(QObject *parent)
  : QObject(parent)
  , m_handle(nullptr)
{
    m_userName                 = QString::fromStdString(getlogin());
    const struct pam_conv conv = {
      .conv        = &handle_conversation,
      .appdata_ptr = NULL,
    };
    if (pam_start("swaylock", m_userName.toLocal8Bit().data(), &conv, &m_handle) != PAM_SUCCESS) {
        qWarning() << "Cannot start pam";
        QTimer::singleShot(0, this, [this] { this->UnLock(); });
        return;
    }
}

void
CommandLine::setPassword(const QString &password)
{
    m_password = password;
    PassWordInfo::instance()->setPassword(password);
    Q_EMIT passwordChanged();
}

void
CommandLine::UnLock()
{
    ExtSessionLockV1Qt::Command::instance()->unLockScreen();
    QGuiApplication::quit();
}

void
CommandLine::RequestUnlock()
{
    int pam_status = pam_authenticate(m_handle, 0);
    if (pam_status != PAM_SUCCESS) {
        qDebug() << "failed";
        // TODO:
        return;
    }
    pam_setcred(m_handle, PAM_REFRESH_CRED);
    if (pam_end(m_handle, pam_status)) {
        qWarning() << "Pam end failer";
    }
    UnLock();
}
