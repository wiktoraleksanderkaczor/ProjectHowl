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
	fileExists = false;
}

//Opens a file in the editor.
void openFile(char *fileName) {
	newFile();

	SendMessage(hWndEdit, SCI_CANCEL, 0, 0);
	SendMessage(hWndEdit, SCI_SETUNDOCOLLECTION, 0, 0);

	strcpy_s(pathToFile, fileName);
	FILE *fp = fopen(pathToFile, "rb");
	if (fp) {
		char data[sizeOfBlock];
		int lenFile = fread(data, 1, sizeof(data), fp);
		while (lenFile > 0) {
			SendMessage(hWndEdit, SCI_ADDTEXT, lenFile,
				reinterpret_cast<LPARAM>(static_cast<char *>(data)));
			lenFile = fread(data, 1, sizeof(data), fp);
		}
		fclose(fp);
		fileExists = true;
	}
	else {
		MessageBox(hWnd, TEXT("ProjectHowl couldn't open the specified file"), TEXT("ProjectHowl"), MB_OK);
	}
	SendMessage(hWndEdit, SCI_SETUNDOCOLLECTION, 1, 0);
	SetFocus(hWndEdit);
	SendMessage(hWndEdit, EM_EMPTYUNDOBUFFER, 0, 0);
	SendMessage(hWndEdit, SCI_SETSAVEPOINT, 0, 0);
	SendMessage(hWndEdit, SCI_GOTOPOS, 0, 0);
}

//Saves the file if it already exists, else it calls saveFileAs();.
void saveFile() {
	if (fileExists == true) {
		isDocModified = false;
		FILE *fp = fopen(pathToFile, "wb");
		if (fp) {
			//This will save "asd" correctly.
			int length = SendMessage(hWndEdit, SCI_GETLENGTH, 0, 0);
			char text[4];
			SendMessage(hWndEdit, SCI_GETTEXT, 4, (LPARAM)&text);
			fwrite(text, sizeof(char), sizeof(text), fp);
		}
		else {
			//Error message
		}
		fclose(fp);
		SendMessage(hWndEdit, SCI_SETSAVEPOINT, 0, 0);
	}
	else {
		saveFileAs();
		isDocModified = false;
	}
}

//Saves the file to a user determined location.
void saveFileAs() {

}

//Function to get the file name and call the openFile() method.
void open() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						WideCharToMultiByte(CP_ACP, 0, pszFilePath, -1, convertedPath, sizeof(convertedPath), NULL, NULL);
						openFile(convertedPath);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
}

void setWindowTitle(char* title) {

}