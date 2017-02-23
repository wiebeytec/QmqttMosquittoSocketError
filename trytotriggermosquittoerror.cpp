#include "trytotriggermosquittoerror.h"
#include <QDateTime>

TryToTriggerMosquittoError::TryToTriggerMosquittoError(const QString &hostname, const QString &username, const QString &password, QObject *parent) : QObject(parent)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QString clientID = QString("TryingToBreakSSL_%1").arg(qrand());

    mMqttClient.reset(new QMQTT::Client(hostname, 8883, true, true)); // TODO: I ignoreSelfSigned can be false and it still works?
    mMqttClient->setUsername(username);
    mMqttClient->setPassword(password);
    mMqttClient->setCleanSession(true);
    mMqttClient->setClientId(clientID);

    connect(mMqttClient.data(), &QMQTT::Client::received, this, &TryToTriggerMosquittoError::onReceived);
    connect(mMqttClient.data(), &QMQTT::Client::error, this, &TryToTriggerMosquittoError::onError);
    connect(mMqttClient.data(), &QMQTT::Client::connected, this, &TryToTriggerMosquittoError::onConnect);
}

void TryToTriggerMosquittoError::doStuff()
{
    mMqttClient->connectToHost();
}

void TryToTriggerMosquittoError::onReceived(const QMQTT::Message &message)
{
    //return;
    QString line = QString("%1: %2").arg(message.topic()).arg(QString::fromUtf8(message.payload()));
    qDebug() << line;
}

void TryToTriggerMosquittoError::onError(const QMQTT::ClientError error)
{
    QString line = QString("Error occurred. QMQTT error code %1").arg(error);
    qDebug() << line;
}

void TryToTriggerMosquittoError::onConnect()
{
    const QString subscribePath = "N/847e4062cb99/#";
    const QString publishPath = "R/847e4062cb99/system/0/Serial";
    QString line = QString("Connection established. Subscribing to %1. Publishing %2").arg(subscribePath).arg(publishPath);
    qDebug() << line;

    mMqttClient->subscribe(subscribePath, 0);

    const int qos = 2;
    QMQTT::Message message(qrand(), publishPath, QByteArray(), qos);
    mMqttClient->publish(message);
}
