import tkinter as tk

# Creating a TK interface with a text console
root = tk.Tk()
#T = Text(root, height=20, width=60)
#T.pack(padx=30,pady=30,side=TOP)
frame = tk.Frame(root)
frame.pack()

button_play = tk.Button(frame,
					text="Connect",
					command="insert") 
button_play.pack(side=tk.TOP)

#if (input()):
#	insert(T, "DANIEL")
   

root.mainloop()