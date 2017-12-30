# Headgets

Headgets (**Head**er only wid**gets** library) is header-only library for building basic, simple applications for Windows which require some easy UI.

## Features

* Header-only
* No dependecies, only native Win32 API
* Features built-in widgets
* Has built-in useful utility functions

## Getting Started

Include **Headgets.h** file in your C++ project:

```cpp
#include "Headgets.h"
```

All Headgets classes and functions lie in **hdg** namespace.

## Creating a window
To create a window, create an **hdg::Application** instance. You also need to pass your **HINSTANCE**, initial window title, width and height.

Example:

```cpp
int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	hdg::Application app(hInstance, "Test App", 800, 600);
	return app.run();
}
```

**hdg::Application::run()** runs the main event loop of the application.


## Handling events

In Windows, each window can receive certain **events**, for example: window created, mouse moved, button clicked, window resized, etc. Usually, you capture theese events and execute some code depending on event type and data.

With Headgets, you can also capture theese events, but in a little bit different way. Generally, Headgets process every native event and **wraps** it in own structure **hdg::Event** and then sends it to you using **event callback**. Here is an example how to capture mouse click event:

First, you need to create your callback function, in which you will process every **hdg::Event**. Function signature is: **void function(hdg::Event);**

Example:

```cpp
//Somewhere in your code
void myCallback(hdg::Event ev) {
  //TODO: process `ev` event
}
```

Then you need to register it:

```cpp
hdg::Application app(hInstance, "Test App", 800, 600);

//Registering
app.setUserCallback(myCallback);

return app.run();
```

After that you will receive events in your callback. For detecting mouse click, you can use next piece of code:


```cpp
void myCallback(hdg::Event ev) {
  if (ev.type == hdg::EventType::MouseEvent) {
    if (ev.mouse == hdg::MouseEvent::LeftPressed) {
      //left click! mouse position can be found in
      //ev.mouse.num1 (X), ev.mouse.num2 (Y)
    }
  }
}
```

### All available events:

**hdg::EventType::Created**
* called on window **creation**
* num1 is 0, num2 is 0
* hwnd is created window handle

**hdg::EventType::Closed**
* called when Close (X) button is clicked
* num1 is 0, num2 is 0
* hwnd is window handle

**hdg::EventType::Destroyed**
* called when window was **destroyed**
* num1 is 0, num2 is 0,
* hwnd is NULL

**hdg::EventType::MouseEvent**
* called on mouse event (click or move)
* num1 is mouse X position, num2 is mouse Y position
* mouse field shows which button was pressed or released.
* hwnd is window handle


## Utilites

**Show a message box**

You can show a message box using next function:
```cpp
hdg::showMessageBox(std::string text, std::string title,
   hdg::MessageBoxType type,
   hdg::MessageBoxButtons buttons
 )
```

You can omit last 3 arguments, then message will use **"Information"** as title, **hdg::MessageBoxType::Information** as type and **hdg::MessageBoxButtons::Ok** as buttons.

## Warning
If you are planning to create cross-platform applications with complex UI, I **highly** recommend using any popular and stable UI framework like [Qt](https://www.qt.io/), [wxWidgets](https://www.wxwidgets.org/) or [GTK](https://www.gtk.org/) instead of Headgets. This library was developed for personal use as an hobby project.

## License:
MIT (see Headgets.h file)
