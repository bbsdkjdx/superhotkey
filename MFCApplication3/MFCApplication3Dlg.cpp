
// MFCApplication3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"
#include "python_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication3Dlg �Ի���



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication3Dlg::IDD, pParent)
	, m_str_pycode(_T(""))
	, m_str_result(_T(""))
	, m_str_comment(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_str_pycode);
	DDX_Text(pDX, IDC_EDIT2, m_str_result);
	//  DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_HOTKEY2, m_hotkey);
	DDX_Text(pDX, IDC_EDIT3, m_str_comment);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT1, m_edit_code);
	DDX_Control(pDX, IDC_EDIT3, m_edit_cmnt);
}

const int IDM_TRAY = 2000;

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_MESSAGE(IDM_TRAY, TrayFunction)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication3Dlg::OnEval)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication3Dlg::OnExec)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication3Dlg::OnEditItem)
//	ON_NOTIFY(NM_OUTOFMEMORY, IDC_HOTKEY2, &CMFCApplication3Dlg::OnNMOutofmemoryHotkey2)
//	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication3Dlg::OnEnChangeComment)
//	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication3Dlg::OnEnChangeCode)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication3Dlg::OnDeleteItem)
	ON_WM_HOTKEY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCApplication3Dlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication3Dlg::OnSaveItem)
	ON_EN_SETFOCUS(IDC_EDIT3, &CMFCApplication3Dlg::OnEnSetfocusComment)
	ON_EN_SETFOCUS(IDC_EDIT1, &CMFCApplication3Dlg::OnEnSetfocusCode)
//	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication3Dlg::OnEnChangeEdit3)
//ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMFCApplication3Dlg ��Ϣ�������


CMFCApplication3Dlg *g_pdlg = nullptr;

int g_add_item(DWORD hk,wchar_t *hkname, wchar_t *cmnt, wchar_t *pycode)
{
	if (g_pdlg)
	{
		g_pdlg->AddItem(hk, hkname, cmnt, pycode);
	}
	return 0;
}


BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	g_pdlg = this;
	// add sys tray.
	m_tnid.cbSize = sizeof(NOTIFYICONDATA);
	m_tnid.hWnd = m_hWnd;
	m_tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnid.uCallbackMessage = IDM_TRAY;
	wcscpy_s(m_tnid.szTip, _TEXT("super hotkey"));
	m_tnid.uID = IDR_MAINFRAME;
	m_tnid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Shell_NotifyIcon(NIM_ADD, &m_tnid);
	//set list control.
	m_list.InsertColumn(0, _T("hotkey"),0,130,-1);
	m_list.InsertColumn(1, _T("description"), 0, 200, -1);
	DWORD styles = m_list.GetExtendedStyle();
	m_list.SetExtendedStyle(styles | LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);
	//do python stuff.
	InitPython();
	CString tmp;
	tmp.Format(_T("g_pdlg=%d"),&g_add_item);
	PyExec(tmp.GetBuffer());//tell g_pdlg to python.
	m_str_result=PyRunFile(_T("autorun.py"));
	UpdateData(FALSE);
	UpdateControls();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CMFCApplication3Dlg::TrayFunction(WPARAM wp, LPARAM lp)
{
	switch (lp)
	{
	case  WM_RBUTTONDBLCLK:
		Sleep(200);
		CDialogEx::OnOK();
		break;
	case  WM_LBUTTONDBLCLK:
		ShowWindow(1);
		break;
	default:
		break;
	}
	return 0;
}


BOOL CMFCApplication3Dlg::DestroyWindow()
{
	// TODO:  �ڴ����ר�ô����/����û���
	Shell_NotifyIcon(NIM_DELETE, &m_tnid);//NIM_MODIFYΪ�������� NIM_DELETEΪɾ������
	SaveConfig();
	return CDialogEx::DestroyWindow();
}


void CMFCApplication3Dlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	ShowWindow(0);
	//CDialogEx::OnOK();
}


void CMFCApplication3Dlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���
	ShowWindow(0);
	//CDialogEx::OnCancel();
}


void CMFCApplication3Dlg::OnEval()
{
	UpdateData();
	int cs = 0, ce = 0;
	m_edit_code.GetSel(cs, ce);
	if (cs == ce)
	{
		m_str_result = PyExec(m_str_pycode.GetBuffer());
	}
	else
	{
		m_str_result = PyEval(m_str_pycode.Mid(cs, ce - cs).GetBuffer());
	}
	UpdateData(FALSE);
}


void CMFCApplication3Dlg::OnExec()
{

	UpdateData();
	int cs=0, ce=0;
	m_edit_code.GetSel(cs, ce);
	if (cs==ce)
	{
		m_str_result = PyExec(m_str_pycode.GetBuffer());
	}
	else
	{
		m_str_result = PyExec(m_str_pycode.Mid(cs, ce - cs).GetBuffer());
	}
	UpdateData(FALSE);
}



void CMFCApplication3Dlg::OnEditItem()
{
	m_edit_cmnt.SetReadOnly(FALSE);
	m_edit_code.SetReadOnly(FALSE);
	m_hotkey.EnableWindow(TRUE);
}


