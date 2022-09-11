/*
    Bu kod .txt dosyasindan komsuluk listesini okur ve BFS algoritmasi ile tum dugumleri gezer. Gezme sirasi ekrana yazdirilir.
    Not: Olusturulan komsuluk listesi yapisini anlamak icin ekte bulunan resme bakiniz.
    Not: Resimde tum dugumler sigmamistir.
*/

#include <stdio.h>
#include <stdlib.h>

struct dugum{
    char dugumAdi; //Dugumun adi
    struct dugum *sonrakiDugum; //Komsuluk listesinde, tek yonlu bagli listeyle komsu dugumlerden sonrakininin yerini gosterir.
    int anaDugum; //Komsuluk listesindeki; ana dugumlerden birisiyse -> 1 , komsu dugumlerden birisiyse -> 0 
    struct dugum *sonrakiAnaDugum; //Komsuluk listesinde, tek yonlu bagli listeyle ana dugumlerden sonrakininin yerini gosterir.
    int ziyaretEdildiMi; //Ana dugumse, ziyaretEdildiMi -> 1 ziyaretEdildiMi -> 0; komsu dugumse, ziyaretEdildiMi -> -1 
};

FILE *dosya;
struct dugum *baslangic=NULL;
struct dugum *dugumGezici=NULL;
struct dugum *anaDugumGezici=NULL;
int kontrol=0;

int anaDugumSayisi=0;
void komsulukListesiniOlusturma(){ //Dosyadan karakterler okunup, bagli listeye eklenme fonksiyonu...
    char okunanKarakter=fgetc(dosya);
    char birOncekiKarakter='\n'; /*Ilk okunan karakterin ilk ana dugum olmasi ve ondan once okunan karakter olmamasi nedeniyle 
                                  algortimada sorun cikarmamasi icin onceki karakteri /n ile baslattim. */
    while(!feof(dosya)){ //Dosyadaki tum karakterler okunana kadar...
        if(okunanKarakter!='\n'&&okunanKarakter!=' '&&birOncekiKarakter=='\n'){ //Bu sartlar yerine getirilirse ana dugumdur.
            if(kontrol==0){ //Daha once dugum olusturulmadiysa...
                dugumGezici->anaDugum=1; //Ana dugum oldugunu ifade etmektedir.
                dugumGezici->ziyaretEdildiMi=0;
                dugumGezici->dugumAdi=okunanKarakter; //Ilk eklenen ana dugumu ekleme algoritmasi...
                anaDugumGezici=dugumGezici;
                baslangic=anaDugumGezici;
                kontrol=1;
                anaDugumSayisi++;
            }
            else{ //Daha once dugum olusturulduysa...
                anaDugumGezici->sonrakiAnaDugum=(struct dugum*)malloc(sizeof(struct dugum)); //Sonraki ana dugumleri ekleme algoritmasi...
                dugumGezici=anaDugumGezici->sonrakiAnaDugum; 
                dugumGezici->anaDugum=1; //Ana dugum oldugunu ifade etmektedir.
                dugumGezici->ziyaretEdildiMi=0;
                dugumGezici->dugumAdi=okunanKarakter;
                anaDugumGezici=dugumGezici;
                anaDugumSayisi++;
            }
            anaDugumGezici->sonrakiAnaDugum=NULL;
        }
        else if(okunanKarakter!='\n'&&okunanKarakter!=' '&&birOncekiKarakter!='\n'){ //Bu sartlar yerine getirilirse komsu dugumdur.
            dugumGezici->sonrakiDugum=(struct dugum*)malloc(sizeof(struct dugum));  //Komsu dugumleri ekleme algoritmasi...
            dugumGezici=dugumGezici->sonrakiDugum;
            dugumGezici->dugumAdi=okunanKarakter;
            dugumGezici->anaDugum=0; //Komsu dugum oldugunu ifade etmektedir.
            dugumGezici->ziyaretEdildiMi=-1;
            dugumGezici->sonrakiDugum=NULL;
        } 
        birOncekiKarakter=okunanKarakter;
        okunanKarakter=fgetc(dosya);
    }
}

