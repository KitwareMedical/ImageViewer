


#ifndef   __MeshTraits_H
#define   __MeshTraits_H

namespace itk
{

/**
 * MeshTraits provides named template parameters for Mesh
 * configuration.  It has one template argument and one member
 * template per trait.  The order of the arguments does not matter.
 */
template <typename TPixelType = char,
          unsigned int NPointDimension = 3;
          unsigned int NMaximumTopologicalDimension = 3;
          typename TCoordRep = float,
          typename TInterpolationWeight = float,
          typename TCellPixelType = char >
struct MeshTraits
{
  typedef TPixelType          PixelType;
  typedef TCellPixelType      CellPixelType;
  typedef TCoordRep           CoordRepType;

  enum { PointDimension = NPointDimension };
  enum { MaxTopologicalDimension = NMaximumTopologicalDimension };
  
  typedef Point< CoordRepType, PointDimension >     PointType;

  typedef itkMakeCellTraitsMacro                    CellTraits;


  /** Typedef this class's own type.  
    * This allows to recursively define a nested type which is
    * identical to the current templated type but with one of
    * the template arguments modified.
    */
  typedef MeshTraits<
                     PixelType,
                     PointDimension,
                     MaxTopologicalDimension,
                     CoordRepType,
                     InterpolationWeightType,
                     CellPixelType
                                      >                   Traits;
  
  /** Template for changing the PointDataType.  */
  template <typename NewPixelType>
  struct SetPixelType: public MeshTraits<
                     NewPixelType,
                     PointDimension,
                     MaxTopologicalDimension,
                     CoordRepType,
                     InterpolationWeightType,
                     CellPixelType 
                      >
  { 
    typedef typename MeshTraits<
                             NewPixelType,
                             PointDimension,
                             MaxTopologicalDimension,
                             CoordRepType,
                             InterpolationWeightType,
                             CellPixelType
                                                >::Traits Traits; };
   
  /** Template for changing the PointDimension.  */
  template <unsigned int  NewPointDimension>
  struct SetPointDimension: public MeshTraits<
                     PixelType,
                     NewPointDimension,
                     MaxTopologicalDimension,
                     CoordRepType,
                     InterpolationWeightType,
                     CellPixelType 
                      >
  { 
    typedef typename MeshTraits<
                             PixelType,
                             NewPointDimension,
                             MaxTopologicalDimension,
                             CoordRepType,
                             InterpolationWeightType,
                             CellPixelType
                                                >::Traits Traits; };


  
  /** Template for changing the MaxTopologicalDimension.  */
  template <unsigned int  NewMaxTopologicalDimension>
  struct SetMaximumTopologicalDimension: public MeshTraits<
                     PixelType,
                     PointDimension,
                     NewMaxTopologicalDimension,
                     CoordRepType,
                     InterpolationWeightType,
                     CellPixelType 
                      >
  { 
    typedef typename MeshTraits<
                             PixelType,
                             PointDimension,
                             NewMaxTopologicalDimension,
                             CoordRepType,
                             InterpolationWeightType,
                             CellPixelType
                                                >::Traits Traits; };
  

  
  /** Template for changing the CoordRepType.  */
  template < typename NewCoordRepType>
  struct SetCoordinateRepresentationType: public MeshTraits<
                     PixelType,
                     PointDimension,
                     MaxTopologicalDimension,
                     NewCoordRepType,
                     InterpolationWeightType,
                     CellPixelType 
                      >
  { 
    typedef typename MeshTraits<
                             PixelType,
                             PointDimension,
                             MaxTopologicalDimension,
                             NewCoordRepType,
                             InterpolationWeightType,
                             CellPixelType
                                                >::Traits Traits; };
  


  
  /** Template for changing the InterpolationWeightType.  */
  template < typename NewInterpolationWeightType>
  struct SetCoordinateRepresentationType: public MeshTraits<
                     PixelType,
                     PointDimension,
                     MaxTopologicalDimension,
                     CoordRepType,
                     NewInterpolationWeightType,
                     CellPixelType 
                      >
  { 
    typedef typename MeshTraits<
                             PixelType,
                             PointDimension,
                             MaxTopologicalDimension,
                             CoordRepType,
                             NewInterpolationWeightType,
                             CellPixelType
                                                >::Traits Traits; };
  



  
  /** Template for changing the CellPixelType.  */
  template < typename NewCellPixelType>
  struct SetCoordinateRepresentationType: public MeshTraits<
                     PixelType,
                     PointDimension,
                     MaxTopologicalDimension,
                     CoordRepType,
                     InterpolationWeightType,
                     NewCellPixelType 
                      >
  { 
    typedef typename MeshTraits<
                             PixelType,
                             PointDimension,
                             MaxTopologicalDimension,
                             CoordRepType,
                             InterpolationWeightType,
                             NewCellPixelType
                                                >::Traits Traits; };
  



};



#endif
