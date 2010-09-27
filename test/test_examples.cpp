/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include "test_glv.h"

using namespace glv;

const int pages = 15;
Buttons tabs(Rect(110, pages*20), 1, pages, false, true);
Group groups[pages];

Button bt1(Rect(20), false, draw::check), bt2(bt1, false, draw::triangleU, draw::triangleD);
Button btl(Rect(100, 20), true);
Buttons bts14(Rect(60, 60), 1, 4, false, true);
Buttons bts41(Rect(120, 15), 4, 1, false, true);
Buttons bts44(Rect(60), 4, 4);
Buttons btsLED(Rect(120), 8, 8, false, false, draw::disc<12>, draw::circle<12>);
Buttons btv(Rect(80), 4, 4);
FunctionGraph fg(Rect(200, 100), 4, 10);
const int plotSize=64;
Plot	plotXY(Rect(150), plotSize, PlotDim::XY, Color(0,0,0.5)),
		plotX(Rect(150,60), Color(0.5,0,0)),
		plotY(Rect(60,150), Color(0,0.5,0));
DensityPlot dplot(Rect(150));

NumberDialer nd1(12,0,0, 4,0, 9999,-9999), nd2(16,0,0, 1,8, 8,0);
Slider sl1H(Rect(100, 20)), sl1V(Rect(20, 100)), sl1HS(sl1H,0), sl1VS(sl1V,0);
Slider2D sl2(Rect(100));
SliderGrid<3> sg3(Rect(100));
SliderGrid<4> sg4(Rect(100));
SliderRange slRH(Rect(100,20)), slRV(Rect(20,100));
Sliders sliders1(Rect(100), 10, 1, true), sliders2(sliders1, 4, 10, true);
Table table(	". v - -,"
				"> p ^ q,"
				"| < x >,"
				"| b v d", 12, 6, Rect(0));
TextView tv1(Rect(200,16), 8);

struct FontView : View {
	FontView(const Rect& r=Rect(0)): View(r){}
	
	virtual void onDraw(){
		draw::color(colors().text);
		float x = tabs.width()+10;
		const char * str = "Amazingly few discotheques provide jukeboxes.";
		font.size( 6).render(str, x, 10);
		font.size( 8).render(str, x, 30);
		font.size(12).render(str, x, 50);
	}
	
	Font font;
} fontView;

struct SubView3D : View3D{
	SubView3D(const Rect& r): View3D(r), angle(0){
		addCallback(Event::MouseDrag, Behavior::mouseResizeCorner);
		addCallback(Event::MouseDrag, Behavior::mouseMove);
		enable(KeepWithinParent);
	}
	
	virtual void onDraw3D(){
		using namespace glv::draw;
		translateZ(-3);
		rotateY(angle+=1);
		const int n=12;

		Point3 pts[n*2];

		color(0.4);

		for(int i=0; i<n; ++i){
			float p = float(i)/n;
			float x = cos(p*6.283)*1.2;
			float z = sin(p*6.283)*1.2;
			
			pts[2*i+0](x, -0.7, z);
			pts[2*i+1](x, +0.7, z);
			
			
//			color(HSV(p,1,1));
//			color(0);
		}

		paint(Triangles, pts, n*2);	
	}

	unsigned angle;
} v3D(Rect(200)), v3D2(Rect(100));


void drawCB(View * v){
	using namespace glv::draw;
	for(int i=0; i<tabs.size(); ++i) groups[i].property(Visible, tabs.getValue(i));
}



