#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#define MAX_UCESNIKA 100
#define MAX_DOGADJAJA 50
#define MAX_ULOGA 200
#define IME_DUZINA 50

typedef struct {
    int id;
    char ime[IME_DUZINA];
    float saldo;
} Ucesnik;

typedef struct {
    int id;
    char tim1[IME_DUZINA];
    char tim2[IME_DUZINA];
    float kvota1;
    float kvota2;
    char datum[11]; // Format "YYYY-MM-DD"
    int odigran; // 0 - nije odigran, 1 - odigran
} Dogadjaj;

typedef struct {
    int idUcesnika;
    int idDogadjaja;
    int izbor; // 1 za tim1, 2 za tim2
    float ulog;
    int odigrano; // 0 - nije odigrano, 1 - odigrano
} Ulog;

Ucesnik ucesnici[MAX_UCESNIKA];
Dogadjaj dogadjaji[MAX_DOGADJAJA];
Ulog ulozi[MAX_ULOGA];

int brojUcesnika = 0, brojDogadjaja = 0, brojUloga = 0;
 
void dodajUcesnika(char* ime, float saldo) {
   
    if (brojUcesnika >= MAX_UCESNIKA) {
        printf("Dostignut maksimalan broj ucesnika.\n");
        return;
    }
    if (saldo < 0) {
        printf("Saldo ne moze biti negativan. Pokusajte ponovo.\n");
        return;
    }
    Ucesnik u;
    u.id = brojUcesnika;
    strncpy(u.ime, ime, IME_DUZINA - 1);
    u.ime[IME_DUZINA - 1] = '\0'; // Obezbediti da string bude null-terminated naso na internetu
    u.saldo = saldo;
    ucesnici[brojUcesnika++] = u;
}

void dodajDogadjaj(char* tim1, char* tim2, float kvota1, float kvota2, char* datum) {
   
    if (brojDogadjaja >= MAX_DOGADJAJA) {
        printf("Dostignut maksimalan broj dogadjaja.\n");
        return;
    }
    if (kvota1 < 1 || kvota2 < 1) {
        printf("Kvote moraju biti vece od 1. Pokusajte ponovo.\n");
        return;
    }
    if (kvota1 == 1 || kvota2 == 1) {
        printf("Kvote moraju biti vece od 1. Pokusajte ponovo.\n");
        return;
    } 
    Dogadjaj d;
    d.id = brojDogadjaja;
    strncpy(d.tim1, tim1, IME_DUZINA - 1);//Kopira naziv prvog tima tim1 u polje d.tim1
    d.tim1[IME_DUZINA - 1] = '\0';
    strncpy(d.tim2, tim2, IME_DUZINA - 1);
    d.tim2[IME_DUZINA - 1] = '\0';
    d.kvota1 = kvota1;
    d.kvota2 = kvota2;
    strncpy(d.datum, datum, 10);
    d.datum[10] = '\0';
    d.odigran = 0;
    dogadjaji[brojDogadjaja++] = d;
}


