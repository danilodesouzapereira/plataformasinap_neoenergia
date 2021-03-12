//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include <Fontes\Figura\Estilos\VTEstiloRetangulo.h>
#include <DLL_Inc\Funcao.h>
#include "TRetangulo.h"
#include "VTFiguraComentario.h"
#include "VTEstilos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRetangulo* __fastcall NewObjRetangulo(VTApl *apl_owner)
   {
   return(new TRetangulo(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TRetangulo::TRetangulo(VTApl *apl_owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //define atributos
   Id             	  = -1;
   Tag            	  =  0;
   Visible        	  = true;
   Transparente   	  = false;
   BordaEspessura 	  = 1;
   BordaEstilo    	  = psSolid;
   BordaCor       	  = clWhite;
   FundoCor       	  = clGray;
   }

//---------------------------------------------------------------------------
__fastcall TRetangulo::~TRetangulo(void)
   {
   //nada a fazer
   }
//---------------------------------------------------------------------------
bool __fastcall TRetangulo::AplicaEstilo(VTEstilo *estilo)
{
	VTEstiloRetangulo *estiloR = NULL;
	bool sucesso = true;

	try
	{
		if ((estilo != NULL) && (estilo->TipoEstilo == estiloRETANGULO))
		{
			estiloR = (VTEstiloRetangulo*) estilo;
		}
		else
		{   //verifica se a figura já possui um comentario
			if ((this->Estilo != NULL) && (this->Estilo->TipoEstilo == estiloRETANGULO))
			{
				estiloR = (VTEstiloRetangulo*) this->Estilo;
			}
		}
		if (estiloR != NULL)
		{
			BordaEspessura 	 = estiloR->BordaEspessura;
			BordaEstilo  	 = estiloR->BordaTipo;
			BordaCor       	 = estiloR->Bordacor;
			FundoCor       	 = estiloR->FundoCor;
			Transparente     = estiloR->FundoTransparente;
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
bool __fastcall TRetangulo::Coordenadas_esq(double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //proteção
   if (IsDoubleZero(escala, 1e-10)) escala = 1;
   //determina coordenadas
   x1 = centro.esq.x - (dim.dx / escala);
   y1 = centro.esq.y - (dim.dy / escala);
   x2 = centro.esq.x + (dim.dx / escala);
   y2 = centro.esq.y + (dim.dy / escala);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRetangulo::Coordenadas_utm(double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //proteção
   if (IsDoubleZero(escala, 1e-10)) escala = 1;
   //determina coordenadas
   x1 = centro.utm.x - (dim.dx / escala);
   y1 = centro.utm.y - (dim.dy / escala);
   x2 = centro.utm.x + (dim.dx / escala);
   y2 = centro.utm.y + (dim.dy / escala);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TRetangulo::GetCentro(strUTM &utm, strCOORD &esq)
   {
   utm = centro.utm;
   esq = centro.esq;
   }

//---------------------------------------------------------------------------
void __fastcall TRetangulo::GetDimensao(int &dx, int &dy)
   {
   //retorna dimensão
   dx = dim.dx << 1;
   dy = dim.dy << 1;
   }

//---------------------------------------------------------------------------
void __fastcall TRetangulo::Move_esq(int dx, int dy)
   {
   centro.esq.x += dx;
   centro.esq.y += dy;
   Status[sttEV_ALTERADO] = true;
   }

//---------------------------------------------------------------------------
void __fastcall TRetangulo::Move_utm(int dx, int dy)
   {
   centro.utm.x += dx;
   centro.utm.y += dy;
   Status[sttEV_ALTERADO] = true;
   }

//---------------------------------------------------------------------------
bool __fastcall TRetangulo::Ponto_esq(int pos_index, double escala, int &x, int &y)
   {
   //proteção
   if (IsDoubleZero(escala, 1e-10)) escala = 1;
   switch (pos_index)
      {
      case posLEFT:
         x = centro.esq.x - (dim.dx / escala);
         y = centro.esq.y;
         break;
      case posTOP:
         x = centro.esq.x;
         y = centro.esq.y - (dim.dy / escala);
         break;
      case posRIGHT:
         x = centro.esq.x + (dim.dx / escala);;
         y = centro.esq.y;
         break;
      case posBOTTOM:
         x = centro.esq.x;
         y = centro.esq.y + (dim.dy / escala);
         break;
      case posCENTER:
      default:
         x = centro.esq.x;
         y = centro.esq.y;
         break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRetangulo::Ponto_utm(int pos_index, double escala, int &x, int &y)
   {
   //proteção
   if (IsDoubleZero(escala, 1e-10)) escala = 1;
   switch (pos_index)
      {
      case posLEFT:
         x = centro.utm.x - (dim.dx / escala);
         y = centro.utm.y;
         break;
      case posTOP:
         x = centro.utm.x;
         y = centro.utm.y - (dim.dy / escala);
         break;
      case posRIGHT:
         x = centro.utm.x + (dim.dx / escala);
         y = centro.utm.y;
         break;
      case posBOTTOM:
         x = centro.utm.x;
         y = centro.utm.y + (dim.dy / escala);
         break;
      case posCENTER:
      default:
         x = centro.utm.x;
         y = centro.utm.y;
         break;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TRetangulo::SalvaInfoNoEstilo(void)
{
	VTEstiloRetangulo *est;

	if (Estilo != NULL)
	{
		est = (VTEstiloRetangulo*)Estilo;
		est->BordaTipo = this->BordaEstilo;
		est->BordaEspessura = this->BordaEspessura;
		est->Bordacor = this->BordaCor;
		est->FundoCor = this->FundoCor;
		est->FundoTransparente = this->Transparente;
	}
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall  TRetangulo::SalvaValorFormaAtributo(VTEstilos *estilos)
{
	bool sucesso = true;
	int dx, dy;
	int formaAtributoId = -1;
	strVALOR_FORMA_ATRIBUTO * str = NULL;

	try
	{
		dx = dy = DIM_FLAG;
		//destroi os objetos
		LimpaLisValorFormaAtributo();
		//preenche novamente
		GetDimensao(dx,dy);
		//largura
		formaAtributoId = estilos->FormaAtributoId(AUXF_FORMAAT_DIMX);
		if (formaAtributoId < 0)
		{
			sucesso = false;
		}
		else
		{
			str = InsereValorFormaAtributo(formaAtributoId, IntToStr(dx));
			if (str != NULL)
			{
				sucesso = false;
			}
		}
        //altura
		formaAtributoId = estilos->FormaAtributoId(AUXF_FORMAAT_DIMY);
		if (formaAtributoId < 0)
		{
			sucesso = false;
		}
		else
		{
			str = InsereValorFormaAtributo(formaAtributoId, IntToStr(dy));
			if (str != NULL)
			{
				sucesso = false;
			}
		}
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TRetangulo::SetCentro(strUTM &utm, strCOORD &esq)
   {
   centro.utm = utm;
   centro.esq = esq;
   //assume dimensões default
   if (dim.dx == 0) dim.dx = 50;
   if (dim.dy == 0) dim.dy =  8;
   }

//---------------------------------------------------------------------------
void __fastcall TRetangulo::SetDimensao(int dx, int dy)
   {
   //define dimensão com valor par
   dim.dx = (dx + 1) >> 1;
   dim.dy = (dy + 1) >> 1;
   }

//---------------------------------------------------------------------------
//eof
