//-----------------------------------------------------------------------------
#ifndef VTResumo_H
#define VTResumo_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTResumo;
class VTEqpto;

//---------------------------------------------------------------------------
// enumerações
//enum enmTipoGrupoPerda  { gpINDEFINIDO=-1, gpEMPRESA=1, gpREGIAO=2, gpLOCALIDADE=3, gpREDE=4, gpAGREGADO=5 };
//CUIDADO: este enum são os IDs da tabela GrupoPerda da base Historico.mdb
//enum enmGrupoPerda      { grupoREDE_CALCULADA=4, grupoREDE_NAOCALCULADA=5, grupoREDE_ISOLADA=6};
enum resumoTIPO {resumoREDE=1, resumoAGREGADO};

//-----------------------------------------------------------------------------
class VTResumo : public TObject
   {
   public:
                   __fastcall  VTResumo(void) {};
                   __fastcall ~VTResumo(void) {};
      virtual void __fastcall  Agrega(TList *lisRESUMO) = 0;
      virtual void __fastcall  Agrega(VTResumo *resumo) = 0;
      virtual void __fastcall  CopiaDe(VTResumo *resumo) = 0;
      virtual void __fastcall  ZeraValores(void) = 0;

   public:  //dados
      //int                    GrupoPerdaId;     // OOPS. Aqui poderia ser somente ID e retirar da estrutura de rede
      //AnsiString             Codigo;           // OOPS. Aqui poderia retirar da estrutura de rede
      //int                    GrupoPerdaPaiId;  // Precisei colocar, pois na leitura da base, é necessário identificar o pai
      //enum enmTipoGrupoPerda TipoGrupoPerdaId; // Tipo de grupo

      int      id;
      int      tipo_resumo;   //ver enum resumoTIPO acima
      VTEqpto  *eqpto;
      TObject  *object;
      struct   {
               int        Id;
               AnsiString Data;
               AnsiString Codigo;
               AnsiString TipoRede;
               int        TipoRedeId;
               int        QtdeRede;
               int        QtdeTrafo;
               int        QtdeCabo;
               int        QtdeCapacitor;
               int        QtdeReator;
               int        QtdeRegulador;
               int        QtdeConsA4;
               int        QtdeConsRes;
               int        QtdeConsCom;
               int        QtdeConsInd;
               int        QtdeConsRur;
               int        QtdeConsOut;
               int        QtdeConsNulo;
               double     EnergiaMedida;
               double     EnergiaMercado;
               }rede;
       struct  {
               double     PerdaCabo;
               double     PerdaTrafoCobre;
               double     PerdaTrafoFerro;
               double     PerdaCapacitor;
               double     PerdaReator;
               double     PerdaRegulador;
               double     PerdaMedidor;
               double     PerdaRamal;
               double     PerdaOutros;
               }perda;
       struct  {
               double     EnergiaEntrada;
               double     EnergiaSuprimento;
               double     EnergiaGerador;
               double     EnergiaRecebidaExt;
               double     EnergiaRecebidaInt;
               double     EnergiaEntregueExt;
               double     EnergiaEntregueInt;
               double     EnergiaCarga;
               double     EnergiaPerdaFluxo;
               double     EnergiaPerdaTotal;
               //double     EnergiaPerdaTotalPerc;
			   }balanco;
       struct  {
			   	double A1_A2;
				double A1_A3;
				double A1_A3A;
				double A1_A4;
				double A2_A3;
				double A2_A3A;
				double A2_A4;
				double A3_A2;
				double A3_A3A;
				double A3_A4;
				double A3A_A2;
				double A3A_A4;
				double A3A_B;
				double A4_A2;
				double A4_A3;
				double A4_A3A;
				double A4_A4;
				double A4_B;
				double B_A3A;
				double B_A4;
			   }energiaPassante;
    };

//-----------------------------------------------------------------------------
//função global
VTResumo* __fastcall NewObjResumo(void);

#endif
//---------------------------------------------------------------------------
//eof
