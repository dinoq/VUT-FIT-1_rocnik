#!/usr/bin/env python3
def can_be_a_set_member_or_frozenset(item):
    #zkoušíme, zda jde vytvořit množina z item. Pokud ano, vrátí se item, jinak se vygeneruje vyjímka a vrátí se frozenset.
    try:
        prom={item}
    except:
        return frozenset(item)
    return item

assert can_be_a_set_member_or_frozenset(1) == 1
assert can_be_a_set_member_or_frozenset((1, 2)) == (1, 2)
assert can_be_a_set_member_or_frozenset([1, 2]) == frozenset([1, 2])

def all_subsets(lst):
    #Vytvoříme seznam, do kterého jako první vložíme prázdný seznam
    sub=[[]]

    for i in range(0,len(lst)):
        for j in range(0,len(sub)):
            #V tomto vnořeném cyklu přidáváme při každé iteraci do námi vytvořeného seznamu sub požadované seznamy, které vytváříme z dříve vytvořených + ze seznamu lst
            sub.append(sub[j]+[lst[i]])
            print((sub[j]+[lst[i]]))
            print(len(sub))
    return sub

assert all_subsets(['a', 'b']) == [[], ['a'], ['b'], ['a', 'b']]
assert all_subsets(['a', 'b', 'c']) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

def all_subsets_excl_empty(*elements, exclude_empty = None):
    #vytvoříme si seznam s názvem lst z přijímaných argumnetů elements
    lst = list(elements)
    #Vytvoříme seznam, do kterého jako první vložíme prázdný seznam
    sub=[[]]
    # V tomto vnořeném cyklu přidáváme při každé iteraci do námi vytvořeného seznamu sub požadované seznamy, které vytváříme z dříve vytvořených + ze seznamu lst
    for i in range(0,len(lst)):
        for j in range(0,len(sub)):
            sub.append(sub[j]+[lst[i]])
    # kontrola zda má seznam, který vracíme zahrnovat prázdný seznam. Pokud ne, odstraní první prvek (kterým je právě prázdný seznam)
    if(exclude_empty != False):
        sub.pop(0)
    return sub

assert all_subsets_excl_empty('a', 'b', 'c') == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = True) == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = False) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]