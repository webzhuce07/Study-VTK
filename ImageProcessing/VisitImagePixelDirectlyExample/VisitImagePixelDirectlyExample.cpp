/**********************************************************************

文件名: VisitImagePixelDirectlyExample.cpp
Copyright (c) 阿Bin先生. All rights reserved.
更多信息请访问: http://blog.csdn.net/webzhuce

**********************************************************************/

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageData.h>

int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkBMPReader> reader =
		vtkSmartPointer<vtkBMPReader>::New();
	//测试图像：E:\TestData\masonry.bmp
	reader->SetFileName("E:\\TestData\\masonry.bmp");
	reader->Update();

	int dims[3];
	reader->GetOutput()->GetDimensions(dims);

	int nbOfComp;
	nbOfComp = reader->GetOutput()->GetNumberOfScalarComponents();

	for(int k=0; k<dims[2]; k++)
	{
		for(int j=0; j<dims[1]; j++)
		{
			for(int i=0; i<dims[0]; i++)
			{
				if(i<100 && j<100)
				{
					unsigned char * pixel = 
						(unsigned char *) ( reader->GetOutput()->GetScalarPointer(i, j, k) );
					*pixel = 0;
					*(pixel+1) = 0;
					*(pixel+2) = 0;
				}
			}
		}
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
	imageViewer->GetRenderWindow()->SetWindowName("VisitImagePixelDirectlyExample");


	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
