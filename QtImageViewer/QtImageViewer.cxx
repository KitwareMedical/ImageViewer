/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
// Qt includes
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSlider>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QApplication>
#include <QStyle>
#include <QScreen>
#include <QSize>
#include <QGuiApplication>

// QtImageViewer includes
#include "QtImageViewer.h"
#include "QtGlSliceView.h"
#include "ui_QtImageViewer.h"

// ITK includes
#include <itkImageFileReader.h>

// STD includes
#include <iostream>

class QtImageViewerPrivate: public Ui_QtImageViewer
{
  Q_DECLARE_PUBLIC(QtImageViewer);
public:
  typedef Ui_QtImageViewer Superclass;

  QtImageViewerPrivate(QtImageViewer& obj);

  virtual void setupUi(QDialog* widgetToSetup);

  template <class PixelType>
  typename itk::Image<PixelType,3>::Pointer loadImage(QString& filePath,
    const QString& imageType = QString());

  template <class PixelType>
  typename itk::Image<PixelType,3>::Pointer readImage(const QString &
    filePath);

  /// Resize the entire dialog based on the current size and to ensure it
  /// fits
  /// the contents.
  /// \sa QWidget::resize(), QWidget::adjustSize()
  void updateSize();

  /// Utility method that returns all the widgets of a layout that belong to
  /// n levels.
  static QWidgetList layoutWidgets(QLayout* layout, int level = -1);

  QDialog* HelpDialog;
  bool IsRedirectingEvent;

protected:
  QtImageViewer* const q_ptr;
};


QtImageViewerPrivate::QtImageViewerPrivate(QtImageViewer& obj)
  : HelpDialog(0)
  , IsRedirectingEvent(false)
  , q_ptr(&obj)
{
}


void QtImageViewerPrivate::setupUi(QDialog* widgetToSetup)
{
  Q_Q(QtImageViewer);
  this->Superclass::setupUi(widgetToSetup);
}

template <class PixelType>
typename itk::Image<PixelType, 3>::Pointer QtImageViewerPrivate
::loadImage(QString& filePath, const QString& imageType)
{
  Q_Q(QtImageViewer);
  typename itk::Image<PixelType, 3>::Pointer res;
  // If the path is empty, prompt a dialog to give a chance to select the
  // image to load.
  if (filePath.isEmpty())
    {
    filePath = QFileDialog::getOpenFileName( q, imageType,
      QDir::currentPath());
    }
  // Empty if the user cancelled the dialog.
  if (filePath.isEmpty())
    {
    return res;
    }
  // Might be a non existing path.
  QFileInfo fileInfo(filePath);
  if (!fileInfo.exists())
    {
    const QString messageTitle("Failed loading");
    const QString message = QString(
      "The file you have selected does not exist. %1").arg(filePath);
    QMessageBox::warning(q, messageTitle, message);
    return res;
    }

  // Read the file.
  res = this->readImage<PixelType>(filePath);
  return res;
}

template <class PixelType>
typename itk::Image<PixelType, 3>::Pointer QtImageViewerPrivate::readImage(
  const QString& filePath )
{
  Q_Q(QtImageViewer);
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( filePath.toLatin1().data() );

  typename itk::Image<PixelType, 3>::Pointer res;
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject & e)
    {
    std::cout << " failed" << std::endl;
    std::cerr << "Exception when reading image" << std::endl;
    std::cerr << e << std::endl;

    const QString title("Failed to read image");
    const QString message = QString("%1").arg(filePath);
    QMessageBox::warning(q, title, message);
    return res;
    }
  res = reader->GetOutput();
  return res;
}

