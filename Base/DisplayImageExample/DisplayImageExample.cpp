/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkMetaImageReader.h>

//≤‚ ‘ÕºœÒ£∫../data/brain.mhd
int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("E:/TestData/brain.mhd");
	reader->Update();

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(renderWindowInteractor);

	imageViewer->SetColorLevel(500);
	imageViewer->SetColorWindow(2000);
	imageViewer->SetSlice(10);
	imageViewer->SetSliceOrientationToXY();
	imageViewer->Render();

	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(640, 480);
	imageViewer->GetRenderWindow()->SetWindowName("DisplayImageExample");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
