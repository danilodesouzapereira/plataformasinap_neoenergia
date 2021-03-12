//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TArquivos.h"
#include "TArqMedicao.h"
#include "TArqRede.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTArquivos* __fastcall NewObjArquivos(VTApl *apl_owner)
   {
   try{
      return(new TArquivos(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TArquivos::TArquivos(VTApl *apl_owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //cria listas
   lisARQ_MEDICAO = new TList();
   lisARQ_REDE    = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TArquivos::~TArquivos(void)
   {
   //destrói listas e seus objetos
   if (lisARQ_MEDICAO) {LimpaTList(lisARQ_MEDICAO); delete lisARQ_MEDICAO; lisARQ_MEDICAO  = NULL;}
   if (lisARQ_REDE)    {LimpaTList(lisARQ_REDE);    delete lisARQ_REDE;    lisARQ_REDE     = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::Clear(void)
   {
   if (lisARQ_MEDICAO) {LimpaTList(lisARQ_MEDICAO);}
   if (lisARQ_REDE)    {LimpaTList(lisARQ_REDE);}
   }

//---------------------------------------------------------------------------
TArqMedicao* __fastcall TArquivos::ExisteArqMedicao(TDateTime dt)
   {
   //variáveis locais
   TArqMedicao *arq_medicao;
   strMES_ANO  data;

   //determina mes/ano de dt_ini
   data.mes = MonthOf(dt);
   data.ano = YearOf(dt);
   //determina ArqRede com mesma DataIni e DataFim
   for (int n = 0; n < lisARQ_MEDICAO->Count; n++)
      {
      arq_medicao = (TArqMedicao*)lisARQ_MEDICAO->Items[n];
      if ((data.mes == arq_medicao->data.mes)&&
          (data.ano == arq_medicao->data.ano)) return(arq_medicao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TArqRede* __fastcall TArquivos::ExisteArqRede(TDateTime dt)
   {
   //variáveis locais
   int      mes_ano, mes_ano_ini, mes_ano_fim;
   TArqRede *arq_rede;

   //determina mes_ano
   mes_ano = MonthOf(dt) + (12 * YearOf(dt));
   for (int n = 0; n < lisARQ_REDE->Count; n++)
      {
      arq_rede    = (TArqRede*)lisARQ_REDE->Items[n];
      mes_ano_ini = arq_rede->data_ini.mes + (12 * arq_rede->data_ini.ano);
      mes_ano_fim = arq_rede->data_fim.mes + (12 * arq_rede->data_fim.ano);
      if ((mes_ano >= mes_ano_ini)&&(mes_ano <= mes_ano_fim)) return(arq_rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TArqRede* __fastcall TArquivos::ExisteArqRede(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   TArqRede *arq_rede;
   strMES_ANO data_ini, data_fim;

   //determina mes/ano de dt_ini
   data_ini.mes = MonthOf(dt_ini);
   data_ini.ano = YearOf(dt_ini);
   //determina mes/ano de dt_fim
   data_fim.mes = MonthOf(dt_fim);
   data_fim.ano = YearOf(dt_fim);
   //determina ArqRede com mesma DataIni e DataFim
   for (int n = 0; n < lisARQ_REDE->Count; n++)
      {
      arq_rede    = (TArqRede*)lisARQ_REDE->Items[n];
      if ((data_ini.mes == arq_rede->data_ini.mes)&&
          (data_ini.ano == arq_rede->data_ini.ano)&&
          (data_fim.mes == arq_rede->data_fim.mes)&&
          (data_fim.ano == arq_rede->data_fim.ano)) return(arq_rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::IniciaArqMedicao(TDateTime dt_ini, TDateTime dt_fim)
   {
   //elimina ArqMedicao existentes fora do período definido
   RemoveArqMedicaoForaDoPeriodo(dt_ini, dt_fim);
   //cria ArqMedicao ainda não existentes do período
   InsereArqMedicaoMensal(dt_ini, dt_fim);
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::IniciaArqRedePorMes(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   int      mes_ano_ini, mes_ano_fim;
   TArqRede *arq_rede;

   //elimina todos ArqRede que não são são mensais
   for (int n = lisARQ_REDE->Count-1; n >= 0; n--)
      {
      arq_rede    = (TArqRede*)lisARQ_REDE->Items[n];
      mes_ano_ini = arq_rede->data_ini.mes + (12 * arq_rede->data_ini.ano);
      mes_ano_fim = arq_rede->data_fim.mes + (12 * arq_rede->data_fim.ano);
      if (mes_ano_ini != mes_ano_fim)
         {
         lisARQ_REDE->Delete(n);
         delete arq_rede;
         }
      }
   //elimina ArqRede fora do período definido
   RemoveArqRedeForaDoPeriodo(dt_ini, dt_fim);
   //cria ArqMedicao ainda não existentes do período
   InsereArqRedeMensal(dt_ini, dt_fim);
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::IniciaArqRedeUnico(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   TArqRede    *arq_rede;

   //verifica se existe ArqRede do Periodo
   if ((arq_rede = ExisteArqRede(dt_ini, dt_fim)) != NULL)
      {//retira o ArqRede de lisARQ_REDE
      lisARQ_REDE->Remove(arq_rede);
      }
   else
      {//cria um novo ArqRede
      arq_rede = new TArqRede(dt_ini, dt_fim);
      }
   //elimina todos os ArqRede
   LimpaTList(lisARQ_REDE);
   //insere ArqRede em lisARQ_REDE
   if (arq_rede) lisARQ_REDE->Add(arq_rede);
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::InsereArqMedicaoMensal(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   int         ano, mes;
   int         mes_ano_ini, mes_ano_fim;
   TDateTime   dt;
   TArqMedicao *arq_medicao;

   //determina mes_ano_ini e mes_ano_fim do período
   mes_ano_ini = (12 * YearOf(dt_ini)) + (MonthOf(dt_ini) - 1) ;
   mes_ano_fim = (12 * YearOf(dt_fim)) + (MonthOf(dt_fim) - 1) ;
   //elimina ArqMedicao fora do período
   for (int mes_ano = mes_ano_ini; mes_ano <= mes_ano_fim; mes_ano++)
      {//verifica se já existe o ArqMedicao
      ano = mes_ano / 12;
      mes = (mes_ano % 12) + 1;
      dt = TDateTime(ano, mes, 1);
      if (ExisteArqMedicao(dt) != NULL) continue;
      //cria um novoArqMedicao e insere em lisARQ_MEDICAO
      if ((arq_medicao = new TArqMedicao(dt)) != NULL)
         {
         lisARQ_MEDICAO->Add(arq_medicao);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::InsereArqRedeMensal(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   int         ano, mes;
   int         mes_ano_ini, mes_ano_fim;
   TDateTime   dt;
   TArqRede    *arq_rede;

   //determina mes_ano_ini e mes_ano_fim do período
   mes_ano_ini = (12 * YearOf(dt_ini)) + (MonthOf(dt_ini) - 1) ;
   mes_ano_fim = (12 * YearOf(dt_fim)) + (MonthOf(dt_fim) - 1) ;
   //elimina ArqRede fora do período
   for (int mes_ano = mes_ano_ini; mes_ano <= mes_ano_fim; mes_ano++)
      {//verifica se já existe o ArqRede
      ano = mes_ano / 12;
      mes = (mes_ano % 12) + 1;
      dt = TDateTime(ano, mes, 1);
      if (ExisteArqRede(dt) != NULL) continue;
      //cria um novo ArqRede e insere em lisARQ_REDE
      if ((arq_rede = new TArqRede(dt, dt)) != NULL)
         {
         lisARQ_REDE->Add(arq_rede);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TArquivos::LisArqMedicao(void)
   {
   return(lisARQ_MEDICAO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TArquivos::LisArqRede(void)
   {
   return(lisARQ_REDE);
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::RemoveArqMedicaoForaDoPeriodo(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   int         mes_ano_arq, mes_ano_ini, mes_ano_fim;
   TArqMedicao *arq_medicao;

   //determina mes_ano_ini e mes_ano_fim do período
   mes_ano_ini = (12 * YearOf(dt_ini)) + (MonthOf(dt_ini) - 1) ;
   mes_ano_fim = (12 * YearOf(dt_fim)) + (MonthOf(dt_fim) - 1) ;
   //elimina ArqMedicao fora do período
   for (int n = lisARQ_MEDICAO->Count - 1; n >= 0  ; n--)
      {
      arq_medicao = (TArqMedicao*)lisARQ_MEDICAO->Items[n];
      mes_ano_arq = (12 * arq_medicao->data.ano) + (arq_medicao->data.mes - 1) ;
      if ((mes_ano_arq < mes_ano_ini) ||
          (mes_ano_arq > mes_ano_fim))
         {//retira ArqRede da lista e destrói objeto
         lisARQ_MEDICAO->Delete(n);
         delete arq_medicao;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TArquivos::RemoveArqRedeForaDoPeriodo(TDateTime dt_ini, TDateTime dt_fim)
   {
   //variáveis locais
   struct{
         int  mes_ano_ini, mes_ano_fim;
         }periodo, arq;
   TArqRede    *arq_rede;

   //determina mes_ano_ini e mes_ano_fim do período
   periodo.mes_ano_ini = (12 * YearOf(dt_ini)) + (MonthOf(dt_ini) - 1);
   periodo.mes_ano_fim = (12 * YearOf(dt_fim)) + (MonthOf(dt_fim) - 1);
   //elimina ArqRede fora do período
   for (int n = lisARQ_REDE->Count - 1; n >= 0  ; n--)
      {
      arq_rede        = (TArqRede*)lisARQ_REDE->Items[n];
      arq.mes_ano_ini = (12 * arq_rede->data_ini.ano) + (arq_rede->data_ini.mes - 1) ;
      arq.mes_ano_fim = (12 * arq_rede->data_fim.ano) + (arq_rede->data_fim.mes - 1) ;
      if ((arq.mes_ano_ini < periodo.mes_ano_ini) ||
          (arq.mes_ano_fim > periodo.mes_ano_fim))
         {//retira ArqRede da lista e destrói objeto
         lisARQ_REDE->Delete(n);
         delete arq_rede;
         }
      }
   }

//---------------------------------------------------------------------------
//eof

