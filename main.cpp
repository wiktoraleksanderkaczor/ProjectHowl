#include <windows.h>
#include <iostream>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Global variables
RECT rect;
HWND hWnd;
HWND hWndEdit;

//Declaring used methods.
void getWindowSize();

//Handling uncaught exceptions.
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SIZE) {
		getWindowSize();
		//The magic numbers 16 and 39 are the adjustments for the window bars.
		SetWindowPos(hWndEdit, NULL, 0, 0, ((rect.right - rect.left) - 16), ((rect.bottom - rect.top) - 39), NULL);
	}

	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return FALSE;
}

//The main entry point for the program.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR nCmdLine, int nCmdShow)
{
	LPTSTR windowClass = TEXT("ProjectHowlApp");
	LPTSTR windowTitle = TEXT("ProjectHowl");
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
	if (!RegisterClassEx(&wcex))
	{
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

	//Creating inner editing window.
	hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("Start Writing..."),
		WS_CHILD | WS_VISIBLE | ES_WANTRETURN, 0, 0, 0, 0, hWnd, NULL, NULL, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//Structure to hold message.
	MSG msg;

	//Loop to handle events.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return EXIT_SUCCESS;
}

void getWindowSize() {
	if (GetWindowRect(hWnd, &rect) == false) {
		std::cout << "Error getting the windows size.\n";
	}
}
