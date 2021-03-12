//---------------------------------------------------------------------------
#ifndef VTImportaRedeGISH
#define VTImportaRedeGISH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
enum opSubestacao    {opSUB_GIS=0, opSUB_TROCAR, opSUB_EXCLUIR, opSUB_SIMPLES}; //DVK 2017.05.10 simples
enum opSecundario    {opSEC_GIS=0, opSEC_UTC,    opSEC_CARGA};
enum opA4            {opENERGIA=0, opCURVA,      opDEM_CONTRATADA};  //DVK 2014.02.14

//---------------------------------------------------------------------------
struct strOPImporta
   {
   TDateTime date;
   struct{
         int topologia;
         }sub;
   struct{
         int  consA4;        //DVK 2014.02.14
		 bool exclui_neutro; //FKM 2017.03.28
         }pri;
   struct{
         int  topologia;
         bool vtap_como_vnom; //FKM 2017.03.28
         }sec;
   struct{
		 bool log;
		 int  progresso;
		 }ihm;
   struct{
		 bool abre_chave_ext;
		 bool abre_chave_int;
		 bool desloca_trecho_ext;
		 bool desloca_trecho_int;
		 }malha;
   struct{
		 bool gera_relatorio_eqpto;
		 bool salva_rede;
		 AnsiString dir_rel_eqpto;
		 }rel_eqpto;
   struct{
		bool redes;
        //bool arranjos;
		bool commons;
		}reinicia;
   bool   valida_eqpto; //DVK 2015.01.19
   bool   valida_fase;
   bool   corrige_fase;
   bool   corrige_perda_fe;
   bool   ajusta_esquematico;
   bool   gerarTXT;
   double fator_carga; //DVK 2014.05.12
   //gambiarra - FKM
   AnsiString pasta_destino;
   AnsiString pasta_destino_correcao;
   AnsiString listaSubs;
   AnsiString listaPris;
   int        destino_rede;
   };

//---------------------------------------------------------------------------
class VTSub : public TObject
   {//classe usada para egrid
   public: //property
      __property AnsiString   CodSub     = {read=PD.cod_sub, write=PD.cod_sub };
	  __property TStringList* Primarios  = {read=PD.lisPRI,  write=PD.lisPRI  };
	  __property TStringList* PrimariosSel  = {read=PD.lisPRI_SEL,  write=PD.lisPRI_SEL  };
	  __property TList* SubFilha  = {read=PD.lisSUB_FILHA,  write=PD.lisSUB_FILHA  };

   public:
				   __fastcall  VTSub(void) {};
	  virtual      __fastcall ~VTSub(void) {};

	  struct
	  {
		 AnsiString cod_sub;
		 TStringList *lisPRI;
		 TStringList *lisPRI_SEL;
		 TList *lisSUB_FILHA;
	  }PD;
   };

//---------------------------------------------------------------------------
class VTImportaRedeGIS : public TObject
   {
   public:
				   __fastcall  VTImportaRedeGIS(void) {};
	  virtual      __fastcall ~VTImportaRedeGIS(void) {};
	  virtual bool __fastcall  ArquivoEgrid(void) {return false;};  //indica se o importador é de egrid
	  //métodos implementados em importadores de egrid
	  virtual bool   __fastcall  ImportaDadosEmpresa(AnsiString arq_egrid) {return false;};
	  virtual bool   __fastcall  ImportaEgrid(strOPImporta *opcao, int destino) {return false;};
	  virtual bool   __fastcall  InsereSubestacao(TList *lisSub) {return false;};
	  virtual TList* __fastcall  LisAllSub(void) {return NULL;};
	  //métodos implementados em outros importadores
      virtual bool __fastcall  ImportaLisArqPrimario(TStrings *files, strOPImporta *opcao) = 0;
	  virtual bool __fastcall  ImportaLisArqSubestacao(TStrings *files, strOPImporta *opcao) = 0;
      virtual void __fastcall  SelecionaArqTxt(TOpenDialog *OpenDialog, TStrings *files) = 0;
	  virtual void __fastcall  SelecionaLisArqTxt(TOpenDialog *OpenDialog, TStrings *files) = 0;
	  //forms
      virtual bool __fastcall  ShowModalFormImportaRedeGIS(void) = 0;
      virtual bool __fastcall  ShowModalFormImportaRedeGIS_SDBT(void) = 0;
      virtual bool __fastcall  ShowModalFormImportaRedeGIS_SDMT(void) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTImportaRedeGIS
VTImportaRedeGIS* __fastcall NewObjImportaRedeGIS(TComponent *Owner, VTApl *apl_owner);
//função para criar objeto VTImportaRedeGIS
VTImportaRedeGIS* __fastcall NewObjImportaBDGD(TComponent *Owner, VTApl *apl_owner);
//função para criar objeto VTSub
VTSub* __fastcall NewObjSub(void);

#endif
//---------------------------------------------------------------------------
//eof
