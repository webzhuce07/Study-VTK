/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit: http://blog.csdn.net/webzhuce

**********************************************************************/
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageReader2.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataNormals.h>
#include <vtkStripper.h>
#include <vtkCellLocator.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkTransform.h>
#include <vtkPlane.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkDataSetMapper.h>
#include <vtkVolumePicker.h>
#include <vtkCommand.h>

class vtkMyMouseCommand : public vtkCommand
{
public:
	static vtkMyMouseCommand *New()
	{
		return new vtkMyMouseCommand;
	}

	//A function to point an actor along a vector
	void PointCone(vtkActor *actor, double nx, double ny, double nz)
	{
		actor->SetOrientation(0.0, 0.0, 0.0);
		double n = std::sqrt(nx*nx + ny*ny + nz*nz);
		if (nx < 0.0)
		{
			actor->RotateWXYZ(180, 0, 1, 0);
			n = -n;

		}
		actor->RotateWXYZ(180, (nx + n)*0.5, ny*0.5, nz*0.5);
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		//renWin->HideCursor();
		int x, y;
		iren->GetEventPosition(x, y);
		picker->Pick(x, y, 0, ren);
		double p[3];
		picker->GetPickPosition(p);
		std::cout << p[0]<< " "<< p[1]<<" " << p[2]<<std::endl;
		double n[3];
		picker->GetPickNormal(n);
		redCone->SetPosition(p[0], p[1], p[2]);
		PointCone(redCone, n[0], n[1], n[2]);
		greenCone->SetPosition(p[0], p[1], p[2]);
		PointCone(greenCone, -n[0], -n[1], -n[2]);
		iren->Render();
	}

	vtkRenderWindow *renWin;
	vtkRenderer *ren;
	vtkRenderWindowInteractor *iren;
	vtkVolumePicker *picker;
	vtkActor *redCone;
	vtkActor *greenCone;
};

