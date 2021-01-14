#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<time.h>
#include "declaracao.hpp"

extern Estatistica estatistica;

pista * geraPista(pista *p){

	p = (pista*)malloc(sizeof(pista));
	p->f1 =(aviao*)malloc(sizeof(aviao));
	p->f1 = NULL;
	p->f2 =(aviao*)malloc(sizeof(aviao));
	p->f2 = NULL;
	p->f3 =(aviao*)malloc(sizeof(aviao));
	p->f3 = NULL;
	
	p->qtd1 = 0;
	p->qtd2 = 0;
	p->qtd3 = 0;
	
	p->situacao = false;

	return p;
}

pista * removeCombustivel(pista *p){
	aviao *aux1  = p->f1;
	aviao *aux2  = p->f2;
	
	while(aux1 != NULL){
		aux1->fuel--;
		aux1 = aux1->prox;
	}
	while(aux2 != NULL){
		aux2->fuel--;
		aux2 = aux2->prox;
	}
	return p;
}

//Media Decolagem(b)
pista * tempoDecol(pista *p){
	aviao *aux  = p->f3;
	
	while(aux != NULL){
		aux->decol++;
		aux = aux->prox;
	}
	return p;
}

aviao *gerar_Aviao(bool tipo){
	aviao *novo=(aviao*)malloc(sizeof(aviao));
	novo->prox=NULL;
	
	if(tipo){
		novo->ID = estatistica.atualImpar;
		estatistica.atualImpar += 2;
		novo->fuel = 0;
		//Media Decolagem(b)
		estatistica.MedDecol++;
		novo->decol=0;
	}else{
		novo->ID = estatistica.atualPar;
		estatistica.atualPar += 2;
		novo->fuel = geracaoFuel();
		novo->fuelinicial=novo->fuel;
		//Media Aterrisagem(c)
		estatistica.MedPouso++;
	}		
	return novo;
}

pista *Insere_Aviao(pista *alvo, bool tipo){ 
	aviao *novo = gerar_Aviao(tipo);
	aviao *aux = NULL, *ant = NULL;
	int indicadorFila; //se i = 1 (f1) se i = 2 (f2) se i = 3 (f3)
  
	if(!tipo){
		if(alvo->qtd1 <= alvo->qtd2){
			aux = alvo->f1;
			indicadorFila = 1;
			alvo->qtd1++;
		}else{
			aux = alvo->f2;
			indicadorFila = 2;
			alvo->qtd2++;
		}
	}else{
		aux = alvo->f3;
		indicadorFila = 3;
		alvo->qtd3++;
	}	

	if(alvo->f1 == NULL && indicadorFila == 1){
		alvo->f1 = novo;
		return alvo;
	}else if(alvo->f2 == NULL && indicadorFila == 2){
		alvo->f2 = novo;
		return alvo;
	}else if(alvo->f3 == NULL && indicadorFila == 3){
		alvo->f3 = novo;
		return alvo;
	}
	
	while(aux != NULL){
		ant = aux;
		aux = aux->prox;
	}
	
	ant->prox = novo;
	
	return alvo;
}

aviao *remove_aviao(aviao *alvo, int id){
	aviao *ant = NULL;
	aviao *aux = alvo;
	
	if(alvo == NULL)
		return alvo;
  
	while(aux != NULL && aux->ID != id){
		ant = aux;
		aux = aux->prox;
	}
	
	if(aux != NULL){
		if(ant == NULL)
			alvo = aux->prox;
		else
			ant->prox = aux->prox;
		
		free(aux);
	}
	return alvo;
}

pista *procuraVoid(pista *alvo){
	aviao *aux1 = alvo->f1;
	aviao *aux2 = alvo->f2;
	int *remocaoF1 = NULL, *remocaoF2 = NULL, auxiliarRemocao1 = 0, auxiliarRemocao2 = 0;
			
	while(aux1 != NULL){
		if(aux1->fuel == 0){
			remocaoF1 = (int *) realloc (remocaoF1,(auxiliarRemocao1 + 1) * sizeof (int));
			remocaoF1[auxiliarRemocao1] = aux1->ID;
			auxiliarRemocao1++;
		}
		aux1 = aux1->prox;
	}
	
	while(aux2 != NULL){
		if(aux2->fuel == 0){
			remocaoF2 = (int *) realloc (remocaoF2,(auxiliarRemocao2 + 1) * sizeof (int));
			remocaoF2[auxiliarRemocao2] = aux2->ID;
			auxiliarRemocao2++;
		}
		aux2 = aux2->prox;
	}
	
	for(int i = 0; i < auxiliarRemocao1; i++){
		alvo->f1 = remove_aviao(alvo->f1, remocaoF1[i]);
		alvo->qtd1--;
		estatistica.caiu++;	
	}
	
	for(int x = 0; x < auxiliarRemocao2; x++){
		alvo->f2 = remove_aviao(alvo->f2, remocaoF2[x]);
		alvo->qtd2--;
		estatistica.caiu++;	
	}
	
	free(remocaoF1);
	free(remocaoF2);
	
	return alvo;
}

