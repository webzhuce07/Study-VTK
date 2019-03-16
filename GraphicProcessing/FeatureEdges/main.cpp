/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkInformation.h>
#include <vtkPolyData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkIdTypeArray.h>
#include <vtkExtractSelection.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataMapper.h>

void GenerateData(vtkSmartPointer<vtkPolyData> input)
{
	vtkNew<vtkSphereSource> spheresource;
	spheresource->Update();

	vtkNew<vtkIdTypeArray> ids;
	ids->SetNumberOfComponents(1);
	ids->InsertNextValue(2);
	ids->InsertNextValue(10);

	vtkNew<vtkSelectionNode> selectionnode;
	selectionnode->SetFieldType(vtkSelectionNode::CELL);
	selectionnode->SetContentType(vtkSelectionNode::INDICES);
	selectionnode->SetSelectionList(ids);
	selectionnode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1);

	vtkNew<vtkSelection> selection;
	selection->AddNode(selectionnode);

	vtkNew<vtkExtractSelection> extractselection;
	extractselection->SetInputData(0, spheresource->GetOutput());
	extractselection->SetInputData(1, selection);
	extractselection->Update();

	vtkNew<vtkDataSetSurfaceFilter> surfacefilter;
	surfacefilter->SetInputConnection(extractselection->GetOutputPort());
	surfacefilter->Update();

	input->ShallowCopy(surfacefilter->GetOutput());
}

int main(int argc, char *argv[])
{
	vtkNew<vtkPolyData> input;
	GenerateData(input);

	vtkNew<vtkFeatureEdges> featureedges;
	featureedges->SetInputData(input);
	featureedges->BoundaryEdgesOn();
	featureedges->FeatureEdgesOff();
	featureedges->ManifoldEdgesOff();
	featureedges->NonManifoldEdgesOff();
	featureedges->Update();


	int numberofopenedges = featureedges->GetOutput()->GetNumberOfCells();
	if (numberofopenedges)
		std::cout << "该网格模型不是封闭的..." << std::endl;
	else {
		std::cout << "该网格模型是封闭的..." << std::endl;
		return EXIT_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

	vtkNew<vtkPolyDataMapper> originalmapper;
	originalmapper->SetInputData(input);

	vtkNew<vtkProperty> backfaceprop;
	backfaceprop->SetDiffuseColor(0.89, 0.81, 0.34);

	vtkNew<vtkActor> originalactor;
	originalactor->SetMapper(originalmapper);
	originalactor->SetBackfaceProperty(backfaceprop);
	originalactor->GetProperty()->SetDiffuseColor(1.0, 0.3882, 0.2784);

	vtkNew<vtkPolyDataMapper> edgemapper;
	edgemapper->SetInputData(featureedges->GetOutput());
	vtkNew<vtkActor> edgeactor;
	edgeactor->SetMapper(edgemapper);
	edgeactor->GetProperty()->SetEdgeColor(0., 0., 1.0);
	edgeactor->GetProperty()->SetEdgeVisibility(1);
	edgeactor->GetProperty()->SetLineWidth(5);

	vtkNew<vtkRenderer> leftrenderer;
	leftrenderer->SetViewport(leftViewport);
	leftrenderer->AddActor(originalactor);
	leftrenderer->SetBackground(1.0, 1.0, 1.0);

	vtkNew<vtkRenderer> rightrenderer;
	rightrenderer->SetViewport(rightViewport);
	rightrenderer->AddActor(edgeactor);
	rightrenderer->SetBackground(1.0, 1.0, 1.0);

	vtkNew<vtkRenderWindow> renderwindow;
	renderwindow->AddRenderer(leftrenderer);
	renderwindow->AddRenderer(rightrenderer);
	renderwindow->SetSize(640, 320);
	renderwindow->Render();
	renderwindow->SetWindowName("FeatureEdges");

	vtkNew<vtkRenderWindowInteractor> renderwindowinteractor;
	renderwindowinteractor->SetRenderWindow(renderwindow);

	leftrenderer->GetActiveCamera()->SetPosition(0, -1, 0);
	leftrenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	leftrenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
	leftrenderer->GetActiveCamera()->Azimuth(30);
	leftrenderer->GetActiveCamera()->Elevation(30);
	leftrenderer->ResetCamera();
	rightrenderer->SetActiveCamera(leftrenderer->GetActiveCamera());

	renderwindow->Render();
	renderwindowinteractor->Start();

	return EXIT_SUCCESS;
}