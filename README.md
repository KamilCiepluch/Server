Wstęp
W ramach projektu z komunikacji międzyprocesowej należy przygotować prostą grę planszową dla czterech niezależnych graczy, grających równolegle. Architekturę gry należy zaprojektować tak, aby gracze-procesy komunikowały się z serwerem-procesem. Zarówno gracze, jak i serwer mają być oddzielnymi procesami.

Ponieważ graczem może być zarówno komputer jak i człowiek to sugerowane jest, aby całość składała się z trzech programów:

serwera gry,
klienta gracza,
klienta bota.

Specyfikacja gry
Gracze (1, 2, 3, 4) zamknięci są w labiryncie i mają za zadanie zbierać pojawiające się skarby w postaci monet (c, t, T). Gracz, który zbierze wystarczająco dużo skarbów, zanosi je do obozowiska ( A ) i tam zostawia. Gracz może w danej chwili nosić dowolną liczbę monet (carried) ale może je stracić w wyniku ataku dzikiej bestii (*) lub poprzez zderzenie się z innym graczem.

W przypadku ataku dzikiej bestii gracz ginie (deaths), a zebrany przez niego łup pozostaje w miejscu śmierci (D). Gracz respawnuje się w swoim punkcie startowym.
W przypadku zderzenia z innym graczem łupy obu pozostają w miejscu zderzenia (D), a gracze respawnują się w swoich punktach startowych.
Pozostawiony łup (D) ma swoją wartość. W przypadku zderzenia jest to suma noszonych monet obu graczy.
Gracz pozbywa się swoich monet w obozowisku (A), gdzie zapisywane są one na jego konto (budget). Po zdaniu skarbu gracz kontynuuje poszukiwania, zaczynając od obozowiska.

Typy graczy
Należy przygotować dwa typy graczy: komputer – bot (CPU) oraz człowiek (HUMAN).

Postać gracza-bota porusza się autonomicznie na podstawie mapy przekazywanej przez serwer.
Postać gracza-człowieka poruszana jest za pomocą klawiszy - strzałek (w górę, w dół, w lewo, w prawo).
Gracz CPU może poruszać się z wykorzystaniem dowolnego algorytmu, np. chaotycznie, A* w dowolny punkt, A* z eksploracją, RL, lewa ściana, itp. Gracz CPU powinien również reagować na pojawienie się bestii, ratując się ucieczką.





Serwer oraz wszyscy czterej gracze muszą byś osobnymi procesami. Boty nie są wymagane.
Bestie muszą być zaimplementowane jako oddzielne wątki na serwerze (1 bestia = 1 wątek).
Kolejność uruchamiania procesów jest następująca:
Proces serwera.
Klienty graczy 1-4.
W przypadku awarii/restartu serwera dopuszczalne jest wprowadzenie wymogu restartu klientów
