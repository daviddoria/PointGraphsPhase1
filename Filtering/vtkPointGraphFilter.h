/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPointGraphFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPointGraphFilter - Encloses a rectangular region of voxel like cells.
//
// .SECTION Description
// vtkPointGraphFilter is an abstract class which takes a vtkPointSet as input
// and creates an undirected graph on the points. This class creates a vertex
// in the graph for every input point, and leaves it to the subclasses to implement
// the CreateEdgesOnPoints() function to determine the connectivity.
//
// .SECTION See Also

#ifndef __vtkPointGraphFilter_h
#define __vtkPointGraphFilter_h

#include "vtkGraphAlgorithm.h"

class vtkMutableUndirectedGraph;
class vtkPointSet;

class vtkPointGraphFilter : public vtkGraphAlgorithm
{
public:
  vtkTypeMacro(vtkPointGraphFilter, vtkGraphAlgorithm);

  vtkPointGraphFilter();
  ~vtkPointGraphFilter();

protected:

  // Description:
  // This function does all of the work. It determines which edges to
  // add to the graph.
  virtual void CreateEdgesOnPoints() = 0;

  int FillInputPortInformation( int port, vtkInformation* info );
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  int RequestDataObject(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  // Description:
  // The graph is created in the constructor and stored here. This is done so that
  // it can be accessed in the RequestData function as well as in subclasses.
  vtkMutableUndirectedGraph* G;

  // Description:
  // This variable is set at the beginning of RequestData to the filters input.
  // This is done so that it can be accessed by the subclasses implementation of
  // CreateEdgesOnPoints().
  vtkPointSet* Input;

private:
  vtkPointGraphFilter(const vtkPointGraphFilter&);  // Not implemented.
  void operator=(const vtkPointGraphFilter&);  // Not implemented.
};

#endif
