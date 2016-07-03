import ctypes,os,json
from shellexecute import shell_execute
from clipboard import *
from ctypes import CFUNCTYPE, c_int32, c_wchar_p
###########do load and save#######################

add_itm=CFUNCTYPE(c_int32,c_int32,c_wchar_p,c_wchar_p,c_wchar_p)(g_pdlg)
config = []

def add_item(hk, name, cmnt, code):
    config.append((hk, name, cmnt, code))

def save():
    global config
    json.dump(config,open('config.json','w'))
    config=[]

def load():
    config=json.load(open('config.json','r'))
    for hk, name, cmnt, code in config:
        cmnt = cmnt.replace('\n', '\r\n')
        code = code.replace('\n', '\r\n')
        hk=int(hk)
        add_itm(hk,name,cmnt,code)

try:
    load()
except Exception as exp:
    ctypes.windll.user32.MessageBoxW(0,str(exp),'',0)
    pass
##########################################################################