void komsulukListesiniYazdirma(){ //Yapimizi ekrana yazdirma fonksiyonu...
    anaDugumGezici=baslangic;
    dugumGezici=anaDugumGezici;
    while(anaDugumGezici!=NULL){ /*Sirayla komsu dugumleri gezer, bir satirdaki komsu dugum bitince
                                  ise bir sonraki ana dugume gecip gezmeye devam eder.*/
        while(dugumGezici!=NULL){  
            if(dugumGezici->anaDugum==1){ //Ana dugumse...
                printf("%c->", dugumGezici->dugumAdi);
            }
            else{ //Komsu Dugumse...
                printf("%c-", dugumGezici->dugumAdi);
            }
            dugumGezici=dugumGezici->sonrakiDugum; 
        }
        printf("NULL\n");
        anaDugumGezici=anaDugumGezici->sonrakiAnaDugum;
        dugumGezici=anaDugumGezici;
    }
}

void komsulukListesindeBul(char arananKarakter){ //Ziyaret edilen dugume 1 degeri atilir.
    anaDugumGezici=baslangic;
    while(anaDugumGezici!=NULL){ 
        if(anaDugumGezici->dugumAdi==arananKarakter){
            anaDugumGezici->ziyaretEdildiMi=1;
            break;
        }
        anaDugumGezici=anaDugumGezici->sonrakiAnaDugum;
    }
}

struct kuyruk{
    char dugumAdi; //Dugumun adi
    struct kuyruk *sonrakiDugum; //Kuyrukta, tek yonlu bagli listeyle komsu dugumlerden sonrakininin yerini gosterir.
};

struct kuyruk *kuyrukIlkDugum=NULL; //Kuyruktaki ilk dugumu tutacak.
struct kuyruk *kuyrukDugumGezici=NULL; //Kuyrukta dugumleri gezecek.

char *gezilenSira=NULL; //BFS algoritmasinda dugumlerin gezilme sirasini tutacak.

void kuyruguYazdir(){ //Kuyruk yazdiriliyor
    struct kuyruk *gecici=(struct kuyruk*)malloc(sizeof(struct kuyruk));
    gecici=kuyrukIlkDugum;
    printf("Kuyruk: ");
    if(gecici==NULL) printf("-");
    while(gecici!=NULL){
        printf("%c ", gecici->dugumAdi);
        gecici=gecici->sonrakiDugum;
    }
    printf("\n");
}

struct dugum *dugumBul(char arananKarakter){ //Kuyruktaki ilk dugum komsuluk listesinde bulunuyor.
    anaDugumGezici=baslangic;
    int sayac=1;
    while(anaDugumGezici!=NULL){ 
        if(anaDugumGezici->dugumAdi==arananKarakter){
            break;
        }
        anaDugumGezici=anaDugumGezici->sonrakiAnaDugum;
        sayac++;
    }
    return (anaDugumGezici);
}

int kontrolEt(char arananKarakter){ //Dugum daha once ziyaret edildi mi kontrol ediliyor.
    anaDugumGezici=baslangic;
    while(anaDugumGezici!=NULL){ 
        if(anaDugumGezici->dugumAdi==arananKarakter){
            if(anaDugumGezici->ziyaretEdildiMi==1){
                printf("%c daha once eklendi. Kuyruga eklenemez!\n", arananKarakter);
                return 0;
            }
            else{
                printf("%c daha once eklenmedi. Kuyruga ekleniyor.\n", arananKarakter);
                return 1;
            }
        }
        anaDugumGezici=anaDugumGezici->sonrakiAnaDugum;
    }
}

void kuyrugaEkle(char ad){ //Kuyruga parametre olarak gelen isme sahip dugum ekleniyor.
    if(kontrolEt(ad)==1){
        if(kuyrukIlkDugum==NULL){
            kuyrukIlkDugum=(struct kuyruk*)malloc(sizeof(struct kuyruk));
            kuyrukIlkDugum->dugumAdi=ad;
            kuyrukIlkDugum->sonrakiDugum=NULL;
            kuyrukDugumGezici=(struct kuyruk*)malloc(sizeof(struct kuyruk));
            kuyrukDugumGezici=kuyrukIlkDugum;
        }
        else{
            while(kuyrukDugumGezici->sonrakiDugum!=NULL){
                kuyrukDugumGezici=kuyrukDugumGezici->sonrakiDugum;
            }
            kuyrukDugumGezici->sonrakiDugum=(struct kuyruk*)malloc(sizeof(struct kuyruk));
            kuyrukDugumGezici=kuyrukDugumGezici->sonrakiDugum;
            kuyrukDugumGezici->dugumAdi=ad;
            kuyrukDugumGezici->sonrakiDugum=NULL;
        }
        komsulukListesindeBul(kuyrukDugumGezici->dugumAdi);
        printf("%c kuyruga eklendi\n", kuyrukDugumGezici->dugumAdi);
    }
}

