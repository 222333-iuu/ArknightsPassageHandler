
// 剧情处理Dlg.cpp: 实现文件
//
#include <shellapi.h>
#include <functional>
#include <atlcomcli.h> 
#include "pch.h"
#include "framework.h"
#include "剧情处理.h"
#include "剧情处理Dlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <shlguid.h>
#include <atlbase.h>
#include <atlconv.h>
#include <vector>
#include <wininet.h>
#include "WebView2.h"
#include <WebView2.h>
#include <wrl.h>
#include <future>

#pragma comment(lib, "wininet.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable : 4996)

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// C剧情处理Dlg 对话框



C剧情处理Dlg::C剧情处理Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
	, m_url(_T(""))
	, m_adddescription(_T(""))
	, m_text(_T(""))
	, linenum(_T(""))
	, Cururl(_T(""))
	, cururl(_T(""))
	, m_quetext(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C剧情处理Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_webbrowser);
	DDX_Text(pDX, IDC_EDIT1, m_url);
	DDX_Control(pDX, IDC_RADIO1, m_needend);
	DDX_Text(pDX, IDC_EDIT2, m_adddescription);
	DDX_Control(pDX, IDC_RADIO3, m_isjuqing);
	DDX_Control(pDX, IDC_CHECK1, m_boxnotice);
	DDX_Text(pDX, IDC_EDIT3, m_text);
	DDX_Control(pDX, IDC_EDIT3, m_readfile);
	DDX_Control(pDX, IDC_RADIO6, m_res_file);
	DDX_Control(pDX, IDC_RADIO5, m_res_web);
	DDX_Control(pDX, IDC_TEXT, m_intro);
	DDX_Text(pDX, IDC_LINENUM, linenum);
	DDX_Control(pDX, IDC_CHECK2, m_issource);
	DDX_Text(pDX, IDC_CurTEXT, Cururl);
	DDX_Control(pDX, IDC_CurTEXT, m_cururl);
	DDX_Text(pDX, IDC_EDIT4, cururl);
	DDX_Control(pDX, IDC_COMBO1, m_todolist);
	DDX_Control(pDX, IDC_CHECK3, autonext);
	DDX_Control(pDX, IDC_EDIT5, m_que);
	DDX_Control(pDX, IDC_BUTTON7, m_query);
	DDX_Control(pDX, IDC_BUTTON8, m_goto);
	DDX_Text(pDX, IDC_EDIT5, m_quetext);
	DDX_Control(pDX, IDC_CHECK4, m_contcopy);
}

