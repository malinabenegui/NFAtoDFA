#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

ifstream f("date.in");

vector < pair < char, int > > stari[100];

struct tabel
{
    int cell[100];

}tab_afn[100][100];

struct tabell
{
    char tranz[20];

} tab_afd[100][100];

pair < char, int > read_pair() //citesc perechea formata din litera si starea in care ajunge ea
{
    pair < char, int > p;
    f >> p.first >> p.second;

    return p;
}

void noduri_finale(int fin[], int &nr_fin) //citesc nodurile finale
{
    int i;
    f >> nr_fin;

    for(i = 0; i < nr_fin; i++)
        f >> fin[i];
}

void creare() //citesc nodul
{
    int nod;
    while(f >> nod)
        stari[nod].push_back(read_pair());
}

bool is_final(int nr_fin, int fin[], char s[]) //verific daca starea din afd e finala
{
    int x, y, i, lung_s = strlen(s);
    for(i = 0; i < lung_s; i++)
    {
            if(s[i] != ' ') //aflu o stare componenta a starii AFD
            {
                if(s[i+1] != ' ' && s[i+1] != NULL)//daca starea e de 2 cifre
                {
                    x = s[i] - '0';
                    y = s[i+1] - '0';
                    x = x*10 + y;
                    i++;
                }
                else //daca e doar dintr-o cifra
                        x = s[i] - '0';
            }
            for(int j = 0; j < nr_fin; j++)
                if(fin[j] == x)
                    return 1;
    }

    return 0;
}

int index_litera(int nr_lit, char litere[], char c) //verific pe ce pozitie in tabel ar trebui sa se afle litera curenta
{
    for(int i = 1; i <= nr_lit; i++)
        if(c == litere[i])
            return i;
}

void tabel_AFN(int n, int nr_lit, char litere[])
{
    int i, poz, k, j;

    for(i = 0; i < n; i++) //populez tabelul cu -1
        memset(tab_afn[i], -1, sizeof(struct tabel) * (nr_lit + 1));

    for(i = 0; i < n; i++)
        for(j = 0; j < nr_lit; j++)
            tab_afn[i][j].cell[0] = 0; //initializez prima coloana a fiecarei litere de pe fiecare linie cu 0, aici urmeaza sa retin nr de tranzitii

    for(j = 0; j < n; j++) //parcurg toate starile
    {
        for(i = 0; i < stari[j].size(); i++) //parcurg toate tranzitiile unei stari
        {
            poz = index_litera(nr_lit, litere, stari[j][i].first) - 1; //caut indexul literei curente
            //cout << poz << '\n';

            k = tab_afn[j][poz].cell[0];
            tab_afn[j][poz].cell[++k] = stari[j][i].second; //pun tranzitia curenta in tabelul AFN
            tab_afn[j][poz].cell[0]++;
        }
    }
}
char* vect_tranzitii(int vect[], char s[], int z) //sortez tranzitiile unei litere si le adaug crescator in sirul s
{
        char a, b;
        int q = 0, lung_s;
        int temp[z + 5];
        sort(vect, vect + z); //sortez crescator tranzitiile literei i de pe linia x

        if (z == 1) //daca am o singura tranzitie
        {
            if (vect[0] < 10)
            {
                s[0] = vect[0] + '0';
                s[1] = ' ';
                s[2] = '\0';
                return s; // daca e un singur element in vectorul de destinatii
            }
            else
            {
                s[0] = vect[0]/10 + '0';
                s[1] = vect[0]%10 + '0';
                s[2] = ' ';
                s[3] = '\0';

                return s;
            }

        }

        for (int i = 0; i < z - 1; ++i)
            if (vect[i] != vect[i + 1])   // elimin duplicatele
                temp[q++] = vect[i];

        temp[q++] = vect[z - 1];

        for(int d = 0; d < q; d++)
        {
            if(temp[d] < 10) //daca starea in care pleaca din 0 < 10, atunci copiez cifra starii in s
            {
                a = temp[d] + '0';
                lung_s = strlen(s);

                s[lung_s] = a;
                s[lung_s + 1] = ' ';
                s[lung_s + 2] = '\0';

            }
            else //daca are 2 cifre, atunci copiez cifrele una cate una
            {
                a = temp[d]%10 + '0';
                b = temp[d]/10 + '0';
                lung_s = strlen(s);

                s[lung_s] = b;
                s[lung_s + 1] = a;
                s[lung_s + 2] = ' ';
                s[lung_s + 3] = '\0';
            }
        }
        return s;
}

