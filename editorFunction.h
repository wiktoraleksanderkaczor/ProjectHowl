#pragma once
//NOTE: COLORREF is RGB().

//Sets the select highlight color. 
void setSelectColor(COLORREF color) {
	SendMessage(hWndEdit, SCI_SETSELBACK, TRUE, color);
}

//Sets the blinking cursor color.
void setBlinkingCursorColor(COLORREF color) {
	SendMessage(hWndEdit, SCI_SETCARETFORE, color, 0);
}

//Sets the tab width in characters.
void setTabWidth(int numberOfCharacters) {
	SendMessage(hWndEdit, SCI_SETTABWIDTH, numberOfCharacters, 0);
}

//Sets foreground color.
void setForegorundColor(COLORREF color) {
	SendMessage(hWndEdit, SCI_STYLESETFORE, STYLE_DEFAULT, (LPARAM)color);
}

//Sets background color.
void setBackgorundColor(COLORREF color) {
	SendMessage(hWndEdit, SCI_STYLESETBACK, STYLE_DEFAULT, (LPARAM)color);
}

//Sets the font.
void setGlobalStyle(LPARAM font) {
	SendMessage(hWndEdit, SCI_STYLESETFONT, STYLE_DEFAULT, font);
}

