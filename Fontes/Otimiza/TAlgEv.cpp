//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stddef.h>
#include <Classes.hpp>
#include "TAlgEv.h"
#include <PlataformaSinap\Fontes\Otimiza\VTIndividuo.h>
#include <PlataformaSinap\Fontes\Otimiza\VTLocus.h>
#include <PlataformaSinap\Fontes\Otimiza\VTAvalia.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTOtimiza* __fastcall NewObjAlgEv(void)
	{
	return(new TAlgEv());
	}
//---------------------------------------------------------------------------
__fastcall TAlgEv::TAlgEv(void)
	{
	lisALL= new TList();
	lisPExt= new TList();
	fout = NULL;
	flag_exe = false;
	avalia = NULL;
	serie1  = NULL;
	serie2  = NULL;
	serie3  = NULL;
	serie4  = NULL;

	//Parametros default
	parAg.ng    = 5;
	parAg.npais = 4;
	parAg.nfilhos  = 4;
	parAg.pmuta_ev = 1.0;
	parAg.pmuta_ag = 0.05;
	parAg.pcruz_ev = 0.8;
	parAg.pcruz_ag = 0.8;
	parAg.flagRelDet = true;

	}
//---------------------------------------------------------------------------
__fastcall TAlgEv::~TAlgEv(void)
   {
   TObject* pobj;
	if(lisALL) {LimpaTList(lisALL); delete lisALL; lisALL = NULL;}
	if(lisPExt) {delete lisPExt; lisPExt = NULL;}
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::DefChartSerie(TChartSeries *serie)
   {
   if(!serie) {serie1 = serie2 = serie3 = serie4 = NULL;
               return;}

   if(!serie1) serie1 = serie;
   else
       {
       if(!serie2) serie2 = serie;
       else
          {
          if(!serie3)
            serie3 = serie;
          else
            serie4 = serie;
          }
       }
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::EncerraRel(void)
	{
	fclose(fout);
	fout = NULL;
	}
//---------------------------------------------------------------------------
bool __fastcall TAlgEv::Executa(VTProgresso* prog)
	{
	int ng      = parAg.ng;
	int npais   = parAg.npais;
	int nfilhos = parAg.nfilhos;
	VTIndividuo* ind_elite;
	bool sucesso = false;

	int ngenes, nger, n1, n2;
	VTLocus* ploc;
	VTIndividuo* individuo;
	VTIndividuo* indiv_filho;
	VTIndividuo* indiv1;
	VTIndividuo* indiv2;
	AnsiString  aux;
	TList* lisP;
	TList* lisF;


	try
	{
		this->prog = prog;
		//Inicia melhor individuo, lista geral de indiv, lisALL,
		//e semente de gerador de aleatórios
		Inicia();

		flag_exe = false;
		if(!(npais > 1))
			{throw new Exception("npais <= 1");}
		if(!avalia)
			{throw new Exception("avalia == NULL");}

		//Cria listas
		lisP = new TList();
		lisF = new TList();

		//Atualiza barra de progresso
		if(prog) prog->Start(progTEXTO);
		if(prog) prog->Add("Otimizando...");

		//Inicia idade do indivíduo
		IniciaIdade();

		//Seta variáveis de referencia    PEND mudar para individuoInicial
		if(!avalia->Executa(NULL))
			{throw new Exception("avalia->Executa(NULL) falhou");}

		//Reinicia relatório (lisRel)
		avalia->ImprimeInd(NULL, "", true);
		//Imprime cabecalho
		avalia->ImprimeInd(avalia->Individuo(), "", true);
		//Imprime individuo inicial
		//ALTERA DESPACHO SAI avalia->ImprimeInd(avalia->Individuo(), "Inicial");

		//GÊNESE
		if(!ExecutaGenese(lisP, npais))
			{throw new Exception("ExecutaGenese(lisP, npais) falhou");}

		//Inicia melhor individuo
		indiv_melhor = (VTIndividuo*)lisP->First();
		if(!avalia->Executa(indiv_melhor, lisALL))
			{throw new Exception("avalia->Executa(indiv_melhor, lisALL) falhou");}
		avalia->ImprimeInd(indiv_melhor, "Inicial");



      aux.sprintf("Otimizando...Geração %2i/%2i", 1, ng);
      if (prog) prog->Add(aux, true);
		//AVALIAÇÃO E GUARDA EM LISALL
		ExecutaAvalia(lisP, parAg.flagRelDet);//hk10 if(!ExecutaAvalia(lisP)) return(false);
		//REPRODUÇÃO - RENOVA LISP - q = %npais
		ExecutaReprodTorneio(lisP, npais, npais, indiv_melhor); //lisP´ = Reprod(lisP)
		//GUARDA indivíduos selecionados da geração
		//lisALL->Assign(lisP, laOr);
		//Imprime o melhor individuo
		aux.sprintf("Ger %2i", nger = 0);
		avalia->ImprimeInd(indiv_melhor, aux);


		nger = 1;

		//Limpa as series
		//InsereDesv(NULL, nger, serie3);
		//InsereNota(NULL, nger, serie4);


		while(nger < ng)
		  {
		  //Limpa as séries
		  InserePonto(NULL, nger, serie1);
		  InserePonto(NULL, nger, serie2);

		  //CRUZAMENTO
		  ExecutaCruz(lisP, lisF);
		  //MUTACAO
		  ExecutaMutacao(lisP, lisF, nfilhos); //altera individuos em lisP e cria n

		  //IMPRIME resultado de indivíduos filhos
		  ImprimeRes(lisF, nger, serie2);
		  //IMPRIME resultado de indivíduos pais
		  ImprimeRes(lisP, nger, serie1);
		  //Resulta em lisP alterado e lista de filhos
		  if(avalia->Individuo()->lisL->Count > 0)
			 {lisP->Assign(lisF, laOr); //lisP = lisF
			  lisF->Clear();
			 }

		  aux.sprintf("Otimizando...Geração %2i/%2i", nger + 1, ng);
        if (prog) prog->Add(aux, true);
		  //AVALIA E GUARDA EM LISALL
		  ExecutaAvalia(lisP, parAg.flagRelDet);//hk10 if(!ExecutaAvalia(lisP)) return(false);
		  //SELEÇÃO elimina indivíduos velhos
		  //ExecutaSelecao(lisP);
		  //REPRODUÇÃO : RENOVA lisP
		  ExecutaReprodTorneio(lisP, npais, npais, indiv_melhor); //lisP = Reprod(lisP)

		  //GUARDA indivíduos selecionados da geração
		  //lisALL->Assign(lisP, laOr);

		  aux.sprintf("Ger %2i", nger);
		  avalia->ImprimeInd(indiv_melhor, aux);

		  nger++;
		  //if (prog) prog->NextPosition();
		  }
		flag_exe = true;
		//Imprime última geração
		//ImprimeRel(nger, lisP);
		//Limpa as séries
		InserePonto(NULL, nger, serie1);
		InserePonto(NULL, nger, serie2);
		ImprimeRes(lisP, ng, serie1);

		//OrdenaLista(lisP);
		//indiv_melhor = (VTIndividuo*)lisP->Last();
		//indiv_melhor = ind_elite;
		//Apaga pontos filhos
		//InserePonto(NULL, nger, serie2);
		//InserePonto(indiv_melhor, nger, serie2);
		if(lisALL->IndexOf(indiv_melhor) < 0) lisALL->Add(indiv_melhor);
		avalia->ImprimeInd(indiv_melhor, "Ótimo");
		//Mudanca 12/2010 avalia->Executa(indiv_melhor);

		//Retorna a rede a configuração inicial
		//avalia->Executa(NULL);
		//Esconde barra de progresso
		if (prog) prog->Stop();

		delete lisP;
		delete lisF;
		sucesso = true;
	}
	catch (Exception &e)
	{
		if (prog) prog->Stop();
		sucesso = false;
		if(lisP) delete lisP;
		if(lisF) delete lisF;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TAlgEv::ExecutaAvalia(TList* lis, bool flagImprime)
	{
	VTIndividuo* individuo;
	bool resposta = true;
	double media, media2, dpad, vmin, vmax, parA, parB;

	//VTIndividuo* ind_elite;

	vmin = 999.;
	vmax = 0.;

	//protecao
	if(!indiv_melhor) return(false);
	//if(lis->Count == 0) return(NULL);
	//inicia melhor individuo
	//ind_elite = (VTIndividuo*)lis->First();
	//lisALL->Add(ind_elite);
	//if(!ind_elite->flag_avalia) avalia->Executa(ind_elite, lisALL);
	//Avaliacao dos individuos pais
	for(int np = 0; np < lis->Count; np++)
	  {
	  individuo = (VTIndividuo*)lis->Items[np];
	  //Insere ponto em serie de pais
	  if(!individuo->flag_avalia)
		  {if(!avalia->Executa(individuo, lisALL))
				{resposta = false;
				 individuo->nota = -1.0;//PENDENCIA
				}
		  }
	  //Adiciona individuo a lisALL
	  lisALL->Add(individuo);
	  //Imprime relatorio do individuo
	  if(flagImprime) avalia->ImprimeInd(individuo, IntToStr(np));

	  //Busca ind_elite com a melhor nota
	  if(individuo->nota > indiv_melhor->nota) indiv_melhor = individuo;

	  if(individuo->nota > vmax) vmax = individuo->nota;
	  if(individuo->nota < vmin) vmin = individuo->nota;

	  media = (media*np + individuo->nota) / (np+1);
	  media2= (media2*np + individuo->nota * individuo->nota) / (np+1);
	  //InserePonto(individuo, nger, serie1);
	  //InsereDesv(individuo, nger, serie3);
	  //if(nger > 1) InsereNota(individuo, nger, serie4);
	  }
	/*if(lis->Count > 1)
	  dpad = (media2 - media * media) * (double)lis->Count / ((double)lis->Count - 1);
	else
	  dpad = 0.0;
	*/
	//Calcula os valores de a e b para reescalonamento
	CalcParametros(parA, parB, media, vmin, vmax);
	for(int np = 0; np < lis->Count; np++)
	  {
	  individuo = (VTIndividuo*)lis->Items[np];
	  individuo->notaEsc = parA * individuo->nota + parB;
	  }


	return(resposta);
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::CalcParametros(double &a, double &b, double media, double min, double max)
	{
	double fmultiple=2.; //Espera-se que f reescalonado seja 2x a media
   double delta;
	//Verifica se o min escalonado é negativo
	if(min > (fmultiple*media - max)/(fmultiple - 1.0) )
		{
		// Escalonamento normal
		delta = max-media;
		if(delta==0)
         {
         a=1.;
         b=0.;
         }
      else
         {
	   	a     = (fmultiple-1.0)*media/delta;
		   b     = media*(max-fmultiple*media)/delta;
         }
		}
   else
		{
		// Escalona o máximo possivel
		delta = max-min;
      if(delta==0)
         {
         a=1.;
         b=0.;
         }
      else
         {
   		a     = media/delta;
	   	b     = -min*media/delta;
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaCruz(TList* lisP, TList* lisF)
	{
	VTIndividuo* indiv_avalia = avalia->Individuo();
	//CRUZAMENTO ALGORITMO GENETICO
	if(indiv_avalia->lisLBin->Count > 0)
	  ExecutaCruzAG(lisP); //lisP = F(lisP): altera individuos de lisP

	//CRUZAMENTO ALGORITMO ESTRATEGIA EVOLUTIVA
	if(indiv_avalia->lisL->Count > 0)
	  ExecutaCruzAE(lisP, lisF); //lisF = F(lisP): cria individuos em lisF

	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaCruzAE(TList* lisP, TList* lisF)
	{
	VTIndividuo* indiv1;
	VTIndividuo* indiv2;
	VTIndividuo* indiv_filho;
	int n1, n2;
	VTLocus*      ploc_1;
	VTLocus*      ploc_2;
	VTLocus*      ploc_f;

	//Faz cruzamentos em lisP
	for(int np = 0; np < lisP->Count; np++)
		 {
		 if(Sorteio() > parAg.pcruz_ev) continue;//if((random(1000)/1000.) > pcruz) continue;
		 n1 = n2 = 0;
		 while(n1 == n2)
			  {
			  n1 = (int)(Sorteio() * (double)lisP->Count);//n1 = random(lisP->Count);
			  n2 = (int)(Sorteio() * (double)lisP->Count);//n2 = random(lisP->Count);
			  }
		 indiv1 = (VTIndividuo*)lisP->Items[n1];
		 indiv2 = (VTIndividuo*)lisP->Items[n2];
		 //Cria novo indivíduo filho com informações de indiv1
		 lisF->Add(indiv_filho = avalia->NovoIndividuo(indiv1));
		 for(int nl = 0; nl < indiv_filho->lisL->Count; nl++)
			{
			ploc_1 = (VTLocus*)indiv1->lisL->Items[nl];
			ploc_2 = (VTLocus*)indiv2->lisL->Items[nl];
			ploc_f = (VTLocus*)indiv_filho->lisL->Items[nl];
			ploc_f->p = (ploc_1->p + ploc_2->p) / 2.;
			ploc_f->s = (ploc_1->s + ploc_2->s) / 2.;
			}
		 //Verifica se existem variáveis binárias
		 if(indiv1->lisLBin->Count == 0) continue;
		 //Cria novo indivíduo filho com informações de indiv2
		 lisF->Add(indiv_filho = avalia->NovoIndividuo(indiv2));
		 for(int nl = 0; nl < indiv_filho->lisL->Count; nl++)
			{
			ploc_1 = (VTLocus*)indiv1->lisL->Items[nl];
			ploc_2 = (VTLocus*)indiv2->lisL->Items[nl];
			ploc_f = (VTLocus*)indiv_filho->lisL->Items[nl];
			ploc_f->p = (ploc_1->p + ploc_2->p) / 2.;
			ploc_f->s = (ploc_1->s + ploc_2->s) / 2.;
			}
		 }
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaCruzAG(TList* lisP)
	{
	VTIndividuo* indiv1;
	VTIndividuo* indiv2;
	VTLocus*      ploc_1;
	VTLocus*      ploc_2;
	int iaux;
	double aux;

	for(int np = 0; np < lisP->Count; np++)
			{
			if(Sorteio() > parAg.pcruz_ag) continue;
			indiv1 = indiv2 = NULL;
			while(indiv1 == indiv2)//gd3
				  {
				  indiv1 = (VTIndividuo*)lisP->Items[(int)(Sorteio() * (double)lisP->Count)];
				  indiv2 = (VTIndividuo*)lisP->Items[(int)(Sorteio() * (double)lisP->Count)];
				  }
			//indiv1->flag_avalia = false;
			//indiv2->flag_avalia = false;
			//sorteia locus a partir do qual haverá cruzamento
			iaux = (int)(Sorteio() * (double)indiv1->lisLBin->Count);
			for(int nl = iaux; nl < indiv1->lisLBin->Count; nl++)
				{
				ploc_1 = (VTLocus*)indiv1->lisLBin->Items[nl];
				ploc_2 = (VTLocus*)indiv2->lisLBin->Items[nl];
				aux = ploc_1->p;
				ploc_1->p = ploc_2->p;
				ploc_2->p = aux;
				}
			}
	}
//---------------------------------------------------------------------------
bool __fastcall TAlgEv::ExecutaGenese(TList* lisP, int npais)
	{
	VTIndividuo* individuo;
	VTIndividuo* indiv_avalia;


	//Busca individuo inicial
	if(!(indiv_avalia = avalia->Individuo())) return(false);
	if((indiv_avalia->lisLBin->Count == 0)&&(indiv_avalia->lisL->Count == 0)) return(false); //{Aviso("Escolha as variáveis"); return(NULL);}
	//Adiciona primeiro indivíduo com configuração atual
	lisP->Add(individuo = avalia->NovoIndividuo(indiv_avalia, true));//ploc->locus_ori = avalia->individuo->lisLBin->ploc
	//Adiciona individuos externos
	for(int ne = 0; ne < lisPExt->Count; ne++)
		{
		if(lisP->Count >= npais) break;
		lisP->Add(lisPExt->Items[ne]);
		}
	lisPExt->Clear();

	SorteioVar(individuo, true);
	//gera a primeira geracao de pais
	while(lisP->Count < npais)
		  {
		  //Cria os indivíduos com a informação das posições dos genes do
		  //indivíduo inicial: indiv_avalia
		  lisP->Add(individuo = avalia->NovoIndividuo(indiv_avalia, true)); //individuo->LisLBin->locus->locus_ori = avalia->indiiduo-LisLBin->Locus
		  SorteioVar(individuo);
		  }
//	for(int np = 1; np < npais; np++)
//	  {
	  //Cria os indivíduos com a informação das posições dos genes do
	  //indivíduo inicial: indiv_avalia
//	  lisP->Add(individuo = avalia->NovoIndividuo(indiv_avalia, true));
//	  SorteioVar(individuo);
//	  }
	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaMutacao(TList* lisP, TList* lisF, int nfilhos)
	{
	VTIndividuo* individuo;
	VTIndividuo* indiv_filho;
	VTIndividuo* indiv_avalia = avalia->Individuo();

	//Executa mutação algoritmo genetico
	if(indiv_avalia->lisLBin->Count > 0)
	{
	for(int np = 0; np < lisP->Count; np++)
	  {
	  individuo = (VTIndividuo*)lisP->Items[np];
	  //Executa mutacao das variáveis binárias em individuos de lisP
	  ExecutaMutacaoAG(individuo);
	  }
	}

	//Executacao mutação estratévia evolutiva
	if(indiv_avalia->lisL->Count > 0)
	{
	for(int np = 0; np < lisP->Count; np++)
	  {
	  individuo = (VTIndividuo*)lisP->Items[np];
	  //Gera filhos de individuo e realiza operacao mutacao
	  for(int nf = 0; nf < nfilhos; nf++)
			{
			//Insere novo individuo filho copia do pai
			lisF->Add(indiv_filho = avalia->NovoIndividuo(individuo));
			//Executa mutações na variáveis contínuas
			ExecutaMutacaoAE(indiv_filho);
			}
	  }
	}
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaMutacaoAE(VTIndividuo* individuo)
	{
	VTLocus* ploc;
	double  saux, paux;
	double randN;

	//Mutacao do indiv_filho
   randN = Normal(0,1);//RandG(0,1);
   for(int nl = 0; nl < individuo->lisL->Count; nl++)
	  {
	  ploc = (VTLocus*)(individuo->lisL)->Items[nl];
	  //verifica se haverá mutação
	  if(Sorteio() > parAg.pmuta_ev) continue; //if((random(1000)/1000.) > pmuta) continue;

	  do
		  saux = ploc->s * exp(randN / 2 + (1/(sqrt((long double)2*sqrt((long double)2)))* Normal(0,1)));//RandG(0,1)));
	  while((saux > ploc->maxS)||(saux <= ploc->minS));  //minimo 0 excludente!
	  ploc->s = saux;

	  do
		  paux = ploc->p + (ploc->s * Normal(0,1));//RandG(0,1));
	  while((paux > ploc->maxP)||(paux < ploc->minP));
	  ploc->p = paux;
	  }
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaMutacaoAG(VTIndividuo* individuo)
	{
	VTLocus* ploc;
	for(int nl = 0; nl < individuo->lisLBin->Count; nl++)
	  {
	  ploc = (VTLocus*)(individuo->lisLBin)->Items[nl];
	  //verifica se haverá mutação
	  if(Sorteio() > parAg.pmuta_ag) continue; //if((random(1000)/1000.) > pmuta) continue;
	  ploc->p = ((int)ploc->p == 0) ? 1. : 0.;
	  individuo->flag_avalia = false;
	  }
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaReprodDet(TList* lis, int n)
	{
	VTIndividuo* individuo;
	//Reproducao determinística n primeiro avaliados reproduzem

	//Ordena a lista em ordem de valores crescentes da funcao de avaliacao
	OrdenaLista(lis);
	//Elimina indivíduos com as piores avaliaçoes
	while(lis->Count > n)
			  {
			  individuo = (VTIndividuo*)lis->Last();
           lis->Remove(individuo);
			  if(lisALL->IndexOf(individuo) < 0) delete individuo;
           }
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaReprodTorneio(TList* lis, int q, int np, VTIndividuo* ind_elite)
	{
	//lis: lista de entrada/ saída
	//q:  número de indivíduos do torneio
	//np: número de indivíduos da lista resultado, saída
	//ind_elite: indivíduo com entrada forçada na lista resultado, saída

	//variáveis locais
	int        n_aux;
	VTIndividuo *indiv, *indiv_max, *indiv_aux;
	TList      *lisTMP;

	//proteção
	if ((lis->Count == 0)||(q < 1)||(q > np)) return;

	//Executa reproducao probabilistica do tipo torneio

	//cria nova população e a armazena em lisTMP
	lisTMP = new TList();
	lisTMP->Clear();

	if(ind_elite)
	  {
	  //Força a entrada de ind_elite
	  lisTMP->Add(ind_elite);
	  lis->Remove(ind_elite);
	  }

	while(lisTMP->Count < np)
		{
		//Sorteia q individuos e guarda o de melhor avaliação
		n_aux = (int)(Sorteio() * (double)lis->Count);
		indiv_max = (VTIndividuo*)lis->Items[n_aux];
		for(int nq = 1; nq < q; nq++)
			{
			n_aux = (int)(Sorteio() * (double)lis->Count);
			indiv = (VTIndividuo*)lis->Items[n_aux];
			if(indiv->notaEsc > indiv_max->notaEsc) indiv_max = indiv;
			//if(indiv->nota > indiv_max->nota) indiv_max = indiv;
			}
		lisTMP->Add(indiv_max);
		//faz uma reprodução por indivíduo
		lis->Remove(indiv_max);
		}

	//for(int n = lis->Count - 1; n >= 0; n--)
	//	{
	//	indiv = (VTIndividuo*)lis->Items[n];
		//Verifica se não faz parte do histórico de indivíduos em lisALL
	//	if(!(lisALL->IndexOf(indiv) < 0)) lis->Remove(indiv);
	//	}
	//LimpaTList(lis);
	lis->Clear();
	for(int n = 0; n < lisTMP->Count; n++)
		{
		indiv = (VTIndividuo*)lisTMP->Items[n];
		lis->Add(avalia->NovoIndividuo(indiv));
		}

	//lisALL->Assign(lis, laOr);
	//lis->Assign(lisTMP, laOr);
	delete lisTMP;
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ExecutaSelecao(TList* lisP)
	{
	VTIndividuo* individuo;
	//Remove individuos velhos
	for(int n = lisP->Count - 1; n >= 0; n--)
		{
      individuo = (VTIndividuo*)lisP->Items[n];
      individuo->idade++;
      if(individuo->idade > 4)//entrada!
        {lisP->Remove(individuo);
        if(lisALL->IndexOf(individuo) < 0) delete individuo;
		}
	  }
   //lisP = Sel(lisP)
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::InserePonto(VTIndividuo* indiv, int nger, TChartSeries* serie)
   {
   double x, y, max_p, min_p;
   bool   flag_x, flag_y;
   VTLocus* ploc;


   if(!serie) return;
   if(indiv == avalia->Individuo()) return;
   if(!indiv) {serie->Clear();
			   Application->ProcessMessages();
			   return;}

   flag_x = flag_y = false;
   for(int n = 0; n < indiv->lisL->Count; n++)
	  {
	  ploc = (VTLocus*)indiv->lisL->Items[n];
	  if(ploc->locus_ori->imostra == -1) continue;
	  if((ploc->locus_ori->imostra == 0)&&(!flag_x))
		{
		flag_x = true;
		x = ploc->p;
		}
	  else
		{
		if(flag_y) continue;
		flag_y = true;
		y = ploc->p;
		}

	  }

   if((!flag_x) && (!flag_y)) return;
   if(!flag_x) x = nger;
   if(!flag_y) y = nger;
   serie->AddXY(x, y);

   Application->ProcessMessages();
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::InsereDesv(VTIndividuo* indiv, int nger, TChartSeries* serie)
   {
   double y;
   VTLocus* ploc;
   bool flag_cor;

   if(!serie) return;
   if(indiv == avalia->Individuo()) return;
   if(!indiv) {serie->Clear();
			   Application->ProcessMessages();
			   return;}

   flag_cor = true;
   for(int n = 0; n < indiv->lisL->Count; n++)
	  {
	  ploc = (VTLocus*)indiv->lisL->Items[n];
      y = ploc->s;
      if(flag_cor)
        serie->AddXY(nger, y, "", clBlue);
      else
        serie->AddXY(nger, y, "", clGreen);

      flag_cor = !flag_cor;
      }

   Application->ProcessMessages();


   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::InsereNota(VTIndividuo* indiv, int nger, TChartSeries* serie)
   {
   double y;
   VTLocus* ploc;
   double max_y;
   double min_y;

   return;
   /*
   if(!serie) return;
   if(indiv == avalia->Individuo()) return;
   if(!indiv) {serie->Clear();
               Application->ProcessMessages();
               return;}

   max_y = serie->MaxYValue();
   if(indiv->nota > max_y) max_y = indiv->nota;
   min_y = serie->MinYValue();
   if(indiv->nota < min_y) min_y = indiv->nota;

   try{
      serie->ParentChart->LeftAxis->SetMinMax(min_y, max_y);
		serie->AddXY(nger, indiv->nota, "", clBlue);
      Application->ProcessMessages();
      }catch(Exception &e)
         {
         return;
         }

   */
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::Inicia(void)
	{
	//Inicia individuo otimizado
	indiv_melhor = NULL;
	flag_exe = false;
	//Inicia gerador de números aleatórios
	srand(51);
	LimpaTList(lisALL);
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::InsereIndividuo(VTIndividuo *indExt)
	{
	lisPExt->Add(indExt);
   }
//---------------------------------------------------------------------------
bool __fastcall TAlgEv::IniciaIdade(void)
	{
	VTIndividuo* individuo;
	//Busca individuo inicial
	if(!(individuo = avalia->Individuo())) return(false);
	individuo->idade = 0;
	return(true);
	}
//---------------------------------------------------------------------------
void __fastcall TAlgEv::IniciaRel(AnsiString rel)
   {
   fout = fopen(rel.c_str(), "wt");
   }
//---------------------------------------------------------------------------
double __fastcall TAlgEv::inv_normal(double u1, double u2, double media, double desvio)
   {
   // Retorna valor pseudo-aleatório com distribuição normal
   double z;
   //protecao
   if(!(u1 > 0)) return(media);

   z = sqrt(-2.0*log(u1)) * cos(2*M_PI*u2);
   return (media + (desvio*z));
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ImprimeRes(TList* lisP, int nger, TChartSeries* serie)
   {
   VTIndividuo* individuo;
   for(int np = 0; np < lisP->Count; np++)
      {
      individuo = (VTIndividuo*)lisP->Items[np];
	  InserePonto(individuo, nger, serie);
      //InsereDesv(individuo, nger, serie3);
      //InsereNota(individuo, nger, serie4);
      }

   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::ImprimeRel(int nger, TList* lista)
   {
   VTIndividuo* individuo;
   AnsiString aux;

   //protecao
   //if(!fout) return;
   //fprintf(fout, "%3i----------------------------------------\n", nger);
   if(!lista) return;
   for(int n = 0; n < lista->Count; n++)
      {
      individuo = (VTIndividuo*)lista->Items[n];
      aux.sprintf("Ger %2i Ind %2i", nger, n);
		avalia->ImprimeInd(individuo, aux);
      //fprintf(fout, "%15.10f\n", individuo->nota);
      }
   }
//---------------------------------------------------------------------------
double __fastcall TAlgEv::Normal(double media, double desv_pad)
   {
   double valor;
   valor = inv_normal(Sorteio(), Sorteio(), media, desv_pad);
   return(valor);
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::OrdenaLista(TList* lista)
   {
   //variaveis locais
   VTIndividuo *indiv;
   VTIndividuo *indiv_aux;
   int iconta;

   //Ordena a lista em ordem de valores crescentes da funcao de avaliacao
   iconta = 1;
   indiv_aux  = (VTIndividuo*)lista->First();
   while(iconta < lista->Count)
     {
     indiv = (VTIndividuo*)lista->Items[iconta];
     if(indiv->nota <= indiv_aux->nota)//alterar
        {
        indiv_aux = indiv;
        iconta++;
        }
     else
        {
        //Troca as posicoes iconta com iconta - 1
        lista->Insert(iconta - 1, indiv);
        lista->Delete(iconta + 1);
        indiv_aux  = (VTIndividuo*)lista->First();
        iconta = 1;
        }
     }
   }
//---------------------------------------------------------------------------
TList* __fastcall TAlgEv::PM_GetLisIndiv(void)
	{
	return(lisALL);
	}
//---------------------------------------------------------------------------
VTIndividuo* __fastcall TAlgEv::PM_GetMelhorIndiv(void)
	{
	if(flag_exe) return(indiv_melhor);
	return(NULL);
	}
//---------------------------------------------------------------------------
strParAg  __fastcall TAlgEv::PM_GetParam(void)
   {
   return(parAg);
   }
//---------------------------------------------------------------------------
VTAvalia* __fastcall TAlgEv::PM_GetAvalia(void)
   {
   return(avalia);
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::PM_SetAvalia(VTAvalia* avalia)
   {
   this->avalia = avalia;
   }
//---------------------------------------------------------------------------
void __fastcall TAlgEv::PM_SetParam(strParAg par)
   {
   this->parAg = par;
	}
//---------------------------------------------------------------------------
double __fastcall TAlgEv::Sorteio(void)
   {
   //variaveis locais
   unsigned int iaux;

   //função que retorna números em [0,1[
   while((iaux = rand()) == RAND_MAX);
   return(((double)iaux) / ((double)RAND_MAX));
	}

//---------------------------------------------------------------------------
void __fastcall TAlgEv::SorteioVar(VTIndividuo* indiv, bool flag_inic)
	{
	VTLocus* ploc;
	double aux;

	//Locus contínuos
	//NOVA DESPACHO
	for(int nl = 0; nl < indiv->lisL->Count; nl++)
		{
		ploc = (VTLocus*)indiv->lisL->Items[nl];
		if(!flag_inic) ploc->p = ploc->minP + Sorteio() * (ploc->maxP - ploc->minP);
		if(!IsZero(ploc->minS - ploc->maxS))
				 {do {ploc->s = ploc->minS + Sorteio() * (ploc->maxS - ploc->minS);
					  } while (IsZero(ploc->s));
				 }
		else ploc->s = ploc->minS;
		}

	//Locus binarios
	if(!flag_inic){
	for(int nl = 0; nl < indiv->lisLBin->Count; nl++)
		{
		ploc = (VTLocus*)indiv->lisLBin->Items[nl];
		aux = Sorteio();
		ploc->p = (aux < 0.5) ? 0 : 1;
		}
		}
	}

//---------------------------------------------------------------------------



