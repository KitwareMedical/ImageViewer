


#include "Genome.h"



namespace bio {




/**
 *    Constructor 
 */ 
Genome
::Genome()
{

}




/**
 *    Destructor   
 */ 
Genome
::~Genome()
{
}



/**
 *    Copy from another genome
 */ 
void
Genome
::Copy( const Genome & genome )
{
  m_Map.clear();
  m_Map.insert( genome.m_Map.begin(), genome.m_Map.end() );
}




/**
 *    Inset a gene in the genome   
 */ 
void
Genome
::InsertGene( const GeneIdType & geneId )
{
  // operator[] will create the geneId if
  // it doesn't exist yet
  // By default the gene is inhibited
  m_Map[ geneId ] = 0.0;
}



/**
 *    Knockout a gene in the genome   
 */ 
void
Genome
::KnockOutGene( const GeneIdType & geneId )
{
  m_Map.erase( geneId );
}



/**
 *    Return the level of expression of a particular gene
 */ 
double
Genome
::GetExpressionLevel( const GeneIdType & geneId )
{
  return m_Map[ geneId ];
}





/**
 *    Set the level of expression of a particular gene
 */ 
double
Genome
::SetExpressionLevel( const GeneIdType & geneId, double level )
{
  m_Map[ geneId ] = level;
}





 
}  // end namespace bio
