/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>
#include <vtkImageIterator.h>

int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkBMPReader> reader =
		vtkSmartPointer<vtkBMPReader>::New();
	//²âÊÔÍ¼Ïñ£ºE:\\TestData\\masonry.bmp
	reader->SetFileName ( "E:\\TestData\\masonry.bmp" );
	reader->Update();

	int subRegion[6] = {0,100, 0, 100, 0, 0};
	vtkImageIterator<unsigned char> it(reader->GetOutput(), subRegion);

	while(!it.IsAtEnd())
	{
		unsigned char *inSI = it.BeginSpan();
		unsigned char *inSIEnd = it.EndSpan();

		while(inSI != inSIEnd)
		{
			*inSI = 255-*inSI;
			++inSI;
		}
		it.NextSpan();
	}

	vtkSmartPointer<vtkImageViewer2> imageViewer =
		vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection(reader->GetOutputPort());

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(renderWindowInteractor);
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();

	imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	imageViewer->SetSize(640, 480);
	imageViewer->GetRenderWindow()->SetWindowName("VisitImagePixelIterativelyExample");

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
