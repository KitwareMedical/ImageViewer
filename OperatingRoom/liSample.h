//--------------------------------------------
//
//		Project Registrator
//
//		Author:  Luis Ibanez
//	
//		Directed by: Elizabeth Bullitt, MD
//
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27514
//
//--------------------------------------------

#ifndef liSAMPLE
#define liSAMPLE

#include <itkPoint.h>


class ITK_EXPORT liSample {

public:
  typedef     double                     CoordType;
  typedef     itk::Point<CoordType,3>    PointType;
  typedef     itk::Vector<CoordType,3>   VectorType;
  PointType   point;
  float       value; 
};



#endif
