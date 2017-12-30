#include "Headgets.h"

void eventProcessor(hdg::Event ev) {
	if (ev.type == hdg::EventType::Created) {
		hdg::showMessageBox("<Created> event received!");
	}

	if (ev.type == hdg::EventType::Closed) {
		hdg::showMessageBox("Window closed :(", "Oh", hdg::MessageBoxType::Warning);
	}
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	app.setUserCallback(eventProcessor);
	return app.run();
}
