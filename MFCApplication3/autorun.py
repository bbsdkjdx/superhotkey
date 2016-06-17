####set add_itm function for load config.
from ctypes import CFUNCTYPE,c_int32,c_wchar_p
add_itm=CFUNCTYPE(c_int32,c_int32,c_wchar_p,c_wchar_p,c_wchar_p)(g_pdlg)

itm_sep='<itm>'
field_sep='<field>'
items=[]

def add_item(hk,name,cmnt,code):
    items.append(field_sep.join([hk,name,cmnt,code]))

def save():
    with open('config.cfg','w') as f:
        for x in items:
            f.write(x+itm_sep)

def load():
    with open('config.cfg') as f:
        for itm in f.read().split(itm_sep)[:-1]:
            itm=itm.replace('\n','\r\n')
            hk,name,cmnt,code= itm.split(field_sep)
            hk=int(hk)
            add_itm(hk,name,cmnt,code)
try:
    load()
except:
    pass