void obradiDogadjaje() {
    time_t trenutnoVreme;
    time(&trenutnoVreme);
    struct tm vremeDogadjaja = { 0 };
    double razlika;
    int imaNeodigranih = 0; // Da li ima neodigranih dogadjaja

    for (int i = 0; i < brojDogadjaja; i++) {
        if (!dogadjaji[i].odigran) {
            imaNeodigranih = 1; // Postavljamo da ima neodigranih dogadjja
            sscanf(dogadjaji[i].datum, "%d-%d-%d", &vremeDogadjaja.tm_year, &vremeDogadjaja.tm_mon, &vremeDogadjaja.tm_mday);
            vremeDogadjaja.tm_year -= 1900;//od ove godine
            vremeDogadjaja.tm_mon -= 1;

            razlika = difftime(trenutnoVreme, mktime(&vremeDogadjaja));

            if (razlika > 0) {
                int pobednik = (rand() % 2) + 1; // 1 za tim1, 2 za tim2
                printf("U dogadjaju izmedju %s i %s, pobednik je: %s\n",
                    dogadjaji[i].tim1, dogadjaji[i].tim2,
                    pobednik == 1 ? dogadjaji[i].tim1 : dogadjaji[i].tim2);

                for (int j = 0; j < brojUloga; j++) {
                    if (ulozi[j].idDogadjaja == dogadjaji[i].id && !ulozi[j].odigrano) {
                        float kvota = (ulozi[j].izbor == pobednik) ? ((pobednik == 1) ? dogadjaji[i].kvota1 : dogadjaji[i].kvota2) : 0;
                        float isplata = ulozi[j].ulog * kvota;
                        ucesnici[ulozi[j].idUcesnika].saldo += isplata;
                        ulozi[j].odigrano = 1;

                        if (kvota > 0) {
                            printf("Igracu %s isplaceno: %.2f\n", ucesnici[ulozi[j].idUcesnika].ime, isplata);
                        }
                        else {
                            printf("Igrac %s nije dobio isplatu.\n", ucesnici[ulozi[j].idUcesnika].ime);
                        }
                    }
                }
                dogadjaji[i].odigran = 1;
            }
        }
    }

    // Da li ima neodigranih dogadjaja
    if (!imaNeodigranih) {
        printf("Trenutno nema neodigranih dogadjaja.\n");
    }
}





void dodajUlog(int idUcesnika, int idDogadjaja, int izbor, float ulog) {
   
    if (brojUloga >= MAX_ULOGA) {
        printf("Dostignut maksimalan broj uloga.\n");
        return;
    }
    if (idUcesnika >= brojUcesnika || idDogadjaja >= brojDogadjaja) {
        printf("Nevazeci ID ucesnika ili dogadaja.\n");
        return;
    }
    if (ucesnici[idUcesnika].saldo < ulog) {
        printf("Nedovoljno sredstava za ulog.\n");
        return;
    }
    if (ulog <= 0) {
        printf("Ulog mora biti pozitivan broj. Pokusajte ponovo.\n");
        return;
    }
    if (izbor != 1 && izbor != 2) {
        printf("Izbor mora biti 1 ili 2. Pokusajte ponovo.\n");
        return;
    }
    if (dogadjaji[idDogadjaja].odigran) {
        printf("Dogadjaj na koji pokusavate postaviti ulog je vec odigran.\n");
        return;
    }
    Ulog u;
    u.idUcesnika = idUcesnika;
    u.idDogadjaja = idDogadjaja;
    u.izbor = izbor; // Dodajemo izbor u strukturu Ulog
    u.ulog = ulog;
    u.odigrano = 0; // Inicijalno postaviti da ulog nije odigran
    ulozi[brojUloga++] = u;
    ucesnici[idUcesnika].saldo -= ulog;
}

void uplatiNovac(int idUcesnika, float iznos) {
   
    if (idUcesnika < 0 || idUcesnika >= brojUcesnika) {
        printf("Nevazeci ID ucesnika.\n");
        return;
    }
    if (iznos <= 0) {
        printf("Iznos mora biti pozitivan broj.\n");
        return;
    }
    ucesnici[idUcesnika].saldo += iznos;
    printf("Uplatili ste %.2f na racun ucesnika %s.\n", iznos, ucesnici[idUcesnika].ime);
}

void isplatiNovac(int idUcesnika, float iznos) {
   
    if (idUcesnika < 0 || idUcesnika >= brojUcesnika) {
        printf("Nevazeci ID ucesnika.\n");
        return;
    }
    if (iznos <= 0) {
        printf("Iznos mora biti pozitivan broj.\n");
        return;
    }
    if (ucesnici[idUcesnika].saldo < iznos) {
        printf("Nedovoljno sredstava za isplatu.\n");
        return;
    }
    ucesnici[idUcesnika].saldo -= iznos;
    printf("Isplatili ste %.2f sa racuna ucesnika %s.\n", iznos, ucesnici[idUcesnika].ime);
}

void ispisiMeni() {
    printf("1. Dodaj ucesnika\n");
    printf("2. Dodaj dogadjaj\n");
    printf("3. Postavi ulog\n");
    printf("4. Prikazi sve ucesnike\n");
    printf("5. Prikazi sve dogadjaje\n");
    printf("6. Obradi dogadjaje\n");
    printf("7. Prikazi sve uloge\n");
    printf("8. Uplati novac ucesniku\n");
    printf("9. Isplati novac ucesniku\n");
    printf("0. Izlaz\n");
    printf("Izaberite opciju: ");
}

