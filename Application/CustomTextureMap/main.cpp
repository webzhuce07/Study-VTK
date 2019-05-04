/**********************************************************************

 Copyright (c) Mr.Bin. All rights reserved.
 For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkLookupTable.h>
#include <vtkContourFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkSliderWidget.h>
#include "vtkPNGReader.h"
#include "vtkTextureMapToIrregularity.h"
#include "vtkXMLPolyDataReader.h"

int main()
{
	//read input data
	vtkSmartPointer<vtkXMLPolyDataReader>  modelReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	modelReader->SetFileName("E:/TestData/cow.vtp");
	modelReader->Update();

	vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
	reader->SetFileName("E:/TestData/cow.png");

	vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(reader->GetOutputPort());
	texture->InterpolateOn();

	vtkSmartPointer<vtkTextureMapToIrregularity> textureMap = vtkSmartPointer<vtkTextureMapToIrregularity>::New();
	textureMap->SetInputData(modelReader->GetOutput());
	double vector[3] = { 1.0, 0.0, 0.0 };
	textureMap->SetVector(vector);

	vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cubeMapper->SetInputConnection(textureMap->GetOutputPort());
	vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
	cubeActor->SetMapper(cubeMapper);
	cubeActor->SetTexture(texture);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(cubeActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(500, 500);
	renWin->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);

	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}