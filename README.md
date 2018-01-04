# Headgets

Headgets (**Head**er only wid**gets** library) is header-only library for building basic, simple applications for Windows which require some easy UI.

## Features

* Header-only
* No third-party dependecies, only native Win32 API
* Features built-in widgets
* Has built-in useful utility functions

## Contents

1. [Getting Started](#getting-started)
2. [Creating a window](#creating-a-window)
3. [Events](#handling-events)
4. [Event types](#all-available-events)
5. [Main application methods](#methods)
6. [Widgets introduction](#widgets)
7. [Widgets list](#all-available-widgets)
8. [Label widget](#label)
9. [Button widget](#button)
10. [Editbox widget](#editbox)
11. [Progressbar widget](#progressbar)
12. [Utilities](#utilites)
13. [File dialogs](#file-dialogs)
14. [License](#license)

## Getting Started

Include **Headgets.h** file in your C++ project:

```cpp
#include "Headgets.h"
```

Headgets require Win32 Common Controls library for some widgets (Progressbar for example). You can force Headgets not to use of Common Controls by commenting/removing **HDG_USE_COMMONCTRLS** macro on line 42.

**Visual C++**:

You can manually link Common Controls library to your project (comctl32.lib) through Project -> Properties: <Project Name> -> Configurations -> Linker -> Input -> Additional Dependecies. Also you need to add manifest value to your project.

**or**

you can make Headgets link it automatically using #pragma directives. Make sure that definition of  **HDG_PRAGMA_COMMONCTRLS** macro on line 47 is not commented, then Headgets will automatically link Common Controls (along with manifest addition):

```cpp
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment( lib, "comctl32.lib" )
```

**MinGW / GCC**:

You have to pass -l parameter to linker manually:

```
-lcomctl32
```

Furthermore, for using Common Controls version 6, you need to add manifest to your project.
[Nice guide on using Common Controls with GCC](http://geekthis.net/post/visual-styles-in-win32-api-c-gcc-mingw/)

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

### All available events

**hdg::Event** structure:
```cpp
struct Event {
  hdg::EventType type;

  int num1;
  int num2;

  hdg::MouseEvent mouse;

  handle handle;

  hdg::Application* app;
};
```

**hdg::EventType::Created**
* sent on window **creation**
* num1 is 0, num2 is 0
* handle is created window handle
* app is pointer to hdg::Application

**hdg::EventType::Closed**
* sent when Close (X) button is clicked
* num1 is 0, num2 is 0
* handle is window handle
* app is pointer to hdg::Application

**hdg::EventType::Destroyed**
* sent when window was **destroyed**
* num1 is 0, num2 is 0,
* handle is NULL
* app is pointer to hdg::Application

**hdg::EventType::Moved**
* sent when window is moved
* num1 is window X position, num2 is window Y position
* handle is window handle
* app is pointer to hdg::Application

**hdg::EventType::Resized**
* sent when window is resized
* num1 is new window width, num2 is new window height
* handle is window handle
* app is pointer to hdg::Application

**hdg::EventType::Command**
* event fired when any control sends a notification
* num1 is control ID, num2 is 0
* mouse field is hdg::MouseEvent::Nothing
* handle is window handle
* app is pointer to hdg::Application

Example on handling hdg::Button click event:

```cpp
hdg::Button button("My Button", 100, 100);

//Event callback
if (ev.type == hdg::EventType::Command) {
		if (ev.num1 == button.getID()) {
			//button was clicked, do something
		}
}


```

**hdg::EventType::MouseEvent**
* sent on mouse event (click or move)
* num1 is mouse X position, num2 is mouse Y position
* mouse field shows which button was pressed or released.
* handle is window handle
* app is pointer to hdg::Application

## Methods

```cpp
void hdg::Application::setUserCallback(std::function<void(hdg::Event)> func);
```
Sets event callback for Application.

```cpp
int hdg::Application::getX();
```
Returns current window X position

```cpp
int hdg::Application::getY();
```
Returns current window Y position

```cpp
int hdg::Application::getWidth();
```
Returns current window width

```cpp
int hdg::Application::getHeight();
```
Returns current window height

```cpp
void hdg::Application::moveTo(int newX, int newY);
```
Moves window to newX and newY position.

```cpp
void hdg::Application::moveBy(int dX, int dY);
```
Moves window by delta X and delta Y. Same as
moveTo(x+dX, y+dY);

```cpp
handle hdg::Application::getNativeHandle();
```
Returns native Win32 handle window handle

```cpp
bool hdg::Application::isOpen();
```
Returns if window is open or not

```cpp
void hdg::Application::setTitle(std::string title);
```
Sets window title

```cpp
void hdg::Application::close();
```
Closes the window

## Widgets

Widgets are different controls in the window: buttons, label, text boxes. Headgets has some of them.

To create a widget, you just need to initialize widget instance.

**Warning!** Widgets depend on hdg::Application instance, so you must create your hdg::Application **before** creating any widgets.

Example:

```cpp
hdg::Application app(hInstance, "Hello World", 800, 600);

hdg::Label label("Hello World!"); //that's all

app.run();
```

Every widget has 4 methods:

```cpp
void hdg::Widget::show();
```
Shows the widget

```cpp
void hdg::Application::hide();
```
Hides the widget

```cpp
void hdg::Widget::setPosition(int x, int y);
```
Sets widget position (relative to parent window)

```cpp
void hdg::Widget::setSize(int w, int h);
```
Sets widget size

## All available widgets:


### Label

Simple static label (text).

```cpp
hdg::Label(std::string text, int x = 0, int y = 0)
```
Constructor

```cpp
void hdg::Label::setText(std::string text);
```
Sets label text

### Button
Push button widget.

```cpp
hdg::Button(std::string text, int x = 0, int y = 0)
```
Constructor

---

```cpp
void hdg::Button::setText(std::string text);
```
Sets button text

```cpp
void hdg::Button::enable();
```
Enables the button

```cpp
void hdg::Button::disable();
```
Disables the button

```cpp
void hdg::Button::setDisabled(bool arg);
```
Sets button disabled state.

```cpp
int hdg::Button::getID();
```
Returns button control ID (used in Command event)

### Editbox

Editable textbox.

```cpp
hdg::Editbox(unsigned int style, int x, int y, int w, int h)
```
Constructor

You can omit all arguments in constructor:

```cpp
hdg::Editbox edit();
```

While specifying style argument you can set multiple styles using bitwise OR (|) operator:

```cpp
hdg::Editbox edit(hdg::EditboxStyle::Uppercase | hdg::EditboxStyle::Password);
```

Available styles (hdg::EditboxStyle::):

* None - default style
* Center - center-align text in editbox
* Left - left-align text
* Lowercase - all characters will be converted into lowercase
* Uppercase - all characters will be converted into uppercase
* Multiline - creates multiline editbox
* Number - only digits are allowed to be entered into editbox
* Password - replaces all characters with dots
* Right - right-aligns editbox text

---

```cpp
void hdg::Editbox::setText(std::string txt);
```

Sets editbox text.

```cpp
std::string hdg::Editbox::value();
```

Returns editbox value as std::string.

```cpp
void hdg::Editbox::setReadonly(bool arg);
```

Toggles readonly mode of editbox.


### Progressbar

Typical widget used to display progress of some action.

```cpp
hdg::Progressbar::Progressbar(bool isMarquee, int x=0, int y=0, int w=100, int h=14)
```

Constructor. If isMarquee is true, progress bar will show some type of activity but won't show progress of task.

---

```cpp
void hdg::Progressbar::setRange(int min, int max)
```
Sets progress bar range. Does nothing if marquee style is on.


```cpp
void hdg::Progressbar::setStep(int step)
```
Sets progress bar step. Default step value is 10.

```cpp
void hdg::Progressbar::step(int count=0)
```
Advances progress by count. If count is 0, step value is used.

```cpp
void hdg::Progressbar::toggleMarquee(bool mode, int time=30)
```
Toggles marquee mode. time determines how fast progress bar part should move across the bar (in milliseconds).

### Fonts

You can change widget text font using hdg::Font class.

```cpp
hdg::Font::Font(const std::string family, int weight=FontWeight::Default, int size = 0, bool italic=false)
```

Where:
* family is font family
* weight is weight of font. You can use predefined values from hdg::FontWeight enum.
* size - font size
* italic - should font be italic?

hdg::Font has setters for each of theese params plus 1 function for setting underline style:

```cpp
hdg::Font::setUnderline(bool arg);
```
---

Apply font to widget using setFont method:
```cpp
hdg::Wigdet::setFont(hdg::Font font);
```
Example:

```cpp
hdg::Label myLabel("Font test", 100, 100);
hdg::Font font("Arial", hdg::FontWeight::Bold, 18, true);
myLabel.setFont(font);
```
Will create bold Arial font with 18 characters' size and italic style and apply it to previously created myLabel Label widget.

## Utilites

### Show a message box

You can show a message box using next function:
```cpp
hdg::showMessageBox(std::string text, std::string title,
   hdg::MessageBoxType type,
   hdg::MessageBoxButtons buttons
 )
```

You can omit last 3 arguments, then message will use **"Information"** as title, **hdg::MessageBoxType::Information** as type and **hdg::MessageBoxButtons::Ok** as buttons.


### Get environment variable

You can get environment variable as std::string using:

```cpp
const std::string getEnvironmentVariable(const std::string var);
```

### File dialogs

**hdg::OpenDialog** and **hdg::SaveDialog** are helper classes for opening system "Save" or "Open" dialogs for browsing files.

Methods (apply for both Save and Open dialogs):

```cpp
void hdg::OpenDialog::setRawFilter(const char* str);
```
Sets file filter. Optional. str must end in double null-characters and each item must be separated by one null character:

Example: **Text files\0*.txt;*.rtf\0\0**

There are some built-in common filters in **hdg::FileFilters**:

* hdg::FileFilters::AllFiles - all files (default dialog filter)
* hdg::FileFilters::TextFiles - .txt files
* hdg::FileFilters::ImageFiles - .jpg, .png, .bmp files
* hdg::FileFilters::VideoFiles - .mp4, .avi
* hdg::FileFilters::AudioFiles - .mp3, .ogg, .wav, .flac
* hdg::FileFilters::ExecutableFiles - .exe
* hdg::FileFilters::DLLFiles - .dll

Example:

```cpp
hdg::OpenDialog dialog;
dialog.setRawFilter(hdg::FileFilters::ImageFiles);
dialog.setTitle("Select an image for uploading");

if (dialog.open()) {
	//you code goes here
	process_file_from_dialog(dialog.getFilename());
}

```
---

```cpp
void hdg::OpenDialog::setTitle(std::string title);
```
Sets dialog title. Optional

```cpp
void hdg::OpenDialog::setInitialPath(std::string dir);
```
Sets dialog initial path. Optional.

```cpp
bool hdg::OpenDialog::open()
```
Opens the file dialog and returns true if user successfully selected a file, false otherwise.

**Note:** calling all prepartion methods (setTitle, setInitialPath, etc) will affect dialog only if they were called **before** opening the dialog.

```cpp
const std::string hdg::OpenDialog::getFilename()
```
Returns filename, which user selected in a dialog.

## Disclaimer
If you are planning to create cross-platform applications with complex UI, I **highly** recommend using any popular and stable UI framework like [Qt](https://www.qt.io/), [wxWidgets](https://www.wxwidgets.org/) or [GTK](https://www.gtk.org/) instead of Headgets. This library was developed for personal use as an hobby project.

## License:
MIT (see Headgets.h file)
