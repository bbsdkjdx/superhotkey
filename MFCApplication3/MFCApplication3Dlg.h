
// MFCApplication3Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"



struct HKItem
{
	DWORD hk;
	CString hk_name;
	CString cmnt;
	CString pycode;
};

const UINT WM_TaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));

// CMFCApplication3Dlg 对话框
class CMFCApplication3Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	LRESULT TrayFunction(WPARAM wp, LPARAM lp); 

	NOTIFYICONDATA m_tnid;
public:
	virtual BOOL DestroyWindow();
//	afx_msg void OnBnClickedOk();
	virtual void OnOK();
	virtual void OnCancel();
//	afx_msg void OnBnClickedOk();
	afx_msg void OnEval();

public:
	afx_msg void OnExec();
protected:
	CListCtrl m_list;
	CHotKeyCtrl m_hotkey;
	CEdit m_edit_code;
	CEdit m_edit_cmnt;
public:
	afx_msg void OnEditItem();
protected:
	CString m_str_pycode;
	CString m_str_result;
	CString m_str_comment;
	HKItem m_hkitm;
public:
//	afx_msg void OnNMOutofmemoryHotkey2(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnEnChangeComment();
//	afx_msg void OnEnChangeCode();
	afx_msg void OnDeleteItem();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	void SaveConfig();

public:
	void AddItem(DWORD hk, CString hk_name, CString cmnt, CString pycode);
	afx_msg void OnSaveItem();
	afx_msg void OnEnSetfocusComment();
	afx_msg void OnEnSetfocusCode();
//	afx_msg void OnEnChangeEdit3();
protected:
	void UpdateControls();
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcPaint();
protected:
	bool m_can_show;
	afx_msg LRESULT OnTaskbarrestart(WPARAM wParam, LPARAM lParam);
	void SetCurrentDir();
};
