#ifndef PressureSensitiveBacteria_H
#define PressureSensitiveBacteria_H

#include "Bacteria.h"


namespace bio {


/**
 * \class Bacteria 
 * \brief This class implements the basic behavior of 
 * pressure sensitive cells. This cells will arrest in
 * G1 if the pressure exceeds a limit.
 */
class PressureSensitiveBacteria  : public Bacteria
{


public:

  typedef  Bacteria  SuperClass;

  PressureSensitiveBacteria();
  virtual ~PressureSensitiveBacteria();

  virtual bool CheckPointDivision(void);

  virtual void ClearForce(void);
  virtual void AddForce(const VectorType & force);
  virtual void Divide( void );

  static  Cell * CreateEgg(void);

  static const char * GetSpeciesName(void) 
                          { return "Pressure Sensitive Bacteria"; }
  virtual void SetColorByPressure( bool );

private:
  
  virtual Cell * CreateNew(void);

  double                   m_Pressure;
  double                   m_PreviousPressure;

  bool                     m_CycleArrest;
  bool                     m_ColorByPressure;
  
  static double            PressureThresold;

  static ColorType         LowPressureColor;
  static ColorType         MediumPressureColor;
  static ColorType         HighPressureColor;
  static ColorType         VeryHighPressureColor;
  
};


} // end namespace bio

#endif
