/*
	Headgets C++ library

	Author: MrOnlineCoder

	github.com/MrOnlineCoder/Headgets

	MIT License

	Copyright (c) 2017 Nikita Kogut (MrOnlineCoder)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#ifndef _HEADGETS_H
#define _HEADGETS_H

#ifndef _WIN32
#error Headgets library supports only Windows platform
#endif

#include <Windows.h>
#include <Windowsx.h>

#include <string>
#include <functional>

#include <cassert>

namespace hdg {
	//Win32 window class name, used in RegisterClassEx
	const char* HDG_CLASSNAME = "HEADGETSWINDOW";

	//Forward declartion
	class Application;

	/*============== Utility ===========*/
	enum class MessageBoxType {
		Empty = 0,
		Information = MB_ICONINFORMATION,
		Warning = MB_ICONWARNING,
		Error = MB_ICONERROR
	};

	enum class MessageBoxButtons {
		Ok = 0,
		OkCancel = 1,
		AbortRetryIgnore = 2,
		YesNoCancel = 3,
		YesNo = 4,
		RetryCancel = 5,
		CancelTryContinue = 6
	};

	static void showMessageBox(std::string text, std::string caption="Information", hdg::MessageBoxType type = hdg::MessageBoxType::Information, hdg::MessageBoxButtons buttons = hdg::MessageBoxButtons::Ok) {

		MessageBox(NULL, text.c_str(), caption.c_str(), static_cast<UINT>(type) | static_cast<UINT>(buttons) );
	}

	/*============== Events ============*/
	enum class EventType {
		Created = 0,

		Resized,
		Moved,

		MouseEvent,

		Closed,
		Destroyed
	};

	enum class MouseEvent {
		Nothing = 0,
		LeftPressed = WM_LBUTTONDOWN,
		RightPressed = WM_RBUTTONDOWN,
		LeftReleased = WM_LBUTTONUP,
		RightReleased = WM_RBUTTONUP
	};

	//Event struct
	//Contains:
	//type - type of event
	//num1, num2 - possible integer values, can be used to store native event data. By default they should be 0
	//handle - handle to window, which send the event, can be NULL
	struct Event {
		hdg::EventType type;

		int num1;
		int num2;

		hdg::MouseEvent mouse;

		HWND handle;

		hdg::Application* app;
	};


	/*============== Application ============*/
	class Application {
	public:
		Application(HINSTANCE _instance, std::string _title, int _width, int _height) {
			if (instance != NULL) {
				_fatal("Only 1 instance of hdg::Application is allowed at any time. Destruct the another one.");
			}

			hinstance = _instance;
			title = _title;
			width = _width;
			height = _height;

			window = NULL;

			open = false;

			instance = this;

			registerWindowClass();

			DWORD dwStyle= (WS_OVERLAPPEDWINDOW);

			this->window = CreateWindow(
				HDG_CLASSNAME,
				title.c_str(),
				dwStyle,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				width,
				height,
				NULL,
				NULL,
				hinstance,
				NULL
				);

			if(window == NULL)
			{
				_fatal("Failed to create window!");
			}
		}

		~Application() {
			instance = NULL;
		}

		int run() {
			ShowWindow(window, SW_SHOW);
			UpdateWindow(window);

			open = true;

			MSG msg;
			while(GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return msg.wParam;
		}

		static LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			return instance->RealWndProc(hwnd, msg, wParam, lParam); 
		}

		LRESULT CALLBACK RealWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch(msg) {
				//Create and destroy events
				case WM_CREATE:
					postSimpleEvent(hdg::EventType::Created, hwnd);
					return 0;
					break;
				case WM_CLOSE:
					postSimpleEvent(hdg::EventType::Closed, hwnd);
					DestroyWindow(hwnd);
					break;
				case WM_DESTROY:
					postSimpleEvent(hdg::EventType::Destroyed);
					open = false;
					PostQuitMessage(0);
					break;
				//Mouse events
				case WM_LBUTTONUP:
				case WM_LBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_RBUTTONDOWN: {
					hdg::Event ev = {
						hdg::EventType::MouseEvent,
						GET_X_LPARAM(lParam),
						GET_Y_LPARAM(lParam),
						static_cast<hdg::MouseEvent>(msg),
						hwnd,
						this
					};
					postEvent(ev);
					break;
				}
				//Move and size events
				case WM_MOVE: {
					hdg::Event ev = {
						hdg::EventType::Moved,
						(int)(short) LOWORD(lParam),
						(int)(short) HIWORD(lParam),
						hdg::MouseEvent::Nothing,
						hwnd,
						this
					};
					this->x = ev.num1;
					this->y = ev.num2;
					postEvent(ev);
					break;
				}
				case WM_SIZE: {
					hdg::Event ev = {
						hdg::EventType::Resized,
						(int)(short) LOWORD(lParam),
						(int)(short) HIWORD(lParam),
						hdg::MouseEvent::Nothing,
						hwnd,
						this
					};
					this->width = ev.num1;
					this->height = ev.num2;
					postEvent(ev);
					break;
				}

				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
		}

		void setUserCallback(std::function<void(hdg::Event)> func) {
			eventCallback = func;

			if (!eventCallback) _fatal("Failed to set event callback");
		}

		int getX() {
			return x;
		}

		int getY() {
			return y;
		}

		void moveTo(int newX, int newY) {
			x = newX;
			y = newY;

			if (SetWindowPos(window, (HWND) -1, x, y, -1, -1, SWP_NOZORDER | SWP_NOSIZE) == 0) {
				hdg::Application::_reportLastError("Application::moveTo()");
			}
		}

		void moveBy(int dX, int dY) {
			moveTo(x+dX, y+dY);
		}

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height;
		}

		HWND getNativeHandle() {
			return window;
		}

		bool isOpen() {
			return open;
		}

		void close() {
			SendMessage(window, WM_CLOSE, 0, 0);
		}

		void setTitle(std::string str) {
			title = str;
			SetWindowText(window, title.c_str());
		}

		static void _reportLastError(std::string func) {
			std::string text = "";
			text = func+" call failed, GetLastError() = "+std::to_string(GetLastError());
			MessageBox(NULL, text.c_str(), "Headgets Error", MB_SYSTEMMODAL | MB_OK | MB_ICONERROR);
		}

		static Application *instance;
	private:
		//Shows fatal error message and exits the application
		//TODO: more polite error handling
		void _fatal(std::string msg) {
			MessageBox(NULL, msg.c_str(), "Headgets Error", MB_SYSTEMMODAL | MB_OK | MB_ICONERROR);
			ExitProcess(0);
		}

		//Registers Win32 window class.
		void registerWindowClass() {
			WNDCLASSEX wc;

			wc.cbSize        = sizeof(WNDCLASSEX);

			wc.style         = 0;

			wc.lpfnWndProc   = this->_WndProc;

			wc.cbClsExtra    = 0;
			wc.cbWndExtra    = 0;

			wc.hInstance     = hinstance;

			wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
			wc.lpszMenuName  = NULL;
			wc.lpszClassName = HDG_CLASSNAME;
			wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

			if(!RegisterClassEx(&wc)) {
				_fatal("Failed to register Headgets Win32 window class.");
			}
		}

		//Helper function to send an event to user quickly
		void postSimpleEvent(hdg::EventType type, HWND wnd=NULL, int n1=0, int n2=0) {
			hdg::Event ev;
			ev.type = type;
			ev.handle = wnd;
			ev.num1 = n1;
			ev.num2 = n2;
			ev.mouse = hdg::MouseEvent::Nothing;
			ev.app = this;


			if (eventCallback) eventCallback(ev);
		}

		void postEvent(hdg::Event ev) {
			if (eventCallback) eventCallback(ev);
		}

		//Handle of main window
		HWND window;

		//Instance of the application in OS
		HINSTANCE hinstance;
		
		//Window title
		std::string title;

		//Size
		int width;
		int height;

		//Pos
		int x;
		int y;

		//Is window open?
		bool open;

		//Event callback
		//Used to send user (library user) an hdg::Event so he can process it.
		std::function<void(hdg::Event)> eventCallback;
	};
	
	/*============== Widgets ================*/

	class Widget {
	public:
		Widget(HWND _parent) {
			assert(_parent != NULL);

			parent = _parent;
			hinstance = (HINSTANCE) GetWindowLong (parent, GWL_HINSTANCE);
		}

		Widget(Application* app) {
			assert(app != NULL);

			parent = app->getNativeHandle();
			hinstance = (HINSTANCE) GetWindowLong (parent, GWL_HINSTANCE);
		}

		void hide() {
			ShowWindow(window, SW_HIDE);
		}

		void show() {
			ShowWindow(window, SW_SHOW);
		}

		void setPosition(int x, int y) {
			if (SetWindowPos(window, (HWND) -1, x, y, -1, -1, SWP_NOZORDER | SWP_NOSIZE) == 0) {
				hdg::Application::_reportLastError("Widget::setPosition()");
			}
		}

		void setSize(int w, int h) {
			if (SetWindowPos(window, (HWND) -1, -1, w, h, -1, SWP_NOZORDER | SWP_NOMOVE) == 0) {
				hdg::Application::_reportLastError("Widget::setSize()");
			}
		}
	protected:
		HWND parent;
		HWND window;

		HINSTANCE hinstance;
	};

	class Label : public hdg::Widget {
	public:
		Label(std::string _text)
		: Widget(hdg::Application::instance){
			text = _text;

			window = CreateWindow("STATIC", text.c_str(),  WS_CHILD | WS_VISIBLE | WS_TABSTOP, 0, 0, 100, 50, parent, NULL, hinstance, NULL);

			if (window == NULL) hdg::Application::_reportLastError("Label::Label()");
		}


		void setText(std::string txt) {
			text = txt;
			SetWindowText(window, text.c_str());
		}
	private:
		std::string text;
	};

	//Instance of the application
	Application * Application::instance = NULL;
};

#endif