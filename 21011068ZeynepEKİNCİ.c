#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>

typedef struct {
    char isim[10];
    char soyisim[10];
    char kullaniciadi[10];
    char sifre[10];
    int maxskor;
} Oyuncu;
void oyun_bilgisi();
void dosyadan_oku(int *oyuncusayisi, Oyuncu **p);
void menu(int *oyuncusayisi, Oyuncu **p);
void hesap_olustur(int *oyuncusayisi, Oyuncu **p);
int giris_yap(int oyuncusayisi, Oyuncu *p);
void oyuna_basla(int indeks, Oyuncu **p,int n);
int oyun(char **m,int satir,int sutun,int yer1,int yer2);
void bubbleSort(int arr[], int n);
void skor_goster(Oyuncu **p, int oyuncusayisi );

int main() {
    int oyuncusayisi = 0;
    Oyuncu *oyuncular = NULL;

    menu(&oyuncusayisi, &oyuncular);

    free(oyuncular); 
    return 0;
}

void dosyadan_oku(int *oyuncusayisi, Oyuncu **p) { 
    FILE *dosya = fopen("oyuncu_bilgileri.bin", "rb");
    int i;
	if (dosya != NULL) {
        fseek(dosya, 0, SEEK_END);
        long dosyaBoyutu = ftell(dosya);
        rewind(dosya);

        *oyuncusayisi = dosyaBoyutu / sizeof(Oyuncu);
        *p = (Oyuncu *)malloc(*oyuncusayisi * sizeof(Oyuncu));

        fread(*p, sizeof(Oyuncu), *oyuncusayisi, dosya);

        fclose(dosya);
    } else {
        printf("Dosya acilamadi.\n");
    }
    FILE *fp = fopen("skorlar.txt", "w"); 

    if (fp == NULL) {
        printf("Dosya acma hatasi!\n");
        
    }

    for(i=0;i<*oyuncusayisi;i++){
    	fprintf(fp, "%d\n", (*p+i)->maxskor);
	}
	fclose(fp);
   
    
}
void bubbleSort(int arr[], int n) {
    int temp;
    int swapped,i,j;

    for ( i=0; i<n-1; i++) {
        swapped=0; 

        for ( j=0; j<n-i-1; j++) {
            if (arr[j]<arr[j+1]) {  
                
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;

                swapped=1; 
            }
        }

        if (swapped==0) {
            return;  
        }
    }
}
void skor_goster(Oyuncu **p, int oyuncusayisi ){
	FILE *dosya = fopen("skorlar.txt", "r"); 
    int i=0,j=0,flag=0;
    if (dosya == NULL) {
        printf("Dosya acma hatasi!\n");
        
    }

    int skor[oyuncusayisi+1];

    while (fscanf(dosya, "%d", &skor[i]) != EOF) {
        i++;
    }

    fclose(dosya);
	bubbleSort(skor,oyuncusayisi);
	while(j<5 && j!=oyuncusayisi){
		
		
		for(i=0;i<oyuncusayisi;i++){
			if(skor[j]==(*p+i)->maxskor){
				printf("isim: %s soyisim: %s maxskor: %d\n ",(*p+i)->isim,(*p+i)->soyisim,(*p+i)->maxskor);
				
			}
			
		}
		while(skor[j]==skor[j+1]){
			j++;
			flag=1;
		}
		if(flag==0){
			j++;
		}
		flag=0;
	}

    
}


