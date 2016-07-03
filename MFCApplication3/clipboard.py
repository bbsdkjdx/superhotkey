#%%
import win32clipboard as w32cb
import os

#%%
def s2cb(s,encoding='gbk'):
    'set text to clipboard.'
    w32cb.OpenClipboard()
    w32cb.EmptyClipboard()
    w32cb.SetClipboardText(s.encode(encoding,'ignore'))
    w32cb.CloseClipboard()

#%%


def cb2s(encoding='gbk'):
    'get text from clipboard'
    ret=b''
    w32cb.OpenClipboard()
    if w32cb.IsClipboardFormatAvailable(w32cb.CF_TEXT):
        ret = w32cb.GetClipboardData(w32cb.CF_TEXT)
    w32cb.CloseClipboard()
    return ret.decode(encoding,'ignore')


def cb2meta_file(fn=None):
    'get meta file from clipboard'
    ret=None
    w32cb.OpenClipboard()
    if w32cb.IsClipboardFormatAvailable(w32cb.CF_ENHMETAFILE):
        ret = w32cb.GetClipboardData(w32cb.CF_ENHMETAFILE)
    w32cb.CloseClipboard()
    if fn and ret:
        open(fn,'wb').write(ret)
        return
    else:
        return ret
#%%


def l2cb(l, sep_line='\t', sep_row='\n'):
    '''set a list to clipboard as a string,joined by \\n.
    warning:the type of list element must be str.'''
    if type(l[0]) in(list, tuple):
        s2cb(sep_row.join(map(lambda x: sep_line.join(map(str, x)), l)))
    else:
        s2cb(sep_row.join(map(str, l)))

#%%


def cb2l(fix_excel=True, encoding='gbk'):
    '''get a list from clipboard text,splited by \\r\\n or \\n.
       if fix_excel==True, the function will trim the last blank of result from MS Excel.'''
    s = cb2s(encoding).replace('\r', '')
    ret = s.split('\n')
    if fix_excel and ret[-1] == '':
        ret = ret[:-1]
    return ret
#%%

def _enum_folders_files(rt,has_folder,has_file):
    if has_file and os.path.isfile(rt):
        yield rt
    if has_folder and os.path.isdir(rt):
        yield rt
    for r,d,f in os.walk(rt):
        if has_folder:
            for _d in d:
                yield r+'\\'+_d
        if has_file:
            for _f in f:
                yield r+'\\'+_f


def cb2fn(recursive=True,has_folder=True,has_file=True):
    "get folder names and file names copied to clipboard"
    if not w32cb.IsClipboardFormatAvailable(w32cb.CF_HDROP):
        return ()
    w32cb.OpenClipboard()
    ret = w32cb.GetClipboardData(w32cb.CF_HDROP)
    w32cb.CloseClipboard()
    if not recursive:
        for x in ret:
            yield x
        return
    for x in ret:
        yield from _enum_folders_files(x,has_folder,has_file)
#%%


def l2dic(li,key_col=0,str_val=True,sep='\t'):
    '''l2dic:covert a list to dict.
    paras:
    li:the list to be converted.
    key_col:the column to be as the key.
    str_val:true if the dict values are string,else list.
    sep:the separator of li's item.
    '''
    ret = dict()
    nc=len(li[0].split(sep))
    if nc<2 or key_col>nc-1:
        return ret
    for x in li:
        tp=x.split(sep)
        ret[tp[key_col]]=x if str_val else tp
    return ret

def find_dup(li):
    dic=dict()
    for x in li:
        if x in dic:
            dic[x]+=1
        else:
            dic[x]=1
    return sorted(set([x for x in li if dic[x]>1]))