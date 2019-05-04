#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include <vtkLookupTable.h>
#include "vtkSmartPointer.h"

int main()
{
	int i;
	//定义立方体的顶点坐标
	static float x[8][3] = { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 },
	{ 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 1 }, { 0, 1, 1 } };
	//定义单元，每4个顶点建立一个四边形单元，共计6个单元
	static vtkIdType pts[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 }, { 0, 1, 5, 4 },
	{ 1, 2, 6, 5 }, { 2, 3, 7, 6 }, { 3, 0, 4, 7 } };
	//创建对象
	vtkSmartPointer<vtkPolyData> cube = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
	//存储顶点
	for (i = 0; i < 8; i++)
		points->InsertPoint(i, x[i]);
	//设定单元
	for (i = 0; i < 6; i++)
		polys->InsertNextCell(4, pts[i]);

	//存储标量值
	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
	//设定每个顶点的标量值
	for (i = 0; i < 8; i++)
		scalars->InsertTuple1(i,  i*4);
	//创建多边形数据
	cube->SetPoints(points);
	//设定单元类型为多边形
	cube->SetPolys(polys);
	//设定每个顶点的标量值
	cube->GetPointData()->SetScalars(scalars);
	//定义颜色映射表
	vtkSmartPointer<vtkLookupTable> pColorTable = vtkSmartPointer<vtkLookupTable>::New();
	//设置颜色表中的颜色
	pColorTable->SetNumberOfColors(256);
	pColorTable->SetHueRange(0.67, 0.0);        //色调范围从红色到蓝色
	pColorTable->Build();
	//数据映射
	vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cubeMapper->SetInputData(cube);
	cubeMapper->SetScalarRange(0, 7);
	cubeMapper->SetLookupTable(pColorTable);
	vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
	cubeActor->SetMapper(cubeMapper);
	
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);
	renderer->AddActor(cubeActor);
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(500, 500);
	renWin->Render();
	iren->Start();
	return 0;
}