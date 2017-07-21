//Global variables
RECT rect;
HWND hWnd;
HWND hWndEdit;
HWND hToolbar;

//Declaring main functions.
void getWindowSize();
void AddMenus(HWND hWnd);
HWND CreateScintillaEdit(HWND hWndOwner, int x, int y, int width, int height, HINSTANCE hInstance);
HWND CreateToolbar(HWND hWndOwner);

//Declaring editor functions.
void setSelectColor(COLORREF color);
void setBlinkingCursorColor(COLORREF color);
void setTabWidth(int numberOfCharacters);
void setForegorundColor(COLORREF color);
void setBackgorundColor(COLORREF color);
void setGlobalStyle(LPARAM font);

//Defining params for menubar messages.
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define IDM_FILE_EXPORT 4
#define IDM_HELP_ABOUT 5
