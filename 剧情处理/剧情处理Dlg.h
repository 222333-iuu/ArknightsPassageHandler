
// 剧情处理Dlg.h: 头文件
//

#pragma once
#include <atlcomcli.h> 
#include <functional>
#include "CEXPLORER1.h"
#include "MsHTML.h"
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include "handler.h"
#define WM_FETCH_COMPLETE (WM_USER + 100) 
#define WM_SELECTION_RESULT (WM_USER + 300)
// C剧情处理Dlg 对话框
class C剧情处理Dlg : public CDialogEx
{
// 构造
public:
	C剧情处理Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEvent m_jsCompleted; // 用于同步等待
	CString m_jsResult;
	void GetSelectedTextAsync();
	CEXPLORER1 m_webbrowser;
	CString m_url;
	afx_msg void OnBnClickedButton1();
	COleVariant noArg;
	IHTMLDocument2* m_pDocument2;
	void OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL); //作为下方获取选中文本的辅助
	CString GetSelectedText();//获取选中文本
	afx_msg void OnBnClickedButton2();
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	void SetIEZoomPercent(int nZoomPercent);
	friend HANDLE;
	afx_msg void OnBnClickedRadio1();
	CButton m_needend;
	CString ReadCopyBoard();
	void WriteCopyBoard(CString strData);
	afx_msg void OnBnClickedButton3();
	CString m_adddescription;
	afx_msg void OnBnClickedRadio3();
	CButton m_isjuqing;
	afx_msg void OnBnClickedButton4();
	CButton m_boxnotice;
	afx_msg void OnBnClickedButton5();
	CString m_text;
	CEdit m_readfile;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CButton m_res_file;
	CButton m_res_web;
	afx_msg void OnEnChangeEdit3();
	CStatic m_intro;
	afx_msg void OnStnClickedLinenum();
	CString linenum;
	void C剧情处理Dlg::GetSourceCode(CString url);
	CButton m_issource;
	CString Cururl;
	CStatic m_cururl;
	afx_msg void OnStnClickedCurtext();
	CString cururl;
	void InitWebView2(HWND hwnd);
	Microsoft::WRL::ComPtr<ICoreWebView2Controller> webViewController;
	Microsoft::WRL::ComPtr<ICoreWebView2> webView;
	HWND m_webViewHwnd = nullptr; // 新增：保存WebView2的窗口句柄
	void NavigateToURL(const std::wstring& url);
	void clear(int mode);
	bool m_bNavEventRegistered = false;
	CString m_strLastHtml;
	afx_msg void OnBnClickedButton6();
	afx_msg LRESULT OnFetchComplete(WPARAM wParam, LPARAM lParam);
	UINT AsyncFetchThreadProc(LPVOID pParam);
	afx_msg LRESULT OnSelectionResult(WPARAM, LPARAM lParam);
	CString SelectedWord;
	CString GetSourceCodeSync(CString url);
};

// 辅助类定义
class AsyncFetchData
{
public:
	C剧情处理Dlg* pThis;
	CString url;

	AsyncFetchData(C剧情处理Dlg* p, const CString& u) : pThis(p), url(u) {}
};

class AsyncResult
{
public:
	bool bSuccess;
	CString url;
	CString afterhand;
	int linenum;

	AsyncResult(bool success, const CString& u, const CString& ah, int ln)
		: bSuccess(success), url(u), afterhand(ah), linenum(ln) {}
};
