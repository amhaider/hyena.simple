#ifndef ELEMENTBEM2D_HPP_
#define ELEMENTBEM2D_HPP_

#include "BEMesh2D.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
//#include<vector>
using namespace boost::numeric::ublas;


//----------------------------------------------------------------

class ElementBEM2D
{
private:
  vector<int> vertex_idx_list;
  vector<double> vertex0_coord_;
  vector<double> vertex1_coord_;
  vector<double> direction_vector;
  vector<double> normal_;
  vector<double> collocation_point_;
  double area_;
  int index;

public:
  ElementBEM2D() : vertex_idx_list(2), vertex0_coord_(2), vertex1_coord_(2), direction_vector(2), normal_(2), collocation_point_(2), index(0)
  {;}
  ~ElementBEM2D(){;}

  void SetBElem(BEMesh2D& mesh, int i)
  {
    //    vector<int> vertex_idx_list(2);
    vertex_idx_list = mesh.GetVertex(i);
    index = i;
    vertex0_coord_ = mesh.GetVertexCoord(vertex_idx_list(0));
    vertex1_coord_ = mesh.GetVertexCoord(vertex_idx_list(1));

    //    vector<double> direction_vector(2);
    direction_vector = (vertex1_coord_ - vertex0_coord_);
    area_=norm_2(direction_vector);
    direction_vector *= 1.0/area_;
    normal_[0] = direction_vector[1];
    normal_[1] = -direction_vector[0];
    collocation_point_ = (vertex0_coord_ + vertex1_coord_)*0.5; // collocation point = midpoint

  }
  void GetBElem()
  {
    std::cout << "GetBElem index: " << index << std::endl;
    std::cout << "Vertex index" << vertex_idx_list << std::endl;
    std::cout << "Vertex0 Koordinate: " << vertex0_coord_ << std::endl;
    std::cout << "Vertex1 Koordinate: " << vertex1_coord_ << std::endl;
    std::cout << "Direction Vector: " << direction_vector << std::endl;
    std::cout << "Normal Vector: " << normal_ << std::endl;
    std::cout << "Midpoint: " << collocation_point_ << std::endl;

  }

  vector<double> GetCollocationPoint()
  {
    return this->collocation_point_;
  }

  vector<double> GetVertexCoord0 ()
  {
    return this->vertex0_coord_;
  }

  vector<double> GetVertexCoord1 ()
  {
    return this->vertex1_coord_;
  }

  vector<double> GetDirection()
  {
    return this->direction_vector;
  }

  vector<double> GetNormal()
  {
    return this->normal_;
  }
  double GetArea()
  {
    return this->area_;
  }

  int GetElementIndex()
  {
    return this->index;
  }

}; //class ElementBEM2D


#endif //ELEMENTBEM2D_HPP_
