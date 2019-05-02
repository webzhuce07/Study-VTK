/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkFixedPointVolumeRayCastMapper.h>

int main(int argc, char *argv[])
{
	vtkNew<vtkStructuredPointsReader> reader;
	reader->SetFileName("E:\\TestData\\mummy.128.vtk");
	reader->Update();
	vtkNew<vtkFixedPointVolumeRayCastMapper> volumeMapper0;
	volumeMapper0->SetInputData(reader->GetOutput());
	volumeMapper0->SetBlendModeToComposite();

	vtkNew<vtkFixedPointVolumeRayCastMapper> volumeMapper1;
	volumeMapper1->SetInputData(reader->GetOutput());
	volumeMapper1->SetBlendModeToComposite();

	vtkNew<vtkVolumeProperty> volumeProperty0;
	volumeProperty0->SetInterpolationTypeToLinear();
	volumeProperty0->SetAmbient(0.4);
	volumeProperty0->SetDiffuse(0.6);
	volumeProperty0->SetSpecular(0.2);

	vtkNew<vtkVolumeProperty> volumeProperty1;
	volumeProperty1->SetInterpolationTypeToLinear();
	volumeProperty1->SetAmbient(0.4);
	volumeProperty1->SetDiffuse(0.6);
	volumeProperty1->SetSpecular(0.2);

	vtkNew<vtkPiecewiseFunction> compositeOpacity;
	compositeOpacity->AddPoint(70,   0.00);
	compositeOpacity->AddPoint(90,   0.40);
	compositeOpacity->AddPoint(180,  0.60);
	volumeProperty0->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数
	volumeProperty1->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数

	vtkNew<vtkPiecewiseFunction> volumeGradientOpacity;
	volumeGradientOpacity->AddPoint(10,  0.0);
	volumeGradientOpacity->AddPoint(90,  0.5);
	volumeGradientOpacity->AddPoint(100, 1.0);
	//volumeProperty0->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比
	volumeProperty1->SetGradientOpacity(volumeGradientOpacity);//设置梯度不透明度效果对比

	vtkNew<vtkColorTransferFunction> color;
	color->AddRGBPoint(0.000,  0.00, 0.00, 0.00);
	color->AddRGBPoint(64.00,  1.00, 0.52, 0.30);
	color->AddRGBPoint(190.0,  1.00, 1.00, 1.00);
	color->AddRGBPoint(220.0,  0.20, 0.20, 0.20);
	volumeProperty0->SetColor(color);
	volumeProperty1->SetColor(color);

	vtkNew<vtkVolume> volume0;
	volume0->SetMapper(volumeMapper0);
	volume0->SetProperty(volumeProperty0);

	vtkNew<vtkVolume> volume1;
	volume1->SetMapper(volumeMapper1);
	volume1->SetProperty(volumeProperty1);

	double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };
	vtkNew<vtkRenderer> renderer0;
	renderer0->SetBackground(1.0, 1.0, 1.0);
	renderer0->SetViewport(leftViewport);
	renderer0->AddVolume(volume0);

	vtkNew<vtkRenderer> renderer1;
	renderer1->SetBackground(1.0, 1.0, 1.0);
	renderer1->SetViewport(rightViewport);
	renderer1->AddVolume(volume1);

	vtkNew<vtkRenderWindow> renWin;
	renWin->AddRenderer(renderer0);
	renWin->AddRenderer(renderer1);
	renWin->SetSize(640, 480);
	renWin->SetWindowName("VolumeMapper");

	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renWin);
	renWin->Render();
	iren->Start();

	return EXIT_SUCCESS;
}