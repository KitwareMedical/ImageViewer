//--------------------------------------------
//
//     Project: Operating Room
//
//     Author:  Luis Ibanez
//
//     Division of Neurosugery
//     Department of Surgery
//     University of North Carolina
//     Chapel Hill, NC 27599
//
//--------------------------------------------


#include <liPatient.h>

namespace li {

//--------------------------------------------------
//
//		Creator
//
//--------------------------------------------------
Patient::Patient()
{

  m_Body = Box3D::New();

	m_Body->SetLength( 1200.0f );
  m_Body->SetWidth(   400.0f );
  m_Body->SetHeight(  150.0f );
  m_Body->SetDrawingMode( lines );

  m_Spine = SpineModel::New();

	m_ClinicalPosition = Ventral;

  m_Vessels = VectorTubes3D::New();
    
  AddComponent( m_Body.GetPointer() );
  AddComponent( m_Spine.GetPointer() );
  AddComponent( m_Vessels.GetPointer() );

}



//--------------------------------------------------
//
//		Destructor	
//
//--------------------------------------------------
Patient::~Patient() 
{
}




	
//--------------------------------------------------
//
//		return extrinsic matrix
//
//--------------------------------------------------
Patient::TransformType::ConstPointer 
Patient::GetExtrinsic(void) const 
{
	return m_Extrinsic.GetPointer();
}


	
//--------------------------------------------------
//
//		Return a Pointer to the Spine Model
//
//--------------------------------------------------
SpineModel::Pointer &
Patient::GetSpineModel(void)  
{
	return m_Spine;
}



	
//--------------------------------------------------
//
//		Return a Pointer to the Vessels Model
//
//--------------------------------------------------
VectorTubes3D::Pointer &
Patient::GetVesselsModel(void)  
{
	return m_Vessels;
}




//--------------------------------------------------
//
//		Draw patient representation in OpenGL
//
//--------------------------------------------------
void 
Patient::DrawGeometry(void)  const 
{

	switch( m_ClinicalPosition ) 
  {
	case Supine: 
    {
		  break;
		}
	case Ventral: 
    {
  		glTranslated( 0.0, 0.0, m_Body->GetHeight() );
	  	glRotated( 180, 1.0, 0.0, 0.0 );
		  break;
		}
  case LateralRight:
    {
  		glTranslated( 0.0, 0.0, m_Body->GetHeight() );
	  	glRotated( 90, 1.0, 0.0, 0.0 );
		  break;
		}
  case LateralLeft:
    {
  		glTranslated( 0.0, 0.0, m_Body->GetHeight() );
	  	glRotated( 90, 1.0, 0.0, 0.0 );
		  break;
		}
  default:
    {
      std::cerr << "Patient: Unknown patient position " << std::cerr;
    }
	}


	m_Body->glDraw();

  glPushMatrix();

    glTranslated( m_Body->GetLength() / 2.0, 0.0, 0.0 );

    glPushMatrix();

      glRotated(  -90.0, 1.0, 0.0, 0.0 );
      glRotated(   90.0, 0.0, 1.0, 0.0 );

      m_Spine->glDraw();

    glPopMatrix();

    glRotated(  90.0, 0.0, 1.0, 0.0 );
    glRotated(  90.0, 0.0, 0.0, 1.0 );
    m_Vessels->glDraw();

  glPopMatrix();
}


	
//--------------------------------------------------
//
//		Sets patient clinical position
//
//--------------------------------------------------
void 
Patient::SetClinicalPosition( ClinicalPosition newPos )
{
	m_ClinicalPosition = newPos;
}




//--------------------------------------------------
//
//		Sets Corrections obtained from registration
//
//--------------------------------------------------
void 
Patient
::SetRegistrationOffset(const VectorType & trans, const VectorType & rot) 
{
	
	m_RegistrationCorrection->Translate( trans );
  m_RegistrationCorrection->Rotate( 1, 2, rot[0]);
  m_RegistrationCorrection->Rotate( 2, 0, rot[1]);
  m_RegistrationCorrection->Rotate( 0, 1, rot[2]);

}


//--------------------------------------------------
//
//		Clear Corrections obtained from registration
//
//--------------------------------------------------
void 
Patient::ClearRegistrationOffset(void) 
{
	
	m_RegistrationCorrection->SetIdentity();

}






//---------------------------------------------------------------------------
//
//  	Set the bound for Correction in translation
//
//---------------------------------------------------------------------------
void 
Patient::SetCorrectionTranslationBound( double limit )
{
  m_CorrectionTranslationBound = fabs(limit);
}





//---------------------------------------------------------------------------
//
//    Return if translation correction is out of bounds
//
//---------------------------------------------------------------------------
bool 
Patient
::CorrectionOutOfBounds(void) const
{

  const TransformType::OffsetType translation = 
                                  m_RegistrationCorrection->GetOffset();

  bool val = false;

  if( fabs( translation[ 0 ] ) > m_CorrectionTranslationBound )
  {
    val = true;
  }
  else if( fabs( translation[ 1 ] ) > m_CorrectionTranslationBound )
  {
    val = true;
  }
  else if( fabs( translation[ 2 ] ) > m_CorrectionTranslationBound )
  {
    val = true;
  }

  return val;

}


} // end namespace li



