//Removing unnecessary headers to decrease compilation time.
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

//Including neccessary headers.
#include <windows.h>
#include <iostream>
#include <Commctrl.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Global variables
RECT rect;
HWND hWnd;
HWND hWndEdit;

//Declaring used methods.
void getWindowSize();
void AddMenus(HWND hWnd);
HWND CreateRichEdit(HWND hWndOwner, int x, int y, int width, int height, HINSTANCE hInstance);

//Defining params for menubar messages.
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define IDM_FILE_EXPORT 4
#define IDM_HELP_ABOUT 5

//Handling uncaught exceptions.
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_SIZE:
		getWindowSize();
		//The magic numbers 16 and 39 are the adjustments for the window bars. First one is width while the second is height.
		SetWindowPos(hWndEdit, NULL, 0, 0, ((rect.right - rect.left) - 16), ((rect.bottom - rect.top) - 39), NULL);
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		break;
	case WM_COMMAND:
		//Handling menu commands.
		//TODO: Implement File IO.
		switch (LOWORD(wParam)) {
		case IDM_FILE_NEW:
			break;
		case IDM_FILE_OPEN:
			break;
		case IDM_FILE_EXPORT:
			break;
		case IDM_FILE_QUIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_HELP_ABOUT:
			MessageBox(hWnd, TEXT("This program was made by Wiktor Kaczor."), TEXT("About"), MB_OK);
			break;
		}
		//End of handling menu commands.
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return FALSE;
}

//The main entry point for the program.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) 
{
	//Suppressing warnings.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdLine);

	//Loading dependencies.
	LoadLibrary(TEXT("Msftedit.dll"));
	LoadLibrary(TEXT("Riched32.dll"));
	LoadLibrary(TEXT("Riched20.dll"));

	//Setting title and class of main window.
	LPTSTR windowClass = TEXT("ProjectHowlApp");
	LPTSTR windowTitle = TEXT("ProjectHowl");
	
	//Creating and setting window attributes for register.
	WNDCLASSEX wcex;
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = windowClass;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	//Registering the window. Will exit gracefully if failed.
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, TEXT("RegisterClassEx Failed!"), TEXT("Error"),
			MB_ICONERROR);
		return EXIT_FAILURE;
	}

	//Creating main window and exiting gracefully if exceptions occur.
	if (!(hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hInstance, NULL)))
	{
		MessageBox(NULL, TEXT("CreateWindow Failed!"), TEXT("Error"), MB_ICONERROR);
		return EXIT_FAILURE;
	}

	getWindowSize();
	
	//Creating rich text editor window.
	hWndEdit = CreateRichEdit(hWnd, 0, 0, 0, 0, hInstance);

	//Setting focus to window.
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//Structure to hold message.
	MSG msg;

	//Loop to handle events.
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return EXIT_SUCCESS;
}

void getWindowSize() {
	//Get size of the window called hWnd and store in RECT rect.
	if (GetWindowRect(hWnd, &rect) == false) {
		std::cout << "Error getting the windows size.\n";
	}
}

void AddMenus(HWND hWnd) {
	//Declaring all menus.
	HMENU hMenubar;
	HMENU hFileMenu;
	HMENU hHelpMenu;

	//Creating main menubar.
	hMenubar = CreateMenu();

	//Creating sub-menus.
	hFileMenu = CreateMenu();
	hHelpMenu = CreateMenu();

	//Creating command buttons in file menu.
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_EXPORT, L"&Export...");
	AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	//Creating command buttons in help menu.
	AppendMenuW(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, L"&About");

	//Filling menubar.
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hHelpMenu, L"&Help");

	//Setting hWnd to use the menubar.
	SetMenu(hWnd, hMenubar);
}

HWND CreateRichEdit(HWND hWndOwner, int x, int y, int width, int height, HINSTANCE hInstance) {
	//Creating edit control
	//Alternatives to ES_CENTER are ES_LEFT and ES_RIGHT.
	HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("RICHEDIT"), TEXT("Start Writing..."),
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VSCROLL | ES_CENTER,
		x, y, width, height, hWndOwner, NULL, hInstance, NULL);

	return hWndEdit;
}