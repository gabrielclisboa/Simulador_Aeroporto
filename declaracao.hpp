typedef struct aviao{
	int fuel,fuelinicial;
	float decol;
	int ID;
	struct aviao *prox; 
}aviao;

typedef struct pista{
	 aviao *f1,*f2,*f3;
	 int qtd1,qtd2,qtd3;
	 bool situacao;
}pista;

typedef struct{
	int atualPar, atualImpar,caiu,avSemFuel;
	float MedDecol,MedPouso,EDeltaFuel,EtempoDecol;
}Estatistica;

int geracaoFuel();
int geracaoAviao();
aviao *gerar_Aviao(bool tipo);
pista *Insere_Aviao(pista *alvo, bool tipo);
int Escolhe_pista(pista *p1,pista *p2);
void imprimeFila(pista *alvo,int num);
pista * geraPista(pista *p);
pista * removeCombustivel(pista *p);
pista * Remove_Aviao_Pista(pista *alvo);
pista *procuraVoid(pista *alvo);
void procura_berg(pista *alvo, int remocao[], int endereco[], int &auxiliarRemocao);
aviao *remove_aviao(aviao *alvo, int id);
pista * remove_fila(int f, pista *p);
int verifica_fila(pista *p);
pista * tempoDecol(pista *p);
