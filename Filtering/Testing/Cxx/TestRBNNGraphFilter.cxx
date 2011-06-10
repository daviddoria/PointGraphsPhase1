#include <vtkPolyData.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkGraph.h>
#include <vtkSmartPointer.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkGraphReader.h>

#include <vtkstd/limits>

#include "vtkRBNNGraphFilter.h"

template<class A>
bool fuzzyCompare(A a, A b)
{
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

int TestRBNNGraphFilter(int, char*[])
{
  // Create data set
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(1,0,0);
  points->InsertNextPoint(0,1,0);

  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);

  // Create nearest neighbor graph
  vtkSmartPointer<vtkRBNNGraphFilter> rbnnGraphFilter =
    vtkSmartPointer<vtkRBNNGraphFilter>::New();
  rbnnGraphFilter->SetRadius(2.0);
  rbnnGraphFilter->SetInputConnection(polydata->GetProducerPort());
  rbnnGraphFilter->Update();

  vtkSmartPointer<vtkMutableUndirectedGraph> groundTruth =
    vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  vtkIdType v0 = groundTruth->AddVertex();
  vtkIdType v1 = groundTruth->AddVertex();
  vtkIdType v2 = groundTruth->AddVertex();
  groundTruth->AddEdge(v0, v1);
  groundTruth->AddEdge(v0, v2);
  groundTruth->AddEdge(v1, v2);

  if(groundTruth->GetNumberOfVertices() != rbnnGraphFilter->GetOutput()->GetNumberOfVertices())
    {
    std::cout << "groundTruth->GetNumberOfVertices() != nearestNeighborGraph->GetNumberOfVertices()" << std::endl;
    std::cout << "ground truth: " << groundTruth->GetNumberOfVertices() << std::endl;
    std::cout << "nearest neighbor graph: " << rbnnGraphFilter->GetOutput()->GetNumberOfVertices() << std::endl;
    return EXIT_FAILURE;
    }

  if(groundTruth->GetNumberOfEdges() != rbnnGraphFilter->GetOutput()->GetNumberOfEdges())
    {
    std::cout << "groundTruth->GetNumberOfEdges() != nearestNeighborGraph->GetNumberOfEdges()" << std::endl;
    std::cout << "ground truth: " << groundTruth->GetNumberOfEdges() << std::endl;
    std::cout << "nearest neighbor graph: " << rbnnGraphFilter->GetOutput()->GetNumberOfEdges() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
