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

HWND CreateScintillaEdit(HWND hWndOwner, int x, int y, int width, int height, HINSTANCE hInstance) {
	//Creating scintilla edit control
	HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Scintilla"), NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VSCROLL,
		x, y, width, height, hWndOwner, NULL, hInstance, NULL);

	return hWndEdit;
}

HWND CreateToolbar(HWND hWndOwner) {
	INITCOMMONCONTROLSEX ccsx;
	TBBUTTON tbb[3];
	TBADDBITMAP tbab;

	ccsx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ccsx.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&ccsx);

	HWND hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
		hWnd, (HMENU)102, GetModuleHandle(NULL), NULL);
	SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	tbab.hInst = HINST_COMMCTRL;
	tbab.nID = IDB_STD_SMALL_COLOR;
	SendMessage(hToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

	//Defining each toolbar button.
	ZeroMemory(tbb, sizeof(tbb));
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
	tbb[2].idCommand = IDM_HELP_ABOUT;

	//Updating toolbar with above specifications.
	SendMessage(hToolbar, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);

	return hToolbar;
}

