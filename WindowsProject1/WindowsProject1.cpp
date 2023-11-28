// WindowsProject1.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "comctl32.lib")
#include "framework.h"
#include "WindowsProject1.h"
#include "ShObjIdl.h"
#include <string>
#include <vector>
#include <windows.h>
#include <commctrl.h>
#include <filesystem>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    editProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    search(HWND, UINT, WPARAM, LPARAM);
WCHAR*              selectFolder(HWND);
//std::vector<std::wstring> searchFile(std::wstring, WCHAR*);
std::vector<std::wstring> searchFile2(std::wstring path, WCHAR* searchText);
std::wstring        joinw(std::vector<std::wstring>);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hEdit;

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_SEARCH:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCHBOX), hEdit, search);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
	    {
            RECT rect{};
            GetWindowRect(hWnd, (LPRECT)&rect);
            hEdit = CreateWindowEx(0, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE,
                                    0, 0, rect.right - rect.left, rect.bottom - rect.top, hWnd, NULL, hInst, NULL);
            HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, L"Yu Gothic UI");
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));
            SetWindowSubclass(hEdit, editProc, 0, 0);
	    }
        break;
    case WM_KEYDOWN:
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
        {
            RECT rect{};
            GetWindowRect(hWnd, (LPRECT)&rect);
            SetWindowPos(hEdit, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK editProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	switch (uMsg)
	{
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case ID_ACCEL_SELECTALL:
                SendMessage(hWnd, EM_SETSEL, 0, -1);
                break;
            case ID_ACCEL_SAVE:
                SendMessage(hWnd, EM_SETSEL, 0, -1);
                break;
            default:
                break;
            }
        }
        break;
	default:
		break;
	}
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK search(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_SEARCH:
        {
            WCHAR* ppath = selectFolder(hDlg);

            if (ppath != nullptr) {
                HWND hEdit = GetWindow(GetParent(hDlg), GW_CHILD);
                WCHAR searchText[256]{};

                GetDlgItemText(hDlg, IDC_TEXTBOX, searchText, sizeof(searchText));
                std::vector<std::wstring> files = searchFile2(ppath, searchText);
                CoTaskMemFree(ppath);

                ShowWindow(GetDlgItem(hDlg, IDC_TEXT1), SW_SHOW);
                SetWindowTextW(GetDlgItem(hDlg, IDC_TEXT1), (L"done (file count:" + std::to_wstring(files.size()) + L")").c_str());
                SetWindowTextW(hEdit, !files.empty() ? joinw(files).c_str() : L"not found");
                files.clear();
                files.shrink_to_fit();

                //HWND hProg = GetDlgItem(hDlg, IDC_PROGRESSBAR);
                //SendMessage(hProg, PBM_SETRANGE, (WPARAM)0, MAKELPARAM(0, files.size()));
                //SendMessage(hProg, PBM_SETSTEP, (WPARAM)1, 0);
                //SendMessage(hProg, PBM_SETPOS, 0, 0);
            }
        }
            break;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;
        default:
            break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

WCHAR* selectFolder(HWND hWnd) {
    IFileOpenDialog* pFileOpen = nullptr;
    WCHAR *pszPath = nullptr;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr)) {
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr)) {
            DWORD options = 0;

            pFileOpen->GetOptions(&options);
            pFileOpen->SetOptions(options | FOS_PICKFOLDERS);

            hr = pFileOpen->Show(hWnd);

            if (SUCCEEDED(hr)) {
                IShellItem* pItem = nullptr;
                hr = pFileOpen->GetResult(&pItem);

                if (SUCCEEDED(hr)) {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
                    pItem->Release();
                }

                pFileOpen->Release();
            }
        }

        CoUninitialize();
    }

    return pszPath;
}

std::vector<std::wstring> searchFile2(std::wstring path, WCHAR* searchText) {
    std::vector<std::wstring> files;

    for (const auto& file : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(path)) {
        if (!file.is_directory() && (searchText[0] == NULL || wcsstr(file.path().filename().c_str(), searchText) != nullptr)) {
            files.push_back(file.path().c_str());
        }
    }

    return files;
}

std::wstring joinw(std::vector<std::wstring> vec) {
    std::wstring str;

    for (std::wstring var : vec) {
        str += var + L"\r\n";
    }

    return str;
}
/*
std::vector<std::wstring> searchFile(std::wstring path, WCHAR* searchText) {
    WIN32_FIND_DATA w32fd;
    std::vector<std::wstring> files;
    std::wstring targetPath = path + L"\\*";
    HANDLE hFile = FindFirstFile(targetPath.c_str(), &w32fd);

    if (hFile != INVALID_HANDLE_VALUE) {
        do {
            WCHAR* pfileName = w32fd.cFileName;
            std::wstring newpath = path + L"\\" + pfileName;

            if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if ((pfileName[0] == L'.' && pfileName[1] == NULL) || (pfileName[0] == L'.' && pfileName[1] == L'.' && pfileName[2] == NULL)) continue;
                std::vector<std::wstring> newVec = searchFile(newpath, searchText);

                files.insert(files.end(), newVec.begin(), newVec.end());
                newVec.clear();
                newVec.shrink_to_fit();
            }
            else {
                if (searchText[0] == NULL || wcsstr(pfileName, searchText) != nullptr) {
                    files.push_back(newpath.c_str());
                }
            }

            newpath.clear();
            newpath.shrink_to_fit();
        } while (FindNextFile(hFile, &w32fd));
    }

    FindClose(hFile);
    targetPath.clear();
    targetPath.shrink_to_fit();

    return files;
}
*/