/**
 * This is a demonstration of how to write and read MetaImage files to and from disk 
 * using the ImageIO framework. It's a simple loopback test: read a sample file from disk
 * and then write it back out. Currently, the user will have to open both files and do a
 * visual comparison to ensure they're identical, but in the future even this can be 
 * automated via a diff command.
 */

#include <itkPhysicalImage.h>
#include <itkFileIOMetaImage.h>
#include <itkWriteMetaImage.h>
#include <itkFileIOToImageFilter.h>

using namespace itk;
using namespace std;

int main ()
{
  /* 
   * This defines our image. Note that while we use a PhysicalImage for this test
   * (because MetaImage defines a mapping between image-space to physical-space),
   * you can just as easily substitute an Image in its place.
   */
  typedef PhysicalImage<unsigned short, 3> myImageType;
  /*
   * This defines our MetaImage writer
   */
  typedef WriteMetaImage<myImageType> myWriterType;
  /*
   * This defines our MetaImage "reader-cum-pixel-translator" (filter).
   * It not only reads the file from disk, but also typecasts the pixels from
   * the native pixel type on disk (say, signed char) into the pixel type of our
   * Image (in this case, unsigned short)
   * Note that by subclassing from FileIOToImageFilter, one can perform more sophisticated
   * pixel conversions, e.g. rgb to grayscale conversion.
   */
  typedef FileIOToImageFilter<myImageType> myFilterType;

  /*
   * This call manually registers the MetaImage reader with the master object factory.
   * This establishes a mapping between the class that can handle MetaImage files
   * (FileIOMetaImage), and the file extensions associated with that filetype
   * (.mha, .mhd, .mda)
   * Note that once we compile the MetaImageIO library as a DLL, this call is no longer
   * necessary. It is only needed when using static libraries, as we are currently doing.
   */
  ObjectFactoryBase::RegisterFactory(new FileIOMetaImageFactory);
  myWriterType::Pointer writer = myWriterType::New();
  myFilterType::Pointer filter = myFilterType::New();

  /*
   * Now tell the filter which file to load
   */
	filter->SetFileToLoad("TestInput.mhd");
  /*
   * This call forces the filter to update itself, which in this case causes
   * the file to be loaded from disk.
   * Note that this call will not be needed once filter pipelining and interconnection is
   * implemented, as the call to writer->SetInput() would automatically cause a cascading
   * series of checks up the pipeline to see what filters/datasources/etc. have been
   * modified and need to be re-evaluated.
   */
  filter->Update();
  /*
   * Tell the writer which file to write to
   */
  writer->SetFileName("TestOutput.mhd");
  /*
   * Hook the output of the reader to the input of the writer
   */
  writer->SetInput(filter->GetOutput());
  /*
   * And finally write the image back out to disk
   */
  writer->Write();

  return 0;
}
