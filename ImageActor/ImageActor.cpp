#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkBMPReader.h>
#include <vtkImageActor.h>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include "vtkPolyDataMapper.h"
#include "vtkSphere.h"
#include "vtkProperty.h"
#include "math.h"
#include "vtkMath.h"

//vector(c) = vector(b) - vector(a)
static void getVectorDiff(const double *a, const double *b, double *c, int n = 3)
{
	for (int i = 0; i < n; i++)
		c[i] = b[i] - a[i];
}

//vector(a) inner product vector(b)
static double getInnerProduct(const double *a, const double *b, int n = 3)
{
	double result = 0.0;
	for (int i = 0; i < n; i++)
		result += a[i] * b[i];
	return result;
}

static void getVectorCenter(const double *a, const double *b, double *c, int n = 3)
{
	for (int i = 0; i < n; i++)
		c[i] = (a[i] + b[i]) / 2.0;
}

//Determinants of third-order matrices
static float getVector3Det(const double *v1, const double *v2, const double *v3)
{
	double a[3][3];
	for (int i = 0; i < 3; ++i) {
		a[i][0] = v1[i];
		a[i][1] = v2[i];
		a[i][2] = v3[i];
	}
	return a[0][0] * a[1][1] * a[2][2] -
		a[0][0] * a[1][2] * a[2][1] -
		a[0][1] * a[1][0] * a[2][2] +
		a[0][1] * a[1][2] * a[2][0] +
		a[0][2] * a[1][0] * a[2][1] -
		a[0][2] * a[1][1] * a[2][0];
}

//Euclidean Distance
static double getEuclideanDistance(const double *v1, const double *v2, int n = 3)
{
	double result = 0.0;
	for (int i = 0; i < n; i++)
		result += pow(v1[i] - v2[i], 2);
	return sqrt(result);
}

/********************************************************************************/
/* Finding the Center and Radius of a Triangle Circle in Three-Dimensional Space*/
/*circle = {x, y, z radius}                                                     */
/********************************************************************************/ 
static bool triangleCircumCentre(double* pt[3], double  circle[4])
{
	double line_center1[3], line_center2[3], line_center3[3];
	double normal1[3], normal2[3], normal3[3];
	double result[3], rep1[3], rep2[3], rep3[3];
	float d_det;

	getVectorCenter(pt[0], pt[1], line_center1);
	getVectorCenter(pt[0], pt[2], line_center2);
	getVectorCenter(pt[1], pt[2], line_center3);
	getVectorDiff(pt[0], pt[1], normal1);
	getVectorDiff(pt[0], pt[2], normal2);
	vtkMath::Cross(normal1, normal2, normal3);

	result[0] = vtkMath::Dot(line_center1, normal1);
	result[1] = vtkMath::Dot(line_center2, normal2);
	result[2] = vtkMath::Dot(pt[0], normal3);
	rep1[0] = normal1[0];
	rep1[1] = normal2[0];
	rep1[2] = normal3[0];
	rep2[0] = normal1[1];
	rep2[1] = normal2[1];
	rep2[2] = normal3[1];
	rep3[0] = normal1[2];
	rep3[1] = normal2[2];
	rep3[2] = normal3[2];

	d_det = getVector3Det(rep1, rep2, rep3);

	if (fabs(d_det) < 1e-15)
		return false;

	//Cramer Rule
	circle[0] = getVector3Det(result, rep2, rep3) / d_det;
	circle[1] = getVector3Det(rep1, result, rep3) / d_det;
	circle[2] = getVector3Det(rep1, rep2, result) / d_det;
	circle[3] = getEuclideanDistance(circle, pt[0]);
	return true;
}

struct Point3
{
	double x, y, z;
	Point3(double P0, double P1, double P2)
	{
		x = P0;
		y = P1;
		z = P2;
	}
};

int solveCenterPointOfCircle(std::vector<Point3> pd, double centerpoint[])
{
	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double a3, b3, c3, d3;

	double x1 = pd[0].x, y1 = pd[0].y, z1 = pd[0].z;
	double x2 = pd[1].x, y2 = pd[1].y, z2 = pd[1].z;
	double x3 = pd[2].x, y3 = pd[2].y, z3 = pd[2].z;

	a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
	b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
	c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
	d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);

	a2 = 2 * (x2 - x1);
	b2 = 2 * (y2 - y1);
	c2 = 2 * (z2 - z1);
	d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

	a3 = 2 * (x3 - x1);
	b3 = 2 * (y3 - y1);
	c3 = 2 * (z3 - z1);
	d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

	centerpoint[0] = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)
		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
	centerpoint[1] = (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)
		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
	centerpoint[2] = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)
		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);

	centerpoint[3] = sqrt(pow(pd[0].x - centerpoint[0], 2) + pow(pd[0].y - centerpoint[1], 2) + pow(pd[0].z - centerpoint[2], 2));

	return 0;
}

//≤‚ ‘ÕºœÒ£∫../data/lena.bmp
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << argv[0] << " " << "ImageFile(*.bmp)" << std::endl;
		return EXIT_FAILURE;
	}
	vtkSmartPointer<vtkBMPReader> reader =
		vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName(argv[1]);
	reader->Update();

	vtkSmartPointer<vtkImageActor> imgActor =
		vtkSmartPointer<vtkImageActor>::New();
	imgActor->SetInputData(reader->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(imgActor);
	renderer->SetBackground(0.0, 0.0, 0.0);

	double p0[3] = { 10.0, 0.0, 0.0 };
	double p1[3] = { 0.0, 10.0, 0.0 };
	double p2[3] = { 0.0, 0.0, -10.0 };
	double* point[3] = { p0, p1, p2 };
	std::vector<Point3> pd;
	pd.push_back({ 10.0, 0.0, 0.0 });
	pd.push_back({ 0.0, 10.0, 0.0 });
	pd.push_back({ 0.0, 0.0, 10.0 });
	for (int i = 0; i < 3; i++) {
		vtkNew<vtkSphereSource> s;
		s->SetCenter(point[i]);
		s->SetRadius(1);
		s->Update();
		vtkNew<vtkPolyDataMapper> m;
		m->SetInputData(s->GetOutput());
		vtkNew<vtkActor> a;
		a->SetMapper(m);
		renderer->AddActor(a);
	}
	double sphere[4];
	triangleCircumCentre(point, sphere);
	//solveCenterPointOfCircle(pd, sphere);
	double center[3] = { sphere[0], sphere[1], sphere[2] };
	vtkNew<vtkSphereSource> s;
	s->SetCenter(center);
	s->SetRadius(sphere[3]);
	s->SetPhiResolution(100);
	s->SetThetaResolution(100);
	s->Update();
	vtkNew<vtkPolyDataMapper> m;
	m->SetInputData(s->GetOutput());
	vtkNew<vtkActor> a;
	a->GetProperty()->SetColor(1.0, 0.0, 0.0);
	a->SetMapper(m);
	a->GetProperty()->SetRepresentationToWireframe();
	renderer->AddActor(a);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("DisplayImageExample2");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	//renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}