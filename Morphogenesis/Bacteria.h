#ifndef Bacteria_H
#define Bacteria_H

#include "Cell.h"


namespace bio {


class Bacteria  : public Cell
{


public:
  Bacteria();
  ~Bacteria();
  void Draw(void) const;
  void Grow(void);

private:

};


}; // end namespace bio

#endif
