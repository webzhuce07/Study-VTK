/*=========================================================================
Program:   Visualization Toolkit
Module:    TestCaptionWidget.cxx
Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
//
// This example tests the vtkCaptionWidget.

// First include the required header files for the VTK classes we are using.
#include "vtkSmartPointer.h"
#include "vtkCaptionWidget.h"
#include "vtkCaptionRepresentation.h"
#include "vtkCaptionActor2D.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkTextActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCommand.h"
#include "vtkInteractorEventRecorder.h"
#include "vtkTextProperty.h"
#include "vtkBezierSurfaceSource.h"
#include "vtkPolyDataNormals.h"
#include "vtkDataSetMapper.h"
#include "vtkBezierSurfaceWidget.h"

int main(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
	vtkNew<vtkBezierSurfaceSource> besizersurfacesource;
	besizersurfacesource->ResetControlPoints();
	int m = 4, n = 4;
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			double pt[3] = {-4.0 + i * 2.0 , 4.0 + j * (-2.0) , 0};
			besizersurfacesource->SetControlPoint(i, j, pt);
		}
	}

	vtkNew<vtkPolyDataNormals> polydatanormals;
	polydatanormals->SetInputConnection(besizersurfacesource->GetOutputPort());
	vtkNew<vtkDataSetMapper> mapper;
	mapper->SetInputConnection(polydatanormals->GetOutputPort());
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	vtkNew<vtkRenderer> renderer;
	vtkNew<vtkRenderWindow> renderwindow;
	renderwindow->AddRenderer(renderer);
	renderer->AddActor(actor);
	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renderwindow);

	vtkNew<vtkBezierSurfaceWidget> widget;
	widget->SetSource(besizersurfacesource);
	widget->SetInteractor(renderwindow->GetInteractor());

	iren->Initialize();
	renderwindow->Render();
	iren->Start();

	return EXIT_SUCCESS;
}