BEGIN_MESSAGE_MAP(C剧情处理Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_DROPFILES()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C剧情处理Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C剧情处理Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &C剧情处理Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON3, &C剧情处理Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO3, &C剧情处理Dlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON4, &C剧情处理Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C剧情处理Dlg::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT3, &C剧情处理Dlg::OnEnChangeEdit3)
	ON_STN_CLICKED(IDC_LINENUM, &C剧情处理Dlg::OnStnClickedLinenum)
	ON_STN_CLICKED(IDC_CurTEXT, &C剧情处理Dlg::OnStnClickedCurtext)
	ON_BN_CLICKED(IDC_BUTTON6, &C剧情处理Dlg::OnBnClickedButton6)
	ON_MESSAGE(WM_FETCH_COMPLETE, OnFetchComplete)
	ON_MESSAGE(WM_SELECTION_RESULT, &C剧情处理Dlg::OnSelectionResult)
	ON_EN_CHANGE(IDC_EDIT5, &C剧情处理Dlg::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON7, &C剧情处理Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C剧情处理Dlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// C剧情处理Dlg 消息处理程序

BOOL C剧情处理Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 初始化 COM
	CoInitialize(NULL);
	if (m_webViewHwnd) {
		::SetWindowPos(
			m_webViewHwnd,       // WebView2的窗口句柄
			HWND_TOPMOST,        // 置于所有窗口之上
			0, 0, 0, 0,          // 忽略位置和大小调整
			SWP_NOMOVE | SWP_NOSIZE // 保持现有位置和大小
		);
	}
	// 传入窗口句柄
	InitWebView2(this->GetSafeHwnd());
	if (m_webViewHwnd) {
		::SetWindowPos(
			m_webViewHwnd,       // WebView2的窗口句柄
			HWND_TOPMOST,        // 置于所有窗口之上
			0, 0, 0, 0,          // 忽略位置和大小调整
			SWP_NOMOVE | SWP_NOSIZE // 保持现有位置和大小
		);
	}
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	clear(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C剧情处理Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C剧情处理Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C剧情处理Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C剧情处理Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString url = m_url, rurl;
	if (m_needend.GetCheck()) {
		int index = url.Find(TEXT(" 行动前"));
		if (index != -1) {
			url = url.Left(index);
			url += TEXT("/BEG");
		}
		else {
			index = url.Find(TEXT(" 行动后"));
			if (index != -1) {
				url = url.Left(index);
				url += TEXT("/END");
			}
			else url += TEXT("/NBT");
		}
		index = url.Find(TEXT(" "));
		url.Replace(TEXT(" "), TEXT("_"));
	}
	rurl = TEXT("https://prts.wiki/index.php?title=");
	rurl += url;
	rurl += TEXT("&action=edit");
	if(!m_issource.GetCheck())NavigateToURL((LPCWSTR)rurl);
	cururl = rurl;
	UpdateData(false);
	if (m_issource.GetCheck()) GetSourceCode(rurl);

}

void C剧情处理Dlg::OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	if (!pDisp)
		return;

	CComQIPtr<IWebBrowser2> spWebBrowser = pDisp;
	if (spWebBrowser)
	{
		CComPtr<IDispatch> spDisp;
		HRESULT hr = spWebBrowser->get_Document(&spDisp);
		if (SUCCEEDED(hr) && spDisp)
		{
			hr = spDisp->QueryInterface(&m_pDocument2);
		}
	}
}

CString C剧情处理Dlg::GetSelectedText() {
	GetSelectedTextAsync();
	return SelectedWord;
}

void C剧情处理Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString result;
	if (m_res_web.GetCheck()) GetSelectedText();
	else {
		UpdateData(true), result = m_text;
		HANDLEER hand(result);
		WriteCopyBoard(hand.afterhand);
		CString sendmessage = m_url;
		char len[10];
		sprintf(len, "%d", hand.linenum);
		sendmessage += len;
		sendmessage += TEXT("\n剧情处理完成，已复制到剪贴板");
		if (m_boxnotice.GetCheck()) MessageBox(sendmessage);
		OnBnClickedButton5();
	}
}

BEGIN_EVENTSINK_MAP(C剧情处理Dlg, CDialogEx)
	ON_EVENT(C剧情处理Dlg, IDC_EXPLORER1, 259, C剧情处理Dlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


void C剧情处理Dlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: 在此处添加消息处理程序代码
	OnDocumentComplete(pDisp, URL);

}

void C剧情处理Dlg::SetIEZoomPercent(int nZoomPercent)
{
	CRegKey regKey;
	LONG lResult = regKey.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Internet Explorer\\Zoom"));
	if (lResult == ERROR_SUCCESS)
	{
		regKey.SetDWORDValue(_T("ZoomFactor"), nZoomPercent);
	}
}

void C剧情处理Dlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}

CString C剧情处理Dlg::ReadCopyBoard() {
	CString displayData;
	if (!IsClipboardFormatAvailable(CF_TEXT))return displayData;
	if (!OpenClipboard())return displayData;
	HGLOBAL hglb = GetClipboardData(CF_TEXT);
	if (hglb != NULL)
	{
		char* lptstr = (char*)GlobalLock(hglb);
		if (lptstr != NULL)
		{
			displayData = CString(lptstr);
			GlobalUnlock(hglb);
		}
	}
	CloseClipboard();
	return displayData;

}

