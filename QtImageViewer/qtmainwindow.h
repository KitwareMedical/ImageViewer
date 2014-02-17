#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>

// ITK include
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
//#include "itkImageToVTKImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"

// VTK include
#include "vtkImageData.h"
#include "ctkVTKSliceView.h"

namespace Ui {
class MainWindow;
}

typedef itk::Image< float, 3 > ImageType;
typedef itk::ImageFileReader< ImageType > ReaderType;

typedef float InputPixelType;
typedef float OutputPixelType;

typedef itk::Image< InputPixelType, 3 > InputImageType;
typedef itk::Image< OutputPixelType, 3 > OutputImageType;
typedef itk::RecursiveGaussianImageFilter<
             InputImageType, OutputImageType > FilterType;

typedef float WritePixelType;
typedef itk::Image< WritePixelType, 3 > WriteImageType;
typedef itk::ImageFileWriter< WriteImageType > WriterType;


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void display();
public slots:
  /// Load the fileName file.
  /// \sa on_LoadButton_clicked()
  void loadFile(const QString& fileName);

  /// Save the file at the fileName directory.
  /// \sa on_SaveButton_clicked()
  void saveFile(const QString& fileName);

private slots:

  void on_LoadButton_clicked();

  void on_BlurButton_clicked();

  void on_SaveButton_clicked();

private:
  Ui::MainWindow *UI;

  ImageType *m_ImageData;
  ReaderType::Pointer m_Reader;
  FilterType::Pointer m_FilterX;
  FilterType::Pointer m_FilterY;
  WriterType::Pointer m_Writer;

  ctkVTKSliceView *m_SliceView;
};

#endif // QTMAINWINDOW_H