void obradiOpciju(int opcija) {
    char ime[IME_DUZINA];
    char naziv[IME_DUZINA];
    float saldo, kvota, ulog;
    int idUcesnika, idDogadjaja;

    switch (opcija) {
        char tim1[IME_DUZINA], tim2[IME_DUZINA], datum[11];
        float kvota1, kvota2, ulog;
        int idUcesnika, idDogadjaja, izbor;
    case 1:
        printf("Unesite ime ucesnika: ");
        scanf("%49s", ime, _countof(ime));
        printf("Unesite saldo: ");
        scanf("%f", &saldo);
        dodajUcesnika(ime, saldo);
        break;
    case 2:
        printf("Unesite ime prvog tima: ");
        scanf("%49s", tim1);
        printf("Unesite ime drugog tima: ");
        scanf("%49s", tim2);
        printf("Unesite kvotu za prvi tim: ");
        scanf("%f", &kvota1);
        printf("Unesite kvotu za drugi tim: ");
        scanf("%f", &kvota2);
        printf("Unesite datum dogadjaja (YYYY-MM-DD): ");
        scanf("%10s", datum);
        dodajDogadjaj(tim1, tim2, kvota1, kvota2, datum);
        break;
    case 3:
        printf("Unesite ID ucesnika: ");
        scanf("%d", &idUcesnika);
        printf("Unesite ID dogadjaja: ");
        scanf("%d", &idDogadjaja);
        printf("Unesite izbor (1 za prvi tim, 2 za drugi tim): ");
        scanf("%d", &izbor);
        printf("Unesite ulog: ");
        scanf("%f", &ulog);
        dodajUlog(idUcesnika, idDogadjaja, izbor, ulog);
        break;
    case 4:
        for (int i = 0; i < brojUcesnika; i++) {
            printf("ID: %d, Ime: %s, Saldo: %.2f\n", ucesnici[i].id, ucesnici[i].ime, ucesnici[i].saldo);
        }
        break;
    case 5:
        printf("\nDogadjaji:\n");
        for (int i = 0; i < brojDogadjaja; i++) {
            printf("ID: %d, Tim 1: %s vs. Tim 2: %s, Kvota1: %.2f, Kvota2: %.2f, Datum: %s, Odigran: %s\n",
                dogadjaji[i].id, dogadjaji[i].tim1, dogadjaji[i].tim2,
                dogadjaji[i].kvota1, dogadjaji[i].kvota2,
                dogadjaji[i].datum,
                dogadjaji[i].odigran ? "Da" : "Ne");
        }
        break;

    case 6:
        // Nova opcija za obradu dogadjaja
        obradiDogadjaje();
        printf("Dogadjaji obradjeni.\n");
        break;
    case 7:
        // Nova opcija za prikazivanje svih uloga
        printf("\nUlozi:\n");
        for (int i = 0; i < brojUloga; i++) {
            printf("Ulog ID: %d, Ucesnik ID: %d, Dogadjaj ID: %d, Izbor: %d, Ulog: %.2f, Odigrano: %d\n",
                i, ulozi[i].idUcesnika, ulozi[i].idDogadjaja, ulozi[i].izbor, ulozi[i].ulog, ulozi[i].odigrano);
        }
        break;
    case 8: {
        int idUcesnika;
        float iznos;
        printf("Unesite ID ucesnika: ");
        scanf("%d", &idUcesnika);
        printf("Unesite iznos za uplatu: ");
        scanf("%f", &iznos);
        uplatiNovac(idUcesnika, iznos);
        break;
    }
    case 9: {
        int idUcesnika;
        float iznos;
        printf("Unesite ID ucesnika: ");
        scanf("%d", &idUcesnika);
        printf("Unesite iznos za isplatu: ");
        scanf("%f", &iznos);
        isplatiNovac(idUcesnika, iznos);
        break;
    }

    default:
        printf("Nepoznata opcija.\n");
    }
}

