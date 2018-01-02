#include "Headgets.h"

//Theese globals are used for an example only!
hdg::Label* label;
hdg::Button* button;
hdg::Editbox* edit;

void eventProcessor(hdg::Event ev) {
	if (ev.type == hdg::EventType::MouseEvent && ev.mouse == hdg::MouseEvent::LeftPressed) {
		ev.app->setTitle("Test App X="+std::to_string(ev.app->getX())+" Y="+std::to_string(ev.app->getY()));
		label->setText("Hello World!");
		label->show();
	}

	if (ev.type == hdg::EventType::Command) {
		if (ev.num1 == button->getID()) {
			button->setText("Clicked!");
			button->disable();

			hdg::showMessageBox("Hello, "+edit->value()+", your system username is "+hdg::getEnvironmentVariable("USERNAME"));
		}
	}
}

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	app.setUserCallback(eventProcessor);

	label = new hdg::Label("Click anywhere to change text.");
	label->setPosition(10,10);

	button = new hdg::Button("Click me!", 10, 150);
	button->show();

	edit = new hdg::Editbox(hdg::EditboxStyle::Uppercase, 10, 75);
	edit->show();

	app.run();

	return 0;
}
