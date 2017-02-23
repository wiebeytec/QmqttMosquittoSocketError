#include <QCoreApplication>
#include <trytotriggermosquittoerror.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TryToTriggerMosquittoError foo("removed", "removed", "removed");
    foo.doStuff();

    return a.exec();
}