void C剧情处理Dlg::WriteCopyBoard(CString strData) {
	if (m_contcopy.GetCheck()) {
		strData = ReadCopyBoard() +TEXT("\n") + m_url + TEXT("\n") + strData;
	}
	int len = strData.GetLength();

	if (len <= 0) return;
	if (!OpenClipboard()) return; // 传入NULL表示使用当前窗口的剪贴板  
	EmptyClipboard();

	// 为Unicode字符串分配内存（注意使用宽字符类型）  
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t));
	if (hglbCopy == NULL)
	{
		CloseClipboard();
		return;
	}

	// 使用宽字符指针  
	wchar_t* lpwstrCopy = (wchar_t*)GlobalLock(hglbCopy);
	// 直接从CString复制到宽字符指针（无需转换）  
	wcscpy(lpwstrCopy, strData.GetString());
	GlobalUnlock(hglbCopy);

	// 设置剪贴板数据为Unicode文本  
	SetClipboardData(CF_UNICODETEXT, hglbCopy);
	CloseClipboard();
}

void C剧情处理Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_url = ReadCopyBoard();
	UpdateData(false);
	OnBnClickedButton1();
}

void C剧情处理Dlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
}

void C剧情处理Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString word = TEXT("文本源自明日方舟WIKI （如果有问题请在下方评论区提醒，会尽快改正）\n");
	UpdateData(true);
	word += m_adddescription;
	if (m_isjuqing.GetCheck()) word += TEXT("\n可通过该文集查看更多主线及活动剧情\n");
	else word += TEXT("\n可通过该文集查看更多干员密录\n");
	word += TEXT("==============================\n");
	WriteCopyBoard(word);
}

void C剧情处理Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	clear(1);
}

void C剧情处理Dlg::OnDropFiles(HDROP hDropInfo)
{
	CDialogEx::OnDropFiles(hDropInfo);

	UINT nFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	if (nFileCount > 0)
	{
		CString strFilePath;
		LPTSTR filePath = strFilePath.GetBuffer(MAX_PATH);
		DragQueryFile(hDropInfo, 0, filePath, MAX_PATH);
		strFilePath.ReleaseBuffer();

		CFile file;
		if (file.Open(filePath, CFile::modeRead))
		{
			DWORD dwFileLength = static_cast<DWORD>(file.GetLength());
			std::vector<char> fileContent(dwFileLength + 1);
			file.Read(&fileContent[0], dwFileLength);
			fileContent[dwFileLength] = '\0'; // 确保以null结尾  

			// 假设文件是UTF-8编码的  
			int len = MultiByteToWideChar(CP_UTF8, 0, &fileContent[0], -1, NULL, 0);
			std::vector<wchar_t> wideString(len + 1);
			MultiByteToWideChar(CP_UTF8, 0, &fileContent[0], -1, &wideString[0], len);
			wideString[len] = L'\0'; // 确保以null结尾  

			// 在编辑控件中显示内容  
			m_text = m_text + TEXT("\n\n") + strFilePath + TEXT("\n") + wideString.data();
			linenum.Format(_T("字数:%d"), m_text.GetLength());
			UpdateData(false);
			file.Close();
		}

		m_readfile.ShowWindow(true);
		m_res_file.EnableWindow(true);
		m_res_web.SetCheck(false);
		m_res_file.SetCheck(true);
		m_intro.ShowWindow(true);
	}

	DragFinish(hDropInfo);
}



void C剧情处理Dlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void C剧情处理Dlg::OnStnClickedLinenum()
{
	// TODO: 在此添加控件通知处理程序代码
}

void C剧情处理Dlg::GetSourceCode(CString url) {
	CString strResult = GetSourceCodeSync(url);
	WriteCopyBoard(strResult);
	CString a = TEXT("<textarea ") , b = TEXT("</textarea>");
	int nStartA = strResult.Find(a);
	if (nStartA == -1) {
		MessageBox(TEXT("不存在可用的文本，请检查网页内容！"));
		return;
	}

	// 计算实际截取起始位置（a之后）
	int nStartPos = nStartA + a.GetLength();
	if (nStartPos >= strResult.GetLength()) {
		MessageBox(TEXT("不存在可用的文本，请检查网页内容！"));
		return;
	}

	// 查找结束位置b（从nStartPos开始）
	int nEndB = strResult.Find(b, nStartPos);
	if (nEndB == -1) {
		MessageBox(TEXT("不存在可用的文本，请检查网页内容！"));
		return;
	}
	CString result = strResult.Mid(nStartPos, nEndB - nStartPos);
	HANDLEER hand(result);
	WriteCopyBoard(hand.afterhand);
	CString sendmessage = m_url;
	char len[10];
	sprintf(len, "%d", hand.linenum);
	sendmessage += len;
	sendmessage += TEXT("\n剧情处理完成，已复制到剪贴板");
	if (m_boxnotice.GetCheck()) MessageBox(sendmessage);
	OnBnClickedButton5();
}



