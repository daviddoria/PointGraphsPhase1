/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPointGraphFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include <vtkAdjacentVertexIterator.h>
#include <vtkCellArray.h>
#include <vtkDataSet.h>
#include <vtkDoubleArray.h>
#include <vtkEdgeListIterator.h>
#include <vtkExecutive.h>
#include <vtkGraph.h>
#include <vtkIdList.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkMutableUndirectedGraph.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkTree.h>
#include <vtkSmartPointer.h>

#include "vtkPointGraphFilter.h"

vtkPointGraphFilter::vtkPointGraphFilter()
{
  // Create a graph
  this->G = vtkMutableUndirectedGraph::New();
}

vtkPointGraphFilter::~vtkPointGraphFilter()
{
  if(this->G)
    {
    this->G->Delete();
    }
}

int vtkPointGraphFilter::RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **vtkNotUsed(inputVector),
                                         vtkInformationVector *vtkNotUsed(outputVector))
{
  vtkUndirectedGraph* output = 0;
  output = vtkUndirectedGraph::New();

  this->GetExecutive()->SetOutputData(0, output);
  output->Delete();

  return 1;
}

int vtkPointGraphFilter::RequestData(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
   // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the input and set the member Input so it can be accessed by subclasses
  this->Input = vtkPointSet::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Get the output
  vtkGraph* output = vtkGraph::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Add a vertex for every point
  for(vtkIdType i = 0; i < this->Input->GetNumberOfPoints(); i++)
    {
    this->G->AddVertex();
    }

  // Allow the subclasses to determine the connectivity of the graph.
  CreateEdgesOnPoints();

  // Add the point coordinates to the graph
  this->G->SetPoints(this->Input->GetPoints());
  this->G->GetVertexData()->PassData(this->Input->GetPointData());

  // Set the output of the filter
  output->ShallowCopy(this->G);

  return 1;
}

int vtkPointGraphFilter::FillInputPortInformation( int port, vtkInformation* info )
{
  if (port == 0)
    {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPointSet");
    return 1;
    }

  return 0;
}
