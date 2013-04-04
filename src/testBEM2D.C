#include <iostream>
#include <cstdlib>

#include "../hyena/simple/BEMesh2D.hpp"
#include "../hyena/simple/ElementBEM2D.hpp"
#include "../hyena/simple/BEMatrices.hpp"
#include "../hyena/simple/BoundaryValues.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

/* simple 2D BEM example to getting used of HyENA
 * implementation of Laplace equation
 * with Mixed (Dirichlet-Neumann) boundary condition
 * using direct approach to getting the boundary integral equation
 * using collocation method for discretization

 */

int main(int argc, char* argv[])
{
  int max_refine = 0;

  std::cout << "argc = " << argc << std::endl;
  for (int i = 0; i < argc; i++)
    std::cout << "argv[" << i << "] = " << argv[i] << std::endl;

  if (argc>1)
    max_refine = std::atoi(argv[1]);
  std::cout << "Refinement level: " << max_refine << std::endl;

  BEMesh2D mesh_obj(max_refine);
  ReadMesh(mesh_obj);
  //mesh_obj.createBEMesh2D(max_refine);

  //initialization of boundary elements
  int nr_edges=mesh_obj.GetNrEdges();
  vector<ElementBEM2D> elem(nr_edges);

  for(int i=0; i<nr_edges; i++)
    elem[i].SetBElem(mesh_obj, i);
  //  for(int i=0; i<nr_edges; i++)
  //  elem[i].GetBElem();

  //assemlbe matrices
  // collocation method with direct approach
  matrix<double> double_col_matrix(nr_edges,nr_edges);
  matrix<double> single_col_matrix(nr_edges,nr_edges);
  computeDirectColMatrix(double_col_matrix,single_col_matrix,elem);

  //Define Dirichlet boundary condition
  //Temperature at collocation node 0 and 1: 50
  //Temperature at collocation node 3 and 4: 0
  //Heat Flow at collocation node 2 and 5: 0
  //The third parameter of SetBoundCond defines the kind of boundary condition, 1 means Dirichlet BC and 2 means Neumann BC

  vector<BoundaryValues> bc_heatcond(nr_edges);
  bc_heatcond[0].SetBoundCond(0,0,1);
  bc_heatcond[1].SetBoundCond(1,0,1);
  bc_heatcond[2].SetBoundCond(2,0,2);
  bc_heatcond[3].SetBoundCond(3,50,1);
  bc_heatcond[4].SetBoundCond(4,50,1);
  bc_heatcond[5].SetBoundCond(5,0,2);

  //rearrange the single and double layer collocation matrix depending on the given boudary values
  std::cout << "Vector size bc_heatcond_: " << bc_heatcond.size() << std::endl;
  matrix<double> rhs_matrix(bc_heatcond.size(),bc_heatcond.size());
  matrix<double> lhs_matrix(bc_heatcond.size(),bc_heatcond.size());
  vector<double> rhs_vector(bc_heatcond.size());
  for(int i=0; i < bc_heatcond.size(); i++)
    {
      if(bc_heatcond[i].GetBC()==1)
	{
	  column(rhs_matrix,i)=column(-double_col_matrix,bc_heatcond[i].GetCollPointID());
	  column(lhs_matrix,i)=column(-single_col_matrix,bc_heatcond[i].GetCollPointID());
	}
      else if (bc_heatcond[i].GetBC()==2)
	{
	  column(rhs_matrix,i)=column(single_col_matrix,bc_heatcond[i].GetCollPointID());
	  column(lhs_matrix,i)=column(double_col_matrix,bc_heatcond[i].GetCollPointID());
	}
      else
	std::cout << "Throw Exceptions: " << std::endl;

      rhs_vector[i]=bc_heatcond[i].GetBoundaryValue();
      // std::cout << "RHS_MATRIX " << std::endl;
      //std::cout << rhs_matrix << std::endl;
      //std::cout << "RHS_VECTOR: " << rhs_vector << std::endl;
    }

    //compute rhs 
  assert(rhs_matrix.size1()==rhs_vector.size());

  rhs_vector = prod(rhs_matrix,rhs_vector);

  //solving linear system lhs_matrix*sol_u=rhs_vector
  //using library boost lu_factorize and lu_substitute

  permutation_matrix<std::size_t> pm(lhs_matrix.size1());
  vector<double> sol_u(rhs_vector);
  //  std::cout << "LHS_MATRIX: " << lhs_matrix << std::endl;
  //std::cout << "Solution u: " << sol_u << std::endl;
  lu_factorize(lhs_matrix,pm);
  //  std::cout << "After LU_FACTORIZE: " << std::endl;
  //  std::cout << lhs_matrix << std::endl;
  //  std::cout << "permutation matrix: " << std::endl;
  //  std::cout << pm << std::endl;
  lu_substitute(lhs_matrix,pm,sol_u);
  //  std::cout << "After LU_SUBSTITUTE: " << std::endl;
  //  std::cout << lhs_matrix << std::endl;
  //  std::cout << "permutation matrix: " << std::endl;
  //  std::cout << pm << std::endl;
  std::cout << "Solution u: " << sol_u << std::endl;

}