void C剧情处理Dlg::OnStnClickedCurtext()
{
	// TODO: 在此添加控件通知处理程序代码
}

void C剧情处理Dlg::InitWebView2(HWND hwnd) {
	CreateCoreWebView2EnvironmentWithOptions(
		nullptr, nullptr, nullptr,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[this, hwnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
				if (env) {
					env->CreateCoreWebView2Controller(hwnd,
						Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
							[this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
								if (controller) {
									this->webViewController = controller;
									controller->get_CoreWebView2(&(this->webView));
									controller->get_ParentWindow(&m_webViewHwnd);
									// 设置 WebView2 的大小
									RECT bounds = {
										0,   // 左边距50像素
										250,   // 上边距50像素
										2000,  // 右边距（50+800）
										1500   // 下边距（50+600）
									};
									controller->put_Bounds(bounds);
									// 加载网页
									this->webView->Navigate(L"https://prts.wiki/w/%E5%89%A7%E6%83%85%E4%B8%80%E8%A7%88");
								}
								return S_OK;
							}).Get());
				}
				return S_OK;
			}).Get());
}

void C剧情处理Dlg::NavigateToURL(const std::wstring& url) {
	if (webView) {
		// 转换为 LPCWSTR 并导航
		webView->Navigate(url.c_str());
	}
	else {
		AfxMessageBox(L"WebView2 未初始化！");
	}
}

void C剧情处理Dlg::clear(int mode) {
	if(mode) NavigateToURL(L"https://prts.wiki/w/%E5%89%A7%E6%83%85%E4%B8%80%E8%A7%88");
	m_needend.SetCheck(true);
	m_isjuqing.SetCheck(true);
	m_boxnotice.SetCheck(true);
	DragAcceptFiles(true);
	m_readfile.ShowWindow(false);
	m_res_web.SetCheck(true);
	m_res_file.EnableWindow(false);
	m_res_file.SetCheck(false);
	m_intro.ShowWindow(true);
	m_issource.SetCheck(true);
	m_url = TEXT("");
	m_adddescription = TEXT("");
	linenum = TEXT("字数:0");
	m_text = TEXT("");
	autonext.SetCheck(true);
	UpdateData(false);
}



void C剧情处理Dlg::GetSelectedTextAsync()
{
	if (webViewController == nullptr)
		return;

	wil::com_ptr<ICoreWebView2> coreWebView;
	if (FAILED(webViewController->get_CoreWebView2(&coreWebView)) || !coreWebView)
		return;

	// 执行 JavaScript 获取选中的文本（注意是异步）
	coreWebView->ExecuteScript(
		L"window.getSelection().toString()",
		Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
			[this](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT
			{
				if (SUCCEEDED(errorCode) && resultObjectAsJson != nullptr)
				{
					// resultObjectAsJson 是 JSON 字符串，例如 "\"hello\""
					CString selectedText = resultObjectAsJson;

					// 去除 JSON 字符串的引号
					if (selectedText.GetLength() > 2)
					{
						selectedText = selectedText.Mid(1, selectedText.GetLength() - 2);
						// 发送结果到主窗口处理
						PostMessage(WM_SELECTION_RESULT, 0, (LPARAM)new CString(selectedText));
					}
				}
				return S_OK;
			}).Get());
}


void C剧情处理Dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (cururl.GetLength())
	HINSTANCE result = ShellExecute(NULL, _T("open"), cururl, NULL, NULL, SW_SHOWNORMAL);
}


