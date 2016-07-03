import sys, os
 
if os.name != 'nt':
   print ("This module only can be used under windows")
   sys.exit()
 
try:
   from ctypes import *
except:
   print ('I need module ctypes')
   sys.exit()
 
try:
   from win32con import *
except:
   print ('I need module win32con')
   sys.exit()
 
# command colors
cc_map = {
    'default'      :0,
    'black'        :1,
    'blue'         :2,
    'green'        :3,
    'cyan'         :4,
    'red'          :5,
    'magenta'      :6,
    'brown'        :7,
    'lightgray'    :8,
    'darkgray'     :9,
    'lightblue'    :10,
    'lightgreen'   :11,
    'lightcyan'    :12,
    'lightred'     :13,
    'lightmagenta' :14,
    'yellow'       :15,
    'white'        :16,
};
         
CloseHandle = windll.kernel32.CloseHandle
GetStdHandle = windll.kernel32.GetStdHandle
GetConsoleScreenBufferInfo = windll.kernel32.GetConsoleScreenBufferInfo
SetConsoleTextAttribute = windll.kernel32.SetConsoleTextAttribute
 
STD_OUTPUT_HANDLE = -11
 
class COORD(Structure):
   _fields_ = [('X', c_short),
               ('Y', c_short),
              ]
 
class SMALL_RECT(Structure):
   _fields_ = [('Left', c_short),
               ('Top', c_short),
               ('Right', c_short),
               ('Bottom', c_short),
              ]
                
class CONSOLE_SCREEN_BUFFER_INFO(Structure):
   _fields_ = [('dwSize', COORD),
               ('dwCursorPosition', COORD),
               ('wAttributes', c_uint),
               ('srWindow', SMALL_RECT),
               ('dwMaximumWindowSize', COORD),
              ]
 
def print_cc(fore_color, back_color, text):
    if not (fore_color in cc_map and
            back_color in cc_map):
        #color not found
        print >>stderr, fore_color, back_lolor, " are invalid color strings"
        return
 
    #prepare
    hconsole = GetStdHandle(STD_OUTPUT_HANDLE)
    cmd_info = CONSOLE_SCREEN_BUFFER_INFO()
    GetConsoleScreenBufferInfo(hconsole, byref(cmd_info))
    old_color = cmd_info.wAttributes
 
    #calculate colors
    fore = cc_map[fore_color]
    if fore: fore = fore - 1
    else: fore = old_color & 0x0F
    back = cc_map[back_color]
    if back: back = (back - 1) << 4
    else: back = old_color & 0xF0
 
    #real output
    SetConsoleTextAttribute(hconsole, fore + back)
    print (text)
    SetConsoleTextAttribute(hconsole, old_color)
 