int main(int argc, char ** argv){
	
	GLV top(drawCB);
	Window win(600, 400, "GLV Examples", &top);

	sl1HS.interval(1,-1);
	sl1VS.interval(1,-1);
	sliders1.interval(1,-1);
	sg3.interval(20,-20);
	sl2.interval(20,-20);
	slRH.interval(2,-2);
	
	glv::Style::standard().color.set(StyleColor::BlackOnWhite);
//	glv::Style::standard().color.set(StyleColor::WhiteOnBlack);
//	glv::Style::standard().color.set(StyleColor::Gray);
//	glv::Style::standard().color.hsv(0.5,0.5,0.2);
	
	for(int i=0; i<pages; ++i){
		top << groups[i].disable(Visible | HitTest).stretch(1,1);
	}
	
	fg.tension(1.);

	table	<< new Label("col span") << new Label("row span", true)
			<< new Label("top-left") << new Label("top-center") << new Label("top-right") 
			<< new Label("center-left") << new Label("center-center") << new Label("center-right")
			<< new Label("bottom-left") << new Label("bottom-center") << new Label("bottom-right");
	table.arrange();
	//table.enable(DrawBorder);
	
	const float ffreq = 2*M_PI/(plotSize-1);

	plotXY.data().stroke(2);
	plotXY.preserveAspect(true);
	plotXY.addCallback(Event::MouseDrag, Behavior::mouseResizeCorner);
	plotX.addCallback(Event::MouseDrag, Behavior::mouseResizeCorner);
	for(int i=0; i<plotSize; ++i){
		float phs = i*ffreq;
		float sigX = (cos(phs) + 0.5*cos(2*phs))*0.6;
		float sigY = (sin(phs) + 0.5*sin(2*phs))*0.6;
		plotXY.data().points(0)[i] = sigX;
		plotXY.data().points(1)[i] = sigY;
	}
	plotX.data().reference(1, plotXY.data().points(0), plotSize);
	plotY.data().reference(0, plotXY.data().points(1), plotSize);
	plotX.tickMajorX(8).rangeFit();
	plotY.tickMajorY(8).rangeFit();

	// Produce matrix of values
	{
		dplot.model().resize(1,32,32);
		Data& data = dplot.model();
		for(int j=0; j<data.size(2); ++j){
			for(int i=0; i<data.size(1); ++i){
				double y = double(j)/data.size(2)*2-1;
				double x = double(i)/data.size(1)*2-1;
				double r = fabs(sin(hypot(x,y)*8));
				data.assign(r, 0, i,j);
			}
		}
	}

	v3D << v3D2.pos(Place::BR).anchor(Place::BR);

//btsLED.enable(Momentary);
	int i=-1;
	groups[++i]<< bt1.pos(Place::BL).anchor(Place::CC)(Event::MouseDrag, Behavior::mouseResize);
	groups[  i]<< bt2.pos(Place::BR, -4,0).anchor(Place::CC)(Event::MouseDrag, Behavior::mouseResize);
	groups[  i]<< (btl.pos(Place::TR, -4, 4).anchor(Place::CC) << new Label("OK", Place::CC, 0,0));
	groups[++i]<< bts14.pos(Place::BR, -4,0).anchor(Place::CC);
	groups[  i]<< bts41.pos(Place::TL, 0, 4).anchor(Place::CC);
	groups[  i]<< bts44.pos(Place::BL).anchor(Place::CC);
	groups[  i]<< btsLED.padding(3).pos(Place::BL, bts44.w+4,0).anchor(Place::CC).enable(SelectOnDrag).disable(DrawGrid);
	groups[++i]<< fg.pos(Place::BL).anchor(Place::CC);
//	groups[++i]<< fontView.stretch(1,1);
	groups[++i]<< (new Label("Horizontal"))->pos(Place::BL).anchor(Place::CC);
	groups[  i]<< (new Label("Vertical", true))->pos(Place::BR,-16,0).anchor(Place::CC);
	groups[++i]<< nd1.pos(Place::BL).anchor(Place::CC);
	groups[  i]<< nd2.showSign(false).pos(Place::TL, 0, 4).anchor(Place::CC);
	groups[++i]<< sl1H.pos(Place::BL).anchor(Place::CC);
	groups[  i]<< sl1V.pos(Place::BR, -4,0).anchor(Place::CC);
	groups[  i]<< sl1HS.pos(Place::BL).anchor(Place::CC); sl1HS.top(sl1H.bottom()+4);
	groups[  i]<< sl1VS.pos(Place::BR, -4,0).anchor(Place::CC); sl1VS.right(sl1V.left()-4);
	groups[++i]<< sl2.pos(Place::BL).anchor(Place::CC);
	groups[++i]<< sg3.pos(Place::BL).anchor(Place::CC);
	groups[  i]<< sg4.pos(Place::TL, 0, 4).anchor(Place::CC);
	groups[++i]<< slRH.pos(Place::BL).anchor(Place::CC);
	groups[  i]<< slRV.pos(Place::BR, -4,0).anchor(Place::CC);
	groups[++i]<< sliders1.pos(Place::BR).anchor(Place::CC); 
	groups[  i]<< sliders2.pos(Place::BL, 4,0).anchor(Place::CC);
	groups[++i]<< table.pos(Place::BC).anchor(Place::CC);
	groups[++i]<< plotXY.pos(Place::BL).anchor(Place::CC);
	groups[  i]<< plotX.pos(Place::TL, 0,4).anchor(Place::CC);
	groups[  i]<< plotY.pos(Place::BR,-4,0).anchor(Place::CC);
	groups[++i]<< dplot.pos(Place::BL).anchor(Place::CC);
	groups[++i]<< tv1.pos(Place::CC, 0,0).anchor(Place::CC);
	groups[++i]<< v3D.pos(Place::CC, 0,0).anchor(Place::CC);

	float y = 1./(tabs.size()*2), dy=2*y;
	tabs << (new Label("Button"			))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("Buttons"		))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("FunctionGraph"	))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("Label"			))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("NumberDialer"	))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("Slider"			))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("Slider2D"		))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("SliderGrid"		))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("SliderRange"	))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("Sliders"		))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("Table"			))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("FunctionPlot"	))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("DensityPlot"	))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("TextView"		))->anchor(0.5, y).pos(Place::CC); y+=dy;
	tabs << (new Label("View3D"			))->anchor(0.5, y).pos(Place::CC); y+=dy;
	top << tabs;

	//tabs(Event::MouseDrag, Behavior::mouseResize);

	//#define PRINT_SIZE(x) printf("sizeof(%s) = %d\n", #x, sizeof(x));
	//PRINT_SIZE(View) PRINT_SIZE(Notifier) PRINT_SIZE(Rect)

	Application::run();
}

