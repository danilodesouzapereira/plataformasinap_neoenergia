//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtRegiao.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mapa\VTPoligono.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrecho.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe TEdtRegiao
//---------------------------------------------------------------------------
VTEdt* NewObjEdtRegiao(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone, TWinControl *parent)
   {
   return(new TEdtRegiao(apl, ResetFocus, SetFocus, OnDone, parent));
   }

//---------------------------------------------------------------------------
__fastcall TEdtRegiao::TEdtRegiao(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone, TWinControl *parent)
                      :TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
   {
	//obtem o form pai
	//formEdtRegiao = (TFormEdtRegiao*) parent;
	//obtém objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //cria Barra e Trecho fictícios p/ exibir rastro do mouse
	rastro.enabled = true;
	rastro.barraUltima  = DLL_NewObjBarra(24);
	rastro.barraMovel  = DLL_NewObjBarra(24);
	rastro.barraInicial  = DLL_NewObjBarra(24);
	rastro.trecho1  = DLL_NewObjTrecho(24);
	rastro.trecho2  = DLL_NewObjTrecho(24);
	rastro.trecho1->DefineObjBarra(rastro.barraUltima, rastro.barraMovel);
	rastro.trecho2->DefineObjBarra(rastro.barraMovel, rastro.barraInicial);
	//cria lista de barras e trechos para o rastro.
	lisBARRastro = new TList;
	lisTRECHORastro = new TList;
	//cria os pontos
	ultimoPonto_pixels = new TPoint();
	pontoInicial_pixels = new TPoint();

	//mais nada a fazer: o contrutor de VTEdt faz todas iniciações
	}

