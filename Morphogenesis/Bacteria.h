#ifndef Bacteria_H
#define Bacteria_H

#include "Cell.h"


namespace bio {


class Bacteria  : public Cell
{


public:
  Bacteria();
  ~Bacteria();
  virtual void Draw(void) const;
  virtual void Grow(void);
  virtual void Divide(void);

private:

};


}; // end namespace bio

#endif
