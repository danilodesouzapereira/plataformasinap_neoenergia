//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Figura\Estilos\VTEstiloLink.h>
#include "VTFiguraComentario.h"
#include "VTRetangulo.h"
#include "TLink.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTLink* __fastcall NewObjLink(VTApl *apl_owner)
   {
   return(new TLink(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TLink::TLink(VTApl *apl_owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //define atributos
   Status[sttEV_NOVO] = true;
   Id        		  = -1;
   Tag       		  =  0;
   Visible            = true;
   PosCom             = posAUTO;
   PosEqp             = posEQP_XmYm;
   BordaTipo          = psSolid;
   Espessura          = 1;
   Color              = clWhite;
   }

//---------------------------------------------------------------------------
__fastcall TLink::~TLink(void)
   {
   //nada a fazer
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TLink::Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xa, ya, xb, yb;

   //determina coordenadas do ponto de conexão com Comentário
   Comentario->Retangulo->Ponto_esq(PosCom, escala, x1, y1);
   //determina coordenadas do ponto de conexão com Eqpto
   Eqpto->CoordenadasEsquematico(xa, ya, xb, yb);
   //verifica o ponto de conexão no Eqpto
   switch(PosEqp)
      {
      case posEQP_X1Y1:
         x2 = xa;
         y2 = ya;
         break;
      case posEQP_X2Y2:
         x2 = xb;
         y2 = yb;
         break;
      case posEQP_XmYm:
      default:
         x2 = (xa + xb) / 2;
         y2 = (ya + yb) / 2;
         break;
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TLink::AplicaEstilo(VTEstilo *estilo)
{
	VTEstiloLink *estiloL = NULL;
	bool sucesso = true;

	try
	{
		if ((estilo != NULL) && (estilo->TipoEstilo == estiloLINK))
		{
			estiloL = (VTEstiloLink*) estilo;
		}
		else
		{   //verifica se a figura já possui um comentario
			if ((this->Estilo != NULL) && (this->Estilo->TipoEstilo == estiloLINK))
			{
				estiloL = (VTEstiloLink*) this->Estilo;
			}
		}
		if (estiloL != NULL)
		{
			Espessura 	 = estiloL->BordaEspessura;
			BordaTipo 	 = estiloL->BordaTipo;
			Color      	 = estiloL->Bordacor;
		}
		else
		{
			sucesso = false;
        }
	}
	catch (...)
	{
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TLink::Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xa, ya, xb, yb;
   int dist[4], dist_min, pos_com;

   //determina coordenadas do ponto de conexão com Eqpto
   Eqpto->CoordenadasEsquematico(xa, ya, xb, yb);
   //verifica o ponto de conexão no Eqpto
   switch(PosEqp)
      {
      case posEQP_X1Y1:
         x2 = xa;
         y2 = ya;
         break;
      case posEQP_X2Y2:
         x2 = xb;
         y2 = yb;
         break;
      case posEQP_XmYm:
      default:
         x2 = (xa + xb) / 2;
         y2 = (ya + yb) / 2;
         break;
      }
   //verifica o ponto de conexão no Comentario
   if (Comentario->Formato == fmtFLAG)
      {//Comentario está sendo exibido apenas como um ícone sem texto
      pos_com = posCENTER;
      }
   else if ((pos_com = PosCom) == posAUTO)
      {//determina melhor ponto de conexão na moldura
      Comentario->Retangulo->Ponto_esq(posLEFT, escala, xa, ya);
      dist[0] = Distancia_cm(xa, ya, x2, y2);
      Comentario->Retangulo->Ponto_esq(posTOP, escala, xa, ya);
      dist[1] = Distancia_cm(xa, ya, x2, y2);
      Comentario->Retangulo->Ponto_esq(posRIGHT, escala, xa, ya);
      dist[2] = Distancia_cm(xa, ya, x2, y2);
      Comentario->Retangulo->Ponto_esq(posBOTTOM, escala, xa, ya);
      dist[3] = Distancia_cm(xa, ya, x2, y2);
      //assume que a menor distância é de posLEFT
      dist_min = dist[0]; pos_com = posLEFT;
      if (dist[1] < dist_min) {dist_min = dist[1]; pos_com = posTOP;}
      if (dist[2] < dist_min) {dist_min = dist[2]; pos_com = posRIGHT;}
      if (dist[3] < dist_min) {dist_min = dist[3]; pos_com = posBOTTOM;}
      }
   //obtém coordenadas do ponto de conexão da moldura
   Comentario->Retangulo->Ponto_esq(pos_com, escala, x1, y1);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TLink::Coordenadas_utm(double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xa, ya, xb, yb;

   //determina coordenadas do ponto de conexão com Comentário
   Comentario->Retangulo->Ponto_esq(PosCom, escala, x1, y1);
   //determina coordenadas do ponto de conexão com Eqpto e aplica escala
   Eqpto->CoordenadasUtm_cm(xa, ya, xb, yb);
   //verific o ponto de conexão no Eqpto
   switch(PosEqp)
      {
      case posEQP_X1Y1:
         x2 = xa;
         y2 = ya;
         break;
      case posEQP_X2Y2:
         x2 = xb;
         y2 = yb;
         break;
      case posEQP_XmYm:
      default:
         x2 = (xa + xb) / 2;
         y2 = (ya + yb) / 2;
         break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TLink::Move_esq(int dx, int dy)
   {
   Status[sttALTERADO] = true;
   }

//---------------------------------------------------------------------------
void __fastcall TLink::Move_utm(int dx, int dy)
   {
   Status[sttALTERADO] = true;
   }
//---------------------------------------------------------------------------
void __fastcall TLink::SalvaInfoNoEstilo(void)
{
	VTEstiloLink *est;

	if (Estilo != NULL)
	{
		est = (VTEstiloLink*)Estilo;
		est->BordaTipo = this->BordaTipo;
		est->BordaEspessura = this->Espessura;
		est->Bordacor = this->Color;
	}
}

//---------------------------------------------------------------------------
//eof
