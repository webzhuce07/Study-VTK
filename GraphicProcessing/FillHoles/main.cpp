/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkFeatureEdges.h>
#include <vtkFillHolesFilter.h>
#include <vtkPLYReader.h>

int main(int argc, char *argv[])
{
	vtkNew<vtkPLYReader> reader;
	reader->SetFileName("E:/github/Study-VTK/data/bunny.ply");
	reader->Update();
	vtkNew<vtkFeatureEdges> featureedges;
	featureedges->SetInputData(reader->GetOutput());
	featureedges->BoundaryEdgesOn();
	featureedges->FeatureEdgesOff();
	featureedges->ManifoldEdgesOff();
	featureedges->NonManifoldEdgesOff();
	featureedges->Update();
	int numberofopenedges = featureedges->GetOutput()->GetNumberOfCells();
	if(numberofopenedges) 
		std::cout<<"该网格模型不是封闭的..."<<std::endl;
	else {
		std::cout<<"该网格模型是封闭的..."<<std::endl;
		return EXIT_SUCCESS;
	}

	vtkNew<vtkFillHolesFilter> fillholesfilter;
	fillholesfilter->SetInputData(reader->GetOutput());
	fillholesfilter->Update();
	//////////////////////////////////////////////////////////////////////////
	double leftviewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightviewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkNew<vtkPolyDataMapper> originalmapper;
	originalmapper->SetInputData(reader->GetOutput());
	vtkNew<vtkActor> originalactor;
	originalactor->SetMapper(originalmapper);

	vtkNew<vtkPolyDataMapper> filledmapper;
	filledmapper->SetInputData(fillholesfilter->GetOutput());
	vtkNew<vtkActor> filledactor;
	filledactor->SetMapper(filledmapper);
	filledactor->GetProperty()->SetRepresentationToWireframe();
	filledactor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	vtkNew<vtkRenderer> leftrenderer;
	leftrenderer->SetViewport(leftviewport);
	leftrenderer->AddActor(originalactor);
	leftrenderer->SetBackground(0.0, 0.0, 0.0);

	vtkNew<vtkRenderer> rightrenderer;
	rightrenderer->SetViewport(rightviewport);
	rightrenderer->AddActor(filledactor);
	rightrenderer->SetBackground(0.0, 0.0, 0.0);

	vtkNew<vtkRenderWindow> renderwindow;
	renderwindow->AddRenderer(leftrenderer);
	renderwindow->AddRenderer(rightrenderer);
	renderwindow->SetSize(640, 320);
	renderwindow->Render();
	renderwindow->SetWindowName("FillHoles");
	vtkNew<vtkRenderWindowInteractor> renderwindowinteractor;
	renderwindowinteractor->SetRenderWindow(renderwindow);
	renderwindow->Render();
	renderwindowinteractor->Start();

	return EXIT_SUCCESS;
}