void ucitajPodatke(const char* putanjaFajla) {
    FILE* f;
    char linija[256];
    int sekcija = 0; // 0: nepoznato, 1: ucesnici, 2: dogadjaji, 3: ulozi

    if (fopen_s(&f, putanjaFajla, "r") != 0) {
        printf("Greska pri otvaranju fajla za čitanje!\n");
        return;
    }

    while (fgets(linija, sizeof(linija), f)) {
        linija[strcspn(linija, "\n")] = 0;

        if (strcmp(linija, "[Ucesnici]") == 0) {
            sekcija = 1;
            continue;
        }
        else if (strcmp(linija, "[Dogadjaji]") == 0) {
            sekcija = 2;
            continue;
        }
        else if (strcmp(linija, "[Ulozi]") == 0) {
            sekcija = 3;
            continue;
        }

        switch (sekcija) {
        case 1: { // Ucesnici
            char ime[IME_DUZINA];
            float saldo;
            if (sscanf(linija, "%*d,%49[^,],%f", ime, &saldo) == 2) { // Koristi %*d da preskočis ID
                dodajUcesnika(ime, saldo);
            }
            break;
        }
        case 2: {
            Dogadjaj d;
            // Pretpostavka da su svi potrebni podaci prisutni u liniji
            if (sscanf(linija, "%d,%49[^,],%49[^,],%f,%f,%10s,%d", &d.id, d.tim1, d.tim2, &d.kvota1, &d.kvota2, d.datum, &d.odigran) != 7) {
                printf("Neuspelo učitavanje dogadjaja: %s\n", linija);
            }

            if (sscanf(linija, "%d,%49[^,],%49[^,],%f,%f,%10s,%d", &d.id, d.tim1, d.tim2, &d.kvota1, &d.kvota2, d.datum, &d.odigran) == 7) {
                dodajDogadjaj(d.tim1, d.tim2, d.kvota1, d.kvota2, d.datum);
            }
            break;
        }
        case 3: {
            int idUcesnika, idDogadjaja, izbor;
            float ulog;
            if (sscanf(linija, "%d,%d,%d,%f", &idUcesnika, &idDogadjaja, &izbor, &ulog) == 4) {
                dodajUlog(idUcesnika, idDogadjaja, izbor, ulog);
            }
            break;
        }
        }
    }

    fclose(f);
}


void sacuvajPodatke(const char* putanjaFajla) {
    FILE* f;
    if (fopen_s(&f, putanjaFajla, "w") != 0) {
        printf("Greska pri otvaranju fajla za pisanje!\n");
        return;
    }

    // Zapisivanje učesnika
    fprintf(f, "[Ucesnici]\n");
    for (int i = 0; i < brojUcesnika; i++) {
        fprintf(f, "%d,%s,%.2f\n", ucesnici[i].id, ucesnici[i].ime, ucesnici[i].saldo);
    }

    // Zapisivanje dogadjaja
    fprintf(f, "\n[Dogadjaji]\n");
    for (int i = 0; i < brojDogadjaja; i++) {
        fprintf(f, "%d,%s,%s,%.2f,%.2f,%s,%d\n", dogadjaji[i].id, dogadjaji[i].tim1, dogadjaji[i].tim2, dogadjaji[i].kvota1, dogadjaji[i].kvota2, dogadjaji[i].datum, dogadjaji[i].odigran);
    }

     
    // Zapisivanje uloga
    fprintf(f, "\n[Ulozi]\n");
    for (int i = 0; i < brojUloga; i++) {
        fprintf(f, "%d,%d,%d,%.2f,%d\n", ulozi[i].idUcesnika, ulozi[i].idDogadjaja, ulozi[i].izbor, ulozi[i].ulog, ulozi[i].odigrano);
    }


    fclose(f);
}



int main() {
    ucitajPodatke("podaci.txt");
    int opcija;
    do {
        ispisiMeni();
        scanf("%d", &opcija);
        obradiOpciju(opcija);
    } while (opcija != 0);

    sacuvajPodatke("podaci.txt");
    return 0;
}

