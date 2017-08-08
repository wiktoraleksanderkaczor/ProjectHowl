#pragma once
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
	HMENU hEditMenu;
	HMENU hSettingsMenu;

	//Creating main menubar.
	hMenubar = CreateMenu();

	//Creating sub-menus.
	hFileMenu = CreateMenu();
	hEditMenu = CreateMenu();
	hSettingsMenu = CreateMenu();

	//Creating command buttons in file menu.
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_SAVEAS, L"&Save As..");
	AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_EXPORT, L"&Export...");
	AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

	//Creating command buttons in edit menu.
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_UNDO, L"&Undo");
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_REDO, L"&Redo");
	AppendMenuW(hEditMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_CUT, L"&Cut");
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_COPY, L"&Copy");
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_PASTE, L"&Paste");
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_DELETE, L"&Delete");
	AppendMenuW(hEditMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_FIND, L"&Find");
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_REPLACE, L"&Replace");
	AppendMenuW(hEditMenu, MF_STRING, IDM_EDIT_SELECTALL, L"&Select All");

	//Creating command buttons in settings menu.
	AppendMenuW(hSettingsMenu, MF_STRING, IDM_SETTINGS_PREFERENCES, L"&Preferences");
	AppendMenuW(hSettingsMenu, MF_STRING, IDM_SETTINGS_UPDATE, L"&Update");
	AppendMenuW(hSettingsMenu, MF_STRING, IDM_SETTINGS_ABOUT, L"&About");

	//Filling menubar.
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hEditMenu, L"&Edit");
	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hSettingsMenu, L"&Settings");

	//Setting hWnd to use the menubar.
	SetMenu(hWnd, hMenubar);
}

HWND CreateScintillaEdit(HWND hWndOwner, int x, int y, int width, int height, HINSTANCE hInstance) {
	//Creating scintilla edit control
	HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Scintilla"), NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VSCROLL,
		x, y, width, height, hWndOwner, NULL, hInstance, NULL);

	return hWndEdit;
}

HWND CreateToolbar(HWND hWndOwner) {
	//Declaring init for common controls and toolbar stuff.
	INITCOMMONCONTROLSEX ccsx;
	TBBUTTON tbb[3];
	TBADDBITMAP tbab;

	//Registering common control classes.
	ccsx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ccsx.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&ccsx);

	//Creating toolbar.
	HWND hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
		hWnd, (HMENU)102, GetModuleHandle(NULL), NULL);
	//Specifying size of TBBUTTON structure.
	SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	//Handle to executable containing bitmaps. In this case system-defined bitmaps.
	tbab.hInst = HINST_COMMCTRL;
	//Setting bitmaps to system-defined (large, with color).
	tbab.nID = IDB_STD_LARGE_COLOR;
	//Adding system-defined bitmaps to toolbar.
	SendMessage(hToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

	//Fill memory with zeros.
	ZeroMemory(tbb, sizeof(tbb));
	//Defining each toolbar button.
	tbb[0].iBitmap = STD_FILENEW;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].idCommand = IDM_FILE_NEW;

	tbb[1].iBitmap = STD_FILEOPEN;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].idCommand = IDM_FILE_OPEN;

	tbb[2].iBitmap = STD_FILESAVE;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;
	tbb[2].idCommand = IDM_FILE_SAVE;

	//Updating toolbar with above specifications.
	SendMessage(hToolbar, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);

	return hToolbar;
}