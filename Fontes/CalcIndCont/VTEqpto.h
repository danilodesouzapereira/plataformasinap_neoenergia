//---------------------------------------------------------------------------
#ifndef VTEqptoH
#define VTEqptoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum eqptoTIPO {//tipos de equipamentos
                eqptoINDEF=-1,
                eqptoARRANJO=1,
                eqptoBARRA,
                eqptoEQBAR,
                eqptoCABO,
                eqptoCAPACITOR,
                eqptoCAPSERIE,
                eqptoCARGA,
                eqptoCORTE,
                eqptoCHAVE,
                eqptoGERADOR,
                eqptoFILTRO,
                eqptoLIGACAO,
                eqptoMEDIDOR,
                eqptoMREDE,
                eqptoMUTUA,
                eqptoNO,
                eqptoNET,
                eqptoREATOR,
                eqptoREDE,
                eqptoREDUCAO,
                eqptoREGULADOR,
                eqptoSUPORTE,
                eqptoSUPRIMENTO,
                eqptoTRAFO,
                eqptoTRAFO3E,
                eqptoTRAFOZZ,
                eqptoTRAMO,
                eqptoTRECHO,
                //eqptoZREF,
                eqptoYREF,
                eqptoARRANJOS,
                eqptoCLUSTER,
                eqptoCANAL,     //canal de medidor
                eqptoDOMINIO,
                eqptoCARGANL
                };

//---------------------------------------------------------------------------
enum eqptoSTATUS {//status do Eqpto
                 sttNOVO     =0x00000001,
                 sttALTERADO =0x00000002,
                 sttEXCLUIDO =0x00000004,
                 sttISOLADO  =0x00000008,
                 sttENABLED  =0x00000010,
                 sttDESLOCADO=0x00000020, //alteração de coordenadas esquemático
                 sttOBRA     =0x00000040, //eqpto associado a uma obra
                 sttCOMENTA  =0x00000080, //comentário associado ao equipamento
                 //específico p/ Carga
                 sttCARGA_VIC =0x10000000,
                 //controle p/ desenhar o Eqpto uma única vez
                 sttREDRAW    =0x80000000 //1=desenhar, 0=não desenhar
                 };

