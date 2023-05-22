#include<stdio.h>
#include<stdlib.h>

typedef struct {											//MUSTERILERI TUTAN STRUCT YAPISI
	int musteri_no;
	char ad[20], soyad[20];
} Musteri;


void initializeHashTablo(int hash_tablo[10]){							//HASH TABLOSUNU EN BASTA BOS OLUCAK SEKILDE OLUSTURAN FONKSIYON
	int i;
	for(i = 0; i < 10; ++i){
		hash_tablo[i] = -1;													//INTEGER OLDUGU ICIN NULL DEGER YERINE -1 ATANIYOR
	}
}

void initializeVeriListe(Musteri *veri_list[10]){							//MUSTERI BILGILERINI TUTACAK DIZIYI OLUSTURAN FONKSIYON
	int i;
	for(i = 0; i < 10; ++i){
		veri_list[i] = NULL;												//HER INDISE NULL ATANIYOR
	}
}

int hash1(int n){															//1. HASH FONKSIYONU
	return (3 * n + 2);
}

int hash2(int n){															//2. HASH FONKSIYONU
	return (7 * n + 2);
}

int isFull(int hash_tablo[10]){												//HASH TABLOSUNUN DOLU OLUP OLMADIGINI KONTROL EDEN FONKSIYON
	int i;
	int flag = 1;
	for(i = 0; i < 10; ++i){
		if(hash_tablo[i] == -1)
			flag = 0;
	}
	return flag;
}

void addHash(Musteri *musteri, int hash_tablo[10], Musteri *veri_list[10], int *turn, int *count){						//GELEN MUSTERI STRUCTINI HASH TABLOSUNA EKLEYEN FONKSIYON
	int index;	
	int i = 0;
	if(isFull(hash_tablo) == 0){										//EGER HASH TABLOSU BOS DEGIL ISE
		do{
			index = (hash1(musteri->musteri_no) + i * hash2(musteri->musteri_no)) % 10;					//HASH FONKSIYONLARINDAN INDEX DEGERI ELDE EDILIYOR
			i++;
		}while(hash_tablo[index] != -1);
		
		veri_list[index] = musteri;													//INTEGER DIZIYE VE MUSTERI DIZISININ HESAPLANAN INDEX DEGERINE MUSTERI YAZILIYOR
		hash_tablo[index] = musteri->musteri_no;
		*turn += i;
		(*count)++;																	//KAC ADIMDA YAZILDIGI VE MUSTERI SAYISINI TUTAN DEGISKENLER UPDATE EDILIYOR
	}
	else{
		printf("Tablo Dolu!\n");
	}
	
}

void readFile(int hash_tablo[10], Musteri *veri_list[10], int *turn, int *count){						//DOSYAYI OKUMAMIZI SAGLAYAN FONKSIYON
	FILE *file = NULL;
	Musteri *musteri;
	if((file = fopen("veri.txt", "r")) != NULL){											//EGER DOSYAYI ACABILDIYSEK
		while(!feof(file)){																	//DOSYANIN SONUNA KADAR DONEN DONGU
			musteri = (Musteri *)malloc(sizeof(Musteri));									//MUSTERI STRUCTINDEN NODE OLUSTURUYORUZ
			fscanf(file, "%d", &musteri->musteri_no);										//MUSTERI BILGILERINI DOSYADAN OKUYORUZ
			fscanf(file, "%s", &musteri->ad);
			fscanf(file, "%s", &musteri->soyad);
			
			addHash(musteri, hash_tablo, veri_list, turn, count);							//OLUSTURDUGUMUZ MUSTERI STRUCTRINA VERIYI EKLEMEYI SAGLAYAN FONKSIYONU CAGIRIYORUZ
		}
	}
}

void display(int hash_tablo[10], Musteri *veri_list[10]){								//MUSTERILERI EKRANA YAZDIRMAMIZI SAGLAYAN FONKSIYON
	int i;
	for(i = 0; i < 10; ++i){
		if(veri_list[i] != NULL){
			printf("%d %s %s\n", veri_list[i]->musteri_no, veri_list[i]->ad, veri_list[i]->soyad);
		}
	}
	printf("\n\n");
}

Musteri* search(int no, Musteri *veri_list[10], int *turn){								//MUSTERILERI HASH TABLOSUNDA ARAYAN FONKSIYON
	int i;
	int index;
	int visited[10] = {0};
	int flag = 0;
	
	(*turn) = 0;
	while(flag == 0){							
		flag = 1;
		for(i = 0; i < 10; ++i){										//KONTROL EDILEN INDEXLERI TUTAN DIZI
			if(visited[i] == 0)
				flag = 0;
		}
		if(flag == 0){
			index = (hash1(no) + (*turn) * hash2(no)) % 10;								//ARANAN NUMARAYA GORE INDEX DEGERI HESAPLANIYOR
			if(veri_list[index] != NULL && veri_list[index]->musteri_no == no){			//EGER ARANAN YERDE ISTEDIGIMIZ NUMARA VARSA DONDURULUYOR
				return veri_list[index];
			}
			else{																			//EGER ARANAN YERDE BASKA BIRISI VAR ISE KONTROLE DEVAM EDILIYOR
				visited[index] = 1;
				(*turn)++;
			}
		}
		else{
			return NULL;									//DIZIDE ARADIGIMIZ ELEMAN YOK ISE NULL DONUYOR
		}
	}
}

int main(){
	
	Musteri *veri_list[10]; 
	int hash_tablo[10];
	int choice;
	int no;
	int turn;
	int searchTurn;
	float avgTurn;
	int totalTurn;
	int count;
	Musteri *musteri;
	
	
	totalTurn = 0;
	count = 0;
	do{
		do{
			system("cls");																			//MENUNUN EKRANA YAZILMASI
			printf("1. Read File\n2. Search\n3. Display\n4. Average Turn\n5. Exit\n\nYour Choice: ");
			scanf("%d",&choice);
		}while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5);
		
		switch(choice){
			case 1:
				initializeHashTablo(hash_tablo);
				initializeVeriListe(veri_list);
				readFile(hash_tablo, veri_list, &totalTurn, &count);								//DOSYAYI OKUMAMIZI SAGLAYAN FONKSIYON CAGRILMASI
				avgTurn = (float)totalTurn / count;													//BUTUN VERILERE ORTALAMA KAC ADIMDA ULASILDIGININ HESAPLANMASI
				printf("File Read Succesfully\n\nPress Any Key!");
				getch();
				break;
			case 2:
				printf("Aranilan Deger: ");										//ARANILAN DEGERIN GIRILMESI
				scanf("%d",&no);
				turn = 0;
				musteri = search(no, veri_list, &searchTurn);					//GIRILEN DEGERE GORE ARAYAN FONKSIYONUN CAGRILMASI
				if(musteri != NULL){
					printf("%d %s %s\nVeri %d Adimda Bulundu.\n", musteri->musteri_no, musteri->ad, musteri->soyad, searchTurn + 1);
				}
				else{
					printf("Veri Bulunamadi.\n");
				}
				printf("\n\nPress Any Key!");
				getch();
				break;
			case 3:
				display(hash_tablo, veri_list);						//TABLOYU EKRANA YAZDIRAN FONKSIYONUN CAGRILMASI
				printf("\n\nPress Any Key!");
				getch();
				break;
			case 4:
				printf("Tum Verilere Ortalama %.2f Adimda Ulasiliyor!", avgTurn);				//ORTALAMA ADIMIN EKRANA YAZDIRILMASI
				printf("\n\nPress Any Key!");
				getch();
				break;
		}
	}while(choice != 5);

	
	return 0;
}