void CMFCApplication3Dlg::OnSaveItem()
{
	UpdateData();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	DWORD hk = m_hotkey.GetHotKey();

	if (!pos)//nothing selected .
	{
		if (hk)//add a new one.
		{
			if (!RegisterHotKey(m_hWnd, hk, hk >> 8, hk & 0xff))
			{
				MessageBox(_T("�ȼ���ռ�ã�"));
				return;
			}
			AddItem(m_hotkey.GetHotKey(), m_hotkey.GetHotKeyName(), m_str_comment, m_str_pycode);
		}
		else//save to blank item.
		{
			m_hkitm.cmnt = m_str_comment;
			m_hkitm.pycode = m_str_pycode;
		}
	}
	else//save an exist one.
	{
		int idx = m_list.GetNextSelectedItem(pos);
		HKItem *p = (HKItem*)m_list.GetItemData(idx);
		p->cmnt = m_str_comment;
		p->pycode = m_str_pycode;
		m_list.SetItemText(idx, 1, m_str_comment);

		if (p->hk!=m_hotkey.GetHotKey())//change the hotkey.
		{
			UnregisterHotKey(m_hWnd, p->hk);
			p->hk = m_hotkey.GetHotKey();
			p->hk_name = m_hotkey.GetHotKeyName();
			m_list.SetItemText(idx, 0,p->hk_name);
			if (p->hk)
			{
				RegisterHotKey(m_hWnd, p->hk, p->hk >> 8, p->hk & 0xff);
			}
		}

		m_edit_cmnt.SetReadOnly(TRUE);
		m_edit_code.SetReadOnly(TRUE);
		m_hotkey.EnableWindow(FALSE);
	}
}



void CMFCApplication3Dlg::OnDeleteItem()
{


	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		int p = m_list.GetNextSelectedItem(pos);
		HKItem *pitm = (HKItem*)m_list.GetItemData(p);
		UnregisterHotKey(m_hWnd, pitm->hk);
		delete pitm;
		m_list.DeleteItem(p);
	}
}




void CMFCApplication3Dlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	for (int x = m_list.GetItemCount() - 1; x > -1; --x)
	{
		HKItem *pitm = (HKItem*)m_list.GetItemData(x);
		if (pitm->hk==nHotKeyId)
		{
			m_str_result=PyExec(pitm->pycode.GetBuffer());
			UpdateData(FALSE);
			break;
		}
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMFCApplication3Dlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UpdateControls();
	*pResult = 0;
}


void CMFCApplication3Dlg::SaveConfig()
{
	CString tmp;
	for (int x = 0, n = m_list.GetItemCount(); x < n;++x)
	{
		HKItem* pitm = (HKItem*)m_list.GetItemData(x);
		tmp.Format(_T("add_item(r\"\"\"%d\"\"\",r\"\"\"%s\"\"\",r\"\"\"%s\"\"\",r\"\"\"%s\"\"\")"),pitm->hk,pitm->hk_name,pitm->cmnt,pitm->pycode);
		PyExec(tmp.GetBuffer());
	}
	//save blank item.
	tmp.Format(_T("add_item(r\"\"\"%d\"\"\",r\"\"\"%s\"\"\",r\"\"\"%s\"\"\",r\"\"\"%s\"\"\")"), 0, m_hkitm.hk_name,m_hkitm.cmnt, m_hkitm.pycode);
	PyExec(tmp.GetBuffer());
	PyExec(_T("save()"));
}


void CMFCApplication3Dlg::AddItem(DWORD hk, CString hk_name, CString cmnt, CString pycode)
{
	if (hk==0)
	{
		m_hkitm.cmnt = cmnt;
		m_hkitm.pycode = pycode;
		return;
	}
	int x = m_list.GetItemCount();
	m_list.InsertItem(x,hk_name);
	m_list.SetItemText(x, 1, cmnt);
	HKItem *pitm = new HKItem;
	pitm->cmnt = cmnt;
	pitm->pycode = pycode;
	pitm->hk = hk;
	pitm->hk_name = hk_name;
	m_list.SetItemData(m_list.GetItemCount() - 1, (DWORD_PTR)pitm);
	RegisterHotKey(m_hWnd, pitm->hk, hk >> 8, hk & 0xff);
}



void CMFCApplication3Dlg::OnEnSetfocusComment()
{
	m_edit_cmnt.SetSel(0xffff0000);
}


void CMFCApplication3Dlg::OnEnSetfocusCode()
{
	m_edit_code.SetSel(0xffff0000);
}


void CMFCApplication3Dlg::UpdateControls()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	HKItem *pitm = &m_hkitm;
	if (pos)
	{
		int p = m_list.GetNextSelectedItem(pos);
		pitm = (HKItem*)m_list.GetItemData(p);
	}
	m_str_pycode = pitm->pycode;
	m_str_comment = pitm->cmnt;
	m_hotkey.SetHotKey(pitm->hk & 0xff, pitm->hk >> 8);
	UpdateData(FALSE);
	m_edit_code.SetReadOnly(pitm != &m_hkitm);
	m_edit_cmnt.SetReadOnly(pitm != &m_hkitm);
	m_hotkey.EnableWindow(pitm == &m_hkitm);
}