//---------------------------------------------------------------------------
enum eqptoSHOW {//atributos comuns a todos Eqptos
               eqp_VISIBLE    =0x00000001,   //visível
               eqp_FIXO       =0x00000002,   //ícone e legenda de tamanho fixo e visíveis (se eqp_VISIBLE = true)
               eqp_ID         =0x00000004,
               eqp_CODIGO     =0x00000008,
               //
               //ARRANJO--------------------------------
               //atributos de topologia
               arr_CFG        =0x00000010,
               //BARRA--------------------------------
               //atributos de topologia
               bar_UTM        =0x00000010,
               bar_VNOM       =0x00000020,
               //atributos de fluxo: desequilíbrio
               bar_DESEQ      =0x00000040,   //desequilíbrio de tensão
               //atributos de fluxo: tensões fase/neutro em pu
               bar_VFN_PU     =0x00000080,   //tensões fase/neutro  em pu nas 3 fases
               bar_VFN_PU_MAX =0x00000100,   //tensão  fase/neutro  em pu máxima
               bar_VFN_PU_MIN =0x00000200,   //tensão  fase/neutro  em pu mínima
               //atributos de fluxo: tensões fase/neutro em kV
               bar_VFN_KV     =0x00000400,   //tensões fase/neutro  em kV nas 3 fases
               bar_VFN_KV_MAX =0x00000800,   //tensão  fase/neutro  em kV máxima
               bar_VFN_KV_MIN =0x00001000,   //tensão  fase/neutro  em kV mínima
               //atributos de fluxo: tensões de linha em pu
               bar_VFF_PU     =0x00002000,   //tensões linha        em pu nas 3 fases
               bar_VFF_PU_MAX =0x00004000,   //tensão  linha        em pu máxima
               bar_VFF_PU_MIN =0x00008000,   //tensão  linha        em pu mínima
               //atributos de fluxo: tensões de linha em kV
               bar_VFF_KV     =0x00010000,   //tensões linha        em kV nas 3 fases
               bar_VFF_KV_MAX =0x00020000,   //tensão  linha        em kV máxima
               bar_VFF_KV_MIN =0x00040000,   //tensão  linha        em kV mínima
               //atributos de fluxo: tensões fase/terra em pu
               bar_VFT_PU     =0x00080000,   //tensões fase/terra   em pu nas 3 fases
               bar_VFT_PU_MAX =0x00100000,   //tensão  fase/terra   em pu máxima
               bar_VFT_PU_MIN =0x00200000,   //tensão  fase/terra   em pu mínima
               //atributos de fluxo: tensões fase/terra em kV
               bar_VFT_KV     =0x00400000,   //tensões fase/terra   em kV nas 3 fases
               bar_VFT_KV_MAX =0x00800000,   //tensão  fase/terra   em kV máxima
               bar_VFT_KV_MIN =0x01000000,   //tensão  fase/terra   em kV mínima
               //
               //Eqbar--------------------------------
               //atributos de topologia
               eqb_BARRA      =0x00000010,
               //CAPACITOR----------------------------
               //atributos de topologia
               cap_Q          =0x00000020,
               cap_LIGA       =0x00000040,
               //CARGA--------------------------------
               //atributos de topologia
               car_P          =0x00000020,
               car_Q          =0x00000040,
               car_S          =0x00000080,
               car_ENERGIA    =0x00000100,
               car_FASES      =0x00000200,
               car_MODELO     =0x00000400,
               car_CLASSE     =0x00000800,
               car_NUMCONS    =0x00001000,
               //atributos de fluxo
               car_DRP_DRC    =0x00002000,
               //GERADOR------------------------------
               //atributos de topologia
               ger_TIPO       =0x00000020,
               ger_PQVF       =0x00000040,
               ger_SNOM       =0x00000080,
               ger_LIGACAO    =0x00000100,
               ger_Z_SEQ      =0x00000200,   //Z0 e Z1 em pu da Ssnom
               //atributos de fluxo
               ger_PIE        =0x00000400,   //carregamento(gráfico)
               ger_CARGTO     =0x00000800,   //carregamento(%)
               ger_S          =0x00001000,   //pot.aparente total (MVA)
               ger_FP         =0x00002000,   //fator de potência
               ger_S_ABC      =0x00004000,   //pot.aparente por fase (MVA)
               ger_FP_ABC     =0x00008000,   //fator de potência por fase
               ger_S_MAX      =0x00010000,   //pot.aparente máxima (MVA)
               ger_S_MIN      =0x00020000,   //pot.aparente mínima (MVA)
               ger_P          =0x00040000,   //pot.ativa total (MW)
               ger_P_ABC      =0x00080000,   //pot.ativa nas 3 fases (MW)
               ger_P_MAX      =0x00100000,   //pot.ativa máxima (MW)
               ger_P_MIN      =0x00200000,   //pot.ativa mínima (MW)
               ger_Q          =0x00400000,   //pot.reativa total (MVAr)
               ger_Q_ABC      =0x00800000,   //pot.reativa nas 3 fases (MVAr)
               ger_Q_MAX      =0x01000000,   //pot.reativa máxima
               ger_Q_MIN      =0x02000000,   //pot.reativa mínima
               ger_I_ABC      =0x04000000,   //corrente nas 3 fases (A)
               ger_I_MAX      =0x08000000,   //corrente máxima
               ger_I_MIN      =0x10000000,   //corrente mínima
               //MEDIDOR-----------------------------
               //atributos de topologia
               med_CANAL      =0x00000020,
               //CANAL DE MEDIDOR---------------------
               //atributos de topologia
               med_P         =0x00000020,
               med_Q         =0x00000040,
               med_V         =0x00000080,
               med_I         =0x00000100,
               med_FP        =0x00000200,
               //REATOR-------------------------------
               //atributos de topologia
               reat_Q          =0x00000020,
               reat_LIGA       =0x00000040,
               //SUPRIMENTO---------------------------
               //atributos de topologia
               sup_TIPO       =0x00000020,
               sup_PQVF       =0x00000040,
               sup_SMAX       =0x00000080,   //Capacidade máxima (MVA)
               sup_Z_EQV      =0x00000100,   //Impedância equivalente(pu) ou Pot.Curto (MVA)
               //atributos de fluxo
               sup_PIE        =0x00000200,   //carregamento(gráfico)
               sup_CARGTO     =0x00000400,   //carregamento(%)
               sup_S          =0x00000800,   //pot.aparente total (MVA)
               sup_FP         =0x00001000,   //fator de potência
               sup_S_ABC      =0x00002000,   //pot.aparente nas 3 fases (MVA)
               sup_FP_ABC     =0x00004000,   //fator de potência por fase
               sup_S_MAX      =0x00008000,   //pot.aparente máxima (MVA)
               sup_S_MIN      =0x00010000,   //pot.aparente mínima (MVA)
               sup_P          =0x00020000,   //pot.ativa total (MW)
               sup_P_ABC      =0x00040000,   //pot.ativa nas 3 fases (MW)
               sup_P_MAX      =0x00080000,   //pot.ativa máxima (MW)
               sup_P_MIN      =0x00100000,   //pot.ativa mínima (MW)
               sup_Q          =0x00200000,   //pot.reativa total (MVAr)
               sup_Q_ABC      =0x00400000,   //pot.reativa nas 3 fases (MVAr)
               sup_Q_MAX      =0x00800000,   //pot.reativa máxima
               sup_Q_MIN      =0x01000000,   //pot.reativa mínima
               sup_I_ABC      =0x02000000,   //corrente nas 3 fases (A)
               sup_I_MAX      =0x04000000,   //corrente máxima
               sup_I_MIN      =0x08000000,   //corrente mínima
               //YREF----------------------------------
               //atributos de topologia
               yref_Y_PU100  =0x00000020,
               yref_Y_SIEMEN =0x00000040,
               yref_S_MVA    =0x00000080,
               //
               //Ligacao-----------------------------
               //atributos de topologia
               lig_REDE       =0x00000010,
               lig_BARRA      =0x00000020,
               //atributos de fluxo
               lig_PIE        =0x00000040,   //carregamento(gráfico)
               lig_CARGTO     =0x00000080,   //carregamento(%)
               lig_S          =0x00000100,   //pot.aparente pri/sec total (MVA)
               lig_S_ABC      =0x00000200,   //pot.aparente nas 3 fases pri/sec (MVA)
               lig_S_MAX      =0x00000400,   //pot.aparente máxima pri/sec (MVA)
             //lig_S_MIN                     //pot.aparente mínima pri/sec (MVA)
               lig_FP         =0x00000800,   //fator de potência
               lig_FP_ABC     =0x00001000,   //fator de potência
               lig_P          =0x00002000,   //pot.ativa total pri/sec (MW)
               lig_P_ABC      =0x00004000,   //pot.ativa nas 3 fases pri/sec (MW)
               lig_P_MAX      =0x00008000,   //pot.ativa máxima pri/sec (MW)
             //lig_P_MIN                     //pot.ativa mínima pri/sec (MW)
               lig_Q          =0x00010000,   //pot.reativa total pri/sec (MVAr)
               lig_Q_ABC      =0x00020000,   //pot.reativa nas 3 fases pri/sec (MVAr)
               lig_Q_MAX      =0x00040000,   //pot.reativa máxima pri/sec
             //lig_Q_MIN                     //pot.reativa mínima pri/sec
               lig_I_ABC      =0x00080000,   //corrente nas 3 fases pri/sec (A)
               lig_I_MAX      =0x00100000,   //corrente máxima pri/sec
             //lig_I_MIN                     //corrente mínima pri/sec
               lig_PERDA      =0x00200000,   //perda de demanda(MW)
               lig_IND_BAL    =0x00400000,   //Índice de balanceamento
             //               =0x00800000,   //não usado
             //               =0x01000000,   //não usado
             //               =0x02000000,   //não usado
               //CAPSERIE-----------------------------
               //atributos de topologia
               caps_SNOM      =0x04000000,   //potência nominal
               caps_X         =0x08000000,   //reatância (ohm)
               //CHAVE--------------------------------
               //atributos de topologia
               chv_TIPO       =0x04000000,
               chv_INOM       =0x08000000,
               chv_ESTADO     =0x10000000,
               //REGULADOR----------------------------
               //atributos de topologia
               reg_SNOM       =0x04000000,   //potência nominal (kVA)
               reg_LIGACAO    =0x08000000,   //tipo ligação
               reg_Z_SEQ      =0x10000000,   //Z0 e Z1 (pu)
               //TRAFO--------------------------------
               //atributos de topologia
               trf_SNOM       =0x04000000,   //potência nominal (kVA)
               trf_VNOM       =0x08000000,   //Vnom pri/sec (kV)
               trf_LIGACAO    =0x10000000,   //tipo ligação pri/sec
               trf_Z_SEQ      =0x20000000,   //Z0 e Z1 (pu)
               trf_ZAT        =0x40000000,   //Impedância de aterramento pri/sec (ohm)
               trf_PERDA_FE   =0x80000000,   //Perda no ferro (%)
               //TRAFO3E------------------------------
               //atributos de topologia
               trf3_SNOM       =0x04000000,   //Snom pri/sec/ter (MVA)
               trf3_VNOM       =0x08000000,   //Vnom pri/sec/ter (kV)
               trf3_LIGACAO    =0x10000000,   //tipo ligação pri/sec/ter
               trf3_Z_SEQ      =0x20000000,   //zps0/zpt0/zst0 e zps1/zpt1/zst1 (pu)
               trf3_ZAT        =0x40000000,   //Impedância de aterramento pri/sec/ter (ohm)
               trf3_PERDA_FE   =0x80000000,   //Perda no ferro (%)
               //TRECHO--------------------------------
               //atributos de topologia
               tre_COMP       =0x04000000,   //comprimento (km)
               tre_ARRANJO    =0x08000000,   //código do arranjo
               tre_TIPO       =0x10000000,   //monofásico, bifásico, trifásico
               tre_FASES      =0x20000000,   //fases do arranjo do trecho
               tre_ZSEQ1      =0x40000000,   //Zseq1 d0 trecho (OBS: pode não existir)
               //
               //MUTUAS-------------------------------
               //atributos de topologia
               mut_TRECHO     =0x04000000,   //trechos da mútua
               mut_Z0         =0x08000000,   //impedância de seq.0
               mut_Z1         =0x10000000,   //impedância de seq.1
               //
               //REDUCOES-----------------------------
               //atributos de fluxo
               rdu_S          =0x00000010,   //pot.aparente total (MVA)
               rdu_S_ABC      =0x00000020,   //pot.aparente nas 3 fases (MVA)
               rdu_S_MAX      =0x00000040,   //pot.aparente máxima (MVA)
               rdu_S_MIN      =0x00000080,   //pot.aparente mínima (MVA)
               rdu_P          =0x00000100,   //pot.ativa total (MW)
               rdu_P_ABC      =0x00000200,   //pot.ativa nas 3 fases (MW)
               rdu_P_MAX      =0x00000400,   //pot.ativa máxima (MW)
               rdu_P_MIN      =0x00000800,   //pot.ativa mínima (MW)
               rdu_Q          =0x00001000,   //pot.reativa total (MVAr)
               rdu_Q_ABC      =0x00002000,   //pot.reativa nas 3 fases (MVAr)
               rdu_Q_MAX      =0x00004000,   //pot.reativa máxima
               rdu_Q_MIN      =0x00008000,   //pot.reativa mínima
               rdu_I_ABC      =0x00010000,   //corrente nas 3 fases (A)
               rdu_I_MAX      =0x00020000,   //corrente máxima
               rdu_I_MIN      =0x00040000,   //corrente mínima
               rdu_EQPTOS     =0x00080000
               };
