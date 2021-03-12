//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCluster.h"
#include "VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTCluster
//---------------------------------------------------------------------------
VTCluster* __fastcall NewObjCluster(void)
   {
   return(new TCluster);
   }

//---------------------------------------------------------------------------
__fastcall TCluster::TCluster(void)
   {
   //cria objetos
   lisREDE = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TCluster::~TCluster(void)
   {
   //vari�veis locais
   VTRede *rede;

   //cancela associa��o c/ as Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Cluster == this) rede->Cluster = NULL;
      }
   //destr�i listas sem destruir seus objetos
   if (lisREDE) {delete lisREDE; lisREDE = NULL;}
   }

//---------------------------------------------------------------------------
VTCluster* __fastcall TCluster::Clone(void)
   {
   //vari�veis locais
   VTCluster *cluster;

   //cria um novo objeto VTCluster
   if ((cluster = NewObjCluster()) != NULL)
      {//copia dados deste objeto
      cluster->CopiaAtributosDe(*this);
      }
   return(cluster);
   }

//---------------------------------------------------------------------------
void __fastcall TCluster::CopiaAtributosDe(VTEqpto &ref)
   {
   //vari�veis locais
   TCluster &cluster = (TCluster&)ref;

   //prote��o
   if (ref.Tipo() != eqptoCLUSTER) return;
   //copia atributos de VTEqpto
   VTEqpto::CopiaAtributosDe(cluster);
   //copia lista de Redes do Cluster de refer�ncia
   lisREDE->Clear();
   cluster.LisRede(lisREDE);
   }

//---------------------------------------------------------------------------
bool __fastcall TCluster::ExisteRede(VTRede *rede)
   {
   return(lisREDE->IndexOf(rede) >= 0);
   }

//---------------------------------------------------------------------------
void __fastcall TCluster::InsereRede(VTRede *rede)
   {
   //prote��o
   if (rede == NULL) return;
   //verifica se a Rede j� existe na lista
   if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
   //associa Rede c/ este CLuster
   rede->Cluster = this;
   }

//---------------------------------------------------------------------------
TList* __fastcall TCluster::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TCluster::LisRede(TList *lisEQP)
   {
   CopiaTList(lisREDE, lisEQP);
   }

//---------------------------------------------------------------------------
int __fastcall TCluster::NumeroRedes(void)
   {
   return(lisREDE->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TCluster::PM_GetCarregado(void)
   {
   //se n�o possuir Rede, assume que est� carregado
   //if (lisREDE->Count == 0) return(true);
   //se n�o possuir Rede, assume que n�o est� carregado
   if (lisREDE->Count == 0) return(false);
   //verifica se suas Redes est�o carregadas
   return(((VTRede*)lisREDE->First())->Carregada);
   }

//---------------------------------------------------------------------------
void __fastcall TCluster::Reinicia(void)
   {
   lisREDE->Clear();
   }
   
//---------------------------------------------------------------------------
void __fastcall TCluster::RemoveRede(VTRede *rede)
   {
   //retira Rede da lista lisREDE
   lisREDE->Remove(rede);
   //cancela associa��o da Rede c/ este CLuster
   if (rede->Cluster == this) rede->Cluster = NULL;
   }

//---------------------------------------------------------------------------
int __fastcall TCluster::Tipo(void)
   {
   return(eqptoCLUSTER);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCluster::TipoAsString(void)
   {
   return("Cluster");
   }

//---------------------------------------------------------------------------
//eof

