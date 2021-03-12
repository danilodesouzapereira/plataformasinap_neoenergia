//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRegiao.h"
#include "VTParede.h"
#include "VTPoligono.h"
#include "TCentrocarga.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRegiao* __fastcall NewObjRegiao(VTApl *apl)
   {
   try{//cria objeto TRegiao
      return(new TRegiao(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TRegiao::TRegiao(VTApl *apl)
   {
   //salva objetos
   this->apl = apl;
   //inicia atributos
   //Id     = -1;
   Id     = FalsoId();
   Codigo = "";
   Color  = clWhite;
   Obj    = NULL;
   Tag    = 0;
   //novos atributos
   RegiaoPai = NULL;
   LisRegiaoFilhas = new TList();
   Level = -1;
   //cria objetos
   Poligono   = NewObjPoligono();
   lisPAREDE  = new TList();

   }

//---------------------------------------------------------------------------
__fastcall TRegiao::~TRegiao(void)
   {
   //destrói objetos
   if (Poligono) {delete Poligono;  Poligono = NULL;}
   //destrói lista
   if (LisRegiaoFilhas ) { delete LisRegiaoFilhas;  LisRegiaoFilhas = NULL;}
   //destrói lista e seus objetos
   if (lisPAREDE ) {LimpaTList(lisPAREDE); delete lisPAREDE;  lisPAREDE = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TRegiao::CalculaCentroCarga(int ind_pat)
   {
   //variáveis locais
   TCentrocarga centrocarga(apl);

   return(centrocarga.Executa(this, ind_pat));
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TRegiao::ExisteEqpto(VTEqpto *eqpto)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
VTParede* __fastcall TRegiao::ExisteParede(VTRede *rede)
   {
   //variáveis locais
   VTParede *parede;

   for (int n = 0; n < lisPAREDE->Count; n++)
      {
      parede = (VTParede*)lisPAREDE->Items[n];
      if (parede->Rede == rede) return(parede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TRegiao::LisEqpto(TList *lisEXT, int tipo_eqpto)
   {
   //variáveis locais
   int      count = 0;
   VTParede *parede;

   for (int n = 0; n < lisPAREDE->Count; n++)
      {
      parede = (VTParede*)lisPAREDE->Items[n];
      count += parede->LisEqpto(lisEXT, tipo_eqpto);
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TRegiao::LisEqpto(TList *lisEXT, VTRede *rede, int tipo_eqpto)
   {
   //variáveis locais
   int      count = 0;
   VTParede *parede;

   //verifica se existe VTParede da Rede
   if ((parede = ExisteParede(rede)) == NULL) return(-1);
   //obtém os Eqptos do tipo indicado da Rede
   count = parede->LisEqpto(lisEXT, tipo_eqpto);
   return(count);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegiao::ReiniciaLisEqpto(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra;
   VTParede  *parede;
   VTLigacao *ligacao;
   TList     *lisBAR = rede->LisBarra();
   TList     *lisLIG = rede->LisLigacao();

   //verifica se existe um Parede para a Rede
   if ((parede = ExisteParede(rede)) == NULL)
      {//cria um novo objeto Parede e insere em lisPAREDE
      lisPAREDE->Add(parede = NewObjParede());
      parede->Rede = rede;
      }
   else
      {//reinicia conjunto de Eqptos do Parede
      parede->ReiniciaLisEqpto();
      }
   //loop p/ todas Barras da Rede
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //verifica se a Barra está localizada dentro do Poligono
      if (Poligono->PontoInternoPoligono(barra->utm.x, barra->utm.y))
         {//insere Barra em Parede
         parede->InsereBarra(barra);
         }
      }
   //loop p/ todas Ligacoes da Rede
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //verifica se alguma Barra da Ligacao existe em Parede
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
         if (parede->ExisteBarra(barra))
            {//insere Ligacao em Parede
            parede->InsereLigacao(ligacao);
            break;
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegiao::ReiniciaLisEqpto(VTRedes *redes)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisREDE = redes->LisRede();

   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      ReiniciaLisEqpto(rede);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

