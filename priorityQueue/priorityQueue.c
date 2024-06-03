#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void matrisOlustur(int **matris, int *kapasite, int N, int M, int K);
void printMatris(int **matris, int N, int M);
int findMax(int **matris, int N);
void dequeue(int **matris, int M, int satir, int *kapasite);
void maxHeapOlustur(int **matris, int satir, int N);
void maxHeapify(int **matris, int satir, int parent, int N);
void swap(int **matris, int satir, int parent, int enBuyuk);

/*
@brief matrisOlustur fonksiyonu N, M, kapasite degerlerine gore matrise random sayilar atar ve bos kalan yerlere -1 yazdirir.
@param matris icine sayilar yazdirilacak matris
@param kapasite her queue'nun(satirin) eleman sayisini tutan dizi
@param N matrisin satir sayisi
@param M matrisin sutun sayisi
*/
void matrisOlustur(int **matris, int *kapasite, int N, int M, int K){
	int i, j, *kullanilanSayilar, t=0, sayiVar=0, sayiAdedi=0;
	
	for(i=0;i<N;i++){                                              //tum kapasiteleri toplayarak uretilmesi gereken toplam random sayi adedini bulma
    	sayiAdedi+=kapasite[i];
	}
    kullanilanSayilar = (int *)malloc(sayiAdedi * sizeof(int));    //kullanilan sayilari tutuan dizi icin dinamik yer ayirma
	if(kullanilanSayilar == NULL) {
        printf("Bellek ayrilamadi.");
        exit(1);
    }
    
	srand(time(NULL));  
 	for(i=0;i<sayiAdedi;i++){                                      //sayiAdedi kadar birbirinden farkli random sayi uretmek icin kullanilan dongu
		kullanilanSayilar[i] = rand() % K+1;                       //kullanilanSayilar dizisine, uretilen random sayiyi atama
		for(j=0;j<i;j++){                                          //onceden eklenmis sayilari gezerek uretilen sayinin daha once kullanilip kullanilmadigini bulma
			if(kullanilanSayilar[i]==kullanilanSayilar[j]){        //eger onceden kullanilmis bir sayiysa sayiVar flag'ini 1 yapma
				sayiVar=1;
			}
		}
		if(sayiVar==1){                                            //onceden eklenmis tum sayilar gezilip kontrol tamamlandiktan sonra sayiVar flag'i 1 ise(yani onceden kullanilan bir sayi tekrar uretilmisse)
			sayiVar=0;                                             //i bir eksiltilerek son eklenen sayi yerine baska bir sayi eklenir ve ayni kontroller tekrarlanir.
			i--;
		}
	}	

	for(i=0;i<N;i++){
		for(j=0;j<kapasite[i];j++){
			matris[i][j]= kullanilanSayilar[t];                    //uretilen unique random sayilari iceren kullanilanSayilar dizisi her queue'nun kapasitesine göre matrise gecirilir.
			t++;
		}
		if(kapasite[i]<M){                                         //her queue'nun kapasitesi matrisin sutun sayisi ile karsilastirilarak bos kalan yerlere -1 yazdirilir.
			for(j=kapasite[i];j<M;j++){
				matris[i][j]=-1;
			}
		}
	}
}
/*
@brief printMatris fonksiyonu matrisi ekrana yazdirir.
@param matris ekrana yazdirilacak matris
@param N ekrana yazdirilacak matrisin satir sayisi
@param M ekrana yazdirilacak matrisin sutun sayisi
*/
void printMatris(int **matris, int N, int M){
	int i, j;
	for(i=0;i<N;i++){
		for(j=0;j<M;j++){
			printf("%d\t",matris[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}
/*
@brief findMax fonksiyonu queue'larýn ilk elemanlarýný karþýlaþtýrarak en buyugunu bulur.
@param matris satirlarinin ilk elemanlarýnýn karsilaltirilacagi matris
@param N matrisin satir sayisi
@return en buyuk elemanin satir sayisini dondurur
*/
int findMax(int **matris, int N){
	int i, max=-1, index;                           	//Satirlarin ilk elemanlarini karsilastirip en buyugunu bulmak icin max=-1 olarak belirlendi. 
	printf("Kuyruklardan degerleri okunan elemanlar:\n");
	for(i=0;i<N;i++){                              		//matrisin satirlarini gezer.
		if(matris[i][0]!=-1){
			printf("%d  ", matris[i][0]);                        
		    if(matris[i][0]>max){                		//gezilen eleman -1 degilse ve max'tan buyukse o eleman max olarak atanir.
				index=i;                                //fonksiyondan dondurmek icin max elemanin index'i tutulur.
				max=matris[i][0];
			}
		}
	}
	printf("\nSecilen eleman: %d\n\n", max=matris[index][0]);
	return index;                                   //max elemanin index'i dondurulur.
}
/*
@brief dequeue fonksiyonu parametre olarak aldigi satirin ilk elemanini dequeue eder.
@param matris dequeue islemi yapilacak satirin bulundugu matris
@param M matrisin sutun sayisi
@param satir dequeue islemi yapilacak satirin index'i
@param kapasite her queue'nun içerdiði eleman sayisini tutan dizi
*/
void dequeue(int **matris, int M, int satir, int *kapasite){
	int i=1;
	while(matris[satir][i]!=-1 && i<M){      //queue icinde gezerek -1'e veya matrisin sutun sayisina gelene kadar elemanlari kaydirarak dequeue islemi yapar.
		matris[satir][i-1]=matris[satir][i]; //her elemani bir önceki elemanin üstüne yazar.
		i++;
	}
	matris[satir][i-1]=-1;                   //queue'dan eleman silindigi icin eleman sayisi azaldi bu yüzden son elemanin yerine -1 yazilir.
	kapasite[satir]--;                       //queue'dan eleman silindigi icin eleman sayisi azaldi bu yüzden satirin kapasitesini 1 azaltilir.
}
/*
@brief maxHeapify fonksiyonu parametre olarak aldigi satiri max heap haline getirir.
@param matris heapify islemi yapilacak satirin bulundugu matris
@param satir heapify islemi yapilacak satirin index'i
@param i karsilastirma islemi yapilacak elemanlarin parent'inin index'i
@param N matrisin satir sayisi
*/
void maxHeapify(int **matris, int satir, int parent, int N){
	int sol=2*parent+1;                                        //sol cocugun index'i parent'in indexinin iki katindan bir fazladir.
	int sag=2*parent+2;                                        //sag cocugun index'i parent'in indexinin iki katindan iki fazladir.
	int enBuyuk;                                               //parent ve cocuklar arasindaki en buyuk elemanini indexi enBuyuk degiskeninde tutulur. 
	if(sol<=N && matris[satir][sol]>matris[satir][parent]){    //sol cocuk varsa parent ile karsilastirilir ve hangisi buyukse enBuyuk degiskeni onun indexine esitlenir.
		enBuyuk=sol;
	}
	else{
		enBuyuk=parent;
	}
	if(sag<=N && matris[satir][sag]>matris[satir][enBuyuk]){   //sag cocuk varsa enBuyuk ile karsilastirilir ve hangisi buyukse enBuyuk degiskeni onun indexine esitlenir.
		enBuyuk=sag;
	}
	if(enBuyuk!=parent){                                       //eger enBuyuk parent degilse max heap ozelliginin saglanmasi icin en buyuk elemanin parent ile yer degistirmesi gerekir.
		swap(matris, satir, parent, enBuyuk);                  //parent ve enBuyuk yer degistirir.
		maxHeapify(matris, satir, enBuyuk, N);                 //yer degisimi yasandigi icin tekrar heapify yapilmasi gerekir.
	}
}
/*
@brief swap fonksiyonu parametre olarak aldigi index'lerdeki elemanlarin yerlerini degistirir.
@param matris swap islemi yapilacak satirin bulundugu matris
@param satir swap islemi yapilacak satirin index'i
@param parent yer degisimi yapilacak elemanlardan biri
@param enBuyuk yer degisimi yapilacak elemanlardan biri
*/
void swap(int **matris, int satir, int parent, int enBuyuk){
	int tmp;
	tmp=matris[satir][parent];
	matris[satir][parent]=matris[satir][enBuyuk];
	matris[satir][enBuyuk]=tmp;
}
/*
@brief maxHeapOlustur fonksiyonu parametre olarak aldigi satirin leaf node'lar haric tum elemanlarini gezerek hepsi icin maxHeapify fonksiyonunu cagirir ve satiri max heap haline getirir.
@param matris heapify islemi yapilacak satirin bulundugu matris
@param satir heapify islemi yapilacak satirin index'i
@param N matrisin satir sayisi
*/
void maxHeapOlustur(int **matris, int satir, int N){
	int i;
	for(i=N/2;i>=0;i--){                     //leaf node'lar haric tum elemanlari gezerek maxHeapify fonksiyonu cagirilir ve satirda bulunan queue max heap haline getirilir.
		maxHeapify(matris, satir, i, N);
	}
}

int main(){
	int i, j, N, M, K, *kapasite, **matris, *bitenKuyruk;
	int maxIndex;
	
	printf("Matris boyutlarini giriniz:\nN: ");            //matris boyutlari kullanicidan alinir
	scanf("%d",&N);
	printf("M: ");
	scanf("%d", &M);
	
	printf("K giriniz: ");                                 //K kullanicidan alinir
	scanf("%d", &K);
	while(K<=N*M){
		printf("K, N*M'den buyuk olmalidir.\nYeniden giris yapin: ");
		scanf("%d", &K);
	}
	
	kapasite = (int *)malloc(N * sizeof(int));             //kullanilan diziler ve matris icin dinamik yer ayirma
	if(kapasite == NULL) {
        printf("Bellek ayrilamadi.");
        exit(1);
    }
	matris = (int **)malloc(N * sizeof(int *));
	if(matris == NULL) {
        printf("Bellek ayrilamadi.");
        exit(1);
    }
	for(i=0; i<N; i++) {
        matris[i]=(int *)malloc(M * sizeof(int));
        if (matris[i]==NULL) {
            printf("Bellek ayrilamadi.");
            exit(1);
        }
    }
    bitenKuyruk = (int *)malloc(N * sizeof(int));
    if(bitenKuyruk == NULL) {
        printf("Bellek ayrilamadi.");
        exit(1);
    }
    
	printf("Her kuyrugun kapasitesini girin:\n");            //queue'larin kapasiteleri kullanicidan alinir ve kapasite dizisinde saklanir.
	for(i=0;i<N;i++){
		scanf("%d", &kapasite[i]);
		while(kapasite[i]>M){
			printf("Kuyrugun kapasitesi M'den buyuk olamaz.\nYeniden giris yapin: ");
			scanf("%d", &kapasite[i]);
		}
	}

    
    matrisOlustur(matris, kapasite, N, M, K);                  //matrisi, kapasite ve matris boyutlarina uygun sekilde unique random sayilar ve -1'ler ile olusturma,
    
	printMatris(matris, N, M);
	for(i=0;i<N;i++){                                          //tum queue'lar max heap haline getirilir
		maxHeapOlustur(matris, i, kapasite[i]);
	}
	printf("Tum queue'lar max-heap-tree haline getirildi.\n");
	printMatris(matris, N, M);
	i=0;                                                        //tum kuyruklarin bosaldigini anlamamizi saglayan sayac
	while(i<N){                                                 //tum kuyruklar bosalana kadar karsilastirma, dequeue ve heapify islemlerinin yapilmasi icin kullanilan dongu.
		maxIndex=findMax(matris, N);                            //tum kuyruklarin ilk elemanlarini karsilastirip en buyugu bulur ve satirinin index'ini dondurur.
		dequeue(matris, M, maxIndex, kapasite);                 //findMax'tan gelen, en buyuk elemanin satirinin index'ini alir ve o satirdaki queue'ya dequeue islemi uygular.
		if(matris[maxIndex][0]==-1){                            //dequeue'dan sonra kuyrugun bosalip bosalmadigini kontrol eder.
			bitenKuyruk[i]=maxIndex;                            //kuyruk bos ise bitenKuyruk dizisine kuyrugun bulundugu satirin index'i eklenir.
			i++;                                                //i bir arttirilir boylece biten kuyruklarin sirasi dogru sekilde tutulur ve dongu dogru yerde sonlanir.
		}
		else{
			maxHeapOlustur(matris, maxIndex, kapasite[maxIndex]);   //kuyruk bos degilse kuyrugu tekrar max heap haline getirir.
		}
		printMatris(matris, N, M);
	}
	printf("Sirasiyla elemanlari tukenen kuyruklar: ");
	for(i=0;i<N;i++){                                           //biten kuyruklarin bulundugu satirlar bitme sirasina gore ekrana yazdirilir.
		printf("%d   ", bitenKuyruk[i]+1);
	}
	/*
	Video'yu hazirladiktan sonra free kisimlarinin eksik oldugunu fark edip ekledim.
	*/
	free(kapasite);                                             //dinamik olarak ayrilan alanlari serbest birakma
	free(bitenKuyruk);
	for (i=0; i<N; i++) {
    	free(matris[i]);
	}
	free(matris);
}
