#ifndef BOUNDARYVALUES_HPP_
#define BOUNDARYVALUES_HPP_

//typedef vector<double> dirichlet_bc_;
//typedef vector<double> neumann_bc_;


class BoundaryValues
{
private:
  //vector<double> dirichlet_bc_(2);
  //vector<double> neumann_bc_(2);

  //  vector<dirichlet_bc_>* temp_;
  // vector<neumann_bc_>* heat_flow_;
  int coll_point_idx_;
  double bound_value_;
  int bc_; //1 Dirichlet BC, 2 Neumann BC

public:

  BoundaryValues(){;}

  ~BoundaryValues(){;}

  void SetBoundCond(int coll_index, double bc_value, int bc)
  {
    coll_point_idx_= coll_index;
    bound_value_=bc_value;
    bc_= bc;
  }

  int GetCollPointID()
  {
    return coll_point_idx_;
  }

  double GetBoundaryValue()
  {
    return bound_value_;
  }

  int GetBC()
  {
    return bc_;
  }
}; //class BoundaryValues

#endif //BOUNDARYVALUES_HPP_
