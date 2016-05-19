#include <QApplication>
#include "aafm2.h"


using namespace std;

int main(int argc, char *argv[])
{
   QCoreApplication app(argc, argv);
   Player p("101010");

   p.play();

   return app.exec();
}