void QtImageViewerPrivate::updateSize()
{
  Q_Q(QtImageViewer);
  q->updateGeometry();

  QWidgetList childWidgets = this->layoutWidgets(q->layout(), 2);
  // Find the true width of the widget. Discard the children that are
  // hidden.
  // We do not need the height information.
  QRect geom;
  foreach(QWidget* child, childWidgets)
    {
    if (child->isVisibleTo(q))
      {
      QPoint childPos = child->mapTo(q, QPoint(0,0));
      int width = child->width() ? child->width() :
        child->sizeHint().width();
      int height = child->height() ? child->height() :
        child->sizeHint().height();
      //qDebug() << child << width << height;
      geom = geom.united(QRect(childPos, QSize(width, height)));
      }
    }
  int width = geom.width();
  int height = geom.height();

  QGuiApplication* app = static_cast<QGuiApplication*>qApp;
  QSize size = app->primaryScreen()->availableSize();
  
  // clamp the width height so that it doesn't extend
  // beyond the screen
  int maxWidth = size.width();
  width = std::min(maxWidth, width);

  height = this->OpenGlWindow->heightForWidth(width);

  // check if the widget is too high for the screen
  // if so, find out by how much, and reduce the width
  // by that relative amount, and recalculate the proper
  // height by ratio to the new width
  // need to consider the height of the window title bar too
  int maxHeight = size.height() - QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
  if (height > maxHeight) {
      double ratio = (double)maxHeight / (double)height;
     
      width = std::floor((double)width * ratio);
      height = this->OpenGlWindow->heightForWidth(width);
  }

  // Can't use QWidget::adjustSize() here because it resizes to the
  // sizeHint of the widget. Instead we want to resize using the current
  // size, just update the height.
  QSize newSize = QSize(width, height);
  q->resize(newSize);
}


QWidgetList QtImageViewerPrivate::layoutWidgets(QLayout* layout, int level)
{
  QWidgetList res;
  if (level == 0)
    {
    return res;
    }
  for (int i = 0; i < layout->count(); ++i)
    {
    QLayoutItem* item = layout->itemAt(i);
    if (item->widget())
      {
      res << item->widget();
      }
    if (item->layout())
      {
      res << layoutWidgets(item->layout(), level - 1);
      }
    }
  return res;
}


QtImageViewer::QtImageViewer(QWidget* widgetParent, Qt::WindowFlags fl )
  : QDialog( widgetParent, fl )
  , d_ptr(new QtImageViewerPrivate(*this))
{
  Q_D(QtImageViewer);
  d->setupUi(this);

  d->OpenGlWindow->setFocus();
}


QtImageViewer::~QtImageViewer()
{
}


QtGlSliceView* QtImageViewer::sliceView()const
{
  Q_D(const QtImageViewer);
  return d->OpenGlWindow;
}


void QtImageViewer::setInputImage(ImageType* newImData)
{
  Q_D(QtImageViewer);
  d->OpenGlWindow->setInputImage(newImData);
  d->OpenGlWindow->changeSlice((d->OpenGlWindow->maxSliceNum() - 1)/2);

  int width = newImData->GetLargestPossibleRegion().GetSize()[0];
  int height = width; //newImData->GetLargestPossibleRegion().GetSize()[1];
  while( width > 500 || height > 500 )
    {
    width /= 2;
    height /= 2;
    }
  QSize newSize = QSize( width, height );
  d->OpenGlWindow->setMinimumSize( newSize );
  while( width < 500 && height < 500 )
    {
    width *= 2;
    height *= 2;
    }
  newSize = QSize( width, height );
  d->OpenGlWindow->resize( newSize );
  // Use adjustSize() instead of updateSize() because there is no valid
  // prior size.
  this->layout()->activate();
  this->adjustSize();
  d->updateSize();
}


void QtImageViewer::setOverlayImage(OverlayImageType* newImData)
{
  Q_D(QtImageViewer);
  d->OpenGlWindow->setInputOverlay(newImData);
  d->updateSize();
}


bool QtImageViewer::loadInputImage(QString filePathToLoad)
{
  Q_D(QtImageViewer);
  
  ImageType::Pointer image = d->loadImage<double>(filePathToLoad);
  if (image.IsNotNull())
    {
    this->setInputImage( image );
    this->sliceView()->setInputImageFilepath(filePathToLoad);
    this->setWindowTitle(filePathToLoad);
    }
  return image.IsNotNull();
}


