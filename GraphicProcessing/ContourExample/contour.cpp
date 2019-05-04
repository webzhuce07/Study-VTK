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

int main()
{
	int i;
	//定义网格的顶点坐标
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//第1行5个点
	points->InsertNextPoint(-2.0, 2.0, 0.0);
	points->InsertNextPoint(-1.0, 2.0, 0.0);
	points->InsertNextPoint(0.0, 2.0, 0.0);
	points->InsertNextPoint(1.0, 2.0, 0.0);
	points->InsertNextPoint(2.0, 2.0, 0.0);
	//第2行5个点
	points->InsertNextPoint(-2.0, 1.0, 0.0);
	points->InsertNextPoint(-1.0, 1.0, 0.0);
	points->InsertNextPoint(0.0, 1.0, 0.0);
	points->InsertNextPoint(1.0, 1.0, 0.0);
	points->InsertNextPoint(2.0, 1.0, 0.0);
	//第3行5个点
	points->InsertNextPoint(-2.0, 0.0, 0.0);
	points->InsertNextPoint(-1.0, 0.0, 0.0);
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 0.0, 0.0);
	points->InsertNextPoint(2.0, 0.0, 0.0);
	//第4行5个点
	points->InsertNextPoint(-2.0, -1.0, 0.0);
	points->InsertNextPoint(-1.0, -1.0, 0.0);
	points->InsertNextPoint(0.0, -1.0, 0.0);
	points->InsertNextPoint(1.0, -1.0, 0.0);
	points->InsertNextPoint(2.0, -1.0, 0.0);
	//第5行5个点
	points->InsertNextPoint(-2.0, -2.0, 0.0);
	points->InsertNextPoint(-1.0, -2.0, 0.0);
	points->InsertNextPoint(0.0, -2.0, 0.0);
	points->InsertNextPoint(1.0, -2.0, 0.0);
	points->InsertNextPoint(2.0, -2.0, 0.0);

	//定义单元，每个顶点建立一个四边形单元，共计个单元
	static vtkIdType pts[16][4] = { { 0, 5, 6, 1 }, { 1, 6, 7, 2 }, { 2, 7, 8, 3 },
	{ 3, 8, 9, 4 }, { 5, 10, 11, 6 }, { 6, 11, 12, 7 }, { 7, 12, 13, 8 }, { 8, 13, 14, 9 },
	{ 10, 15, 16, 11 }, { 11, 16, 17, 12 }, { 12, 17, 18, 13 }, { 13, 18, 19, 14 },
	{ 15, 20, 21, 16 }, { 16, 21, 22, 17 }, { 17, 22, 23, 18 }, { 18, 23, 24, 19 } };

	//创建对象
	vtkSmartPointer<vtkPolyData> pGrid = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
	for (i = 0; i < 16; i++)
	{
		polys->InsertNextCell(4, pts[i]);
	}
	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
	//第1行
	scalars->InsertTuple1(0, 0);
	scalars->InsertTuple1(1, 1);
	scalars->InsertTuple1(2, 1);
	scalars->InsertTuple1(3, 3);
	scalars->InsertTuple1(4, 2);
	//第2行
	scalars->InsertTuple1(5, 1);
	scalars->InsertTuple1(6, 3);
	scalars->InsertTuple1(7, 6);
	scalars->InsertTuple1(8, 6);
	scalars->InsertTuple1(9, 3);
	//第3行
	scalars->InsertTuple1(10, 3);
	scalars->InsertTuple1(11, 7);
	scalars->InsertTuple1(12, 9);
	scalars->InsertTuple1(13, 7);
	scalars->InsertTuple1(14, 3);
	//第4行
	scalars->InsertTuple1(15, 2);
	scalars->InsertTuple1(16, 7);
	scalars->InsertTuple1(17, 8);
	scalars->InsertTuple1(18, 6);
	scalars->InsertTuple1(19, 2);
	//第5行
	scalars->InsertTuple1(20, 1);
	scalars->InsertTuple1(21, 2);
	scalars->InsertTuple1(22, 3);
	scalars->InsertTuple1(23, 4);
	scalars->InsertTuple1(24, 3);

	pGrid->SetPoints(points);
	pGrid->SetPolys(polys);
	pGrid->GetPointData()->SetScalars(scalars);

	//定义颜色映射表
	vtkSmartPointer<vtkLookupTable> pColorTable = vtkSmartPointer<vtkLookupTable>::New();
	pColorTable->SetHueRange(0.667, 0.887);
	pColorTable->SetNumberOfColors(255);
	pColorTable->Build();

	vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cubeMapper->SetInputData(pGrid);
	cubeMapper->SetScalarRange(0, 9);
	cubeMapper->SetLookupTable(pColorTable);
	vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
	cubeActor->SetMapper(cubeMapper);

	//提取轮廓线
	vtkSmartPointer<vtkContourFilter> pContourFilter = vtkSmartPointer<vtkContourFilter>::New();
	pContourFilter->SetValue(0, 5.0);
	pContourFilter->SetInputData(pGrid);
	pContourFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> pContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	pContourMapper->SetInputData(pContourFilter->GetOutput());

	pContourMapper->ScalarVisibilityOff();
	vtkSmartPointer<vtkActor> pContourActor = vtkSmartPointer<vtkActor>::New();
	pContourActor->SetMapper(pContourMapper);
	pContourActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
	pContourActor->GetProperty()->SetLineWidth(5.0);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(pContourActor);
	renderer->AddActor(cubeActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->SetSize(300, 300);
	renWin->AddRenderer(renderer);
	renWin->SetWindowName("vtkContourFilter");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renWin);

	interactor->Initialize();
	interactor->Start();
}