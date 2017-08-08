//Global variables
RECT rect;
HWND hWnd;
HWND hWndEdit;
HWND hToolbar;
bool fileExists;
bool isDocModified;
char pathToFile[MAX_PATH];
const int sizeOfBlock = 128 * 1024;
char programName[20] = { "ProjectHowl" };

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

//Declaring file IO functions.
void openFile(char *fileName);
void newFile();
void saveFile();
void saveFileAs();
void open();
void setWindowTitle(bool isNew);
void writeToFile();

//Defining params for menubar messages.
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_SAVEAS 4
#define IDM_FILE_EXPORT 5
#define IDM_FILE_QUIT 6

#define IDM_EDIT_UNDO 7
#define IDM_EDIT_REDO 8
#define IDM_EDIT_CUT 9
#define IDM_EDIT_COPY 10
#define IDM_EDIT_PASTE 11
#define IDM_EDIT_DELETE 12
#define IDM_EDIT_FIND 13
#define IDM_EDIT_REPLACE 14
#define IDM_EDIT_SELECTALL 15

#define IDM_SETTINGS_PREFERENCES 16
#define IDM_SETTINGS_UPDATE 17
#define IDM_SETTINGS_ABOUT 18