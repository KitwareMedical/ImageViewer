//#include <itkImage.h>
#include <itkPhysicalImage.h>
#include <itkFileIOMetaImage.h>
#include <itkWriteMetaImage.h>
#include <itkFileIOToImageFilter.h>

using namespace itk;
using namespace std;

int main ()
{
//  typedef Image<unsigned short, 3> myImageType;
  typedef PhysicalImage<unsigned short, 3> myImageType;
  typedef WriteMetaImage<myImageType> myWriterType;
  typedef FileIOToImageFilter<myImageType> myFilterType;

  ObjectFactoryBase::RegisterFactory(new FileIOMetaImageFactory);
  myWriterType::Pointer writer = myWriterType::New();
  myFilterType::Pointer filter = myFilterType::New();

	filter->SetFileToLoad("D:/research/Angio002Series005.mhd");
  filter->Update();
  writer->SetFileName("D:/research/TestOutput.mhd");
  writer->SetInput(filter->GetOutput());
  writer->Write();

  return 0;
}
