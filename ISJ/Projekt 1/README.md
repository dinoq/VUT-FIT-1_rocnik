Zadání projektu 1:

Stáhněte si soubor https://www.fit.vutbr.cz/study/courses/ISJ/private/isj_proj01_xnovak00.py a místo xnovak00 dejte do názvu svůj login.


Dopište definici regulárního výrazu word_parse tak, aby do skupin rozděloval slova z řetězce, oddělovaného mezerami, za nimiž nenásleduje konstrukce (P...), a slova, za nimiž tato konstrukce následuje (v tomto případě do další skupiny obsah závorky).
Například ve vstupu:
bee(P: insect honey) dog  cat (P:milk) ant(P) ape
jsou slova 'dog' a 'ape' prvního typu,
s 'bee' je asociováno 'insect honey'
s 'ant' nic (prázdný řetězec)
Mělo by být tedy možné (jak to dělá skript) získat trojice:
('', 'bee', 'insect honey')
('dog', '', '')
('', 'cat', 'milk')
('', 'ant', '')
('ape', '', '')
Dopište definici regulárního výrazu punct tak, aby odpovídal pozici za čárkou nebo tečkou, s výjimkou případu, kdy bezprostředně před znakem a za daným místem stojí číslice (a aby tedy ani následný test assert nevypisoval chybu).

Nic jiného v kódu neměňte (i kdyby se vám nelíbil nedostatek komentářů apod.).
Cvičné hodnocení můžete vyzkoušet v systému http://athena3.fit.vutbr.cz:8088/proj1, ale pro získání bodů za projekt musíte výsledný skript (se správným názvem) odevzdat prostřednictví WIS.
