#!/usr/bin/env python3
import math
def first_nonrepeating(s):
    '''

    :param s: Retezec ve kterem se bude vyhledavat
    :return: Prvni neopakujici se znak
    '''
    znak=None
    retezec=str(s)
    znaky = []
    pocet = []


    for c in range(len(retezec)):
        if(c==0):
            znaky.append(retezec[c])
            pocet.append(1)
            continue
        obsahuje=0
        for x in range(len(znaky)):
            if(retezec[c]==znaky[x]):
                obsahuje = 1
                pocet[x]=pocet[x]+1
        if(obsahuje==0):
            znaky.append(retezec[c])
            pocet.append(1)

    for i in range(len(pocet)):
        if(pocet[i]==1):
            znak=znaky[i]
            if(znak<'!' or znak>'~'):
            #if(not((znak>'!' and znak<'~')or(znak>'a' and znak<'z')or(znak>'0' and znak<'9'))):
                znak = None
            return znak
    return None

def combine4(arg, res):
    '''

    :param arg: List se 4 vstupnimi cisli
    :param res: Pozadovany vysledek
    :return: Vsechny mozne kombinace 4 cisel (a matematickych operaci +, -, * a /) ze seznamu arg, ktere vedou k vysledku v res
    '''
    sign = ['+','-','*','/']
    num = arg
    comb = []
    o=0.0
    if not len(arg) == 4 or not type(res) == int:
        return None

    if(num[0]<1 or num[1]<1 or num[2]<1 or num[3]<1):
        return comb

    for i in range(4):
        for j in range(4):
            for k in range(4):
                for q in range(4):
                    for w in range(4):
                        for e in range(4):
                            for r in range(4):
                                #a se nesmi rovnat b!
                                if(q==w or q==e or q==r or w==e or w==r or e==r):
                                    continue
                                # Pro závorkování ((w sign x) sign y) sign z
                                o = oper(num[q], num[w], sign[i])
                                if (o == None):
                                    continue
                                o = oper(o, num[e], sign[j])
                                if (o == None):
                                    continue
                                o = oper(o, num[r], sign[k])
                                if (o == None):
                                    continue

                                o = round(o, 10);
                                if (o == res):
                                    comb.append(
                                        "((" + str(num[q]) + sign[i] + str(num[w]) + ")" + sign[j] + str(
                                            num[e]) + ")" +
                                        sign[k] + str(num[r]))

                                # Pro závorkování w sign (x sign (y sign z))
                                o = oper(num[q], num[w], sign[i])
                                if (o == None):
                                    continue
                                o = oper(num[e], o, sign[j])
                                if (o == None):
                                    continue
                                o = oper(num[r], o, sign[k])
                                if (o == None):
                                    continue

                                o=round(o,10);
                                if (o == res):
                                    #print(str(o)+str(q)+str(w)+str(e)+str(r))
                                    comb.append(
                                        str(num[r]) + sign[k] + "(" + str(num[e]) + sign[j] + "(" + str(
                                            num[q]) +
                                        sign[i] + str(num[w])+"))")

                                #Pro závorkování (w sign x) sign (y sign z)
                                o1 = oper(num[q], num[w], sign[i])
                                if (o == None):
                                    continue
                                o2 = oper(num[e], num[r], sign[k])
                                if (o == None):
                                    continue
                                o = oper(o1, o2, sign[j])
                                if (o == None):
                                    continue

                                o = round(o, 10);
                                if (o == res):
                                    comb.append(
                                        "(" + str(num[q]) + sign[i] + str(num[w]) + ")" + sign[j] + "(" + str(num[e])+
                                        sign[k] + str(num[r]) + ")" )

    #print(list(set(comb)))
    return list(set(comb))

def oper(a,b, sign):
    '''

    :param a: Cislo 1
    :param b: Cislo 2
    :param sign: Pozadovana matematicka operace
    :return: Vysledek po provedeni matematicke operace dane argumentem sign mezi cisli a a b
    '''
    if(sign=='+'):
        return a+b
    elif(sign=='-'):
        return a-b
    elif(sign=='*'):
        return a*b
    elif(sign=='/'):
        if(b==0):
            return None
        else:
            return (a/b)
    else:
        return -1
