#include "Headgets.h"

//Theese globals are used for an example only!
hdg::Label* label;

void eventProcessor(hdg::Event ev) {
	if (ev.type == hdg::EventType::MouseEvent && ev.mouse == hdg::MouseEvent::LeftPressed) {
		ev.app->setTitle("Test App X="+std::to_string(ev.app->getX())+" Y="+std::to_string(ev.app->getY()));
		label->setPosition(100,100);
		label->show();
	}
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	app.setUserCallback(eventProcessor);

	label = new hdg::Label("Hello World!");
	label->hide();

	app.run();

	return 0;
}
