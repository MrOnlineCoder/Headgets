#include "Headgets.h"

void eventProcessor(hdg::Event ev) {
	if (ev.type == hdg::EventType::Created) {
		hdg::showMessageBox("<Created> event received!");
	}

	if (ev.type == hdg::EventType::Closed) {
		hdg::showMessageBox("Window closed :(", "Oh", hdg::MessageBoxType::Warning);
	}

	if (ev.type == hdg::EventType::MouseEvent && ev.mouse == hdg::MouseEvent::LeftPressed) {
		hdg::showMessageBox("* click! * on x: "+std::to_string(ev.num1)+", y: "+std::to_string(ev.num2));
	}
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	app.setUserCallback(eventProcessor);
	return app.run();
}
