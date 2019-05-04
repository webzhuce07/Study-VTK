/**********************************************************************

  Author: 阿Bin先生
  Purpose: test roi
 
**********************************************************************/
#include <vtkQuadric.h> 
#include <vtkSmartPointer.h>
#include <vtkSampleFunction.h> 
#include <vtkSphere.h> 
#include <vtkTransform.h> 
#include <vtkImplicitBoolean.h>
#include <vtkExtractGeometry.h> 
#include <vtkDataSetMapper.h> 
#include <vtkActor.h>
#include <vtkDataSet.h>
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include <vtkRenderWindowInteractor.h> 
#include <vtkCamera.h>
#include <vtkShrinkFilter.h> 

int main()
{
 //创建二次曲面隐函数
 vtkSmartPointer<vtkQuadric>pQuadric=vtkQuadric::New();
 //设置二次曲面方程的系数
 pQuadric->SetCoefficients(0.5,1.0,0.2,0,0.1,0,0,0.2,0,0);
 //对二次曲面进行采样,生成结构化点集数据对象
 vtkSmartPointer<vtkSampleFunction>pSmapleFun=vtkSampleFunction::New();
 pSmapleFun->SetSampleDimensions(50,50,50);
 pSmapleFun->SetImplicitFunction(pQuadric);
 //定义缩放变换
 vtkSmartPointer<vtkTransform>pTrans1=vtkTransform::New();
 pTrans1->Scale(1,0.5,0.333);
    vtkSmartPointer<vtkTransform>pTrans2=vtkTransform::New();
 pTrans2->Scale(0.25,0.5,1.0);
    //定义两个椭圆体
 vtkSmartPointer<vtkSphere>pSphere1=vtkSphere::New();
 pSphere1->SetRadius(0.25);
 pSphere1->SetTransform(pTrans1);
    vtkSmartPointer<vtkSphere>pSphere2=vtkSphere::New();
    pSphere1->SetRadius(0.25);
 pSphere1->SetTransform(pTrans2);
    //合并椭球体
 vtkSmartPointer<vtkImplicitBoolean>pUnionSphere=vtkImplicitBoolean::New();
    //并运算
    pUnionSphere->SetOperationType(0);
    pUnionSphere->AddFunction(pSphere1);
    pUnionSphere->AddFunction(pSphere2);
    //提取数据集中位于隐函数内部的单元
 vtkSmartPointer<vtkExtractGeometry>pExtracFilter=vtkExtractGeometry::New();
 //设置被提取数据的数据集对象
 pExtracFilter->SetInput((vtkDataObject *)pSmapleFun->GetOutput());
 //设置隐函数
 pExtracFilter->SetImplicitFunction(pUnionSphere);
    //收缩单元
 vtkSmartPointer<vtkShrinkFilter>pShrinkFilter=vtkShrinkFilter::New();
 //设置收缩系数
 pShrinkFilter->SetShrinkFactor(0.5);
 pShrinkFilter->SetInput((vtkDataObject *)pExtracFilter->GetOutput());
    //定义数据集对象映射器
 vtkSmartPointer<vtkDataSetMapper>pDataSetMapper=vtkDataSetMapper::New();
 pDataSetMapper->SetInput((vtkDataSet *)pShrinkFilter->GetOutput());
 vtkSmartPointer<vtkActor>pActor=vtkActor::New();
 pActor->SetMapper(pDataSetMapper); 
    //绘制
    vtkSmartPointer<vtkRenderer>renderer=vtkRenderer::New();
 renderer->GetActiveCamera()->Zoom(1.5);
 vtkSmartPointer<vtkRenderWindow>renWin = vtkRenderWindow::New();
 renWin->AddRenderer(renderer);
 vtkSmartPointer<vtkRenderWindowInteractor>iren = vtkRenderWindowInteractor::New();
 iren->SetRenderWindow(renWin);
 renderer->AddActor(pActor);
 renderer->ResetCamera();
 renderer->SetBackground(1,1,1);
 renWin->SetSize(300,300);
 renWin->Render();
 iren->Start();
 return 0;
}
