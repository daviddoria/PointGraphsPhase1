/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRBNNGraphFilter.cxx

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
#include <vtkMutableUndirectedGraph.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkTree.h>

#include "vtkRBNNGraphFilter.h"

vtkStandardNewMacro(vtkRBNNGraphFilter);

vtkRBNNGraphFilter::vtkRBNNGraphFilter()
{
  // Initialize to an invalid value. The user must specify a valid radius or a
  // reasonable guess will be made in the CreateEdgesOnPoints() function
  this->Radius = -1.0;

  // Initialize to zero, indicating that we want all of the neighbors within
  // Radius to be included.
  this->MaxNeighbors = 0;
}

void vtkRBNNGraphFilter::CreateEdgesOnPoints()
{
  // If the user has not set a radius, guess 1/20 of the data bounds diagonal
  if(this->Radius == -1.0)
    {
    double bounds[6];
    this->Input->GetBounds(bounds);
    this->Radius = sqrt(pow(bounds[1]-bounds[0],2) + pow(bounds[3]-bounds[2],2) + pow(bounds[5]-bounds[4],2));
    }

  // Create a KDTree of the points
  vtkSmartPointer<vtkKdTreePointLocator> kdTree =
    vtkSmartPointer<vtkKdTreePointLocator>::New();
  kdTree->SetDataSet(this->Input);
  kdTree->BuildLocator();

  // Find the neighbors to each point (inside Radius) and add an edge between them
  for(vtkIdType pointID = 0; pointID < this->Input->GetNumberOfPoints(); pointID++)
    {
    vtkSmartPointer<vtkIdList> neighbors =
      vtkSmartPointer<vtkIdList>::New();

    kdTree->FindPointsWithinRadius(this->Radius, pointID, neighbors, false);

    for(vtkIdType neighbor = 0; neighbor < neighbors->GetNumberOfIds(); neighbor++)
      {
      // Stop when we reach MaxNeighbors unless MaxNeighbors is set to 0, indicating
      // that we want all of the neighbors inside Radius
      if(neighbor > this->MaxNeighbors && this->MaxNeighbors != 0)
        {
        break;
        }
      vtkIdType neighborID = neighbors->GetId(neighbor);

      if(this->G->GetEdgeId(pointID, neighborID) == -1) // Edge does not already exist
        {
        this->G->AddEdge(pointID, neighborID);
        }
      }
    }
}

void vtkRBNNGraphFilter::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Radius: " << this->Radius << std::endl;
}
