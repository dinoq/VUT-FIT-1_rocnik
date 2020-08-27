#!/usr/bin/env python3
class Polynomial:
    #Slouzi pro uchovani polynomu, kde jednotlive prvky listu budou jednotlive mantisy polynomu
    polynom=[]

    #Inicializace objektu
    def __init__(self, *pol, **members):
        self.polynom = []
        #Pokud byli zadany argumenty(resp. argument)
        if(len(pol)>0):
            #Pokud byl zadany jeden argument - cely polynom
            if(len(pol)==1):
                self.polynom = pol[0]
            #Pokud byli zadany jednotlive mantisy za sebou
            else:
                for x in range(len(pol)):
                    self.polynom.append(pol[x])
        #Pokud byly zadány argumenty přímo ve tvaru (např.) x0=neco,x2=neco atd...
        if(len(members)>0):
            dict={}
            dict2={}
            key=[]
            full=[]
            max=-1

            #najde maximalni mocninu pro naslednou implementaci prazdneho seznamu "full" naplneneho nulami
            for actual in members:
                actual=actual[1:2]
                if(int(actual)>max):
                    max=int(actual)
            #naplni list full nulami
            for i in range(max+1):
                full.append(0)
            #Vytvori slovnik z argumentu (kde klic je nazev a hodnota je predavana hodnota)
            for x in members.keys():
                dict[x]=members[x]
            #ulozime si serazene klice ze slovniku dict
            key=sorted(dict.keys(), key=lambda x:x.lower())
            #do slovniku dict2 okopirujeme dict ale serazeny
            for k in key:
                dict2[k] = dict[k]
            #do listu full doplnime hodnoty mantis (ty ktere se nerovnaji nula)
            for k, di in zip(key,dict2.values()):
                full[int(k[1:])]=di
            #Odkaz na list full ve kterem se nachazi mantisy dame do listu polynom
            self.polynom=full
        #Orezeme nuly na konci
        for i in reversed(range(len(self.polynom))):
            if((self.polynom[i]==0)):
                self.polynom.pop()
            else:
                return

    #Prepiseme zpusob jak se bude objekt tisknout
    def __str__(self):
        #Do tmp si ulozime puvodni list, ale otoceny
        tmp=self.polynom[::-1]
        l=len(tmp)-1
        #vysledny retezec, ktery predstavuje polynom
        retezec=""
        #V tomto cyklu vytvarime retezec z listu polynom
        for i in range(len(self.polynom)):
            mocnina=len(self.polynom)-i-1
            #Chceme absolutni hodnotu mantisy, protoze pozdeji do retezce pridavame znamenko zvlast
            mantisa=abs(tmp[i])
            #pokud se daná mocnina v polynomu nenachází, přeskoč cyklus
            if(tmp[i]==0):
                continue
            #nastaví znaménko
            if(tmp[i]>0):
                sign="+ "
            else:
                sign="- "
            #Nastavi jednak mantisu a pak take retezec, ktery za ni bude nasledovat
            if(mocnina>1):
                if (mantisa == 1):
                    mantisa = ""
                umocneni = "x^" + str(mocnina)+" "
            elif(mocnina==1):
                if (mantisa == 1):
                    mantisa = ""
                umocneni = "x "
            else:
                umocneni=" "

            #Ziskanou mantisu a "umocneni" pripoji k retezci
            #Pokud se jedna o pocatecni clen a je kladny tak retezec se nastavi
            if((not retezec) and (tmp[i]>0)):
                retezec += str(mantisa) + umocneni
            #jinak se nastavi zde (bude tisknout i znamenko)
            else:
                retezec += sign + str(mantisa) + umocneni
        #Pokud je retezec prazdny(napriklad se jedna o prazdny polynom), vrati nulu
        if(not retezec):
            return "0"
        #Pokud retezec neni prazdny tak jej vrati krome posledniho znaku, coz je mezera
        return retezec[:len(retezec)-1]

    # Prepiseme zpusob porovnavani objektu vytvorenych ze tridy Polynomial
    def __eq__(self, pol):
        #Porovnavame polynomy objektu
        return (self.polynom==pol.polynom)

    #Prepisuje scitani
    def __add__(self, pol):
        tmp=[]
        #Najde vetsi polynom a velikost vetsiho ulozi do promenne max
        if(len(self.polynom)>len(pol.polynom)):
            max=len(self.polynom)
        else:
            max = len(pol.polynom)
        #Do listu tmp ulozi vysledny polynom (soucet polynomu)
        for i in range(max):
            #Pokud je index "i" mensi nez velikost nektereho z polynomu tak je secti a uloz do promenne soucet
            if((i<len(pol.polynom))and(i<len(self.polynom))):
                soucet=self.polynom[i]+pol.polynom[i]
            #jinak (pokud je na tomto indexu jeden z polynumu nulovy) uloz do promenne hodnotu na indexu "i" delsiho polynomu
            else:
                if(max==len(self.polynom)):
                    soucet=self.polynom[i]
                else:
                    soucet=pol.polynom[i]
            #Na konci kazdeho cyklu pripoj nakonec listu vysledek ulozeny v promenne soucet
            tmp.append(soucet)
        #Nakonec vytvor z listu "tmp" objekt typu Polynomial a pouzij jej jako navratovou hodnotu
        return Polynomial(tmp)

    #Prepisuje umocneni
    def __pow__(self, power, modulo=None):
        pol = Polynomial(self.polynom)
        #Pokud je mocnina 1, vrat ten samy polynom
        if(power==1):
            return pol
        pol2=self.polynom
        #Cuklus ve kterem se uklada vysledek nasobeni polynomu. Poprve nasobime puvodni polynom krat puvodni polynom a pozdeji vzdy ziskany polynom krat puvodni polynom
        for i in range(power-1):
            mezivysledek=self.multiplyPol(self.polynom,pol2)
            pol2=mezivysledek
        #Nakonec posledni ziskane nasobeni polynomu je pozadovana mocnina puvodniho polynomu a tento list ulozime do objektu pol a vratime jako navratovou hodnotu
        pol.polynom = mezivysledek
        return pol

    #funkce vynásobí mezi sebou 2 polygony a vrátí výsledek
    def multiplyPol(self, pol1, pol2):
        tmp=[]
        #Naplni list "tmp" nulami
        for i in range(len(pol1) + len(pol2)):
            tmp.append(0)
        #Vnoreny cyklus, ktery na jednotlivych indexech listu "tmp" ulozi vysledek po vynasobeni zadanych argumentu (polynomu pol1 a pol2)
        for i in range(len(pol1)):
            for j in range(len(pol2)):
                tmp[i + j] += pol1[i] * pol2[j]
                mezivysledek = tmp[:]
        return mezivysledek

    #Funkce pro derivaci polynomu
    def derivative(self):
        tmp = []
        # Naplni list "tmp" nulami
        for i in range(len(self.polynom)):
            tmp.append(0)
        #V cyklu zderivujeme jednotlive cleny polynomu a ulozime do temp
        for x in range(len(self.polynom)):
            tmp[x]=x*self.polynom[x]
        #Vynechej první prvek (derivace konstanty je 0)
        tmp=tmp[1:]
        # Nakonec vytvor z listu "tmp" objekt typu Polynomial a pouzij jej jako navratovou hodnotu
        return(Polynomial(tmp))

    #Vycisleni polynomu
    def at_value(self,*x):
        res1=0
        #umocni a uloz do vysledku
        for g in range(1,len(self.polynom)):
            res1+=self.polynom[g]*(x[0]**g)
        res1+=self.polynom[0]
        #Pokud byl zadany jeden argument tak vrat
        if(len(x)==1):
            return res1
        #jinak vypocitej druhy argument a vrat jejich rozdil
        if(len(x)==2):
            res2=0
            for g in range(1, len(self.polynom)):
                res2 += (self.polynom[g])*( x[1]** g)
            res2 += self.polynom[0]
            return(res2-res1)
def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()