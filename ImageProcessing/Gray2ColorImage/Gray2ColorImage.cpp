/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageActor.h>
#include <vtkJPEGReader.h>
#include <vtkImageLuminance.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>

//≤‚ ‘ÕºœÒ£∫../data/lena-gray.jpg
int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName ( "E:\\TestData\\lena-gray.jpg" );

	vtkSmartPointer<vtkLookupTable> colorTable = 
		vtkSmartPointer<vtkLookupTable>::New(); 
	colorTable->SetRange( 0.0, 255.0 );
	colorTable->SetHueRange( 0.1, 0.5 );
	colorTable->SetValueRange( 0.6, 1.0 );
	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap = 
		vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetInputConnection( reader->GetOutputPort() );
	colorMap->SetLookupTable( colorTable );
	colorMap->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> shiftscaleActor =
		vtkSmartPointer<vtkImageActor>::New();
	shiftscaleActor->SetInputData(colorMap->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double shiftscaleViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> shiftscaleRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	shiftscaleRenderer->SetViewport(shiftscaleViewport);
	shiftscaleRenderer->AddActor(shiftscaleActor);
	shiftscaleRenderer->ResetCamera();
	shiftscaleRenderer->SetBackground(1.0, 1.0, 0.8);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shiftscaleRenderer);
	renderWindow->SetSize(900, 300);
	renderWindow->Render();
	renderWindow->SetWindowName("Gray2ColorImageExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