int main(int, char *[])
{
	//renderer and interactor
	vtkNew<vtkRenderer> ren;
	vtkNew<vtkRenderWindow> renWin;
	renWin->AddRenderer(ren);
	vtkNew<vtkRenderWindowInteractor> iren;
	iren->SetRenderWindow(renWin);

	//read the volume
	vtkNew<vtkImageReader2> reader;
	reader->SetDataExtent(0, 63, 0, 63, 0, 92);
	reader->SetFileNameSliceOffset(1);
	reader->SetDataScalarTypeToUnsignedShort();
	reader->SetDataByteOrderToLittleEndian();
	reader->SetFilePrefix("H:/VTK/Build-VTK-8.2.0/ExternalData/Testing/Data/headsq/quarter");
	reader->SetDataSpacing(3.2, 3.2, 1.5);

	//set up the volume rendering
	vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
	volumeMapper->SetInputConnection(reader->GetOutputPort());
	volumeMapper->CroppingOn();
	double croppingRegionPlanes[6] = {0.0, 141.6, 0.0, 201.6, 0.0, 138.0};
	volumeMapper->SetCroppingRegionPlanes(croppingRegionPlanes);

	vtkNew<vtkColorTransferFunction> volumeColor;
	volumeColor->AddRGBPoint(0, 0.0, 0.0, 0.0);
	volumeColor->AddRGBPoint(180, 0.3, 0.1, 0.2);
	volumeColor->AddRGBPoint(1000, 1.0, 0.7, 0.6);
	volumeColor->AddRGBPoint(2000, 1.0, 1.0, 0.9);

	vtkNew<vtkPiecewiseFunction> volumeScalarOpacity;
	volumeScalarOpacity->AddPoint(0, 0.0);
	volumeScalarOpacity->AddPoint(180, 0.0);
	volumeScalarOpacity->AddPoint(1000, 0.2);
	volumeScalarOpacity->AddPoint(2000, 0.8);

	vtkNew<vtkPiecewiseFunction> volumeGradientOpacity;
	volumeGradientOpacity->AddPoint(0, 0.0);
	volumeGradientOpacity->AddPoint(90, 0.5);
	volumeGradientOpacity->AddPoint(100, 1.0);

	vtkNew<vtkVolumeProperty> volumeProperty;
	volumeProperty->SetColor(volumeColor);
	volumeProperty->SetScalarOpacity(volumeScalarOpacity);
	volumeProperty->SetGradientOpacity(volumeGradientOpacity);
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->ShadeOff();
	volumeProperty->SetAmbient(0.6);
	volumeProperty->SetDiffuse(0.6);
	volumeProperty->SetSpecular(0.1);

	vtkNew<vtkVolume> volume;
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// Do the surface rendering
	vtkNew<vtkMarchingCubes> boneExtractor;
	boneExtractor->SetInputConnection(reader->GetOutputPort());
	boneExtractor->SetValue(0, 1150);
	boneExtractor->Update();

	vtkNew<vtkPolyDataNormals>boneNormals;
	boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
	boneNormals->SetFeatureAngle(60.0);

	vtkNew<vtkStripper> boneStripper;
	boneStripper->SetInputConnection(boneNormals->GetOutputPort());

	vtkNew<vtkCellLocator> boneLocator;
	boneLocator->SetDataSet(boneExtractor->GetOutput());
	boneLocator->LazyEvaluationOn();

	vtkNew<vtkPolyDataMapper> boneMapper;
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	boneMapper->ScalarVisibilityOff();

	vtkNew<vtkProperty> boneProperty;
	boneProperty->SetColor(1.0, 1.0, 0.9);

	vtkNew<vtkActor> bone;
	bone->SetMapper(boneMapper);
	bone->SetProperty(boneProperty);

	//Create an image actor
	vtkNew<vtkLookupTable> table;
	table->SetRange(0, 2000);
	table->SetRampToLinear();
	table->SetValueRange(0, 1);
	table->SetHueRange(0, 0);
	table->SetSaturationRange(0, 0);

	vtkNew<vtkImageMapToColors> mapToColors;
	mapToColors->SetInputConnection(reader->GetOutputPort());
	mapToColors->SetLookupTable(table);
	mapToColors->Update();

	vtkNew<vtkImageActor> imageActor;
	imageActor->GetMapper()->SetInputConnection(mapToColors->GetOutputPort());
	imageActor->SetDisplayExtent(32, 32, 0, 63, 0, 92);

	//make a transform and some clipping planes
	vtkNew<vtkTransform> transform;
	transform->RotateWXYZ(-20, 0.0, -0.7, 0.7);
	volume->SetUserTransform(transform);
	bone->SetUserTransform(transform);
	imageActor->SetUserTransform(transform);

	auto c = volume->GetCenter();

	vtkNew<vtkPlane> volumeClip;
	volumeClip->SetNormal(0, 1, 0);
	volumeClip->SetOrigin(c[0], c[1], c[2]);

	vtkNew<vtkPlane> boneClip;
	boneClip->SetNormal(1, 0, 0);
	boneClip->SetOrigin(c[0], c[1], c[2]);

	volumeMapper->AddClippingPlane(volumeClip);
	boneMapper->AddClippingPlane(boneClip);

	ren->AddViewProp(volume);
	ren->AddViewProp(bone);
	ren->AddViewProp(imageActor);

	auto camera = ren->GetActiveCamera();
	camera->SetFocalPoint(c[0], c[1], c[2]);
	camera->SetPosition(c[0] + 500, c[1] - 100, c[2] - 100);
	camera->SetViewUp(0, 0, -1);

	//the cone points along the - x axis
	vtkNew<vtkConeSource> coneSource;
	coneSource->CappingOn();
	coneSource->SetHeight(12);
	coneSource->SetRadius(5);
	coneSource->SetResolution(31);
	coneSource->SetCenter(6, 0, 0);
	coneSource->SetDirection(-1, 0, 0);

	vtkNew<vtkDataSetMapper> coneMapper;
	coneMapper->SetInputConnection(coneSource->GetOutputPort());

	vtkNew<vtkActor> redCone;
	redCone->PickableOff();
	redCone->SetMapper(coneMapper);
	redCone->GetProperty()->SetColor(1, 0, 0);

	vtkNew<vtkActor> greenCone;
	greenCone->PickableOff();
	greenCone->SetMapper(coneMapper);
	greenCone->GetProperty()->SetColor(0, 1, 0);

	//Add the two cones(or just one, if you want);
	ren->AddViewProp(redCone);
	ren->AddViewProp(greenCone);

	//the picker
	vtkNew<vtkVolumePicker> picker ;
	picker->SetTolerance(1e-6);
	picker->SetVolumeOpacityIsovalue(0.1);
	//locator is optional, but improves performance for large polydata
	picker->AddLocator(boneLocator);
	
	renWin->Render();
	vtkNew<vtkMyMouseCommand> mouseCommand;
	mouseCommand->renWin = renWin;
	mouseCommand->ren= ren;
	mouseCommand->iren = iren;
	mouseCommand->picker = picker;
	mouseCommand->redCone = redCone;
	mouseCommand->greenCone = greenCone;
	iren->AddObserver(vtkCommand::MouseMoveEvent, mouseCommand);
	iren->Start();
	return EXIT_SUCCESS;
}