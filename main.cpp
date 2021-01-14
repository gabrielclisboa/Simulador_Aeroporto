#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<time.h>
#include "declaracao.hpp"

Estatistica estatistica;

int main(){
	estatistica.atualImpar = 1;
	estatistica.atualPar = 0;
	estatistica.caiu = 0;
	estatistica.avSemFuel=0;
	estatistica.MedDecol=0;
	estatistica.MedPouso=0;
	estatistica.EDeltaFuel=0;
	estatistica.EtempoDecol=0;
	
	
	FILE *graficoDecol=fopen("graficoMediaDecol.xls","w");
	FILE *graficoAterri=fopen("graficoMediaAterri.xls","w");
	FILE *graficoSemReserva=fopen("graficoSemReserva.xls","w");
	FILE *graficoCairam=fopen("graficoCairam.xls","w");
	
	
	bool tipo;
	int tempo = 0, escolha, sorteioPouso, sorteioDecolagem;
	float mediaPouso=-1,mediaDecol=-1;
	
	pista *pista1 = geraPista(pista1);
	pista *pista2 = geraPista(pista2);
	pista *pista3 = geraPista(pista3);
	
			
	srand(time(NULL));
	while(tempo != 100000){
		
		int remocaoP1[4], enderecoP1[4], qtd1 = 1, auxiliarRemocao1 = 1;
		int remocaoP2[4], enderecoP2[4], qtd2 = 1, auxiliarRemocao2 = 1;
		int limite = 0, escolhido = 0;
		
		pista1->situacao = false;
		pista2->situacao = false;
		pista3->situacao = false;
		
	
		remocaoP1[0] = 0;
		remocaoP2[0] = 0;
		
		sorteioPouso = geracaoAviao();
		tipo = false;
	//	printf("%02d Avioes para pouso.\n",sorteioPouso);
		for(int x = 0; x < sorteioPouso; x++){
			escolha=Escolhe_pista(pista1,pista2);
			if(escolha==1)
				pista1 = Insere_Aviao(pista1,tipo); 
			else
				pista2 = Insere_Aviao(pista2,tipo);
		}
	
		sorteioDecolagem = geracaoAviao();
		tipo = true;
	//	printf("%02d Avioes para decolagem.\n",sorteioDecolagem);
		for(int x = 0; x < sorteioDecolagem; x++)
			pista3 = Insere_Aviao(pista3,tipo);
		
		
	//	imprimeFila(pista1,1);
	//	imprimeFila(pista2,2);
	//	imprimeFila(pista3,3);
	
		//Funcoes para remover avioes com 0 de combustivel	
		pista1 = procuraVoid(pista1);
		pista2 = procuraVoid(pista2);
		
		//procura os avioes que estao na beira do precipicio
		procura_berg(pista1, remocaoP1, enderecoP1, auxiliarRemocao1);
		procura_berg(pista2, remocaoP2, enderecoP2, auxiliarRemocao2);
		
		if(remocaoP1[0] == 1){
			while(limite < 3 && qtd1 < auxiliarRemocao1){		
				switch(enderecoP1[qtd1]){
					case 1: pista1->f1 = remove_aviao(pista1->f1, remocaoP1[qtd1]);
					pista1->qtd1--;
					break;
					
					case 2: pista1->f2 = remove_aviao(pista1->f2, remocaoP1[qtd1]);
					pista1->qtd2--;
					break;
				}
				limite++;
				qtd1++;
			}
		}
			
		if(limite == 1)
			pista3->situacao = true;
			
		if(limite == 2){
			pista1->situacao = true;
			pista3->situacao = true;
		}
			
		if(limite == 3){
			pista2->situacao = true;
			pista1->situacao = true;
			pista3->situacao = true;
		}
					
		if(limite < 3){
			if(remocaoP2[0] == 1){
				while(limite < 3 && qtd2 < auxiliarRemocao2){
					
					if(limite == 0)
						pista3->situacao = true;
					else if(limite == 1)
						pista2->situacao = true;
					else if(limite == 2)
						pista1->situacao = true;
					
					switch(enderecoP2[qtd2]){
						case 1: pista2->f1 = remove_aviao(pista2->f1, remocaoP2[qtd2]);
						pista2->qtd1--;
						break;
					
						case 2: pista2->f2 = remove_aviao(pista2->f2, remocaoP2[qtd2]);
						pista2->qtd2--;
						break;
					}
					limite++;
					qtd2++;
				}
			}
		}
		
		if(limite == 3){
			pista1->situacao = true;
			pista2->situacao = true;
			pista3->situacao = true;
		}
		
		
		//normais
		if(limite == 0){
			escolhido = verifica_fila(pista1);
			if(escolhido != 0)	
				pista1 = remove_fila(escolhido, pista1);
			
			escolhido = verifica_fila(pista2);
			if(escolhido != 0)
				pista2 = remove_fila(escolhido, pista2);
			
			//Media Decolagem(b)
			if(pista3->f3!=NULL)
				estatistica.EtempoDecol=estatistica.EtempoDecol+pista3->f3->decol;
				
			pista3 = remove_fila(3, pista3);
						
		}else if(limite == 1){
			escolhido = verifica_fila(pista1);
			if(escolhido != 0)
				pista1 = remove_fila(escolhido, pista1);
			
			escolhido = verifica_fila(pista2);
			if(escolhido != 0)
				pista2 = remove_fila(escolhido, pista2);
		
		}else if(limite == 2){
			if(pista1->situacao == true){
				escolhido = verifica_fila(pista2);
				if(escolhido != 0)
					pista2 = remove_fila(escolhido, pista2);
			}else if(pista2->situacao == true){
				escolhido = verifica_fila(pista1);
				if(escolhido != 0)
					pista1 = remove_fila(escolhido, pista1);
			}
		}
		
		if(pista1->situacao == false && pista3->f3!=NULL ){
			//Media Decolagem(b)
			estatistica.EtempoDecol=estatistica.EtempoDecol+pista3->f3->decol;
			pista3 = remove_fila(3, pista3);	
		}
			
			
		if(pista2->situacao == false&& pista3->f3!=NULL){
			//Media Decolagem(b)
			estatistica.EtempoDecol=estatistica.EtempoDecol+pista3->f3->decol;
			pista3 = remove_fila(3, pista3);
		}
		
			
		removeCombustivel(pista1);
		removeCombustivel(pista2);
	    pista3=tempoDecol(pista3); 
	 		
		if(estatistica.MedPouso!=0){
			mediaPouso=estatistica.EDeltaFuel/estatistica.MedPouso;
		}
		if(estatistica.MedDecol!=0){
     		mediaDecol= estatistica.EtempoDecol/estatistica.MedDecol;
    	}
			
		
		if(mediaDecol!=-1){                                         //
			fprintf(graficoDecol,"%d;%f\n",tempo,mediaDecol);          //     
		}															      //		 
		if(mediaPouso!=-1){													//Escrevendo nos arquivos que vão ser ,posteriormente, os gráficos.
			fprintf(graficoAterri,"%d;%f\n",tempo,mediaPouso);              //
		}																   //			
		fprintf(graficoSemReserva,"%d;%d\n",tempo,estatistica.avSemFuel);//
		fprintf(graficoCairam,"%d;%d\n",tempo,estatistica.caiu);     	//
	/*	
		printf("TEMPO MEDIO DE ESPERA PARA DECOLAGEM: %.2f\n",mediaDecol);
		printf("TEMPO MEDIO DE ESPERA PARA ATERRISAGEM: %.2f\n",mediaPouso);
		printf("NUMERO DE AVIOES QUE ATERRISSAM SEM RESERVA DE GASOSA: %d\n",estatistica.avSemFuel);
		printf("CAIRAM COITADOS: %d\n",estatistica.caiu);
		
		printf("%d\t%d\t%d\n",pista1->qtd1,pista1->qtd2,pista1->qtd3);
		printf("%d\t%d\t%d\n",pista2->qtd1,pista2->qtd2,pista2->qtd3);
		printf("%d\t%d\t%d\n",pista3->qtd1,pista3->qtd2,pista3->qtd3);
	*/	
	//	system("pause");
	//	system("cls");
			 
		tempo++;
	}
	
	printf("TEMPO MEDIO DE ESPERA PARA DECOLAGEM: %.2f\n",mediaDecol);
	printf("TEMPO MEDIO DE ESPERA PARA ATERRISAGEM: %.2f\n",mediaPouso);
	printf("NUMERO DE AVIOES QUE ATERRISSAM SEM RESERVA DE GASOSA: %d\n",estatistica.avSemFuel);
	printf("CAIRAM COITADOS: %d\n",estatistica.caiu);
	
	fclose(graficoDecol);
	fclose(graficoAterri);
	fclose(graficoSemReserva);
	fclose(graficoCairam);
	return 0;
}
