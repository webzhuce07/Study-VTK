/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkLight.h"

int main()
{
	//创建多边形球体
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetThetaResolution(100);
	sphere->SetPhiResolution(50);

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	//actor是一个分组机制，除了将处理映射器的信息外，还具有属性及转换矩阵、光谱系数等参数
	//下面八个球体因为环境光的设置不同，其可见度将呈现变化
	vtkSmartPointer<vtkActor> sphereActor1 = vtkSmartPointer<vtkActor>::New();
	sphereActor1->SetMapper(sphereMapper);
	sphereActor1->GetProperty()->SetColor(1, 0, 0);
	sphereActor1->GetProperty()->SetAmbient(0.125);
	sphereActor1->GetProperty()->SetDiffuse(0.0);
	sphereActor1->GetProperty()->SetSpecular(0.0);

	vtkSmartPointer<vtkActor> sphereActor2 = vtkSmartPointer<vtkActor>::New();
	sphereActor2->SetMapper(sphereMapper);
	sphereActor2->GetProperty()->SetColor(1, 0, 0);
	sphereActor2->GetProperty()->SetAmbient(0.25);
	sphereActor2->GetProperty()->SetDiffuse(0.0);
	sphereActor2->GetProperty()->SetSpecular(0.0);
	sphereActor2->AddPosition(1.25, 0, 0);

	vtkSmartPointer<vtkActor> sphereActor3 = vtkSmartPointer<vtkActor>::New();
	sphereActor3->SetMapper(sphereMapper);
	sphereActor3->GetProperty()->SetColor(1, 0, 0);
	sphereActor3->GetProperty()->SetAmbient(0.375);
	sphereActor3->GetProperty()->SetDiffuse(0.0);
	sphereActor3->GetProperty()->SetSpecular(0.0);
	sphereActor3->AddPosition(2.5, 0, 0);

	vtkSmartPointer<vtkActor> sphereActor4 = vtkSmartPointer<vtkActor>::New();
	sphereActor4->SetMapper(sphereMapper);
	sphereActor4->GetProperty()->SetColor(1, 0, 0);
	sphereActor4->GetProperty()->SetAmbient(0.5);
	sphereActor4->GetProperty()->SetDiffuse(0.0);
	sphereActor4->GetProperty()->SetSpecular(0.0);
	sphereActor4->AddPosition(3.75, 0, 0);

	vtkSmartPointer<vtkActor> sphereActor5 = vtkSmartPointer<vtkActor>::New();
	sphereActor5->SetMapper(sphereMapper);
	sphereActor5->GetProperty()->SetColor(1, 0, 0);
	sphereActor5->GetProperty()->SetAmbient(0.625);
	sphereActor5->GetProperty()->SetDiffuse(0.0);
	sphereActor5->GetProperty()->SetSpecular(0.0);
	sphereActor5->AddPosition(0, 1.25, 0);

	vtkSmartPointer<vtkActor> sphereActor6 = vtkSmartPointer<vtkActor>::New();
	sphereActor6->SetMapper(sphereMapper);
	sphereActor6->GetProperty()->SetColor(1, 0, 0);
	sphereActor6->GetProperty()->SetAmbient(0.75);
	sphereActor6->GetProperty()->SetDiffuse(0.0);
	sphereActor6->GetProperty()->SetSpecular(0.0);
	sphereActor6->AddPosition(1.25, 1.25, 0);

	vtkSmartPointer<vtkActor> sphereActor7 = vtkSmartPointer<vtkActor>::New();
	sphereActor7->SetMapper(sphereMapper);
	sphereActor7->GetProperty()->SetColor(1, 0, 0);
	sphereActor7->GetProperty()->SetAmbient(0.825);
	sphereActor7->GetProperty()->SetDiffuse(0.0);
	sphereActor7->GetProperty()->SetSpecular(0.0);
	sphereActor7->AddPosition(2.5, 1.25, 0);

	vtkSmartPointer<vtkActor> sphereActor8 = vtkSmartPointer<vtkActor>::New();
	sphereActor8->SetMapper(sphereMapper);
	sphereActor8->GetProperty()->SetColor(1, 0, 0);
	sphereActor8->GetProperty()->SetAmbient(1.0);
	sphereActor8->GetProperty()->SetDiffuse(0.0);
	sphereActor8->GetProperty()->SetSpecular(0.0);
	sphereActor8->AddPosition(3.75, 1.25, 0);

	vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
	ren->AddActor(sphereActor1);
	ren->AddActor(sphereActor2);
	ren->AddActor(sphereActor3);
	ren->AddActor(sphereActor4);
	ren->AddActor(sphereActor5);
	ren->AddActor(sphereActor6);
	ren->AddActor(sphereActor7);
	ren->AddActor(sphereActor8);
	ren->SetBackground(0.1, 0.2, 0.4);
	//设置灯光及属性
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetFocalPoint(1.875, 0.6125, 0);
	light->SetPosition(0.875, 1.6125, 1);
	ren->AddLight(light);
	//如果想消除背景的阴暗，可以采用平行投射模式
	ren->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	ren->GetActiveCamera()->SetPosition(0, 0, 1);
	ren->GetActiveCamera()->SetViewUp(0, 1, 0);
	ren->GetActiveCamera()->ParallelProjectionOn();
	ren->ResetCamera();
	ren->GetActiveCamera()->SetParallelScale(1.5);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(ren);
	renWin->SetSize(400, 200);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	//启动事件进入循环并开始渲染
	iren->Initialize();
	iren->Start();
	return EXIT_SUCCESS;
}