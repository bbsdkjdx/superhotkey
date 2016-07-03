#version 2.1
from win32com.client import DispatchEx as _dispatch
from os.path import isdir as _isdir
from os import walk as _walk
from os.path import join as _join

def enum_files(root,*allow_ext):
    allow_ext=list(map(lambda s:s.lower(),allow_ext))
    if len(allow_ext)==0:
        allow_ext=['']
    if not _isdir(root):
        return
    for di,fd,fl in _walk(root):
        for f in fl:
            for ext in allow_ext:
                if f.lower().endswith(ext):
                    yield _join(di,f)

class Excel(object):
    """An Excel application instance"""
    def __init__(self, visible):
        self.app=_dispatch('Excel.Application')
        self.app.Visible=visible

    def __del__(self):
        for x in range(len(self.books)):
            self.app.Workbooks(1).Close(0)
        self.app.Quit()

    @property
    def raw(self):
        "return the COM object."
        return self.app

    @property
    def books(self):
        return list(map(self.Book,self.app.Workbooks))

    def open(self,fn):
        _bk=self.app.Workbooks.Open(fn)
        return self.Book(_bk)

    def new(self):
        _bk=self.app.Workbooks.Add()
        return self.Book(_bk)

    class Book(object):
        """docstring for Book"""
        def __init__(self, _book):
            self.book=_book

        def __del__(self):
            pass
            #self.close()

        @property
        def raw(self):
            return self.book

        @property
        def sheets(self):
            return list(map(self.Sheet,self.book.Sheets))

        def save(self):
            if self.book==None:
                return
            self.book.Save()

        def saveas(self,fn):
            if self.book==None:
                return
            self.book.SaveAs(fn)

        def close(self):
            if self.book:
                self.book.Close(0)
                self.book=None
        class Sheet(object):
            """docstring for Sheet"""
            def __init__(self, sht):
                self.sheet = sht

            @property
            def raw(self):
                return self.sheet

            def n_rows(self):
                return self.sheet.UsedRange.Rows.Count

            def n_cols(self):
                return self.sheet.UsedRange.Columns.Count

            def get_text(self,row,col):
                return self.sheet.Cells(row,col).Value

            def set_text(self,row,col,txt):
                self.sheet.Cells(row,col).Value=txt

            def get_name(self):
                return self.sheet.Name

            def set_name(self,name):
                self.sheet.Name=name

            @property
            def name(self):
                return self.sheet.Name

            @name.setter
            def name(self,nm):
                self.sheet.Name=nm
            

            def print_out(self):
                self.sheet.PrintOut()

            def copy(self,sht):
                pass


class Word(object):
    """docstring for Word"""
    def __init__(self, visible):
        self.app=_dispatch('Word.Application')
        self.app.Visible=visible

    def __del__(self):
        for x in range(self.n_docs()):
            self.app.Documents(1).Close(0)
        self.app.Quit()
    @property
    def raw(self):
        return self.app
    
    @property
    def n_docs(self):
        return self.app.Documents.Count

    def get_doc(self,idx_1):
        return self.Doc(self.app.Documents(idx_1))
    
    @property
    def docs(self):
        return list(map(self.Doc,self.app.Documents))

    def open(self,fn):
        _dk=self.app.Documents.Open(fn)
        return self.Doc(_dk)

    def new(self):
        _dk=self.app.Documents.Add()
        return self.Doc(_dk)

    class Doc(object):
        """docstring for Doc"""
        def __init__(self, _doc):
            self.doc=_doc

        def __del__(self):
            pass

        @property
        def raw(self):
            return self.doc

        @property
        def shapes(self):
            return list(map(self.Shape,self.doc.Shapes))
        
        
        def get_text(self):
            return self.doc.Content.Text
        def get_shape(self,text):
            for sp in self.doc.Shapes:
                try:
                    txt=sp.TextFrame.TextRange.Text
                    if txt==text+'\r':
                        return self.Shape(sp)
                except:
                    continue
            raise Exception("can't find shape named:"+text)

        def get_table(self,idx_0):
            idx=0
            for ret in self.doc.Tables:#under some circumstances,Tables doesn't support indexing.
                if idx==idx_0:
                    break
                idx+=1
            return self.Table(ret)
        @property
        def tables(self):
            return [self.Table(x) for x in self.doc.Tables]

        def saveas(self,fn):
            self.doc.SaveAs(fn)

        @property
        def raw(self):
            return self.doc

        def close(self):
            self.doc.Close(0)

        class Shape(object):
            """docstring for Shape"""
            def __init__(self, _shp):
                self.shape=_shp
            @property
            def raw(self):
                return self.shape
            @property
            def text(self):
                return self.shape.TextFrame.TextRange.Text

            @text.setter
            def text(self,txt):
                self.shape.TextFrame.TextRange.Text=txt
            def adjust_spacing(self):
                n_line=self.shape.TextFrame.TextRange.ComputeStatistics(1)
                t_b=self.shape.TextFrame.MarginBottom+self.shape.TextFrame.MarginTop
                _pf=self.shape.TextFrame.TextRange.ParagraphFormat
                _pf.LineSpacingRule=4
                _pf.LineSpacing=(self.shape.Height-t_b)/n_line
                return _pf.LineSpacing

        class Table(object):
            def __init__(self,_tb):
                self.table=_tb
            @property
            def raw(self):
                return self.table
            def get_cell(self,row_1,col_1):
                return self.Cell(self.table.Cell(row_1,col_1))

            class Cell(object):
                """docstring for Cell"""
                def __init__(self, _cel):
                    self.cell = _cel
                @property
                def raw(self):
                    return self.cell
                @property
                def text(self):
                    return self.cell.Range.Text
                @text.setter
                def text(self,txt):
                    self.cell.Range.Text=txt
                def add_picture(self,pic_fn):
                    pic=self.cell.Range.InlineShapes.AddPicture(pic_fn)
                    sp=pic.ConvertToShape()
                    sp.Top=0
                    sp.Left=0
                    sp.Width=self.cell.Width-1
                    return sp
                    #sp.Height=self.cell.Height-1

