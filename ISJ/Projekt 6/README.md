Zadání projektu 6:

Do souboru, nazvaného podle konvence isj_proj06_xnovak00.py, implementujte:

- funkci first_nonrepeating, která dostane na vstup řetězec a vrátí první neopakující se znak ze vstupního řetězce:
tooth -> h, lool -> None

- funkci combine4, která dostane čtveřici 4 kladných celých čísel a očekávaný výsledek a vrátí setříděný seznam  unikátních řešení matematických hádanek s výsledkem operací +, -, *, / nad 4 čísly - viz např. http://blog.plover.com/math/17-puzzle.html
Například combine4([6,6,5,2],36) by mohlo vrátit něco jako:
['(2+5)*6-6', '(5+2)*6-6', '6*(2+5)-6', '6*(5+2)-6'],
případně jen ['(2+5)*6-6'], pokud se vám podaří odstraňovat výrazy kvůli komutativitě sčítání a násobení,
případně i delší seznam, jehož prvky se budou lišit jen uzávorkováním.

Řešení si můžete zkontrolovat v novém systému na adrese http://isj.fit.vutbr.cz:8090/proj. Případné problémy s kontrolním systémem prosím hlaste Juraji Kyseľovi - xkysel12@stud.fit.vutbr.cz - který systém vytváří. Odevzdání opět prostřednictvím WIS.