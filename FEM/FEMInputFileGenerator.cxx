// disable debug warnings in MS  compiler
#ifdef _MSC_VER
#pragma  warning(disable: 4786)
#endif

#include <stdio.h>
#include <iostream.h>

void main()
{
  int N = 5;      // # elements in each direction
  int dim = 3;    // # spatial dimensions

  // nodes
  float* pt = new float[3];
  for (int k=0; k<N+1; k++) {
    for(int i=0; i<N+1; i++) {
      for(int j=0; j<N+1; j++) {
  pt[0]=-5.0+10.0/N*j;
  pt[1]=-5.0+10.0/N*i;
  pt[2]=-5.0+10.0/N*k;
  
  std::cout << "<Node>\n";
  std::cout << "\t" << ( k*(N+1)*(N+1) + i*(N+1) + j ) << "\t% Global object number\n";
  std::cout << "\t" << dim << " " << pt[0] << " " << pt[1] << " " << pt[2] << "\t% Nodal coordinates\n";
      }
    }
  } 
  delete(pt);

  std::cout << std::endl << "<END>\t% End of nodes" << std::endl;

  // material

  std::cout << std::endl << "<MaterialLinearElasticity>" << std::endl;
  std::cout << "\t0\n";
  std::cout << "\tE: 3000 % E\n";
  std::cout << "\tA: 0  % A\n";
  std::cout << "\tI: 0  % I\n";
  std::cout << "\tnu: 0.29  % nu\n";
  std::cout << "\th: 1\n";
  std::cout << "\tRhoC: 1\n";
  std::cout << "\tEND:  % End of material definition\n\n";
  
  std::cout << "<END>  % End of materials\n\n";
  
  // elements
  for (int k=0; k<N; k++) {
    for(int j=0; j<N; j++) {
      for(int i=0; i<N; i++) {
  std::cout << "<Element3DC0LinearHexahedronStrain>\n";
  std::cout << "\t" << ( k*N*N + j*N + i ) << "\t% Global element number\n";
  std::cout << "\t" << ( k*(N+1)*(N+1) + j*(N+1) + i ) << "\t% Node 1 ID\n";
  std::cout << "\t" << ( k*(N+1)*(N+1) + j*(N+1) + (i+1) ) << "\t% Node 2 ID\n";
  std::cout << "\t" << ( k*(N+1)*(N+1) + (j+1)*(N+1) + (i+1) ) << "\t% Node 3 ID\n";
  std::cout << "\t" << ( k*(N+1)*(N+1) + (j+1)*(N+1) + i ) << "\t% Node 4 ID\n";
  std::cout << "\t" << ( (k+1)*(N+1)*(N+1) + j*(N+1) + i ) << "\t% Node 5 ID\n";
  std::cout << "\t" << ( (k+1)*(N+1)*(N+1) + j*(N+1) + (i+1) ) << "\t% Node 6 ID\n";
  std::cout << "\t" << ( (k+1)*(N+1)*(N+1) + (j+1)*(N+1) + (i+1) ) << "\t% Node 7 ID\n";
  std::cout << "\t" << ( (k+1)*(N+1)*(N+1) + (j+1)*(N+1) + i ) << "\t% Node 8 ID\n";
  std::cout << "\t" << "0" << "\t% Material ID\n";
      }
    }
  }

  std::cout << "\n<END>  % End of elements\n\n";

  // gravity load
  
  std::cout << "<LoadGravConst>\n";
  std::cout << "\t0\t% Global load  number\n";
  std::cout << "\t-1\t% Load acts on all elements\n";
  std::cout << "\t" << dim << "\t% Size of the gravity force vector\n";
  std::cout << "\t" << "0 0 -100" << "\t% Force vector\n";

  // BCs - fix all nodes in the x-y plane
  int nload = 0;

  for (int el=0; el < N*N; el++) {
    // Fix node 0 in the corner (origin) element
    if (el == 0) {
      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "0" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";

      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "1" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";

      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "2" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";

    }
    
    // Fix node 3 in elements along the y-axis
    if (el % N == 0) {
      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "9" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";

      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "10" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";
 
      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "11" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";
    }

    // Fix node 1 in elements along the x-axis
    if (el / N == 0) {
      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "3" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";

      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "4" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";
 
      std::cout << "<LoadBC>\n";
      std::cout << "\t" << ++nload << "\t% Global load number\n";
      std::cout << "\t" << el << "\t% GN of element\n";
      std::cout << "\t" << "5" << "\t% DOF# in element\n";
      std::cout << "\t" << "1 0" << "\t% rhs of MFC\n"; 
    }

    // Finally, fix node 2 in all these elements
    std::cout << "<LoadBC>\n";
    std::cout << "\t" << ++nload << "\t% Global load number\n";
    std::cout << "\t" << el << "\t% GN of element\n";
    std::cout << "\t" << "6" << "\t% DOF# in element\n";
    std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";
    
    std::cout << "<LoadBC>\n";
    std::cout << "\t" << ++nload << "\t% Global load number\n";
    std::cout << "\t" << el << "\t% GN of element\n";
    std::cout << "\t" << "7" << "\t% DOF# in element\n";
    std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";
    
    std::cout << "<LoadBC>\n";
    std::cout << "\t" << ++nload << "\t% Global load number\n";
    std::cout << "\t" << el << "\t% GN of element\n";
    std::cout << "\t" << "8" << "\t% DOF# in element\n";
    std::cout << "\t" << "1 0" << "\t% rhs of MFC\n";
  }    
   
  std::cout << "\n<END>  % End of loads\n";

  return 0;
}