//---------------------------------------------------------------------------
__fastcall TEdtRegiao::~TEdtRegiao(void)
   {
	//destrói objetos locais
	if (rastro.trecho1)  {DLL_DeleteEqpto(rastro.trecho1);  	rastro.trecho1  = NULL;}
	if (rastro.trecho2)  {DLL_DeleteEqpto(rastro.trecho2);  	rastro.trecho2  = NULL;}
	if (rastro.barraInicial)  	{DLL_DeleteEqpto(rastro.barraInicial);		rastro.barraInicial   = NULL;}
	if (rastro.barraMovel)  	{DLL_DeleteEqpto(rastro.barraMovel);  	rastro.barraMovel   = NULL;}
	if (rastro.barraUltima)  	{DLL_DeleteEqpto(rastro.barraUltima);  	rastro.barraUltima   = NULL;}
	//Destroi todos os elementos das lista e a lista
	if (lisBARRastro) LimpaLista(lisBARRastro);
	if (lisTRECHORastro) LimpaLista(lisTRECHORastro);
	//destroi os pontos criados
	if (ultimoPonto_pixels)  {delete ultimoPonto_pixels; ultimoPonto_pixels  = NULL;}
	if (pontoInicial_pixels) {delete pontoInicial_pixels; pontoInicial_pixels  = NULL;}
	//mais nada a fazer: o destrutor de VTEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::EvtEnter(void)
   {
   //executa ActionOnDone
	if (extAction.OnDone) extAction.OnDone->OnExecute(regiao);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::EvtEscape(void)
   {
   //executa ActionOnDone
	if (extAction.OnDone) extAction.OnDone->OnExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::EvtMouseDown(void)
	{
	//variáveis locais
	int index;
	int atualX, atualY;
	VTBarra 		*barra, *barraAnterior;
	VTTrecho		*trecho;

	//verifica se o tratamento dos eventos de mouse está habilitado
	if (! evt_mouse_enabled) return;
	//cancela exibição da lista de Eqpto novo
	LisEqptoNovoApaga();
	//adquire as coordenadas em pixel
	atualX = evt.coord_pixel.x;
	atualY = evt.coord_pixel.y;
	//Primeiro ponto
	if(regiao->Poligono->Size == 0)
	{//insere sem verificação
		regiao->Poligono->InserePonto(evt.x, evt.y);
		//indice do ultimo ponto inserido
		index = regiao->Poligono->Size - 1;
		//atualiza o treeview
		//formEdtRegiao->InsereVerticeNoTreeView(evt.x, evt.y);
		//associa o ponto como sendo o inicial (em pixels)
		pontoInicial_pixels->X = atualX;
		pontoInicial_pixels->Y = atualY;
		//associa o ponto como sendo o ultimo clicado (em pixels)
		ultimoPonto_pixels->X = atualX;
		ultimoPonto_pixels->Y = atualY;
		//por ser o 1o ponto, limpamos as listas de trechos e barras
		if(lisBARRastro)LimpaTList(lisBARRastro);
		if(lisTRECHORastro)LimpaTList(lisTRECHORastro);
		//adiciona a 1a barra
		lisBARRastro->Add(rastro.barraInicial);
	}//proximos pontos é necessário verificação
	else
	{  //verifica se o novo ponto é igual ao ultimo ou o primeiro
		if(VerificaPontoInicial(atualX, atualY) || VerificaUltimoPonto(atualX, atualY))
		{ //em caso positivo ele finaliza a regiao
			if (extAction.OnDone)
			{
			extAction.OnDone->OnExecute(regiao);
			return;
			}
		}
		else
		{
			//cria um novo Ponto na Regiao
			regiao->Poligono->InserePonto(evt.x, evt.y);
			//torna a regiao visivel
			regiao->Visible = true;
			//atualiza indice do ultimo ponto inserido
			index = regiao->Poligono->Size - 1;
			//atualiza o ultimo ponto guardado
			ultimoPonto_pixels->X = atualX;
			ultimoPonto_pixels->Y = atualY;
			//atualiza o treeview
			//formEdtRegiao->InsereVerticeNoTreeView(evt.x, evt.y);
			//adiciona a nova barra e o novo trecho
			barra = DLL_NewObjBarra(24);
			//define as coordenadas da nova barra
			barra->utm.x = evt.x;
			barra->utm.y = evt.y;
			//recupera o ponteiro da ultima barra inserida para associar ao trecho
			barraAnterior = (VTBarra*) lisBARRastro->Items[lisBARRastro->Count - 1];
			//adiciona a barra à lista de barras
			lisBARRastro->Add(barra);
			//cria o novo trecho
			trecho = DLL_NewObjTrecho(24);
			trecho->DefineObjBarra(barraAnterior , barra);
			//adiciona o trecho à lista de trechos
			lisTRECHORastro->Add(trecho);
		}
	}
//	//verifica se o Poligono pssui mais de 1 ponto
//	regiao->Poligono->LisPoint();
//	if (regiao->Poligono->Size >= 2)
//		{//reapresenta o gráfico para desenhar as linhas do polígono
//		regiao->Visible = true;
//		graf->Redraw();
//		}
	//atualiza dados para desenhar o rastro
	rastro.enabled = true;
	//define coordenadas da barra do rastro
	index = regiao->Poligono->Size - 1;
	rastro.barraUltima->utm.x = regiao->Poligono->Point[index]->x;
	rastro.barraUltima->utm.y = regiao->Poligono->Point[index]->y;
	rastro.barraMovel->utm   = rastro.barraUltima->utm;
	rastro.barraInicial->utm.x = regiao->Poligono->Point[0]->x;
	rastro.barraInicial->utm.y = regiao->Poligono->Point[0]->y;
	//insere Trecho em lisEQN
	lisEQN->Clear();
	lisEQN->Add(rastro.trecho2);
	lisEQN->Add(rastro.trecho1);
	//adiciona tambem os trechos da lista de trechos
	for(int nt = 0; nt < lisTRECHORastro->Count; nt++)
	{
		trecho = (VTTrecho*) lisTRECHORastro->Items[nt];
		lisEQN->Add(trecho);
	}
	//exibe lista de Eqptos novos
	LisEqptoNovoExibe();
	}

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::EvtMouseDownMove(void)
	{
	//nada a fazer
	}

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::EvtMouseMove(void)
	{//variaveis locais
   VTTrecho *trecho;

   //verifica se o tratamento dos eventos de mouse está habilitado
   if (! evt_mouse_enabled) return;
   //verifica se o mouse foi movimentado
   if ((evt.x == evt.x_ant)&&(evt.y == evt.y_ant)) return;
   //cancela exibição da lista de Eqpto novo
   LisEqptoNovoApaga();
   //define coordenadas utm da Barra2
	rastro.barraMovel->utm.x = evt.x;
	rastro.barraMovel->utm.y = evt.y;
   //insere Trecho em lisEQN
	lisEQN->Add(rastro.trecho1);
	lisEQN->Add(rastro.trecho2);
	//adiciona tambem os trechos da lista de trechos
	for(int nt = 0; nt < lisTRECHORastro->Count; nt++)
	{
		trecho = (VTTrecho*) lisTRECHORastro->Items[nt];
		lisEQN->Add(trecho);
	}
   //exibe da lista de Eqpto novo
   LisEqptoNovoExibe();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::EvtMouseUp(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::Inicia(VTRegiao *regiao)
   {
   //salva ponteiro para Regiao
	this->regiao = regiao;
	//garante modo geo-referenciado
	if (redegraf->ModoGrafico != modoGEOREFERENCIADO)
	   {
		redegraf->ModoGrafico = modoGEOREFERENCIADO;
		//if(regiao->Poligono->Size >=3)
      graf->Redraw();
      }
	evt_mouse_enabled = true;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::LisEqptoNovoApaga(void)
   {
   //verifica se há Eqpto em lisEQN
   if (lisEQN->Count > 0)
      {//atualiza o grafico e limpa lisEQN
      graf->Redraw(redrawEQP_NOVO, this);
      lisEQN->Clear();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::LisEqptoNovoExibe(void)
   {
	//verifica se há Eqpto em lisEQN
	if (lisEQN->Count > 0)
      {//atualiza o grafico
      graf->Redraw(redrawEQP_NOVO, this);
      }
	}

//---------------------------------------------------------------------------
void __fastcall TEdtRegiao::LimpaLista(TList *lista)
{  //variaveis locais
	TObject *objeto;

	for (int ne = lista->Count -1 ; ne > 0; ne--)
	{
		objeto = (TObject*) lista->Items[ne];
		lista->Remove(objeto);
		delete objeto;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TEdtRegiao::VerificaPontoInicial(int x, int y)
{
	bool	isPtoInicial = false;
	long	x_i, y_i;

	if(pontoInicial_pixels)
	{
		x_i = pontoInicial_pixels->X;
		y_i = pontoInicial_pixels->Y;
		//verifica se está no quadrado de lado 2*TOL centrado no ponto inicial
		if( (labs( x_i - x ) < TOL ) && (labs(y_i - y) < TOL) )
		{
			isPtoInicial = true;
		}
	}

	return isPtoInicial;

}
//---------------------------------------------------------------------------
bool __fastcall TEdtRegiao::VerificaUltimoPonto(int x, int y)
{
	bool isEqual = false;
	long x_i, y_i;

	if(ultimoPonto_pixels)
	{
		x_i = ultimoPonto_pixels->X;
		y_i = ultimoPonto_pixels->Y;
		//verifica se está no quadrado de lado 2*TOL centrado no ponto inicial
		if( (labs( x_i - x ) < TOL ) && (labs(y_i - y) < TOL) )
		{
			isEqual = true;
		}
	}
	return isEqual;
}
//---------------------------------------------------------------------------
//eof
