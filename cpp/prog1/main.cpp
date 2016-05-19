#include <QApplication>
#include "prog1.h"


using namespace std;

int main(int argc, char *argv[])
{       


   QCoreApplication app(argc, argv);
   Player p("1111");

   p.play();

   return app.exec();
}
