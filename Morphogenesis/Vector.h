#ifndef Vector_H
#define Vector_H


class Vector 
{

public:

	Vector() {
	  m_X = 0;
	  m_Y = 0;
	};

   Vector(double ix, double iy)
   {
	 m_X = ix;
     m_Y = iy;
   }

   const Vector & operator=( const Vector & p )
   {
	   m_X = p.m_X;
	   m_Y = p.m_Y;
	   return *this;
   }


   ~Vector();

   double X(void) const 
   { return m_X;  }

   double Y(void) const 
   { return m_Y;  }



private:
   double m_X;
   double m_Y;

};

Vector operator-(const Point & a, const Point & b)
{
	return Vector( a.X() - b.X(), a.Y() - b.Y() );
}

Point   operator+(const Point & a, const Vector & v )'
{   
	return Point( a.X() + b.X() , a.Y() + b.Y() );
}


Point   operator+(const Vector & a, const Point & v )'
{   
	return Point( a.X() + b.X() , a.Y() + b.Y() );
}


#endif
