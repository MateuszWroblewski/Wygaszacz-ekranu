#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define szer 640
#define wys 480

typedef struct
{
    int x;      /*wspolrzedna x srodka*/
    int y;      /*wspolrzedna y srodka*/
    int rad;    /*promien*/
    int col;    /*kolor*/
    int PL;     /*czy aktualnie jedziemy w prawo [1] czy w lewo [0] */
    int GD;     /*czy aktualnie jedziemy w gore [1] czy w dol [0]*/
    int pion;   /*predkosc pionowa, co ile klatek do gory/dolu - im mniejsza tym szybciej!*/
    int poziom; /*predkosc pozioma, co ile klatek w prawo/lewo  - im mniejsza tym szybciej!*/
} babelek;

void prawo_lewo(babelek *B)
{
    int X = B->x;
    int Y = B->y;
    int R = B->rad;
    setcolor(0);
    fillellipse(X, Y, R, R); /*najpierw kasuje starego babelka*/
    setcolor(B->col);
    /*aktualizuje wspolrzedne*/
    if (B->PL == 1)
    {
        B->x += 1;
    }
    else
    {
        B->x -= 1;
    }
    X = B->x;
    fillellipse(X, Y, R, R); /*rysuje nowy babelek*/
} /*prawo_lewo*/

void gora_dol(babelek *B)
{
    int X = B->x;
    int Y = B->y;
    int R = B->rad;
    setcolor(0);
    fillellipse(X, Y, R, R); /*najpierw kasuje starego babelka*/
    setcolor(B->col);
    /*aktualizuje wspolrzedne*/
    if (B->GD == 1)
    {
        (B->y) += 1;
    }
    else
    {
        (B->y) -= 1;
    }
    Y = B->y;
    fillellipse(X, Y, R, R); /*rysuje nowy babelek*/
} /*gora_dol*/

void odbijsie(babelek *B)
{
    if ((B->x) <= (B->rad))
    {
        (B->PL) = 1; /*jesli podejdziemy o promien do lewej strony, to od tej pory jedziemy w prawo*/
    }
    if (B->x + B->rad >= szer)
    {
        B->PL = 0; /*jesli podejdziemy o promien do prawej strony to od tej pory jedziemy w lewo*/
    }
    if (B->y <= B->rad)
    {
        B->GD = 1; /*jesli podejdziemy o promien do dolnej krawedzi to od tej pory jedziemy w dol*/
    }
    if (B->y + B->rad >= wys)
    {
        B->GD = 0; /*jesli podejdziemy o promien do gornej krawedzi to od tej pory jedziemy w gore*/
    }
} /*odbijsie*/

void zrob_babelka(babelek *B)
{
    setcolor(B->col);
    fillellipse(B->x, B->y, B->rad, B->rad);
} /*zrob_bableka*/

void wygaszacz(int n)
{
    unsigned long long Licznik = 0;
    int i, j, pos, obw;
    babelek t[n];
    int Promien = sqrt((szer * wys) / (6 * n * M_PI)); /*zeby koleczka zajmowaly mniej wiecej 1/6 ekranu*/
    obw = 2 * szer + 2 * wys + 8 * Promien;
    /*chcielibysmy zeby babelki sie nie materializowaly na ekranie, tylko na niego wplywaly z zewnatrz*/
    /*zatem najpierw buduje obwod naokolo naszego ekranu*/
    /*jest on odpowiednio wiekszy, zeby mogl ukryc babelki, ktore najpierw lokujemy na "ramce", a potem wyplywaly na ekran*/
    for (i = 0; i < n; i++)
    {
        pos = (rand()) % obw;

        if (pos <= szer + 2 * Promien)
        {
            t[i].x = pos;
            t[i].y = -Promien;
            t[i].GD = 0;
            t[i].PL = rand() % 2;
        }
        else if (pos <= szer + wys + 4 * Promien)
        {
            t[i].x = szer + Promien;
            t[i].y = pos - szer - 2 * Promien;
            t[i].PL = 0;
            t[i].GD = rand() % 2;
        }
        else if (pos <= 2 * szer + wys + 6 * Promien)
        {
            t[i].x = pos - (szer + wys + 4 * Promien);
            t[i].y = wys + Promien;
            t[i].GD = 1;
            t[i].PL = rand() % 2;
        }
        else
        {
            t[i].x = -Promien;
            t[i].y = pos - (2 * szer + wys + 6 * Promien);
            t[i].PL = 1;
            t[i].GD = 0;
        }

        t[i].col = (rand() % 14) + 1; /*tylko tyle mamy kolorkow*/
        t[i].rad = Promien;
        t[i].pion = (rand() % 10) + 1;
        t[i].poziom = (rand() % 10) + 1;
    }
    for (i = 0; i < n; i++)
    {
        zrob_babelka(t + i);
    }
    while (Licznik < 10000)
    {
        for (i = 0; i < n; i++)
        {
            if (Licznik % (t[i].pion) == 0)
            {
                gora_dol(t + i); /*zgodnie z definicja w 17 linijce, co tyle klatek przesuwamy sie gora/dol*/
            }
            if (Licznik % (t[i].poziom) == 0)
            {
                prawo_lewo(t + i); /*zgodnie z definicja w 18 linijce, co tyle klatek przesuwamy sie prawo/lewo*/
            }
        }
        for (i = 0; i < n; i++)
        {
            odbijsie(t + i);
        }
        delay(1);
        Licznik++;
    }
} /*wygaszacz*/

int main()
{
    srand(time(0));
    int n; /*liczba pileczek*/
    scanf("%d", &n);
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    wygaszacz(n);
    closegraph();
    return 0;
}
