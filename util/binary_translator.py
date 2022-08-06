import tkinter as tk
import pyperclip

def digit(n, r):
    n = str(n)
    l = len(n)
    for i in range(r - l):
        n = '0' + n
    return n

vacant = '   .    '
active = '  %  '

root = tk.Tk()
root.geometry("1000x500")
buttons = []
txts = []

def change_txt(coord):
    if txts[coord].get() == vacant:
        txts[coord].set(active)
    else:
        txts[coord].set(vacant)

def go():
    s = ''
    for txt in txts:
        s += '1' if txt.get() == active else '0'
    #print(s)
    bin_num = int(s, 2)
    hex_s = hex(bin_num)[2:]
    hex_s_fill0 = digit(hex_s, 16).upper()
    res = '0x' + hex_s_fill0 + 'ULL'
    print(res)
    pyperclip.copy(res)

def reset():
    for txt in txts:
        if txt.get() == active:
            txt.set(vacant)

for coord in range(64):
    txt = tk.StringVar(root)
    txt.set(vacant)
    button = tk.Button(root, textvariable=txt, command = lambda coord=coord: change_txt(coord))
    buttons.append(button)
    txts.append(txt)
for coord, button in enumerate(buttons):
    z = coord // 16
    y = coord // 4 % 4
    x = coord % 4
    button.grid(row=y, column=x + z * 5)
labels = []
for i in range(4):
    labels.append(tk.Label(text=str(i)))
for coord, label in enumerate(labels):
    label.grid(row=0, column=coord * 5 + 4)

reset_button = tk.Button(root, text='RST', command=reset)
reset_button.grid(row=8, column=0)

go_button = tk.Button(root, text='GO', command=go)
go_button.grid(row=9, column=0)

root.mainloop()


'''
s = ''
while True:
    ss = input()
    if ss == '':
        break
    s += ss
print(s)

bin_num = int(s, 2)
hex_s = hex(bin_num)[2:]
hex_s_fill0 = digit(hex_s, 16)
print('0x' + hex_s_fill0 + 'ULL')
'''