void procura_berg(pista *alvo, int remocao[], int endereco[], int &auxiliarRemocao){
	aviao *aux1 = alvo->f1;
	aviao *aux2 = alvo->f2;
	int i = 0;
	
	while(aux1 != NULL){
		if(aux1->fuel == 1){
			//Aterrisagem sem reserva(d)
			estatistica.avSemFuel++;
			remocao[auxiliarRemocao] = aux1->ID;
			endereco[auxiliarRemocao] = 1;
			
			remocao[0] = 1;
			endereco[0] = 1;
			
			auxiliarRemocao++;
				
		}
		aux1 = aux1->prox;
	}
		
	while(aux2 != NULL){
		if(aux2->fuel == 1){
			//Aterrisagem sem reserva(d)
			estatistica.avSemFuel++;
			remocao[auxiliarRemocao] = aux2->ID;
			endereco[auxiliarRemocao] = 2;
			
			remocao[0] = 1;
			endereco[0] = 1;
			
			auxiliarRemocao++;
				
		}
		aux2 = aux2->prox;
	}
		
}

int geracaoFuel(){
	int newFuel = rand() % 20 + 1;
	return newFuel;
}

int geracaoAviao(){
	int newAviao = rand() % 4;
	return newAviao;
}

int Escolhe_pista(pista *p1,pista *p2){
	if((p1->qtd1+p1->qtd2)==(p2->qtd1+p2->qtd2)){
		return 1;
	}else if((p1->qtd1+p1->qtd2)<(p2->qtd1+p2->qtd2)){
		return 1;
	}else if((p1->qtd1+p1->qtd2)>(p2->qtd1+p2->qtd2)){
		return 2;
	}
} 

void imprimeFila(pista *alvo,int num){
	aviao *aux1 = alvo->f1;
	aviao *aux2 = alvo->f2;
	aviao *aux3 = alvo->f3;
	      			  	 						
	printf("PISTA %d:\n",num);
	printf("\tFILA 1:\n");
	while(aux1 != NULL){
		printf("\t\t|ID: %d  FUEL: %d|\n",aux1->ID,aux1->fuel);
		aux1 = aux1->prox;
	}
	printf("\n");
	printf("\tFILA 2:\n");
	while(aux2 != NULL){
		printf("\t\t|ID: %d  FUEL: %d|\n",aux2->ID,aux2->fuel);
		aux2 = aux2->prox;
	}
	printf("\n");
	printf("\tFILA 3:\n");
	while(aux3 != NULL){
		printf("\t\t|ID: %d  FUEL: %d  DECOL: %2.f|\n",aux3->ID,aux3->fuel,aux3->decol);
		aux3 = aux3->prox;
	}
	printf("\n");
}

pista * remove_fila(int f, pista *p){
	if(f ==1){
		aviao *ant = p->f1;
		
		if(ant == NULL)
			return p;
      
		aviao *aux = ant->prox;
		//Media Aterrissagem(c)
		estatistica.EDeltaFuel=(p->f1->fuelinicial - p->f1->fuel)+estatistica.EDeltaFuel;
		free(p->f1);
		p->f1 = aux;
		
		p->qtd1--;
		p->situacao = true;

	} if(f ==2){
		aviao *ant = p->f2;
		
		if(ant == NULL)
			return p;
		
		aviao *aux = ant->prox;
		//Media Aterrisagem(c)
		estatistica.EDeltaFuel=(p->f2->fuelinicial - p->f2->fuel)+estatistica.EDeltaFuel;
		free(p->f2);
		p->f2 = aux;
		
		p->qtd2--;
		p->situacao = true;
		
	}if(f ==3){		
		aviao *aux = p->f3;
		if(aux == NULL)
			return p;
			
		p->f3 = aux->prox;
		free(aux);

		p->qtd3--;
		p->situacao = true;
	}
	return p;
}

int verifica_fila(pista *p){

	aviao *p1 = p->f1;
	aviao *p2 = p->f2;

	int cont1=0, cont2=0, cont3=0;
	
	if(p1 == NULL)
		return 2;
	else if(p2 == NULL)
		return 1;
	else if(p1 == NULL && p2 == NULL)
		return 0;

	for(p1 = p->f1; p1 != NULL; p1 = p1->prox)
		cont1 += p1->fuel;
	
	for(p2 = p->f2; p2 != NULL; p2 = p2->prox)
		cont2 += p2->fuel;
		
	if(cont1 <= cont2)
		return 1;
	else
		return 2;
}