UINT C剧情处理Dlg::AsyncFetchThreadProc(LPVOID pParam)
{
	// 获取线程参数
	std::unique_ptr<AsyncFetchData> pData(static_cast<AsyncFetchData*>(pParam));
	C剧情处理Dlg* pThis = pData->pThis;
	CString url = pData->url;

	CString strResult;
	bool bSuccess = false;
	int linenum = 0;
	CString afterhand;

	// 初始化WinINet
	HINTERNET hInternet = InternetOpen(
		_T("MFC_FetchAgent"),
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0
	);

	if (hInternet)
	{
		// 设置请求标志
		DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;

		// 自动识别HTTPS
		if (url.Left(8) == _T("https://")) {
			dwFlags |= INTERNET_FLAG_SECURE;
		}

		// 打开URL连接
		HINTERNET hUrl = InternetOpenUrl(
			hInternet,
			url,
			NULL,
			0,
			dwFlags,
			0
		);

		if (hUrl)
		{
			// 读取数据到CString
			std::string rawData;
			char buffer[4096];
			DWORD dwRead = 0;

			while (InternetReadFile(hUrl, buffer, sizeof(buffer), &dwRead) && dwRead > 0)
			{
				rawData.append(buffer, dwRead);
			}

			// 清理资源
			InternetCloseHandle(hUrl);

			if (!rawData.empty())
			{
				int wideSize = MultiByteToWideChar(
					CP_UTF8,
					0,
					rawData.data(),
					rawData.size(),
					NULL,
					0
				);

				if (wideSize > 0)
				{
					std::wstring wstr(wideSize, 0);
					MultiByteToWideChar(
						CP_UTF8,
						0,
						rawData.data(),
						rawData.size(),
						&wstr[0],
						wideSize
					);
					strResult = wstr.c_str();
					bSuccess = true;
				}
			}
		}
		InternetCloseHandle(hInternet);
	}

	if (bSuccess)
	{
		CString a = TEXT("<script type=\"csv\" id=\"datas_txt\">"), b = TEXT("</script>");
		int nStartA = strResult.Find(a);

		if (nStartA != -1)
		{
			// 计算实际截取起始位置（a之后）
			int nStartPos = nStartA + a.GetLength();

			if (nStartPos < strResult.GetLength())
			{
				// 查找结束位置b（从nStartPos开始）
				int nEndB = strResult.Find(b, nStartPos);

				if (nEndB != -1)
				{
					CString result = strResult.Mid(nStartPos, nEndB - nStartPos);
					HANDLEER hand(result);
					afterhand = hand.afterhand;
					linenum = hand.linenum;
				}
			}
		}
	}

	// 发送消息到主线程处理结果
	if (pThis && pThis->GetSafeHwnd())
	{
		pThis->PostMessage(WM_FETCH_COMPLETE,
			bSuccess ? 1 : 0,
			reinterpret_cast<LPARAM>(new AsyncResult(bSuccess, url, afterhand, linenum)));
	}

	return 0;
}

// 在对话框类中添加消息处理


LRESULT C剧情处理Dlg::OnFetchComplete(WPARAM wParam, LPARAM lParam)
{
	std::unique_ptr<AsyncResult> pResult(reinterpret_cast<AsyncResult*>(lParam));
	if (wParam) // 成功
	{
		WriteCopyBoard(pResult->afterhand);
		CString sendmessage = m_url;
		char len[10];
		sprintf(len, "%d", pResult->linenum);
		sendmessage += len;
		sendmessage += TEXT("\n剧情处理完成，已复制到剪贴板");
		if (m_boxnotice.GetCheck()) MessageBox(sendmessage);
		OnBnClickedButton5();
	}
	else
	{
		MessageBox(TEXT("获取网页内容失败"));
	}

	return 0;
}

LRESULT C剧情处理Dlg::OnSelectionResult(WPARAM wParam, LPARAM lParam)
{
	CString* pText = (CString*)lParam;
	if (pText != nullptr)
	{
		SelectedWord = *pText;  // 示例：弹出结果
		SelectedWord.Replace(L"\\n", L"\n");
		SelectedWord.Replace(L"\\", L"");
		HANDLEER hand(SelectedWord);
		WriteCopyBoard(hand.afterhand);
		CString sendmessage = m_url;
		char len[10];
		sprintf(len, "%d", hand.linenum);
		sendmessage += len;
		sendmessage += TEXT("\n剧情处理完成，已复制到剪贴板");
		if (m_boxnotice.GetCheck()) MessageBox(sendmessage);
		OnBnClickedButton5();
	}
	return 0;
}

