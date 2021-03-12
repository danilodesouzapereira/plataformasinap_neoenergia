//---------------------------------------------------------------------------
#ifndef VTMsgH
#define VTMsgH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTProtocolo.h"

//---------------------------------------------------------------------------
class TMsgAck;
class TMsgNAck;
class TMsgAlteraCapacitor;
class TMsgAlteraTrafo;
class TMsgEliminaFalta;
class TMsgExisteEqpto;
class TMsgManobraChave;
class TMsgRegistroBarra;
class TMsgRegistroCapacitor;
class TMsgRegistroChave;
class TMsgRegistroTrafo;
class TMsgRegistroTrecho;
class TMsgSimulaFalta;
class TMsgSolicitaRegistro;
class TMsgSupervisaoAuto;

//---------------------------------------------------------------------------
class VTMsg : public TObject
   {
   public: //property
      __property char  Funcao    = {read=PM_GetFuncao};
      __property short NumBytes  = {read=PM_GetNumBytes};
      __property char  EqptoTipo = {read=PM_GetEqptoTipo, write=PM_SetEqptoTipo};
      __property int   EqptoId   = {read=PM_GetEqptoId,   write=PM_SetEqptoId};

   public:
                         __fastcall  VTMsg(void) {};
                         __fastcall ~VTMsg(void) {};
      virtual bool       __fastcall  ConverteBufToMsg(strBUF *buf, int index) = 0;
      virtual strBUF*    __fastcall  ConverteMsgToBuf(void) = 0;
      virtual bool       __fastcall  HoraMinutoSegundo(strHMS &hms) {return(false);}
      virtual AnsiString __fastcall  MsgAsString(void) = 0;

   public:  //dados
      //char  funcao;
      //short num_bytes;
      //char  hora;
      //char  minuto;
      //char  segundo;
      //char  eqpto_tipo;
      //int   eqpto_id;

   protected:  //métodos acessados via property
      virtual int   __fastcall PM_GetEqptoId(void) = 0;
      virtual char  __fastcall PM_GetEqptoTipo(void) = 0;
      virtual char  __fastcall PM_GetFuncao(void)   = 0;
      virtual short __fastcall PM_GetNumBytes(void) = 0;
      //set
      virtual void  __fastcall PM_SetEqptoId(int eqpto_id) = 0;
      virtual void  __fastcall PM_SetEqptoTipo(char eqpto_tipo) = 0;

   protected:  //métodos
      virtual short __fastcall Length(void) = 0;

   protected:  //dados locais
      strBUF buffer;
   };

//---------------------------------------------------------------------------
TMsgAck*               __fastcall NewMsgAck(void);
TMsgNAck*              __fastcall NewMsgNAck(void);
TMsgAlteraCapacitor*   __fastcall NewMsgAlteraCapacitor(void);
TMsgAlteraTrafo*       __fastcall NewMsgAlteraTrafo(void);
TMsgEliminaFalta*      __fastcall NewMsgEliminaFalta(void);
TMsgExisteEqpto*       __fastcall NewMsgExisteEqpto(void);
TMsgManobraChave*      __fastcall NewMsgManobraChave(void);
TMsgRegistroBarra*     __fastcall NewMsgRegistroBarra(void);
TMsgRegistroCapacitor* __fastcall NewMsgRegistroCapacitor(void);
TMsgRegistroChave*     __fastcall NewMsgRegistroChave(void);
TMsgRegistroTrafo*     __fastcall NewMsgRegistroTrafo(void);
TMsgRegistroTrecho*    __fastcall NewMsgRegistroTrecho(void);
TMsgSimulaFalta*       __fastcall NewMsgSimulaFalta(void);
TMsgSolicitaRegistro*  __fastcall NewMsgSolicitaRegistro(void);
TMsgSupervisaoAuto*    __fastcall NewMsgSupervisaoAuto(void);

//---------------------------------------------------------------------------
#endif
//eof