void menu(int *oyuncusayisi, Oyuncu **p) {
    int secim = 0, indeks,kontrol=0,cikis=0;
    FILE *dosya;
    while (secim != 6 && cikis==0) {
      
        printf("\n--- Oyun Menusu ---\n");
        printf("1. Hesap Olustur\n");
        printf("2. Giris Yap\n");
        printf("3. En Yuksek Skorlari Goruntule\n");
        printf("4. Oyuna Basla\n");
        printf("5. Oyun Klavuzu\n");
        printf("6. Cikis\n");
        scanf("%d", &secim);

        if (secim == 1) {
            dosya = fopen("oyuncu_bilgileri.bin", "r");
            if (dosya != NULL) {
            	dosyadan_oku(oyuncusayisi,p);
            }
			hesap_olustur(oyuncusayisi,p);
			
            
        } else if (secim == 2) {
            dosyadan_oku(oyuncusayisi, p);
			indeks = giris_yap(*oyuncusayisi, *p); //fonksiyon kullaniciya erismemizi saglayan indeks degerini dondurur.
            if (indeks == -1) {
                printf("Kullanici adi veya sifreniz yanlis.\n");
            }
        } else if (secim == 3) {
            dosyadan_oku(oyuncusayisi,p);
			skor_goster(p,*oyuncusayisi);
        } else if (secim == 4) {
            oyuna_basla(indeks, p,*oyuncusayisi); 
        } else if (secim == 6) {
            cikis=1;
        } 
        else if(secim==5){
        	oyun_bilgisi();
		}
		else {
            printf("Gecersiz secim. Lutfen tekrar deneyin.\n");
        }
    }
}
void oyun_bilgisi(){
	printf("Karsiniza cikacak labirentte atomalti parcaciklar bulunmaktadir\n");
	printf("Amac karsit hidrojen uretmektir. Karsit hidrojen uretmek icin cikis yaptiginizda elinizde sadece E ve p bulunmalidir\n");
	printf("Bir parcacikla o parcacigin zit isaretli parcacigi bir araya gelirse birbirini yok ederler.");
	printf("Karadelikler K ile gosterilmistir karadelige giren oyuncunun oyunu sonlanir.\n");
	printf("Sureniz 60 saniyedir bu sureyi asarsaniz oyun sonlanir.");
}

void hesap_olustur(int *oyuncusayisi, Oyuncu **p) { //bu fonksiyonda kullanici adi baska kullanicilarla eslesmezse yeni hesap olusturulur
    int i=0,flag=0;
	(*oyuncusayisi)++;
    *p = (Oyuncu *)realloc(*p, *oyuncusayisi * sizeof(Oyuncu));

    printf("Isminiz nedir? ");
    scanf("%s", (*p)[*oyuncusayisi - 1].isim);
    printf("Soyisminiz nedir? ");
    scanf("%s", (*p)[*oyuncusayisi - 1].soyisim);
    printf("Kullanici adiniz nedir? ");
    scanf("%s", (*p)[*oyuncusayisi - 1].kullaniciadi);
    printf("Sifrenizi girin: ");
    scanf("%s", (*p)[*oyuncusayisi - 1].sifre);
    (*p)[*oyuncusayisi - 1].maxskor = 0;
    for(i=0;i<*oyuncusayisi-1;i++){
    	if(strcmp((*p)[i].kullaniciadi, (*p)[*oyuncusayisi - 1].kullaniciadi) == 0){
    		flag=1;
    		printf("Bu kullanici ismi baska bir kullanici tarafindan kullaniliyor.\nTekrar deneyin\n");
		}
	}
    if(flag==0){
    	FILE *dosya = fopen("oyuncu_bilgileri.bin", "ab");
        if (dosya != NULL) {
            fwrite(&(*p)[*oyuncusayisi - 1], sizeof(Oyuncu), 1, dosya);
            fclose(dosya);
            printf("Hesap olusturuldu.\n");
        } 
		else {
        printf("Dosya acilamadi ya da olusturulamadi.\n");
        }
	}

}

int giris_yap(int oyuncusayisi, Oyuncu *p) {
    char kullaniciismi[10];
    char sifre[10];
    printf("Kullanici adi nedir? ");
    scanf("%s", kullaniciismi);
    printf("Sifrenizi girin: ");
    scanf("%s", sifre);
    int i;

    for ( i = 0; i < oyuncusayisi; i++) {
        if (strcmp((p + i)->kullaniciadi, kullaniciismi) == 0) {
            if (strcmp((p + i)->sifre, sifre) == 0) {
                printf("Basariyla giris yapildi.\n");
                return i;
            } else {
                printf("Sifre yanlis.\n");
                return -1; 
            }
        }
    }

    printf("Kullanici adi bulunamadi.\n");
    return -1; 
}