void tabel_AFD(int nr_lit, int &k)
{
    int i, j, nr_tranz, vect[100], z = 0;
    char s[100], v[100][100];

    tab_afd[0][0].tranz[0] = '0'; //notez in matrice starea de pornire, i.e. 0

    v[k][0] = '0';
    v[k++][1] = ' ';

    for(i = 0; i < nr_lit; i++) //copiez prima linie din AFN in AFD
    {
        s[0] = '\0';
        nr_tranz = tab_afn[0][i].cell[0]; //daca am tranzitii cu litere

        z = 0;
        if(nr_tranz)
        {
            for(j = 1; j <= nr_tranz; j++)
                vect[z++] = tab_afn[0][i].cell[j];

            strcpy(s, vect_tranzitii(vect, s, nr_tranz));
            //cout << s << '\n';

            s[strlen(s)] = '\0';
            strcpy(tab_afd[0][i + 1].tranz, s);

            int x = k;
            strcpy(v[x], s);
            k++;
        }
    }

    int poz_v = 1, poz_afd = 1, x, y, ok, lung_s;

    while(poz_v < k) //cat timp mai am stari pt afd in vectorul de tranzitii
    {
        strcpy(tab_afd[poz_v][0].tranz, v[poz_v]); //pun pe noua linie noua stare

        for(i = 0; i < nr_lit; i++) //parcurg fiecare litera a starii curente din AFD
        {
            for (int var = strlen(s) - 1; var >= 0; --var)
                s[var] = '\0'; // nullam sirul

            z = 0;
            lung_s = strlen(v[poz_v]);

            for(j = 0; j < lung_s; j++) //parcurg fiecare stare componenta a starii curente din afd
            {
                if(v[poz_v][j] != ' ') //aflu una cate una starile din AFN pe care trebuie sa le reunesc
                {
                    if(v[poz_v][j+1] != ' ')//daca starea din AFN e de 2 cifre
                    {
                        x = v[poz_v][j] - '0';
                        y = v[poz_v][j+1] - '0';
                        x = x*10 + y;
                        i++;
                    }
                    else //daca e doar dintr-o cifra
                        x = v[poz_v][j] - '0';


                    nr_tranz = tab_afn[x][i].cell[0]; // nr_tranz = nr tranzitii coloana x litera i

                    if(nr_tranz > 0) //daca am tranzitii in AFN pe linia si cu litera curenta
                    {
                        for(int d = 1; d <= nr_tranz; d++)
                            vect[z++] = tab_afn[x][i].cell[d];
                    }
                }
            }
            if(z)
                strcpy(s, vect_tranzitii(vect, s, z));

            //s[strlen(s)] = '\0';
            strcpy(tab_afd[poz_v][i + 1].tranz, s); //copiez in tabela AFD la pozitia urmatoare tranzitiile liniei respective

            ok = 1;
            for(int d = 0; d < k; d++) //verific daca starea AFD a mai aparut in vectorul v. Daca nu a aparut, o adaug
                if(strcmp(s, v[d]) == 0)
                {
                    ok = 0;
                    break;
                }

            if(ok == 1)
                strcpy(v[k++], s);
        }
        poz_v++;//ma mut la urmatoarea stare AFD din vector
    }
}

void AFD(int k, int nr_lit, int nr_fin, int fin[], char litere[])
{
    int i, j;

    cout << "AFD-ul este: " << '\n';

    for(i = 0; i < k; i ++) //linia starii
    {
        for(j = 0; j <= nr_lit; j++) //litera
            {
                if (tab_afd[i][j].tranz[0] != NULL)
                {
                    if(j == 0)
                    cout << "Starea: " << tab_afd[i][j].tranz << " Tranzitiile: ";
                    else
                    {
                        cout << litere[j];
                        cout << " : " << tab_afd[i][j].tranz << "   ";
                    }
                }

            }
        if(tab_afd[i][0].tranz[0] != NULL)
            cout << '\n';
    }

    cout << "Starile Finale sunt: ";

    for(i = 0; i < k; i++)
    {
        if(is_final(nr_fin, fin, tab_afd[i][0].tranz) == 1)
           cout << tab_afd[i][0].tranz << " ";
    }

}

int main()
{
    int n, nr_fin, fin[100], nr_lit, k = 0;
    char litere[50];

    noduri_finale(fin, nr_fin);

    f >> n; //nr de stari
    f >> nr_lit; //nr de litere

    for(int i = 1; i <= nr_lit; i++)
        f >> litere[i];

    creare();
    tabel_AFN(n, nr_lit, litere);
    tabel_AFD(nr_lit, k);

    AFD(k, nr_lit, nr_fin, fin, litere);

    return 0;
}
