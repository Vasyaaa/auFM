#include <QApplication>
#include "aafm1.h"


using namespace std;

int main(int argc, char *argv[])
{       


   QCoreApplication app(argc, argv);
   Player p("10101010");

   p.play();

   return app.exec();
}
