//---------------------------------------------------------------------------
#ifndef VTTipoRedeH
#define VTTipoRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
//Modulo 1 do prodist
#define REDESEC_V_LIM_SUP_KV 1.0   // FKM menor e igual
#define REDEPRI_V_LIM_SUP_KV 69.0  // FKM menor
#define REDESUB_V_LIM_SUP_KV 230.0 // FKM menor
//---------------------------------------------------------------------------
enum redeSEGMENTO
{
	redeETT=1,
	redeSUB,
	redeETD,
	redePRI,
	redeSEC,
	redeTRANS,
	redeINDEF,
	redeDEFINIDA_USUARIO,
	redeCOUNT//FKM 2017.08.09
};
/*
//---------------------------------------------------------------------------
enum redeTIPO  {//constantes p/ TipoRede
               redeDIST_PRI,
               redeDIST_SEC,
               redeDIST_MINIRETICULADO,
               redeTRAN_ET_TRANS_SUBTRANS,
               redeET_MTBT,
               redeETT,
               redeIP,
               redeHIBRIDA,
               redeSPOT,
               redeSUBTRANSMISSAO,
               redeTRANSMISSAO,
               redeE
               }
*/
//---------------------------------------------------------------------------
class VTTipoRede : public TObject
   {
   public:  //property
      __property int        Id                   = {read=PM_GetId,              write=PM_SetId};
      __property AnsiString Codigo               = {read=PD.codigo,             write=PD.codigo};
      __property AnsiString Descricao            = {read=PD.descricao,          write=PD.descricao};
      __property AnsiString Extern_id            = {read=PD.extern_id,          write=PD.extern_id};
      __property TColor     Color                = {read=PM_GetColor,           write=PM_SetColor};
      __property int        Segmento             = {read=PM_GetSegmento,        write=PM_SetSegmento};
      __property bool       Status[unsigned bit] = {read=PM_GetStatus,          write=PM_SetStatus};
      __property unsigned   StatusAsUnsigned     = {read=PM_GetStatusAsUnsigned,write=PM_SetStatusAsUnsigned};
      __property double     Vmax_kv              = {read=PD.vmax_kv,            write=PD.vmax_kv};
      __property double     Vmin_kv              = {read=PD.vmin_kv,            write=PD.vmin_kv};
      //usada p/ receber o número de redes desse tipo existente na base de dados
      __property int        ValorAux             = {read=PD.valor_aux,          write=PD.valor_aux};
      __property TObject*   Obj                  = {read=PD.obj,                write=PD.obj};

   public:
                          __fastcall  VTTipoRede(void) {};
      virtual             __fastcall ~VTTipoRede(void) {};
      virtual VTTipoRede* __fastcall  Clone(void) = 0;
      virtual void        __fastcall  CopiaAtributosDe(VTTipoRede &tiporede) = 0;

   protected: //métodos acessados via property
      virtual TColor   __fastcall PM_GetColor(void) = 0;
      virtual int      __fastcall PM_GetId(void) = 0;
      virtual int      __fastcall PM_GetSegmento(void) = 0;
      virtual bool     __fastcall PM_GetStatus(unsigned bit) = 0;
      virtual unsigned __fastcall PM_GetStatusAsUnsigned(void) = 0;
      virtual void     __fastcall PM_SetColor(TColor color) = 0;
      virtual void     __fastcall PM_SetId(int tipo_id) = 0;
      virtual void     __fastcall PM_SetSegmento(int seg_id) = 0;
      virtual void     __fastcall PM_SetStatus(unsigned bit, bool enabled) = 0;
      virtual void     __fastcall PM_SetStatusAsUnsigned(unsigned status) = 0;

   protected: //dados acessados via property
      struct   {
               int        id;
               int        seg_id;
               unsigned   status;
               TColor     color;
               AnsiString codigo;
               AnsiString descricao;
               AnsiString extern_id;
               double     vmax_kv;
               double     vmin_kv;
               int        valor_aux;
               TObject    *obj;           //ponteiro auxiliar
               } PD;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTipoRede
//---------------------------------------------------------------------------
VTTipoRede* __fastcall NewObjTipoRede(void);

//---------------------------------------------------------------------------
#endif
//eof