bool QtImageViewer::loadOverlayImage(QString filePathToLoad)
{
  Q_D(QtImageViewer);

  OverlayImageType::Pointer image = d->loadImage<OverlayPixelType>(filePathToLoad);
  if (image.IsNotNull())
    {
    this->setOverlayImage( image );
    }
  return image.IsNotNull();
}

bool QtImageViewer::loadJSONAnnotations(QString filePathToLoad)
{
  Q_D(QtImageViewer);

  QFile file(filePathToLoad);
  if (!file.open(QIODevice::ReadOnly)) {
    return false;
  }

  const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
  file.close();

  if (doc.isNull() || doc.isEmpty() || !doc.isObject()) {
    return false;
  }

  bool status = false;

  const QJsonObject root = doc.object();
  status |= this->loadBoxAnnotations(root);

  int maxRainbowId = 0;
  int maxOnsdId = 0;
  status |= this->loadRulerAnnotations(root, maxRainbowId, maxOnsdId);

  status |= this->loadCornerTextAnnotations(root);

  this->sliceView()->setViewOverlayData(true);
  return status;
}

void QtImageViewer::keyPressEvent(QKeyEvent* keyEvent)
{
  Q_D(QtImageViewer);
  if (keyEvent->key() != Qt::Key_Escape &&
      keyEvent->key() != Qt::Key_Enter &&
      keyEvent->key() != Qt::Key_Return)
    {
    if (!d->IsRedirectingEvent)
      {
      d->IsRedirectingEvent = true;
      d->OpenGlWindow->keyPressEvent(keyEvent);
      d->IsRedirectingEvent = false;
      return;
      }
    }
  this->Superclass::keyPressEvent(keyEvent);
}

void QtImageViewer::releaseFixedSize()
{
  Q_D(QtImageViewer);
  d->OpenGlWindow->setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}

bool QtImageViewer::loadBoxAnnotations(const QJsonObject &root)
{
  if (!(root.contains("boxes") && root["boxes"].isArray()))
  {
    return false;
  }

  const QJsonArray slices = root["boxes"].toArray();
  for (auto val1 : slices)
  {
    if (val1.isObject())
    {
      const QJsonObject slice = val1.toObject();
      if (
          slice.contains("axis") && slice["axis"].isDouble() &&
          slice.contains("slice") && slice["slice"].isDouble() &&
          slice.contains("boxes") && slice["boxes"].isArray())
      {
        const int axis = (int)slice["axis"].toDouble();
        const int sliceNum = (int)slice["slice"].toDouble();
        const QJsonArray boxes = slice["boxes"].toArray();
        for (auto val2 : boxes)
        {
          if (val2.isObject())
          {
            const QJsonObject box = val2.toObject();
            if (
                box.contains("name") && box["name"].isString() &&
                box.contains("indices") && box["indices"].isArray())
            {
              const QString name = box["name"].toString();
              const QJsonArray indices = box["indices"].toArray();
              if (
                  indices.size() == 2 &&
                  indices[0].isArray() &&
                  indices[1].isArray())
              {
                const QJsonArray pt1Array = indices[0].toArray();
                const QJsonArray pt2Array = indices[1].toArray();
                if (pt1Array.size() == 3 && pt2Array.size() == 3)
                {
                  double point1[] = {
                      pt1Array[0].toDouble(),
                      pt1Array[1].toDouble(),
                      pt1Array[2].toDouble(),
                  };
                  double point2[] = {
                      pt2Array[0].toDouble(),
                      pt2Array[1].toDouble(),
                      pt2Array[2].toDouble(),
                  };
                  this->sliceView()->addBox(name.toStdString(), axis, sliceNum, point1, point2);
                }
              }
            }
          }
        }
      }
    }
  }
  return true;
}

