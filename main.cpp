//Removing unnecessary headers to decrease compilation time.
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

//Including neccessary headers.
#include <windows.h>
#include <iostream>
#include <Commctrl.h>
#include </ActualProject/ProjectHowl/include/Scintilla.h>
#include </ActualProject/ProjectHowl/include/SciLexer.h>

//Ensuring correct version of the library is used.
#pragma comment(lib, "comctl32.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Including the global declarations.
#include <\ActualProject\ProjectHowl\GlobalDeclarations.h>

//Including function headers.
#include <\ActualProject\ProjectHowl\MainFunctionsDefinition.h>
#include <\ActualProject\ProjectHowl\EditorConfigFunctions.h>

//Handling messages and uncaught exceptions.
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_SIZE:
		getWindowSize();
		//The magic numbers 30, 16 and 39 are the adjustments for the window bars and toolbar. First one is y (height) position,
		//second is width while the third is height.
		SetWindowPos(hWndEdit, NULL, 0, 30, ((rect.right - rect.left) - 16), ((rect.bottom - rect.top) - 39), NULL);
		SetWindowPos(hToolbar, NULL, 0, 0, ((rect.right - rect.left) - 16), ((rect.bottom - rect.top) - 39), NULL);
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
	LoadLibrary(TEXT("SciLexer.dll"));
	LoadLibrary(TEXT("Scintilla.dll"));

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
	if (!(hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hInstance, NULL)))
	{
		MessageBox(NULL, TEXT("CreateWindow Failed!"), TEXT("Error"), MB_ICONERROR);
		return EXIT_FAILURE;
	}

	getWindowSize();
	
	//Creating rich text editor window and toolbar.
	hWndEdit = CreateScintillaEdit(hWnd, 0, 0, 0, 0, hInstance);
	hToolbar = CreateToolbar(hWnd);

	//Configuring editor.
	setGlobalStyle((LPARAM)"Times New Roman");

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