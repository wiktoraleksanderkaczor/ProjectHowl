//Removing unnecessary headers to decrease compilation time.
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

//Including neccessary headers.
#include <windows.h>
#include <iostream>
#include <string>
#include <ShObjIdl.h>
#include "Dependencies\Scintilla.h"
#include "Dependencies\SciLexer.h"
#include </ActualProject/ProjectHowl/jagpdf/api.h>

//Ensuring correct version of the library is used.
#pragma comment(lib, "comctl32.lib")

//Visual Studio generated stuff.
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Including headers.
#include "global.h"
#include "main.h"
#include "editorFunction.h"
#include "fileIO.h"

using namespace jag;

//Handling messages and uncaught exceptions.
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_SIZE:
		getWindowSize();
		//The magic numbers 32, 16 and 39 are the adjustments for the window bars and toolbar. First one is y (height) position,
		//second is width while the third is height.
		SetWindowPos(hWndEdit, NULL, 0, 32, ((rect.right - rect.left) - 16), ((rect.bottom - rect.top) - 90), NULL);
		SetWindowPos(hToolbar, NULL, 0, 0, ((rect.right - rect.left) - 16), ((rect.bottom - rect.top) - 90), NULL);
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		newFile();
		break;
	case WM_COMMAND:
		//Handling commands.
		//TODO: Implement File IO.
		switch (LOWORD(wParam)) {
		case IDM_FILE_NEW:
			newFile();
			break;
		case IDM_FILE_OPEN:
			open();
			break;
		case IDM_FILE_SAVE:
			saveFile();
			break;
		case IDM_FILE_SAVEAS:
			saveFileAs();
			break;
		case IDM_FILE_QUIT:
			//Checks if unmodified changes are pending and gives the option to go back.
			if (isDocModified) {
				int result = MessageBox(hWnd, TEXT("Your modifications to the file have not been saved, are you sure you wish to exit?"), TEXT("ProjectHowl"), MB_YESNOCANCEL);
				switch (result)
				{
				case IDYES:
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				case IDNO:
					saveFileAs();
					break;
				case IDCANCEL:
					break;
				}
			}
			else {
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
			break;

		case IDM_EDIT_UNDO:
			SendMessage(hWndEdit, SCI_UNDO, 0, 0);
			break;
		case IDM_EDIT_REDO:
			SendMessage(hWndEdit, SCI_REDO, 0, 0);
			break;
		case IDM_EDIT_CUT:
			SendMessage(hWndEdit, WM_CUT, 0, 0);
			break;
		case IDM_EDIT_COPY:
			SendMessage(hWndEdit, WM_COPY, 0, 0);
			break;
		case IDM_EDIT_PASTE:
			SendMessage(hWndEdit, WM_PASTE, 0, 0);
			break;
		case IDM_EDIT_DELETE:
			SendMessage(hWndEdit, WM_CLEAR, 0, 0);
			break;
		case IDM_EDIT_FIND:
			break;
		case IDM_EDIT_REPLACE:
			break;
		case IDM_EDIT_SELECTALL:
			SendMessage(hWndEdit, SCI_SELECTALL, 0, 0);
			break;

		case IDM_SETTINGS_PREFERENCES:
			break;
		case IDM_SETTINGS_UPDATE:
			break;
		case IDM_SETTINGS_ABOUT:
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
	LoadLibrary(TEXT("jagpdf-1.4.dll"));

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
	SendMessage(hWndEdit, SCI_SETSCROLLWIDTHTRACKING, TRUE, 0);
	//SC_WRAP_NONE (0), SC_WRAP_WORD (1) or SC_WRAP_CHAR (2)
	SendMessage(hWndEdit, SCI_SETWRAPMODE, 1, 0);

	//Configuring editor.
	setGlobalStyle((LPARAM)"Times New Roman");

	//Setting focus to window.
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//Structure to hold message.
	MSG msg;
	pdf::Document doc(pdf::create_file("hello.pdf"));
	doc.page_start(597.6, 848.68);
	doc.page().canvas().text(50, 800, "Hello, world!");
	doc.page_end();
	doc.finalize();

	//Loop to handle events.
	while (GetMessage(&msg, NULL, 0, 0)) {
		//If control is pressed down:
		if (GetKeyState(VK_CONTROL) & 0x8000) {
			//New file:
			if (GetKeyState('N') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_FILE_NEW, 0);
			}
			//Open file:
			if (GetKeyState('O') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_FILE_OPEN, 0);
			}
			//Save file:
			if (GetKeyState('S') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_FILE_SAVE, 0);
			}
			//Quit program:
			if (GetKeyState('Q') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_FILE_QUIT, 0);
			}
			//Undo:
			if (GetKeyState('Z') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_UNDO, 0);
			}
			//Redo:
			if (GetKeyState('Y') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_REDO, 0);
			}
			//Cut:
			if (GetKeyState('X') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_CUT, 0);
			}
			//Copy:
			if (GetKeyState('C') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_COPY, 0);
			}
			//Paste:
			if (GetKeyState('V') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_PASTE, 0);
			}
			//Delete:
			if (GetKeyState(VK_DELETE) & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_DELETE, 0);
			}
			//Find:
			if (GetKeyState('F') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_FIND, 0);
			}
			//Replace:
			if (GetKeyState('H') & 0x8000) {
				SendMessage(hWnd, WM_COMMAND, IDM_EDIT_REPLACE, 0);
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return EXIT_SUCCESS;
}