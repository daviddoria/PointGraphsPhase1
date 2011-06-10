/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkConnectGraph.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include <vtkAdjacentVertexIterator.h>
#include <vtkBoostConnectedComponents.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkExecutive.h>
#include <vtkGraph.h>
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkIntArray.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUndirectedGraph.h>

#include <vtkstd/limits>
#include <vtkstd/vector>

#include "vtkConnectGraph.h"

vtkStandardNewMacro(vtkConnectGraph);

int vtkConnectGraph::RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **vtkNotUsed(inputVector),
                                         vtkInformationVector *vtkNotUsed(outputVector))
{
  vtkUndirectedGraph *output = 0;
  output = vtkUndirectedGraph::New();

  this->GetExecutive()->SetOutputData(0, output);
  output->Delete();

  return 1;
}

int vtkConnectGraph::RequestData(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and ouptut
  vtkGraph *input = vtkGraph::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkGraph *output = vtkGraph::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Label the disconnected components of the graph
  vtkSmartPointer<vtkBoostConnectedComponents> connectedComponents =
    vtkSmartPointer<vtkBoostConnectedComponents>::New();
  connectedComponents->SetInput(input);
  connectedComponents->Update();

  vtkIntArray* components = vtkIntArray::SafeDownCast(connectedComponents->GetOutput()->GetVertexData()->GetArray("component"));

  // Get the lowest and highest component label
  int range[2];
  components->GetValueRange(range);

  // Check that the lowest component Id is 0.
  // The strategy is to connect everything to component 0, so if there is no
  // region 0, something is wrong!
  if(range[0] != 0)
    {
    vtkErrorMacro(<< "Region 0 does not exist!");
    return -1;
    }

  // Check that the graph is not already connected
  int highestComponentId = range[1];

  if(highestComponentId == 0) // There are no disconnected components
    {
    return -1;
    }

  // We must force the graph to be mutable because we will add edges
  vtkSmartPointer<vtkMutableUndirectedGraph> disconnectedGraph =
    vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  disconnectedGraph->ShallowCopy(connectedComponents->GetOutput());

  // Extract the points from the graph - these are needed to compute the distances
  // between vertices
  vtkPoints* points = disconnectedGraph->GetPoints();

  // We will be merging everything into this mainComponent by connecting disconnected
  // components to it.
  vtkSmartPointer<vtkIdList> mainComponent =
    vtkSmartPointer<vtkIdList>::New();
  components->LookupValue(0, mainComponent);

  // Start with component 1 because component 0 is the "base" region and connect
  // them to component 0
  for(int i = 1; i <= highestComponentId; i++)
  {
    // Get a list of the Ids of the vertices in the current disconnected component
    vtkSmartPointer<vtkIdList> disconnectedComponent =
      vtkSmartPointer<vtkIdList>::New();
    components->LookupValue(i, disconnectedComponent);

    // Make a vtkPolyData object consisting of the points currently in the mainComponent
    vtkSmartPointer<vtkPoints> mainComponentPoints = vtkSmartPointer<vtkPoints>::New();
    for(vtkIdType p = 0; p < mainComponent->GetNumberOfIds(); p++)
      {
      double point[3];
      points->GetPoint(mainComponent->GetId(p), point);
      mainComponentPoints->InsertNextPoint(point);
      }

    vtkSmartPointer<vtkPolyData> mainComponentPolyData =
      vtkSmartPointer<vtkPolyData>::New();
    mainComponentPolyData->SetPoints(mainComponentPoints);

    // Build a KDTree on the main component points in a KDTree
    vtkSmartPointer<vtkKdTreePointLocator> mainComponentKDTree =
      vtkSmartPointer<vtkKdTreePointLocator>::New();
    mainComponentKDTree->SetDataSet(mainComponentPolyData);
    mainComponentKDTree->BuildLocator();

    // Find the closest pair of points - one from the main component
    // and the other from the disconnected component
    double minimumDistance = std::numeric_limits<double>::infinity();
    unsigned int mainComponentPairID = 0;
    unsigned int disconnectedComponentPairID = 0;
    for(vtkIdType p = 0; p < disconnectedComponent->GetNumberOfIds(); p++)
      {
      unsigned int disconnectedComponentPointID = disconnectedComponent->GetId(p);

      double queryPoint[3];
      points->GetPoint(disconnectedComponentPointID, queryPoint);

      unsigned int closestPointID = mainComponentKDTree->FindClosestPoint(disconnectedComponentPointID);
      double closestPoint[3];
      points->GetPoint(closestPointID, closestPoint);

      double distance = vtkMath::Distance2BetweenPoints(queryPoint, closestPoint);
      if(distance < minimumDistance)
        {
        minimumDistance = distance;
        mainComponentPairID = mainComponent->GetId(closestPointID);
        disconnectedComponentPairID = disconnectedComponent->GetId(p);
        }
      }

    // Add an edge between the closest pair
    disconnectedGraph->AddEdge(mainComponentPairID, disconnectedComponentPairID);

    // Combine the components
    // There should be a function more like this:
    // mainGroup->Append(nextGroup);
    for(vtkIdType index = 0; index < disconnectedComponent->GetNumberOfIds(); index++)
      {
      mainComponent->InsertNextId(disconnectedComponent->GetId(index));
      }
  }

  output->ShallowCopy(disconnectedGraph);

  return 1;
}
