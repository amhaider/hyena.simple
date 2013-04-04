#ifndef BEMESH2D_HPP_
#define BEMESH2D_HPP_

#include <iostream> //std::cin, std::cout
#include <fstream>  //std::ifstream
#include <assert.h> //assert()
//#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
using namespace boost::numeric::ublas;

//class BEMesh2D;

struct Vertex
{
  vector<double> vertex_coord;
  //vector<int> edge_list;
  int vertex_index;
  Vertex() : vertex_coord(2){}   // , edge_list(2){}
};

struct Edge
{
  int edge_index;
  vector<int> vertex_list;
  Edge() : vertex_list(2){}
};

class BEMesh2D
{

private:
  int nr_levels_;
  vector<Vertex>* vertices_;
  vector<Edge>* edges_;

public:
  BEMesh2D(){;}

  BEMesh2D(int refinNr)
  {
    std::cout << "Konstruktor" << std::endl;
    nr_levels_ = refinNr;
    vertices_= new vector<Vertex>((refinNr+1)*6);
    edges_ = new vector<Edge>((refinNr+1)*6);
  }

  ~BEMesh2D()
  {
    std::cout << "Destruktor" << std::endl;
    delete[] vertices_;
    delete[] edges_;
  }

  void AddRefinementLevel(int nr_vertices, int nr_edges);

  void SetVertex(float x, float y, int index)
  {
    //    std::cout << "SetVertex Function: " << std::endl;
    (*vertices_)[index].vertex_index = index;
    (*vertices_)[index].vertex_coord[0] = x;
    (*vertices_)[index].vertex_coord[1] = y;
  }

  void SetEdge(int index, int v_index1, int v_index2)
  {
    (*edges_)[index].edge_index = index;
    (*edges_)[index].vertex_list[0] = v_index1;
    (*edges_)[index].vertex_list[1] = v_index2;

  }

  int GetVertexIndex(int index)
  {
    return (*vertices_)[index].vertex_index;
  }

  vector<double> GetVertexCoord(int index)
  {
    return (*vertices_)[index].vertex_coord;
  }

  int GetEdgeIndex(int idx)
  {
    return (*edges_)[idx].edge_index;
  }
  vector<int> GetVertex (int idx)
  {
    return (*edges_)[idx].vertex_list;
  }
  int GetNrEdges()
  {
    return edges_->size();
  }
  
  void createBEMesh2D(int refine_level)
  {
    std::cout << "Klasse BEMesh2D " << refine_level << std::endl;
    std::cout << "Vector Size: " << vertices_->size() << std::endl;
  }
}; //class BEMesh2D

//void RefineMesh(BEMesh2D& mesh);


void ReadMesh(BEMesh2D& mesh) // second input ( const char* fileName)
{
  std::ifstream rectangle_file; //read only from files
  int nr_vertices, nr_edges, index_vertex, index_edge, index_vertex01, index_vertex02;
  float x, y;
  //vector<double> xy(2);
  //vector<int> node(2);


  //open file
  rectangle_file.open ("/home/anita/HYENAProject/hyena-simple/exampleMesh/rectangle.msh", std::ifstream::in);
  assert(rectangle_file.good()); // if a problem occurs when opening the file

  rectangle_file >> nr_vertices;
  std::cout << "Nr. of vertices: " << nr_vertices << std::endl;
  for(int i=1; i<=nr_vertices; i++)
    { 
      rectangle_file >> index_vertex >> x >> y ;
      mesh.SetVertex(x,y,index_vertex);
    
      // std::cout << "Index Vertex: " << mesh.GetVertexIndex(index_vertex) << " x-coordinate: " << mesh.GetVertexCoord(index_vertex)[0] << " y-coordinate: " << mesh.GetVertexCoord(index_vertex)[1] << std::endl;
    }
 
  rectangle_file >> nr_edges;
  std::cout << "Nr. of edges: " << nr_edges << std::endl;
  for(int i=1; i<=nr_vertices; i++)
    { 
      rectangle_file >> index_edge >> index_vertex01 >> index_vertex02 ;
      mesh.SetEdge(index_edge, index_vertex01, index_vertex02);
     
      // std::cout << "Index edge: " << mesh.GetEdgeIndex(index_edge) << " Index Vertex01: " << mesh.GetVertex(index_edge)[0] << " Index Vertex02: " << mesh.GetVertex(index_edge)[1] << std::endl;
    }

  // while (rectangle_file.good()) //loop while extraction from file is possible
  //   {
     
  //     char c = rectangle_file.get();
  //     if (rectangle_file.good())
  // 	std::cout << c;
  //   }
  rectangle_file.close();
}



#endif //BEMESH2D_HPP_
