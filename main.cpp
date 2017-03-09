#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <trytotriggermosquittoerror.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Mosquitto socket error example");

    QCommandLineParser parser;
    parser.setApplicationDescription(QString("https://github.com/eclipse/mosquitto/issues/383"));
    parser.addHelpOption();

    QCommandLineOption hostname("hostname", "Mosquitto server hostname", "hostname" );
    QCommandLineOption username("username", "Mosquitto username", "username");
    QCommandLineOption password("password", "Mosquitto password", "password");
    QCommandLineOption port("port", "port", "port", "8883");


    parser.addOption(hostname);
    parser.addOption(port);
    parser.addOption(username);
    parser.addOption(password);

    parser.process(a);

    // No 'required' option on QCommandLineOption?
    if (!parser.isSet(hostname) || !parser.isSet(username) || !parser.isSet(password))
    {
        qCritical() << "Missing options. Do --help.";
        return 1;
    }

    bool parseOK = false;
    int port_int = parser.value(port).toInt(&parseOK);

    if (!parseOK)
    {
        qCritical() << "Not a valid port number";
        return 2;
    }

    TryToTriggerMosquittoError foo(parser.value(hostname), port_int, parser.value(username), parser.value(password));
    foo.doStuff();

    return a.exec();
}
