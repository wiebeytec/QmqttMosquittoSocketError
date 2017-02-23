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

    parser.addOption(hostname);
    parser.addOption(username);
    parser.addOption(password);

    parser.process(a);

    // No 'required' option on QCommandLineOption?
    if (!parser.isSet(hostname) || !parser.isSet(username) || !parser.isSet(password))
    {
        qCritical() << "Missing options. Do --help.";
        return 1;
    }

    TryToTriggerMosquittoError foo(parser.value(hostname), parser.value(username), parser.value(password));
    foo.doStuff();

    return a.exec();
}
