Tema 3 - Structuri de Date
Nume: Mihalache Florin-Razvan
Grupa: 313CC

In cadrul acestei teme, am construit o functie de initializare a unui graf (init),
3 functii de citire din fisier (una de vector - build_array - si celelalte doua
de matrici - build_matrix_1 si build_matrix_2), o functie care verifica existenta
unei muchii intre 2 noduri date (check_edge), doua functii de eliberare de memoriei
(free_matrix si free_graph). Am folosit algorimii Dijkstra si Floyd_Warshall,
implementati la laborator, cu adaugarea faptului ca la Dijkstra am mai adaugat
o functie de afisare a caii intre 2 noduri date (print_path). Fiecarui task ii
corespunde cate o functie (task_one, task_two, task_three).

Task 1
In cadrul acestui task, am folosit functia auxiliare determine_farm, care
determina indicele fermei la care drona se intoarce dupa fiecare livrare. Ca la
restul taskurilor, am folosit 4 grafuri: one (prima matrice din fisier, cu
culoarele aeriene), two (a doua matrice din fisier), graph (nodurile si culoarele
aeriene) si distances (distantele minime dintre noduri). Afisarea drumului am
facut-o in felul urmator: mai intai afisez ferma (de start), drumul intre ferma
(de start) si primul client, apoi intr-un for afisez un prim client, drumul de
la client la ferma unde se aprovizioneaza drona, ferma, drumul de la ferma la
al doilea client si apoi al doilea client. Dupa ce se iese din for, se afiseaza
drumul dintre ultimul client si cea mai apropiata ferma, apoi e afisata cea mai
apropiata ferma, dupa aceea pe ultimul rand e afisata suma costurilor minime.
Acest procedeu il aplic (putin adaptat fiecarui task) la restul taskurilor.

Task 2
In cadrul acestui task, am folosit urmatoarele functii auxiliare: number_of_farms
(determina numarul de ferme de un anumit tip, in acest caz de tip 1 - functie
refolosita la taskul 3), list_farms (determina fermele de un anumit tip, in
acest caz de tip 1 - functie refolosita la taskul 3), nearest (determina ferma
cea mai apropiata de un nod), minimum_farm (determina ferma pentru care
drumul parcurs de drone intre 2 clienti este minim, cu tot cu aprovizionare)

Task 3
In cadrul acestui task, am folosit urmatoarele functii auxiliare: backtracking
(generez permutarile unui vector dat), save_line_in_matrix (salvez un vector
in linia unei matrice), maximum_element (cu ajutorul acesteia vad numarul de
tipuri de ferme), minimum_element (cu ajutorul acesteia pot obtine suma minima
din vectorul de sume), swap (interschimbare de 2 valori, folosita la backtracking),
factorial (folosita la permutari), all_farms (sunt memorate toate fermele,
indiferent de tip), apparitions (numarul de aparitii al unui tip de ferma in
vectorul determinat de functia all_farms, pentru a folosi cand lucram cu un
anumit tip de ferma), type_farm (returneaza tipul de ferma pentru o un client dat,
in caz contrar -1), nearest_farm (returneaza cea mai apropiata ferma, exact ca
functia nearest, insa are ca parametru o matrice de ferme) si what_type_farm
(tipul de ferma pentru o ferma data), find_position (returneaza pozitia pentru
un element dat, functie folosita pentru a gasi permutarea optima din matricea
de permutari) si total_sum (folosita pentru a calcula suma de cost minim pentru
fiecare permutare in parte, construind un vector de sume folosit pentru a
alege permutarea cea mai optima).

MENTIUNI:
La testul 2 de la taskul 3 obtin o cale mai scurta decat cea din fisierul de
referinta, mai precis e mai scurta intre clientul 2 si clientul 3 (1 -> 8)
si drona trece prin ferma corecta, vectorul fiind ca cel din fisierul de referinta:
2 1 8 9
