#include "Headgets.h"

void eventProcessor(hdg::Event ev) {
	if (ev.type == hdg::EventType::Closed) {
		//hdg::showMessageBox("Window closed :(", "Oh", hdg::MessageBoxType::Warning);
	}

	if (ev.type == hdg::EventType::MouseEvent && ev.mouse == hdg::MouseEvent::LeftPressed) {
		ev.app->moveTo(50, 50);
		ev.app->setTitle("Test App X="+std::to_string(ev.app->getX())+" Y="+std::to_string(ev.app->getY()));
	}
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	app.setUserCallback(eventProcessor);
	return app.run();
}
