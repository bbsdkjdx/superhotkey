import ctypes
class _SHELLEXECUTEINFO (ctypes.Structure):
  """docstring for _SHELLEXECUTEINFO """
  _fields_ = [("cbSize", ctypes.c_uint32),
                ("fMask", ctypes.c_uint32),
                ("HWND", ctypes.c_uint32),
                ("lpVerb",ctypes.c_wchar_p),
                ("lpFile", ctypes.c_wchar_p),
                ("lpParameters", ctypes.c_wchar_p),
                ("lpDirectory", ctypes.c_wchar_p),
                ("nShow", ctypes.c_uint32),
                ("hInstApp", ctypes.c_uint32),
                ("lpIDList", ctypes.c_void_p),
                ("lpClass", ctypes.c_wchar_p),
                ("hkeyClass", ctypes.c_uint32),
                ("dwHotKey", ctypes.c_uint32),
                ("DUMMYUNIONNAME", ctypes.c_uint32),
                ("hProcess", ctypes.c_uint32)]

def shell_execute(cmd,show=0,wait=0):
    se=ctypes.windll.shell32.ShellExecuteExW
    wfso=ctypes.windll.kernel32.WaitForSingleObject
    para=_SHELLEXECUTEINFO()
    para.cbSize=ctypes.sizeof(para)
    para.fMask=0x40
    para.HWND=0
    para.lpVerb='open'
    para.lpFile='explorer.exe' if show else cmd
    para.lpParameters=r'/select,'+cmd if show else 0
    para.lpDirectory=0
    para.nShow=5
    para.hInstApp=0
    para.lpIDList=0
    para.lpClass=0
    para.hkeyClass=0
    para.dwHotKey=0
    para.DUMMYUNIONNAME=0
    se(ctypes.byref(para))
    if wait:
      wfso(para.hProcess,0xffffffff)
 