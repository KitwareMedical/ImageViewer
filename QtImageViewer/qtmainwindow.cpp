#include "qtmainwindow.h"
#include "ui_qtmainwindow.h"


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , UI(new Ui::MainWindow)
{
  UI->setupUi(this);

  m_ImageData = ImageType::New();
  m_Reader = ReaderType::New();
  m_FilterX = FilterType::New();
  m_FilterY = FilterType::New();
  m_Writer = WriterType::New();
}

MainWindow::~MainWindow()
{
  delete UI;
}

void MainWindow::on_LoadButton_clicked()
{
  QString filePathToLoad = QFileDialog::getOpenFileName(this,
                                     "", QDir::currentPath());

  this->loadFile(filePathToLoad);
}

void MainWindow::loadFile(const QString& fname)
{

  if(fname.isEmpty())
    {
    return;
    }
  qDebug() << "Loading File: " << fname ;

  m_Reader->SetFileName(fname.toLatin1().data());

    try
      {
      m_Reader->Update();
      }
   catch (itk::ExceptionObject &e)
      {
      std::cerr << e << std::endl;
      return;
      }
   std::cout << "...Done Loading File" << std::endl;

    //    char mainName[255];
    //    sprintf(mainName, "metaView: %s", fname.toLatin1().data()); //fName

    //    std::cout << std::endl;
    //    std::cout << "For directions on interacting with the window," << std::endl;
    //    std::cout << "   type 'h' within the window" << std::endl;

  m_ImageData = m_Reader->GetOutput();
}

void MainWindow::on_BlurButton_clicked()
{
  qDebug()<< "ImageData" << m_ImageData->GetImageDimension();

  if(m_ImageData == NULL)
    {
    return;
    }

  m_FilterX->SetDirection( 0 );   // 0 --> X direction
  m_FilterY->SetDirection( 1 );   // 1 --> Y direction

  m_FilterX->SetOrder( FilterType::ZeroOrder );
  m_FilterY->SetOrder( FilterType::ZeroOrder );

  m_FilterX->SetNormalizeAcrossScale( true );
  m_FilterY->SetNormalizeAcrossScale( true );

  m_FilterX->SetInput( this->m_ImageData );  //Reader->GetOutput()
  m_FilterY->SetInput( m_FilterX->GetOutput() );

  const double sigma =  0.2; //atof( argv[3] );

  m_FilterX->SetSigma( sigma );
  m_FilterY->SetSigma( sigma );

  m_FilterX->Update();
  m_FilterY->Update();

}

void MainWindow::on_SaveButton_clicked()
{
  QString filename = QFileDialog::getSaveFileName(this,
                                "", QDir::currentPath(), tr(""));

  this->saveFile(filename);
}

void MainWindow::saveFile(const QString& fileName)
{
  m_Writer->SetFileName( fileName.toLatin1().data() );
  m_Writer->SetInput( m_FilterY->GetOutput() );

  try
    {
    m_Writer->Update();
    }
  catch ( itk::ExceptionObject &ex )
    {
    std::cout << ex << std::endl;
    return;
    }
}