/*
//---------------------------------------------------------------------------
enum tipoCHAVE  {//tipos de Chave
                 chvDISJUNTOR=1,
                 chvRELIGADORA,
                 chvFUSIVEL,
                 chvFACA};
*/
//---------------------------------------------------------------------------
enum tipoBARRA {//tipos de barras
               barraSWING=0,
               barraPV,
               barraCARGA,
               barraPQ};

//---------------------------------------------------------------------------
enum modCARGA    {//modelos de Carga
                  cargaAUTO=-1,
                  cargaICTE=1,
                  cargaSCTE,
                  cargaZCTE};

//---------------------------------------------------------------------------
enum tipoCARGA    {//tipos de Carga
                  cargaMONOFASICA=1,
                  cargaBIFASICA,
                  cargaTRIFASICA};

//---------------------------------------------------------------------------
enum tipoSUPRIMENTO  {//tipos de Suprimento
                     supPV=1,
                     supPQ,
                     supVF};

//---------------------------------------------------------------------------
enum enumLIGTRF {//tipos de ligação p/ Trafos e Geradores
                lgEST_AT=0,
                lgTRIANG,
                lgEST_ISO,
                lgMONOFASICO,
                lgDELTA_ABERTO,
                lgDELTA_FECHADO
                };
/*
//---------------------------------------------------------------------------
enum enumLIGTRF {//tipos de ligação p/ Trafos e Geradores
                lgEST_AT=0,
                lgTRIANG,
                lgEST_ISO,
                lgMONO_AT,
                lgMONO_ISO};
*/

