/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "vtkActor.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkCutter.h"
#include "vtkPlane.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkStripper.h"
#include "vtkTriangleFilter.h"
#include "vtkSmartPointer.h"
#include "vtkInteractorStyleTrackballCamera.h"

int main()
{
	//read input data
	vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	reader->SetFileName("E:\\TestData\\cow.vtp");

	vtkSmartPointer<vtkPolyDataNormals> cow_normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	cow_normals->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin(0.25, 0.0, 0.0);
	plane->SetNormal(-1, -1, 0);

	vtkSmartPointer<vtkCutter> cutEdges = vtkSmartPointer<vtkCutter>::New();
	cutEdges->SetInputConnection(cow_normals->GetOutputPort());
	cutEdges->SetCutFunction(plane);
	cutEdges->GenerateCutScalarsOn();
	cutEdges->SetValue(0, 0.5);

	vtkSmartPointer<vtkStripper> cutStrips = vtkSmartPointer<vtkStripper>::New();
	cutStrips->SetInputConnection(cutEdges->GetOutputPort());
	cutStrips->Update();

	vtkSmartPointer<vtkPolyData> cutPolyData = vtkSmartPointer<vtkPolyData>::New();
	cutPolyData->SetPoints(cutStrips->GetOutput()->GetPoints());
	cutPolyData->SetPolys(cutStrips->GetOutput()->GetLines());

	vtkSmartPointer<vtkTriangleFilter> cutTriangles = vtkSmartPointer<vtkTriangleFilter>::New();
	cutTriangles->SetInputData(cutPolyData);

	vtkSmartPointer<vtkPolyDataMapper> cut_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cut_mapper->SetInputConnection(cutEdges->GetOutputPort());
	cut_mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> cut_actor = vtkSmartPointer<vtkActor>::New();
	cut_actor->SetMapper(cut_mapper);
	cut_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkPolyDataMapper> cow_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cow_mapper->SetInputConnection(cow_normals->GetOutputPort());
	cow_mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> cow_actor = vtkSmartPointer<vtkActor>::New();
	cow_actor->SetMapper(cow_mapper);
	cow_actor->GetProperty()->SetRepresentationToWireframe();

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(cut_actor);
	renderer->AddActor(cow_actor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(640, 480);
	renWin->Render();
	renWin->SetWindowName("CutPolyData");

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start();

	return EXIT_SUCCESS;
}