

fel_principal = ["mamaliga", "laba","oua cu pui"]
fel_desert = ["tarta cu smantana si branza dulce", "tiramigiu","inghetata de cariceps"]
bautura = ["cola","fanta","sprita"]

comanda_totala = []

print("Incepe comanda /n")
print("Avem fel principal, secundar, desert si bauturi de club")
   a = fel_principal()
  b = fel_secundar()
  c = fel_bautura()
  print("Comanda dvs este:/n")
  print(a)
  print("/n")
  print(b)
  print("/n")
  print(c)




def fel_principal_metoda(): 
    print("Felurile principale sunt:")
    print(fel_principal)
    while True :
        var selectie = str(input("Ce fel principal doriti?"))
         if fel_principal.count(selectie) > 0 : 
             return selectie    
             break
         else :
                 print("Nu avem pe stoc")

def fel_secundar_metoda(): 
    print("Felurile secundare sunt:")
    print(fel_secundar)
    while True :
        var selectie = str(input("Ce fel secundar doriti?"))
         if fel_secundar.count(selectie) > 0 : 
             return selectie    
             break
         else :
            print("Nu avem pe stoc")


def fel_bautura_metoda(): 
    print("Bauturile sunt:")
    print(bautura)
    while True :
        var selectie = str(input("Ce bautura doriti?"))
         if bautura.count(selectie) > 0 : 
             return selectie    
             break
        else :
                 print("Nu avem pe stoc")







with open('muie.txt', 'w') as mon_fichier:
    mon_fichier.write("Comanda clientului Mr Alex Zen este: + '\n')
    mon_fichier.write(a+ '\n')
    mon_fichier.write(b+ '\n')
    mon_fichier.write(c+ '\n')
    mon_fichier.close