//---------------------------------------------------------------------------
class VTObra;
class VTCurva;

//---------------------------------------------------------------------------
class VTEqpto : public TObject
   {
   public:  //property
      __property AnsiString Codigo                    = {read=PM_GetCodigo,          write=PM_SetCodigo};
      __property AnsiString Extern_id                 = {read=PM_GetExtern_id,       write=PM_SetExtern_id};
      __property AnsiString Padrao                    = {read=PM_GetPadrao,          write=PM_SetPadrao};
      __property int        Id                        = {read=PM_GetId,              write=PM_SetId};
      __property unsigned   ShowAsUnsigned            = {read=PM_GetShowAsUnsigned,  write=PM_SetShowAsUnsigned};
      __property bool       Show[unsigned mask_bit]   = {read=PM_GetShow,            write=PM_SetShow};
      __property unsigned   StatusAsUnsigned          = {read=PM_GetStatusAsUnsigned,write=PM_SetStatusAsUnsigned};
      __property bool       Status[unsigned mask_bit] = {read=PM_GetStatus,          write=PM_SetStatus};
      __property TObject*   Obj                       = {read=PM_GetObj,             write=PM_SetObj};
      __property VTObra*    Obra                      = {read=PM_GetObra,            write=PM_SetObra};

   public:
                         __fastcall VTEqpto(void);
      virtual            __fastcall ~VTEqpto(void);
      virtual VTEqpto*   __fastcall Clone(void);
      virtual void       __fastcall CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
      virtual void       __fastcall CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
                                                           int &x2, int &y2, int &x3, int &y3);
      virtual bool       __fastcall CoordenadasUtm_cm(int &x, int &y);
      virtual bool       __fastcall CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
      virtual bool       __fastcall CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
                                                      int &x2, int &y2, int &x3, int &y3);
      virtual bool       __fastcall CoordenadasUtm_m(int &x, int &y);
      virtual bool       __fastcall CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2);
      virtual bool       __fastcall CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
                                                     int &x2, int &y2, int &x3, int &y3);
      virtual void       __fastcall DefineStatus(unsigned mask_bit, bool enabled);
      virtual VTCurva*   __fastcall ExisteCurva(void);
      virtual VTEqpto&   __fastcall operator<<(VTEqpto &eqpto);
      virtual bool       __fastcall StatusAlterado(void);
      virtual bool       __fastcall StatusDeslocado(void);
      virtual bool       __fastcall StatusExcluido(void);
      virtual bool       __fastcall StatusNovo(void);
      virtual bool       __fastcall TipoBarra(void);
      virtual bool       __fastcall TipoEqbar(void);
      virtual bool       __fastcall TipoLigacao(void);
      virtual int        __fastcall Tipo(void)=0;
      virtual AnsiString __fastcall TipoAsString(void)=0;
      virtual bool       __fastcall Visible(void);

   public:  //método da classe
      static VTEqpto*   __fastcall TipoEqpto(TObject *obj);
      static AnsiString __fastcall TipoEqpto_AsString(int tipo);
      static int        __fastcall TipoEqpto_BarraEqbarLigacao(int tipo);


   protected: //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodigo(void);
      virtual AnsiString __fastcall PM_GetExtern_id(void);
      virtual int        __fastcall PM_GetId(void);
      virtual TObject*   __fastcall PM_GetObj(void);
      virtual VTObra*    __fastcall PM_GetObra(void);
      virtual AnsiString __fastcall PM_GetPadrao(void);
      virtual bool       __fastcall PM_GetShow(unsigned bit);
      virtual unsigned   __fastcall PM_GetShowAsUnsigned(void);
      virtual bool       __fastcall PM_GetStatus(unsigned bit);
      virtual unsigned   __fastcall PM_GetStatusAsUnsigned(void);
      virtual void       __fastcall PM_SetCodigo(AnsiString codigo);
      virtual void       __fastcall PM_SetExtern_id(AnsiString extern_id);
      virtual void       __fastcall PM_SetId(int id);
      virtual void       __fastcall PM_SetObj(TObject* obj);
      virtual void       __fastcall PM_SetObra(VTObra *obra);
      virtual void       __fastcall PM_SetPadrao(AnsiString padrao);
      virtual void       __fastcall PM_SetShow(unsigned mask_bit, bool enabled);
      virtual void       __fastcall PM_SetShowAsUnsigned(unsigned show);
      virtual void       __fastcall PM_SetStatus(unsigned mask_bit, bool enabled);
      virtual void       __fastcall PM_SetStatusAsUnsigned(unsigned status);

   protected: //dados acessados via property
      struct   {
               int        id;
               unsigned   status;  //ver enum eqptoSTATUS
               unsigned   show;    //ver enum eqptoSHOW
               AnsiString codigo;
               AnsiString extern_id;
               TObject    *obj;  //ponteiro auxiliar
               VTObra     *obra;
               } PD;
   };

//---------------------------------------------------------------------------
#endif
//eof


 