void kuyruktanSil(){ //Kuyruktan ilk sirada bulunan eleman siliniyor.
    if(kuyrukIlkDugum->sonrakiDugum==NULL){
        printf("%c kuyruktan silindi\n", kuyrukIlkDugum->dugumAdi);
        kuyrukIlkDugum=NULL;
    }
    else{
        struct kuyruk *gecici=(struct kuyruk*)malloc(sizeof(struct kuyruk));
        gecici=kuyrukIlkDugum;
        kuyrukIlkDugum=kuyrukIlkDugum->sonrakiDugum;
        printf("%c kuyruktan silindi\n", gecici->dugumAdi);
        free(gecici);
    }
}

int c=0; //BFS algoritmasinda ilk dugum ve komsulari kuyruga eklenmediyse -> 0, eklendiyse -> 1
int a=0; //Dugumlerin gezilme sirasinda dizinin son indeksini tutacak.
void BFS(){ //BFS Algoritması ozyinelemeli sekilde calisiyor.
    if(c==0){
        dugumGezici=baslangic;
        kuyrugaEkle(dugumGezici->dugumAdi);
        kuyruguYazdir();
        gezilenSira[a++]=kuyrukIlkDugum->dugumAdi;
        kuyruktanSil();
        kuyruguYazdir();
        dugumGezici=dugumGezici->sonrakiDugum;
        while(dugumGezici!=NULL){
            kuyrugaEkle(dugumGezici->dugumAdi);
            kuyruguYazdir();
            dugumGezici=dugumGezici->sonrakiDugum;
        }
        c=1;
    }
    else{
        dugumGezici=dugumBul(kuyrukIlkDugum->dugumAdi);
        gezilenSira[a++]=kuyrukIlkDugum->dugumAdi;
        kuyruktanSil();
        kuyruguYazdir();
        dugumGezici=dugumGezici->sonrakiDugum;
        while(dugumGezici!=NULL){
            kuyrugaEkle(dugumGezici->dugumAdi);
            kuyruguYazdir();
            dugumGezici=dugumGezici->sonrakiDugum;
        }
    }
    printf("\n***********************\n\n");
    if(kuyrukIlkDugum!=NULL){ //Ozyinemeli sekilde...
        BFS();
    }
}

int main(){
    dosya=fopen("Komsuluk_Listesi.txt", "r"); //Dosya okumak icin aciliyor.
    if(dosya==NULL){
        printf("\nDosya Bos!");
        return 0;
    }
    else{
        printf("\nDosyadan Okunan Komsuluk Listesi:\n");
        baslangic=(struct dugum*)malloc(sizeof(struct dugum));
        dugumGezici=(struct dugum*)malloc(sizeof(struct dugum));
        anaDugumGezici=(struct dugum*)malloc(sizeof(struct dugum));
        dugumGezici=baslangic;
        anaDugumGezici=baslangic;
        komsulukListesiniOlusturma();
        komsulukListesiniYazdirma();
    }   
    fclose(dosya); //Dosya Kapatiliyor.
    dugumGezici=baslangic;
    printf("\n***********************\n\n");
    gezilenSira=(char*)malloc(anaDugumSayisi*sizeof(char)); //Ana dugum sayisi kadar yer ayriliyor.
    BFS();
    free(anaDugumGezici);
    free(dugumGezici);
    free(baslangic);
    free(kuyrukIlkDugum);
    free(kuyrukDugumGezici);
    printf("BFS algoritmasi ile dugumlerin gezilme sirasi: ");
    for(int i=0 ; i<anaDugumSayisi ; i++){ 
        printf("%c ", gezilenSira[i]);
    }
    printf("\n\n");
    return 0;
}
