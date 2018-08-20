"""

AUDIO RECORDER

O script executa a funcao "audio_recorder_callback_mode,py" de modo que possibilite ao usuario
visualizar/editar melhor os parâmentros utilizados


"""



print("")

"""
name = input("What's your name? ")
print("Nice to meet you " + name + "!")
age = input("Your age? ")
print("So, you are already " + str(age) + " years old, " + name + "!")


"""
#!/usr/bin/python3

from tkinter import *
import os.path

fields = ('Audio file name (.wav)', 'Length (s)', 'Bits for quantization (nº of bits)', 'New sample rate (Hz)')

def monthly_payment(entries):
   # period rate:
   r = (float(entries['Annual Rate'].get()) / 100) / 12
   print("r", r)
   # principal loan:
   loan = float(entries['Loan Principle'].get())
   n =  float(entries['Number of Payments'].get())
   remaining_loan = float(entries['Remaining Loan'].get())
   q = (1 + r)** n
   monthly = r * ( (q * loan - remaining_loan) / ( q - 1 ))
   monthly = ("%8.2f" % monthly).strip()
   entries['Monthly Payment'].delete(0,END)
   entries['Monthly Payment'].insert(0, monthly )
   print("Monthly Payment: %f" % float(monthly))

def save_parameters(entries):

    print("SALVAR VARIAVEIS")

   #get audio name
    audio_name = entries['Audio file name (.wav)'].get()
   #get audio length from input:
    audio_len = int(entries['Length (s)'].get())

   #get number of bits for quantization
    audio_format =int(entries['Bits for quantization (nº of bits)'].get())

    new_rate = int(entries['New sample rate (Hz)'].get())

    print(audio_name,audio_len,audio_format,new_rate)

    arq = open('parameters.txt','w')
    arq.seek(0)   #puts cursor at first position
    arq.write(audio_name+str(audio_len)+"\n"+str(audio_format)+"\n"+str(new_rate)+"\n")


def makeform(root, fields):

    arq = open('parameters.txt','r')

    fields_str = arq.readlines()

    print(fields_str)

    i = 0;
    entries = {}
    for field in fields:
        row = Frame(root)
        lab = Label(row, width=22, text=field+": ", anchor='w')
        ent = Entry(row)
        ent.insert(0,fields_str[i])
        row.pack(side=TOP, fill=X, padx=5, pady=5)
        lab.pack(side=LEFT)
        ent.pack(side=RIGHT, expand=YES, fill=X)
        entries[field] = ent
        i = i+1
    return entries

if __name__ == '__main__':
   root = Tk()
   ents = makeform(root, fields)
   root.bind('<Return>', (lambda event, e=ents: fetch(e)))
   b1 = Button(root, text='Save Parameters',
          command=(lambda e=ents: save_parameters(e)))
   b1.pack(side=LEFT, padx=5, pady=5)
   b2 = Button(root, text='Monthly Payment',
          command=(lambda e=ents: monthly_payment(e)))
   b2.pack(side=LEFT, padx=5, pady=5)
   b3 = Button(root, text='Quit', command=root.quit)
   b3.pack(side=LEFT, padx=5, pady=5)
   root.mainloop()