CString C剧情处理Dlg::GetSourceCodeSync(CString url)
{
	std::promise<CString> promise;
	auto future = promise.get_future();

	// 1. 注册 NavigationCompleted 事件
	EventRegistrationToken token;
	webView->add_NavigationCompleted(
		Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>(
			[this, &promise](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT
			{
				BOOL isSuccess = FALSE;
				args->get_IsSuccess(&isSuccess);

				if (isSuccess)
				{
					// 2. 页面加载完成后执行脚本
					this->webView->ExecuteScript(
						L"document.documentElement.outerHTML;",
						Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
							[&promise](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT
							{
								CString result;
								if (SUCCEEDED(errorCode) && resultObjectAsJson)
								{
									std::wstring wjson(resultObjectAsJson);
									if (wjson.size() >= 2 && wjson.front() == L'"' && wjson.back() == L'"')
										wjson = wjson.substr(1, wjson.size() - 2);

									std::wstring whtml;
									for (size_t i = 0; i < wjson.size(); i++)
									{
										if (wjson[i] == L'\\' && i + 1 < wjson.size())
										{
											if (wjson[i + 1] == L'n')
											{
												whtml += L'\n'; // 换行
												i++;
											}
											else if (wjson[i + 1] == L'r')
											{
												whtml += L'\r'; // 回车
												i++;
											}
											else if (wjson[i + 1] == L't')
											{
												whtml += L'\t'; // 制表符
												i++;
											}
											else if (wjson[i + 1] == L'u' && i + 5 < wjson.size())
											{
												// 处理 Unicode 转义，如 \u003C -> '<'
												std::wstring hex = wjson.substr(i + 2, 4);
												wchar_t ch = static_cast<wchar_t>(std::stoi(hex, nullptr, 16));
												whtml += ch;
												i += 5;
											}
											else
											{
												// 其他情况直接取后一个字符（不做额外转义）
												whtml += wjson[i + 1];
												i++;
											}
										}
										else
										{
											whtml += wjson[i];
										}
									}

									result = CString(whtml.c_str());
								}
								promise.set_value(result);
								return S_OK;
							}
						).Get()
								);
				}
				else
				{
					promise.set_value(L""); // 加载失败也必须返回
				}
				return S_OK;
			}
		).Get(),
				&token
				);

	// 3. 开始导航
	this->webView->Navigate(url);

	// 4. 消息循环等待
	while (future.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(10);
	}

	// 5. 获取结果
	CString result = future.get();

	// 6. 移除事件处理（可选）
	webView->remove_NavigationCompleted(token);

	return result;
}






void C剧情处理Dlg::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void C剧情处理Dlg::OnBnClickedButton7()
{
	UpdateData(true);
	CString url = TEXT("https://prts.wiki/w/%E5%89%A7%E6%83%85%E4%B8%80%E8%A7%88");
	CString scode = GetSourceCodeSync(url);
	m_quetext.MakeUpper();
	CString target = TEXT(">") + m_quetext + TEXT("-");
	m_todolist.ResetContent();
	int pos = -1;
	while ((pos = scode.Find(target, pos + 1)) != -1)
	{
		int pos2 = scode.Find(TEXT("</a>"), pos + 1);
		CString res = scode.Mid(pos + 1, pos2 - pos - 1);
		if (m_todolist.FindStringExact(0, res) == CB_ERR) {
			m_todolist.AddString(res); 
		}
	}
	m_todolist.SetCurSel(0);
}


void C剧情处理Dlg::OnBnClickedButton8()
{
	UpdateData(true);
	int nIndex = m_todolist.GetCurSel();
	m_todolist.GetLBText(nIndex, m_url);
	UpdateData(false);
	if(autonext.GetCheck()) m_todolist.SetCurSel(nIndex + 1);
	OnBnClickedButton1();
}