bool QtImageViewer::loadRulerAnnotations(const QJsonObject &root, int& maxRainbowId, int& maxOnsdId)
{
  if (!(root.contains("rulers") && root["rulers"].isArray()))
  {
    return false;
  }

  int maxRainbowIdTemp = 0;
  int maxOnsdIdTemp = 0;

  const QJsonArray slices = root["rulers"].toArray();
  for (auto val1 : slices)
  {
    if (val1.isObject())
    {
      const QJsonObject slice = val1.toObject();
      if (
          slice.contains("axis") && slice["axis"].isDouble() &&
          slice.contains("slice") && slice["slice"].isDouble() &&
          slice.contains("rulers") && slice["rulers"].isArray())
      {
        const int axis = (int)slice["axis"].toDouble();
        const int sliceNum = (int)slice["slice"].toDouble();
        const QJsonArray rulers = slice["rulers"].toArray();
        for (auto val2 : rulers)
        {
          if (val2.isObject())
          {
            const QJsonObject ruler = val2.toObject();
            if (
                ruler.contains("name") && ruler["name"].isString() &&
                ruler.contains("indices") && ruler["indices"].isArray() &&
                ruler.contains("color") && ruler["color"].isString() &&
                ruler.contains("sortId"))
            {
              const std::string name = ruler["name"].toString().toStdString();
              const QJsonArray indices = ruler["indices"].toArray();
              const QColor color = ruler["color"].toString();
              // Note that here we assume -1 will never be used as an ID
              int sortId = ruler["sortId"].toInt(-1);
              if (sortId == -1)
              {
                return false;
              }

              if (name == std::to_string(sortId) && sortId > maxRainbowIdTemp)
              {
                maxRainbowIdTemp = sortId;
              }
              else if ((name == "R1" || name == "ONSD") && sortId > maxOnsdIdTemp)
              {
                maxOnsdIdTemp = sortId;
              }

              if (
                  indices.size() == 2 &&
                  indices[0].isArray() &&
                  indices[1].isArray())
              {
                const QJsonArray pt1Array = indices[0].toArray();
                const QJsonArray pt2Array = indices[1].toArray();
                if (pt1Array.size() == 3 && pt2Array.size() == 3)
                {
                  double point1[] = {
                      pt1Array[0].toDouble(),
                      pt1Array[1].toDouble(),
                      pt1Array[2].toDouble(),
                  };
                  double point2[] = {
                      pt2Array[0].toDouble(),
                      pt2Array[1].toDouble(),
                      pt2Array[2].toDouble(),
                  };
                  std::unique_ptr< RulerToolMetaData > metaData(new RulerToolMetaData{ sortId, name, color });
                  this->sliceView()->addRuler(axis, sliceNum, point1, point2, std::move(metaData));
                }
              }
            }
          }
        }
      }
    }
  }
  // Successfully parsed all of the serialized rulers,
  // set the max ids and return true.
  maxRainbowId = maxRainbowIdTemp;
  maxOnsdId = maxOnsdIdTemp;

  // Initialize the state of the metadata factories
  this->sliceView()->initializeRulerMetadataFactories(maxRainbowId + 1, maxOnsdId + 1);
  return true;
}

bool QtImageViewer::loadRulerAnnotations(const QJsonObject &root)
{
  // Do nothing with the passed integers
  int maxRainbowId = 0;
  int maxOnsdId = 0;
  return this->loadRulerAnnotations(root, maxRainbowId, maxOnsdId);
}

bool QtImageViewer::loadCornerTextAnnotations(const QJsonObject &root)
{
  if (!(root.contains("cornerTexts") && root["cornerTexts"].isArray()))
  {
    return false;
  }

  const QJsonArray slices = root["cornerTexts"].toArray();
  for (auto val1 : slices)
  {
    if (val1.isObject())
    {
      const QJsonObject slice = val1.toObject();
      if (
          slice.contains("axis") && slice["axis"].isDouble() &&
          slice.contains("slice") && slice["slice"].isDouble() &&
          slice.contains("text") && slice["text"].isString())
      {
        const int axis = (int)slice["axis"].toDouble();
        const int sliceNum = (int)slice["slice"].toDouble();
        const QString cornerText = slice["text"].toString();
        this->sliceView()->setCornerText(axis, sliceNum, cornerText);
      }
    }
  }
  return true;
}