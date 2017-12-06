from tkinter import *
from PIL import ImageTk, Image
import time
import math
import serial
import struct

racine= Tk()

canvas = Canvas(racine, width=600, height=600) #Définit les dimensions du canvas
racine.configure(background='grey')
canvas.configure(background='grey')
canvas.create_oval(5, 5, 600, 600, outline="#9EFD38", fill="#18391E", width=2)
canvas.create_oval(55, 55, 550, 550, outline="#9EFD38", fill="#18391E", width=2)
canvas.create_oval(105, 105, 500, 500, outline="#9EFD38", fill="#18391E", width=2)
canvas.create_oval(155, 155, 450, 450, outline="#9EFD38", fill="#18391E", width=2)
canvas.create_oval(205, 205, 400, 400, outline="#9EFD38", fill="#18391E", width=2)
canvas.create_oval(255, 255, 350, 350, outline="#9EFD38", fill="#18391E", width=2)
#create_line(x0, y0, x1, y1)
canvas.create_line(300, 0, 300, 600, fill="#9EFD38", width=1)
canvas.create_line(0, 300, 600, 300, fill="#9EFD38", width=1)
canvas.create_line(90, 90, 510, 510, fill="#9EFD38", width=1)
canvas.create_line(510, 90, 90, 510, fill="#9EFD38", width=1)
canvas.pack() #Affiche le canvas

R = 300
centreX = R
centreY = R
id_line = []
id_point = []
id_tmp = []
remove = False
remove2 = False
delete = 0

timeDebut = time.time()
id_repere = 0
for i in range(181):
    i_rad = 2 * math.pi * i / 360
    id_line.append(canvas.create_line(300, 300, centreX + (R * math.cos(2 * i_rad)), centreY + (R * math.sin(2 * i_rad)), fill="#F7FE2E", width=5))
    if i > 0:
        canvas.delete(id_line[id_repere - 1])
    racine.update_idletasks()
    racine.update()
    id_repere = id_repere + 1
timeFin = time.time()
timeBoucle = timeFin - timeDebut
print(timeBoucle)

bouton_sortir = Button(racine,text="Sortir",command=racine.destroy)
bouton_sortir.pack()

ser = serial.Serial('COM5')
while True:
        buffern1 = ser.read(2)
        count = int(len(buffern1)/2)
        sonar = struct.unpack('>'+'H'*count, buffern1)
        #print(sonar[0])
        
        distance = int(sonar[0])
        buffern1 = ser.read(2)
        count = int(len(buffern1)/2)
        sonar = struct.unpack('>'+'H'*count, buffern1)
        
        #print(sonar[0])
        i = int(sonar[0])
        i_rad = 2 * math.pi * (i*2) / 360
        rad = math.radians(i*4)
        x = math.cos(rad) * distance + 300
        y = math.sin(rad) * distance + 300
        id_line.append(canvas.create_line(300, 300, x, y, fill="#F7FE2E", width=5))
        canvas.delete(id_line[id_repere-1])
        id_point.append(canvas.create_line(x,y,x,y+4,fill="#ff0000",width = 4))
        if i == 90:
            remove = True
        if remove == True:
            canvas.delete(id_point[i-1])
            if i == 0:
                it = 90
                while it != 179:
                    canvas.delete(id_point[it])
                    it = it + 1
                id_point = id_tmp
                remove = False  
        racine.update_idletasks()
        racine.update()
        id_repere = id_repere + 1

racine.mainloop()


