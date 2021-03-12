//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Util\TExtraiCampos.h>
#include <DLL_Inc\Funcao.h>
#include "TMedicoes.h"
#include "TMedicao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função estática usada para ordenar as Medicoes
static int __fastcall ComparaMedicaoLevel(void *obj1, void* obj2)
   {
   int level1 = ((TMedicao*)obj1)->level;
   int level2 = ((TMedicao*)obj2)->level;
   if      (level1 == level2) return( 0);
   else if (level1 <  level2) return( 1);
   else                 return(-1);
   }

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTMedicoes* __fastcall NewObjMedicoes(VTApl *apl_owner)
   {
   try{
      return(new TMedicoes(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMedicoes::TMedicoes(VTApl *apl_owner)
   {
   //guarda ponteiro
   apl = apl_owner;
   //cria objetos
   lisMEDICAO  = new TList();
   lines       = new TStringList();
   //inicia variavel //DVK 2016.07.28
   PD.alterado = false;
   }

//---------------------------------------------------------------------------
__fastcall TMedicoes::~TMedicoes(void)
   {
   //destrói objetos
   if (lines) {delete lines; lines = NULL;}
   //destrói lista e seus objetos
   if (lisMEDICAO) {LimpaTList(lisMEDICAO); delete lisMEDICAO; lisMEDICAO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TMedicoes::Clear(void)
   {
   LimpaTList(lisMEDICAO);
   }

//---------------------------------------------------------------------------
TMedicao* __fastcall TMedicoes::ExisteMedicao(VTRede *rede)
   {
   //variáveis locais
   TMedicao *medicao;

   //procura Medidor em lisMEDIDOR
   for (int n = 0; n < lisMEDICAO->Count; n++)
      {
      medicao = (TMedicao*)lisMEDICAO->Items[n];
      if (medicao->rede == rede) return(medicao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMedicoes::ImportaMedicao(TStrings *files)
   {
   //variáveis locais
   AnsiString    filename;
   double        energia_mwhmes, perda_sdmt_perc, perda_sdbt_perc;
   TExtraiCampos extrai_campos;
   TMedicao      *medicao;
   TStrings      *campos;
   VTRede        *rede;
   VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//configura leitura do arquivo
      extrai_campos.SeparadorInsere(';');
      extrai_campos.SetOpcaoFiltraComentario(true);
      extrai_campos.SetOpcaoTrimCampos(true);
      //loop para todos arquivos em files
      for (int nf = 0; nf < files->Count; nf++)
         {//verifica se existe o arquivo
         filename = files->Strings[nf];
         if (! FileExists(filename)) continue;
         //lê arquivo de medição
         lines->LoadFromFile(filename);
         //loop p/ todas linhas do arquivo
         for (int nl = 0; nl < lines->Count; nl++)
            {//lê campos da linha
            if ((campos = extrai_campos.Executa(lines->Strings[nl])) == NULL) continue;
            //verifica o número de campos
            if (campos->Count < 4) continue;
            //lê demais campos
            energia_mwhmes  = StrToDouble(campos->Strings[1]);
            perda_sdmt_perc = StrToDouble(campos->Strings[2]);
            perda_sdbt_perc = StrToDouble(campos->Strings[3]);
            //determina Rede da medição
            if ((rede = redes->ExisteRede(campos->Strings[0])) == NULL) continue;
            //verifica se já existe Medicao para a Rede
            if ((medicao = ExisteMedicao(rede)) == NULL)
               {//cria uma nova Medicao
               medicao = InsereMedicao(rede);
               }
            //lê valores da Medicao
            medicao->energia.medida_mwhmes            = energia_mwhmes;
			medicao->alocacao_perda_comercial.sdmt_pu = perda_sdmt_perc * 0.01;
			medicao->alocacao_perda_comercial.sdbt_pu = perda_sdbt_perc * 0.01;
			//verifica validade
			if (IsDoubleZero(100 - (perda_sdmt_perc + perda_sdbt_perc)))
				{
                    medicao->alocacao_perda_comercial.valida = true;
				}
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
TMedicao* __fastcall TMedicoes::InsereMedicao(VTRede *rede)
   {
   //variáveis locais
   TMedicao *medicao;

   //proteção: verifica se já existe Medicao
   if ((medicao = ExisteMedicao(rede)) == NULL)
      {
      if ((medicao = new TMedicao(rede)) != NULL)
         {
         lisMEDICAO->Add(medicao);
         }
      }
   return(medicao);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMedicoes::LisMedicao(void)
   {
   return(lisMEDICAO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMedicoes::OrdenaLisMedicao(void)
   {
   //ordena lista de Medicoes
   lisMEDICAO->Sort(ComparaMedicaoLevel);

   return(lisMEDICAO);
   }

//---------------------------------------------------------------------------
void __fastcall TMedicoes::RetiraMedicao(TMedicao *medicao)
   {
   //proteção
   if (medicao != NULL)
      {
      lisMEDICAO->Remove(medicao);
      delete medicao;
      }
   }

//---------------------------------------------------------------------------
//eof

