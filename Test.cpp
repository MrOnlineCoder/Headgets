#include "Headgets.h"

void eventProcessor(hdg::Event ev) {
	if (ev.type == hdg::EventType::Closed) {
		//hdg::showMessageBox("Window closed :(", "Oh", hdg::MessageBoxType::Warning);
	}

	if (ev.type == hdg::EventType::MouseEvent && ev.mouse == hdg::MouseEvent::LeftPressed) {
		ev.app->moveBy(5, 5);
	}

	if (ev.type == hdg::EventType::Resized) {
		//hdg::showMessageBox("resized to  w: "+std::to_string(ev.num1)+", h: "+std::to_string(ev.num2));
	}
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	app.setUserCallback(eventProcessor);
	return app.run();
}
