#ifndef BEMATRICES_HPP_
#define BEMATRICES_HPP_

#include <iostream>
#include <assert.h>
#include <math.h>

#include "ElementBEM2D.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
using namespace boost::numeric::ublas;

#define PI (3.14159265358979323846)

double evalSinglePotCol2D (ElementBEM2D &elem, vector<double> coll_point_)
{
  if (norm_2(elem.GetCollocationPoint()-coll_point_)==0.0)
	    return (0.5+log(2))/(2*PI);   //habe ich analytisch berechnet
  else
    {
      vector<double> t = elem.GetVertexCoord0() - coll_point_;
      //  double a = inner_prod(t,elem.GetNormal());
      double b = inner_prod(t,t);
      double c = inner_prod(elem.GetDirection(),t);
      //      std::cout << "A: " << a << std::endl;
      //      std::cout << "C: " << b << std::endl;
      //      std::cout << "D: " << c << std::endl;
      return (-1/(4*PI))*((2*sqrt(b-pow(c,2))*(atan((c+elem.GetArea())/sqrt(b-pow(c,2)))-atan(c/sqrt(b-pow(c,2))))) + (c+elem.GetArea())*log(b+2*c*elem.GetArea()+pow(elem.GetArea(),2)) - 2*elem.GetArea() -c*log(b));
    }
      
}

double evalDoublePotCol2D (ElementBEM2D &elem, vector<double> coll_point_)
{
  
  if (norm_2(elem.GetCollocationPoint()-coll_point_)==0.0)
	    return 0.5; //siehe Gaul Waermeleitung 2D
  else if ((((elem.GetVertexCoord0())[0]==(elem.GetVertexCoord1())[0]) && (elem.GetVertexCoord1()[0]==coll_point_[0])) || ((elem.GetVertexCoord0()[1]==elem.GetVertexCoord1()[1])&&(elem.GetVertexCoord1()[1]==coll_point_[1])))
       return 0;
  else 
    {
      vector<double> t = elem.GetVertexCoord0() - coll_point_;
      double a = inner_prod(t,elem.GetNormal());
      double b = inner_prod(t,t);
      double c = inner_prod(elem.GetDirection(),t);
      //      std::cout << "A: " << a << std::endl;
      //      std::cout << "C: " << b << std::endl;
      //      std::cout << "D: " << c << std::endl;
      std::cout << "index of element " << elem.GetElementIndex() << std::endl;
      std::cout << "collocation point: " << coll_point_ << std::endl;
      return -(1/(2*PI))*(a/(sqrt(b-pow(c,2))))*(atan((c+elem.GetArea())/(sqrt(b-pow(c,2))))-atan(c/(sqrt(b-pow(c,2)))));
    }
     
}


void computeDirectColMatrix (matrix<double> &m, matrix<double> &n, vector<ElementBEM2D> &elem)
{
  //std::cout << "Collocation Matrix before: " << std::endl;
  //std::cout <<  m << std::endl;

  assert(m.size1()==elem.size());
  assert(m.size2()==elem.size());

  assert(n.size1()==elem.size());
  assert(n.size2()==elem.size());

  for (int i=0; i < elem.size(); i++)  // element
    {
      for (int j=0; j <elem.size(); j++) // collocation points
	{
	  //std::cout << "Collocation point: " << elem[j].GetCollocationPoint() << std::endl;
	  m(j,i) = evalDoublePotCol2D(elem[i],elem[j].GetCollocationPoint());
	  n(j,i) = evalSinglePotCol2D(elem[i],elem[j].GetCollocationPoint());
	}
    }
  std::cout << "Double Layer Matrix: " << std::endl;
  std::cout << m << std::endl;
  std::cout << "Single Layer Matrix: " << std::endl;
  std::cout << n << std::endl;
  std::cout << "n(0,1): " << n(0,1) << std::endl;

};


#endif  //BEMATRICES_HPP_
