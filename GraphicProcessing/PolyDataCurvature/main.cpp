/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkCurvatures.h>
#include <vtkPolyDataReader.h>
#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include <vtkColorSeries.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkScalarBarActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

//test data : ../data/fran_cut.vtk
int main(int argc, char *argv[])
{
	vtkNew<vtkPolyDataReader> reader;
	reader->SetFileName("E:/github/Study-VTK/data/fran_cut.vtk");
	reader->Update();

	vtkNew<vtkCurvatures> curvaturesfilter;
	curvaturesfilter->SetInputConnection(reader->GetOutputPort());
	//curvaturesFilter->SetCurvatureTypeToMinimum();
	curvaturesfilter->SetCurvatureTypeToMaximum();
	//curvaturesFilter->SetCurvatureTypeToGaussian();
	//curvaturesFilter->SetCurvatureTypeToMean();
	curvaturesfilter->Update();

	double scalarrange[2];
	curvaturesfilter->GetOutput()->GetScalarRange(scalarrange);
	vtkNew<vtkLookupTable> lut;
	lut->SetHueRange(0.0,0.6);
	lut->SetAlphaRange(1.0,1.0);
	lut->SetValueRange(1.0,1.0);
	lut->SetSaturationRange(1.0,1.0);
	lut->SetNumberOfTableValues(256);
	lut->SetRange(scalarrange);
	lut->Build();

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(curvaturesfilter->GetOutputPort());
	mapper->SetLookupTable(lut);
	mapper->SetScalarRange(scalarrange);
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	vtkNew<vtkScalarBarActor> scalarbar;
	scalarbar->SetLookupTable(mapper->GetLookupTable());
	scalarbar->SetTitle(curvaturesfilter->GetOutput()->GetPointData()->GetScalars()->GetName());
	scalarbar->SetNumberOfLabels(5);
	vtkNew<vtkRenderer> renderer;
	renderer->AddActor(actor);
	renderer->AddActor2D(scalarbar);
	renderer->SetBackground(1.0, 1.0, 1.0);
	vtkNew<vtkRenderWindow> renderwindow;
	renderwindow->AddRenderer(renderer);
	renderwindow->SetSize(640, 480);
	renderwindow->Render();
	renderwindow->SetWindowName("PolyDataCurvature");
	vtkNew<vtkRenderWindowInteractor> renderwindowinteractor;
	renderwindowinteractor->SetRenderWindow(renderwindow);
	renderwindow->Render();
	renderwindowinteractor->Start();

	return EXIT_SUCCESS;
}