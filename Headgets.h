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

#include <string>
#include <functional>

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
		Closed,
		Destroyed
	};

	//Event struct
	//Contains:
	//type - type of event
	//num1, num2 - possible integer values, can be used to store native event data. By default they should be 0
	//handle - handle to window, which send the event, can be NULL
	struct Event {
		EventType type;

		int num1;
		int num2;

		HWND handle;
	};

	/*============== Application ============*/
	class Application {
	public:
		Application(HINSTANCE _instance, std::string _title, int _width, int _height) {
			hinstance = _instance;
			title = _title;
			width = _width;
			height = _height;

			open = false;

			instance = this;

			registerWindowClass();
		}

		int run() {
			DWORD dwStyle= (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

			window = CreateWindow(
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

		bool isOpen() {
			return open;
		}

		void setTitle(std::string str) {
			title = str;
		}

		static LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			return instance->RealWndProc(hwnd, msg, wParam, lParam); 
		}

		LRESULT CALLBACK RealWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch(msg) {
				case WM_CREATE:
					postEvent(hdg::EventType::Created, hwnd);
					return 0;
					break;
				case WM_CLOSE:
					postEvent(hdg::EventType::Closed, hwnd);
					DestroyWindow(hwnd);
					break;
				case WM_DESTROY:
					postEvent(hdg::EventType::Destroyed, hwnd);
					PostQuitMessage(0);
					break;
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
				}
		}

		void setUserCallback(std::function<void(hdg::Event)> func) {
			eventCallback = func;

			if (!eventCallback) _fatal("Failed to set event callback");
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
		void postEvent(hdg::EventType type, HWND wnd=0, int n1=0, int n2=0) {
			hdg::Event ev;
			ev.type = type;
			ev.handle = wnd;
			ev.num1 = n1;
			ev.num2 = n2;


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

		//Is window open?
		bool open;

		//Event callback
		//Used to send user (library user) an hdg::Event so he can process it.
		std::function<void(hdg::Event)> eventCallback;
	};

	//Instance of the application
	Application * Application::instance = NULL;
};

#endif