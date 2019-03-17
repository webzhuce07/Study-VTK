/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "vtkActor.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkClipPolyData.h"
#include "vtkPlane.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
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

	vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
	clipper->SetInputConnection(cow_normals->GetOutputPort());
	clipper->SetClipFunction(plane);
	clipper->GenerateClipScalarsOn();
	clipper->GenerateClippedOutputOn();
	clipper->SetValue(0.5);

	vtkSmartPointer<vtkPolyDataMapper> clip_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	clip_mapper->SetInputConnection(clipper->GetOutputPort());
	clip_mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkProperty> backProp = vtkSmartPointer<vtkProperty>::New();
	backProp->SetDiffuseColor(1.00, 0.388, 0.278);

	vtkSmartPointer<vtkActor> clip_actor = vtkSmartPointer<vtkActor>::New();
	clip_actor->SetMapper(clip_mapper);
	clip_actor->GetProperty()->SetColor(.200, .631, .788);
	clip_actor->SetBackfaceProperty(backProp);


	vtkSmartPointer<vtkPolyDataMapper> rest_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	rest_mapper->SetInputConnection(clipper->GetClippedOutputPort());
	rest_mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> rest_actor = vtkSmartPointer<vtkActor>::New();
	rest_actor->SetMapper(rest_mapper);
	rest_actor->GetProperty()->SetRepresentationToWireframe();


	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(clip_actor);
	renderer->AddActor(rest_actor);
	renderer->SetBackground(1.0, 1.0, 1.0);


	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(640, 480);
	renWin->Render();
	renWin->SetWindowName("ClipPolyData");

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start();

	return EXIT_SUCCESS;
}