/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkConnectGraph.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkConnectGraph - connects disconnected regions of a graph.
//
// .SECTION Description
// vtkConnectGraph connects disconnected regions of a graph.
//
// .SECTION See Also

#ifndef __vtkConnectGraph_h
#define __vtkConnectGraph_h

#include "vtkGraphAlgorithm.h"

class vtkGraph;
class vtkMutableUndirectedGraph;
class vtkIntArray;

#include <vector>

class vtkConnectGraph : public vtkGraphAlgorithm
{
public:
  static vtkConnectGraph *New();
  vtkTypeMacro(vtkConnectGraph, vtkGraphAlgorithm);

protected:
  vtkConnectGraph(){}
  ~vtkConnectGraph(){}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  int RequestDataObject(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkConnectGraph(const vtkConnectGraph&);  // Not implemented.
  void operator=(const vtkConnectGraph&);  // Not implemented.
};

#endif
