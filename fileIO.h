#pragma once
//Makes a new file.
void newFile() {
	//Clears the text and undo history.
	SendMessage(hWndEdit, SCI_CLEARALL, 0, 0);
	SendMessage(hWndEdit, SCI_EMPTYUNDOBUFFER, 0, 0);
	//Says that current open document is unmodified.
	SendMessage(hWndEdit, SCI_SETSAVEPOINT, 0, 0);
	isDocModified = false;
	//Makes the path to file terminate on the first character.
	pathToFile[0] = '\n';
}

//Opens a file in the editor.
void openFile(char *fileName) {
	
}

//Saves the file if it already exists, else it calls saveFileAs();.
void saveFile() {
	if (fileExists == true) {

	}
	else {
		saveFileAs();
	}
}

//Saves the file to a user determined location.
void saveFileAs() {

}