void oyuna_basla(int indeks, Oyuncu **p,int n) { //harita secimi bu fonksiyonda yapilir
    int secim;
    char karakter;
    FILE *dosya;
	printf("Oyuna baslaniyor... Hos geldiniz, %s!\n Sureniz 60 saniye...\n", (*p + indeks)->isim);
	char dosyaAdi[10];
	int karakterSayac=0,satirSayisi=0,i,sutunSayisi,j,yer1,yer2,puan;
	printf("1. Harita 1\n2. Harita 2\n3. Harita 3\n");
	scanf("%d",&secim);
	if(secim==1){
		dosya=fopen("1.txt","r");
	}
	else if(secim==2){
		dosya=fopen("2.txt","r");
	}
	else if(secim==3){
		dosya=fopen("a.txt","r");
	}
	
	if (dosya != NULL) {
        
        while ((karakter = fgetc(dosya)) != EOF) {
           if (karakter != '\n') {
                karakterSayac++;
            }
            else{
            	satirSayisi++;
			}
        }

        rewind(dosya);
        
    } else {
        printf("Dosya acma hatasi.\n");
    }
  
    sutunSayisi=karakterSayac/satirSayisi;
    
	char **m;
    m=(char **)calloc(satirSayisi,sizeof(char*));  //karakter sayisina gore dinamik bellekle alan olusturma
    for(i=0;i<satirSayisi;i++){
    	m[i]=(char*)calloc(sutunSayisi,sizeof(char));
	}
	
	for (i = 0; i < satirSayisi; i++) {
        for (j = 0; j < sutunSayisi; j++) {
            karakter = fgetc(dosya);
            m[i][j] = karakter;
        }
       
        karakter = fgetc(dosya);
    }

    fclose(dosya);
  
	for (i = 0; i < satirSayisi; i++) {
        
		for (j = 0; j < sutunSayisi; j++) {
            
			if(m[i][j]=='X'){
            
				yer1=i;
            	yer2=j;
			}
        }
        
    }
	
	
	puan = oyun(m, satirSayisi, sutunSayisi, yer1, yer2); //oyun metodunda kullanici oyun oynar ve puani bu fonksiyona geri dondurulur.

    if ((*p + indeks)->maxskor < puan) {    //alinan puan kullanicinin max puannini gectiyse skorlar dosyasinda oyuncunun max puani guncellenir
        (*p + indeks)->maxskor = puan;
        printf("Yeni max puaniniz: %d\n", (*p + indeks)->maxskor);

        
        FILE *dosya = fopen("oyuncu_bilgileri.bin", "rb+");
        if (dosya != NULL) {
            fseek(dosya, indeks * sizeof(Oyuncu) + offsetof(Oyuncu, maxskor), SEEK_SET);
            fwrite(&(*p + indeks)->maxskor, sizeof(int), 1, dosya);
            fclose(dosya);
        } else {
            printf("Dosya acilamadi ya da yazma hatasi.\n");
        }
    } else {
        printf("Mevcut max skorunuz: %d\n", (*p + indeks)->maxskor);
    }


	
  
}
void matris_yazdir(char **m,int satir,int sutun){
	int i,j;
	for(i=0;i<satir;i++){
		for(j=0;j<sutun;j++){
			printf("%c ",m[i][j]);
		}
		printf("\n");
	}
}
int oyun(char **m,int satir,int sutun,int yer1,int yer2){
	clock_t baslangic, simdiki;
    double sureLimiti = 60.0; 
    double sure;
    int cik=0, i=0,j,cikis=0,p=0,e=0,E=0,P=0,puan=0;
    char t[10],a;
   
    baslangic = clock();
    
    do {
       matris_yazdir(m,satir,sutun);
       printf("\ntoplanan parcaciklar:\n");
	   for(j=0;j<i;j++){
       	printf("%c,",t[j]);
       	
	   }
	   printf("yon:");
	   a = getch();
		if (a==27) 
		{
			printf("Cikis\n");
			break;
		}
		else
		{
			a = getch();
			
			if(a==72){
				//printf("yukari");
				if(yer1-1<0 || m[yer1-1][yer2]=='1' ){
					printf("hatali hamle");
				}
				
				else if(m[yer1-1][yer2]=='K' || m[yer1-1][yer2]=='C'){
					cik=1;
					if(m[yer1-1][yer2]=='C'){
						cikis=1;
					}
				}
				else if(m[yer1-1][yer2]=='0'){
					m[yer1][yer2]='0';
					m[yer1-1][yer2]='X';
					yer1=yer1-1;
				}
				else{
					t[i]=m[yer1-1][yer2];
					m[yer1-1][yer2]='X';
					m[yer1][yer2]='0';
					yer1=yer1-1;
					i++;
				}
				
				
			}
			if(a==80){
				//printf("asagi");
				
				if(yer1+1==satir || m[yer1+1][yer2]=='1' ){
					printf("hatali hamle");
				}
				
				else if(m[yer1+1][yer2]=='K' || m[yer1+1][yer2]=='C'){
					cik=1;
					if(m[yer1+1][yer2]=='C'){
						cikis=1;
					}
				}
				else if(m[yer1+1][yer2]=='0'){
					m[yer1][yer2]='0';
					m[yer1+1][yer2]='X';
					yer1=yer1+1;
				}
				else{
					t[i]=m[yer1+1][yer2];
					m[yer1+1][yer2]='X';
					m[yer1][yer2]='0';
					yer1=yer1+1;
					i++;
				}
			}
			if(a==77){
				//printf("sag");
				if(yer2+1==sutun || m[yer1][yer2+1]=='1' ){
					printf("hatali hamle");
				}
				
				else if(m[yer1][yer2+1]=='K' || m[yer1][yer2+1]=='C'){
					cik=1;
					if(m[yer1][yer2+1]=='C'){
						cikis=1;
					}
				}
				else if(m[yer1][yer2+1]=='0'){
					m[yer1][yer2]='0';
					m[yer1][yer2+1]='X';
					yer2=yer2+1;
				}
				else{
					t[i]=m[yer1][yer2+1];
					m[yer1][yer2+1]='X';
					m[yer1][yer2]='0';
					yer2=yer2+1;
					i++;
				}
			}
			if(a==75){
				//printf("sol");
				if(yer2-1<0 || m[yer1][yer2-1]=='1' ){
					printf("hatali hamle");
				}
				
				else if(m[yer1][yer2-1]=='K' || m[yer1][yer2-1]=='C'){
					cik=1;
					if(m[yer1][yer2-1]=='C'){
						cikis=1;
					}
				}
				else if(m[yer1][yer2-1]=='0'){
					m[yer1][yer2]='0';
					m[yer1][yer2-1]='X';
					yer2=yer2-1;
				}
				else{
					t[i]=m[yer1][yer2-1];
					m[yer1][yer2-1]='X';
					m[yer1][yer2]='0';
					yer2=yer2-1;
					i++;
				}
			}
		}
		
		sleep(1); system("cls"); 
	
	   simdiki = clock();

        
        sure = (double)(simdiki - baslangic) / CLOCKS_PER_SEC;
	
	}while (sure < sureLimiti && cik==0);
	if(sure>=sureLimiti || cikis==0){
		printf("BOOOOM!");
	}
	if(cikis==1){
		for(j=0;j<i;j++){
       	    if(t[j]=='p'){
       	    	p++;
			   }
			else if(t[j]=='E'){
				E++;
			}
			else if(t[j]=='P'){
				P++;
			}  
			else if(t[j]=='e'){
				e++;
			}
       	
	   }
	   
	}
	if( cikis==1){
		if(p-P>0 && E-e>0){
			if((p-P)>=(E-e)){
				printf("%d adet karsit madde uretildi.\n",(E-e));
				printf("kalan sure: %lf\n",sure);
				puan=E-e;
				
				puan+=sureLimiti-sure;
			}
			else{
				printf("%d adet karsit madde uretildi.\n",(p-P));
					printf("kalan sure: %lf\n",sure);
				puan=p-P;
				puan+=sureLimiti-sure;
			}
		}
		else{
			printf("karsit madde uretilemedi.\n");
		}
	}
	printf("puan= %d\n",puan);
	
	
	return puan;


}
