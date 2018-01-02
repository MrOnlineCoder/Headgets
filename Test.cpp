#include "Headgets.h"

class TestApp {
public:
	TestApp(HINSTANCE hInstance)
		: app(hInstance, "Headgets Test application", 700, 200) {

			//Create all widgets
			label = new hdg::Label("This is test label. Hello, "+hdg::getEnvironmentVariable("USERNAME"), 10, 10, 150);
			button = new hdg::Button("Test button.", 10, 50);
			
			editDesc = new hdg::Label("Example of password field:", 200, 10, 200);
			edit = new hdg::Editbox(hdg::EditboxStyle::Password, 400, 10, 200);
			editTest = new hdg::Button("Get password value", 200, 40);

			//Set event callback
			app.setUserCallback(std::bind(&TestApp::eventCallback, this, std::placeholders::_1));
	}

	void eventCallback(hdg::Event ev) {
		if (ev.type == hdg::EventType::Command) {
			if (ev.num1 == button->getID()) {
				hdg::showMessageBox("You clicked the button!");
				app.setTitle("Headgets Test application - already clicked");
				button->disable();
				return;
			}

			if (ev.num1 == editTest->getID()) {
				hdg::showMessageBox("Entered password is: "+edit->value());
			}
		}
	}

	int run() {
		return app.run();
	}
private:
	hdg::Application app;

	hdg::Label* label;
	hdg::Button* button;
	hdg::Editbox* edit;

	hdg::Label* editDesc;
	hdg::Button* editTest;
};

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TestApp app(hInstance);

	return app.run();
}