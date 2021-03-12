//---------------------------------------------------------------------------
#ifndef VTTipoChaveH
#define VTTipoChaveH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum chaveTIPO {//chaves básicas SINAP
               chaveDEFINIDA_USUARIO=-1,
               chaveDJ=1,   chaveDISJUNTOR   =chaveDJ,
               chaveRA=2,   chaveRELIGADORA  =chaveRA,
               chaveBF=3,   chaveBASE_FUSIVEL=chaveBF,
               chaveSEC=4,  chaveSECCIONADORA=chaveSEC,
               chaveFACA=5,
               chaveINDEFINIDA=999,
               //chaves Eletropaulo/Subterrâneo
               chaveDESCONECTAVEL=1000,
               chaveNETWORK_PROTECTOR
               };

//---------------------------------------------------------------------------
enum chaveMODO_OPERACAO {chaveOP_INDEFINIDA=-1, chaveOP_MANUAL=1, chaveOP_TELECOMANDADA, chaveOP_AUTOMATICA};

//---------------------------------------------------------------------------
class VTTipoChave : public TObject
   {
   public:  //property
      __property int        Id                   = {read=PM_GetId,              write=PM_SetId};
      __property int        Tipo                 = {read=PM_GetTipo,            write=PM_SetTipo};
      __property AnsiString Codigo               = {read=PM_GetCodigo,          write=PM_SetCodigo};
      __property AnsiString Extern_id            = {read=PM_GetExtern_id,       write=PM_SetExtern_id};
      __property int        Operacao             = {read=PM_GetOperacao,        write=PM_SetOperacao};
      __property bool       Status[unsigned bit] = {read=PM_GetStatus,          write=PM_SetStatus};
      __property unsigned   StatusAsUnsigned     = {read=PM_GetStatusAsUnsigned,write=PM_SetStatusAsUnsigned};
      __property TColor     Color                = {read=PM_GetColor,           write=PM_SetColor};
      __property TObject*   Obj                  = {read=PM_GetObj,             write=PM_SetObj};

   public:
                           __fastcall  VTTipoChave(void) {};
      virtual              __fastcall ~VTTipoChave(void) {};
      virtual VTTipoChave* __fastcall  Clone(void)= 0 ;
      virtual void         __fastcall  CopiaAtributosDe(VTTipoChave &tipochave) = 0;

   protected: //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
      virtual TColor     __fastcall PM_GetColor(void) = 0;
      virtual AnsiString __fastcall PM_GetExtern_id(void) = 0;
      virtual int        __fastcall PM_GetId(void) = 0;
      virtual TObject*   __fastcall PM_GetObj(void) = 0;
      virtual bool       __fastcall PM_GetStatus(unsigned bit) = 0 ;
      virtual unsigned   __fastcall PM_GetStatusAsUnsigned(void) = 0;
      virtual int        __fastcall PM_GetOperacao(void) = 0;
      virtual int        __fastcall PM_GetTipo(void) = 0;
      virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
      virtual void       __fastcall PM_SetColor(TColor color) = 0;
      virtual void       __fastcall PM_SetExtern_id(AnsiString extern_id) = 0;
      virtual void       __fastcall PM_SetId(int id) = 0;
      virtual void       __fastcall PM_SetObj(TObject* obj) = 0;
      virtual void       __fastcall PM_SetOperacao(int modo) = 0;
      virtual void       __fastcall PM_SetStatus(unsigned bit, bool enabled) = 0;
      virtual void       __fastcall PM_SetStatusAsUnsigned(unsigned status) = 0;
      virtual void       __fastcall PM_SetTipo(int tipo) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTipoChave
//---------------------------------------------------------------------------
VTTipoChave* __fastcall NewObjTipoChave(void);

//---------------------------------------------------------------------------
#endif
//eof


