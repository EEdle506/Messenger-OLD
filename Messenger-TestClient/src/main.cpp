#include "client/clientclass.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("Samarin I.V.");
    QApplication::setApplicationName("Messenger-Client");
    QApplication::setWindowIcon(QIcon(":/app/ui/images/AppIcon.ico"));
    
    qApp->setStyle(QStyleFactory::create("Fusion"));

    ClientClass::Start();

    QFont defaultFont = QApplication::font();
    // increase font size for better reading
    defaultFont.setPointSize(defaultFont.pointSize() + 2);
    qApp->setFont(defaultFont);

    return a.exec();
}
