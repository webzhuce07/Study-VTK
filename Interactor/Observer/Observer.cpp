/**********************************************************************

  Author: 阿Bin先生
  Purpose: test Observer/Command
 
**********************************************************************/

#include <vtkSmartPointer.h>
#include <vtkPNGReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

long pressCounts = 0;

//第一步，定义回调函数。注意回调函数的签名，不能更改。
void MyCallbackFunc(vtkObject*, unsigned long eid, void* clientdata, void *calldata)
{
	std::cout<<"You have clicked: "<<++pressCounts<<" times."<<std::endl;
}

//测试图像：../data/VTK-logo.png
int main(int argc, char* argv[])
{
	vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
	reader->SetFileName("VTK-logo.png");

	vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
	viewer->SetInput(reader->GetOutput());

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
	viewer->SetupInteractor(interactor);
	viewer->Render();

	viewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	viewer->SetSize(640, 480);
	viewer->GetRenderWindow()->SetWindowName("ObserverCommandDemo1");

	//第二步，设置回调函数。
	vtkSmartPointer<vtkCallbackCommand> mouseCallback = 
		vtkSmartPointer<vtkCallbackCommand>::New();
	mouseCallback->SetCallback ( MyCallbackFunc );

	//第三步，将vtkCallbackCommand对象添加到观察者列表。
	interactor->SetRenderWindow(viewer->GetRenderWindow());
	interactor->AddObserver(vtkCommand::LeftButtonPressEvent, mouseCallback);

	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}
