#include <vtkSmartPointer.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkGraphReader.h>

#include <vtkstd/limits>

#include "vtkConnectGraph.h"

template<class A>
bool fuzzyCompare(A a, A b)
{
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

int TestConnectGraph(int, char *[])
{
  // Create a graph.
  vtkSmartPointer<vtkMutableUndirectedGraph> input =
    vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  {
  vtkIdType v0 = input->AddVertex();
  vtkIdType v1 = input->AddVertex();
  input->AddVertex();
  input->AddEdge(v0,v1);
  }
  // Connect graph
  vtkSmartPointer<vtkConnectGraph> connectGraphFilter =
    vtkSmartPointer<vtkConnectGraph>::New();
  connectGraphFilter->SetInputConnection(input->GetProducerPort());
  connectGraphFilter->Update();

  // Create ground truth (the input graph with the disconnected componets
  // connected in the best (shortest) way)
  vtkSmartPointer<vtkMutableUndirectedGraph> groundTruth =
    vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  {
  vtkIdType v0 = groundTruth->AddVertex();
  vtkIdType v1 = groundTruth->AddVertex();
  vtkIdType v2 = groundTruth->AddVertex();
  groundTruth->AddEdge(v0,v1);
  groundTruth->AddEdge(v0,v2);
  }

  if(groundTruth->GetNumberOfVertices() != connectGraphFilter->GetOutput()->GetNumberOfVertices())
    {
    std::cerr << "groundTruth->GetNumberOfVertices() != connectedGraph->GetNumberOfVertices() : "
              << groundTruth->GetNumberOfVertices() << " vs " << connectGraphFilter->GetOutput()->GetNumberOfVertices() << std::endl;
    return EXIT_FAILURE;
    }

  if(groundTruth->GetNumberOfEdges() != connectGraphFilter->GetOutput()->GetNumberOfEdges())
    {
    std::cerr << "GroundTruth->GetNumberOfEdges() != connectedGraph->GetNumberOfEdges() : "
              << groundTruth->GetNumberOfEdges() << " vs " << connectGraphFilter->GetOutput()->GetNumberOfEdges() << std::endl;
    return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}
