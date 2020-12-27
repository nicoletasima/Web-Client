	PROTOCOALE DE COMUNICATIE
	Client Web. Comunicatie cu REST API
	Sima Nicoleta-Lavinia


	Pentru realizarea temei in C am ales sa folosesc biblioteca parson
pentru parsarea payload-ului in format JSON deoarece mi s-a parut foarte 
usor de folosit in urma parcurgerii tutorialului de pe github.
	Am folosit fisierele si functiile din cadrul laboratorului 10. 
Mi-au fost foarte utile functiile send_to_server, receive_from server, precum
si cele care formau cererea pentru trimitere la server pe care le-am adaptat
conform cerintei si ideilor mele.
	In cadrul functiei main, am considerat bucla while in care citesc
un sir de caractere de la server pana la intalnirea comenzii care produce
inchiderea programului (exit). 
	Utilizatorul poate introduce de la tastatura urmatoarele comenzi:
	-register: folosita pentru a crea un cont nou. In acest caz, ofer promt
pentru introducerea username-ului si a parolei. Folosesc functia fgets pentru a
citi de la tastatura toate informatiile necesare crearii cererii. Folosind 
functiile din parson.h si parson.c formez payload-ul pe care vreau sa il adaug cererii.
Am modificat functia compute_post_request de la laborator astfel incat sa concateneze
direct un sir de caractere la cerere reprezentand payload-ul. In rest, adaugarea 
headerelor este la fel ca in laboratorul 10. Deschid intotdeauna conexiunea cu serverul
dupa citirea datelor de la tastatura si crearea mesajului deoarece dupa 30 de secunde
expira si ofer astfel timp utilizatorului sa introduca datele. Folosesc macro-ul
DIE in cadrul temei pentru a semnala eroarea la deschiderea conexiunii. Folosind
send_to_server trimit cererea la server si primesc raspunsul prin receive_from_server.
Am ales sa afisez un mesaj acolo unde serverul nu facea acest lucru deja pentru a fi 
mai user friendly. De asemenea, am lasat si afisarea completa a raspunsului deoarece 
vazand raspunsul primit, am inteles mai bine cum functioneaza protocolul HTTP.
	-login: la fel ca la register se citesc datele de la tastatura si se formeaza
payload-ul de tip json. Pentru raspunsul Ok de la server afisez si un mesaj 
pentru a confirma logarea, iar in caz contrar un mesaj corespunzator. Aici am creat
functia care extrage cookie-ul din raspuns. get_cookie_from_response: parcurg
raspunsul folosind strtok, iar la linia la care se afla cookie-ul folosesc iar 
strtok pentru a extrage fix ceea ce imi este necesar. 
	-enter_library: Pentru enter_library apelez functia compute_get_request
care creaza o cerere de tip get si concateneaza si cookie-ul la ea. Pentru
cazul in care raspunsul de la server este OK, afisez un mesaj de confirmare deoarece
serverul nu facea acest lucru. In caz contrar se va afisa direct de la server 
faptul ca s-a produs o eroare, cel mai frecvent caz fiind faptul ca nu logarea
nu a avut loc. Din raspuns extrag token-ul JWT folosinf functia get_authorization_token
creata de mine. La fel ca la cookie, extrag token-ul folosind strtok si il retin
pentru cand voi aveam nevoie de el.
	- get_books: aici apelez o functie de get pe care am creat-o astfel incat sa
concateneze tokenul la cerere: auth_compute_get_request (se afla in requests.c).
In cazul in care nu exista nicio carte in cont se afiseaza lista vida. 
	- get_book dupa id. Citesc id-ul de la tastura ca un sir de caractere si folosind
functia sprintf si atoi ul adaug rutei de acces catre server. Serverul afiseaza
si pentru Ok si pentru alt caz de cerere gresita un mesaj. 
	- add_book: dupa citirea de la tastatura, creez payload-ul in formal JSON
si folosind auth_get_requests din requests.c concatenez tokenul JWT la cerere pentru
demonstrarea accesului. Pentru raspunsul OK de la server am ales sa afisez un mesaj
corespunzator. Pentru cazul in care nu exista acces la biblioteca serverul va mentiona
acest lucru in raspuns.
	- Pentru delete_book, am format tot folosind sprintf ruta corecta catre server
si am creat, de asemenea, functia care formeaza cererea de stergere asemanatoare cu 
GET si POST.
	-logout: Se trimite o cererea de tip GET la server in care se concateneaza si
cookie-ul de login. Afisez mesajul de logout pentru logarea realizata cu succes.
	- Ultima ramura este pentru cazul in care comanda introdusa nu este valida. 
Programul va rula in continuare prin afisarea unul mesaj care demonstreaza
acest lucru si permiterea intrducerii unei comenzi valide.


Timp aproximativ de lucru: aproximativ 10 ore deoarece in prima faza mi-a fost dificil sa inteleg
cum functioneaza biblioteca de parsare precum si notiuni ca token-ul JWT. Nu am intampinat multe
dificultati dupa ce am inteles cum functioneaza protocolul. Tema aceasta m-a ajutat foarte
mult sa inteleg mai bine notiunile din cadrul laboratorului 10 deoarece la momentul
la care am participat la laborator nu mi-a fost foarte clar cum are loc comunicarea cu serverul.
Tema in sine mi-a placut si nu mi s-a parut greu de realizat comparativ cu celelalte 2 teme.
