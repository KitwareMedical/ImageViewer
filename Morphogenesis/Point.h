#ifndef Point_H
#define Point_H


class Point 
{

public:
	Point() {
		m_X = 0;
		m_Y = 0;
	};

   Point(double ix, double iy)
   { 
	 m_X = ix;
     m_Y = iy;
   }

   ~Point()
   {
   }

   const Point & operator=( const Point & p )
   {
	   m_X = p.m_X;
	   m_Y = p.m_Y;
	   return *this;
   }

   double X(void) const 
   { return m_X;  }

   double Y(void) const 
   { return m_Y;  }

    

private:
   double m_X;
   double m_Y;

};

#endif
