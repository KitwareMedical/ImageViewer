
#include <qapplication.h>
#include <qpushbutton.h>

int main(int argc, char **argv)
{
  QApplication app( argc, argv );
  QPushButton  bb( "Insight + Qt", 0 );
  bb.resize( 100, 30 );
  app.setMainWidget( &bb );
  bb.show();
  return app.exec();
}
