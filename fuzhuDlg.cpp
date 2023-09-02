
// fuzhuDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "fuzhu.h"
#include "fuzhuDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CfuzhuDlg 对话框



CfuzhuDlg::CfuzhuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FUZHU_DIALOG, pParent)
	, m_strshowdata(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfuzhuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strshowdata);
}

BEGIN_MESSAGE_MAP(CfuzhuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CfuzhuDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CfuzhuDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CfuzhuDlg 消息处理程序

BOOL CfuzhuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfuzhuDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CfuzhuDlg::OnPaint()
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
HCURSOR CfuzhuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CfuzhuDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned short xypos[2] = { 0 };
	CString strTemp = _T("");
	short gamex = 18;
	short gamey = 82;
	DWORD dwHigth = 0;
	DWORD pid;
	HWND hWnd = ::FindWindow(NULL, _T("扫雷")); //获取游戏的窗口句柄

	if (NULL == hWnd) {
		::MessageBox(NULL, _T("扫雷游戏未打开"), _T("错误"), MB_OK);
		return;
	}

	GetWindowThreadProcessId(hWnd, &pid); //通过窗口句柄拿到进程ID

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); //通过进程 id拿到进程句柄

	if (NULL == hProcess) {
		::MessageBox(NULL, _T("打开扫雷游戏失败"), _T("错误"), MB_OK);
		return;
	}

	//雷区的基地址 01005361   雷的宽01005334  雷的高01005338 每个雷的距离为16  0x8F为雷
	unsigned char gamedata[24][32] = { 0 };
	if (!ReadProcessMemory(hProcess, (LPCVOID)0x01005361, &gamedata, 32 * 24, &pid)) {
		::MessageBox(NULL, _T("读取扫雷游戏的数据失败"), _T("错误"), MB_OK);
	}

	if (gamedata[0][0] == 0x8F) {     //如果第一个是雷，点开然后重新读取内存数据，因为雷会重新部署
		xypos[0] = gamex;
		xypos[1] = gamey;
		::PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, *(int*)xypos);
		::PostMessage(hWnd, WM_LBUTTONUP, 0, *(int*)xypos);
		return;
	}

	m_strshowdata.Empty();   //每次读取的时候，把上次的数据清空一下
	
	if (!ReadProcessMemory(hProcess, (LPCVOID)0x01005338, &dwHigth, sizeof(dwHigth), &pid)) {
		::MessageBox(NULL, _T("读取扫雷游戏的数据失败"), _T("错误"), MB_OK);
	}
	
	for (int i = 0; i < dwHigth; ++i) {     //自动化扫雷
		for (int j = 0; j < 32; ++j) {
			if (0x10 == gamedata[i][j]) //每行结尾
				break;
			if (0x8F != gamedata[i][j]) {
				xypos[0] = gamex +j*24;
				xypos[1] = gamey +i*24;
				::PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, *(int*)xypos);
				::PostMessage(hWnd, WM_LBUTTONUP, 0, *(int*)xypos);
			}
			strTemp.Format(_T("%02X "), gamedata[i][j]);
			m_strshowdata += strTemp;
		}
		m_strshowdata += _T("\r\n");
	}


	UpdateData(FALSE);
}


void CfuzhuDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
