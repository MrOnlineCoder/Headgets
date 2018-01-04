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
			editTest = new hdg::Button("Get password value", 200, 50);

			bombBtn = new hdg::Button("Open file and destroy", 100, 90);

			bar = new hdg::Progressbar(false, 10, 125, 660, 20);
			hdg::Font font("Arial", hdg::FontWeight::Bold, 18, true);
			editDesc->setFont(font);

			//Set event callback
			app.setUserCallback(std::bind(&TestApp::eventCallback, this, std::placeholders::_1));
	}

	void eventCallback(hdg::Event ev) {
		if (ev.type == hdg::EventType::Command) {

			//ev.num1 holds control ID
			//So we can use it for determining - which control sent the event

			if (ev.num1 == button->getID()) {
				hdg::showMessageBox("You clicked the button!");
				app.setTitle("Headgets Test application - already clicked");
				button->disable();
				bar->step();
				return;
			}

			if (ev.num1 == editTest->getID()) {
				hdg::showMessageBox("Entered password is: "+edit->value());
				bar->step();
				return;
			}

			if (ev.num1 == bombBtn->getID()) {
				bar->step();

				hdg::OpenDialog dlg;
				dlg.setTitle("Select an image file");
				dlg.setRawFilter(hdg::FileFilters::ImageFiles);
				if (dlg.open()) {
					hdg::showMessageBox("You choosed file: "+dlg.getFilename());
				}				

				delete bombBtn;
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

	hdg::Button* bombBtn;
	hdg::Progressbar* bar;
};

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	TestApp app(hInstance);

	return app.run();
}