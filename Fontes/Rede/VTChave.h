//---------------------------------------------------------------------------
#ifndef VTChaveH
#define VTChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TLigacao.h"

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
#define chvABERTA      0x00
#define chvFECHADA     0x01
#define chvBLOQUEADA   0x02

//---------------------------------------------------------------------------
//tecnologia de comunicação
enum tecnoCOM {comNAO_DISPONIVEL = 0, comFIBRA_OTICA = 1, comCELULAR, comRADIO,
               comMICOROONDAS, comSATELITE, comDESCONHECIDO};

//---------------------------------------------------------------------------
class VTTipoChave;

//---------------------------------------------------------------------------
class VTChave : public TLigacao
   {
   public:  //property
      __property bool         Aberta            = {read=PM_GetAberta};
      __property bool         Fechada           = {read=PM_GetFechada};
      __property bool         Bloqueada         = {read=PM_GetBloqueada,         write=PM_SetBloqueada};
      __property unsigned     Estado            = {read=PM_GetEstado,            write=PM_SetEstado};
      __property AnsiString   EloFusivel        = {read=PM_GetEloFusivel,        write=PM_SetEloFusivel};
      __property unsigned     EstadoNormal      = {read=PM_GetEstadoNormal,      write=PM_SetEstadoNormal};
      __property double       Inom_amp          = {read=PM_GetInom_amp,          write=PM_SetInom_amp};
      __property VTTipoChave* TipoChave         = {read=PM_GetTipoChave,         write=PM_SetTipoChave};
      __property bool         TipoBaseFusivel   = {read=PM_GetTipoBaseFusivel};
      __property bool         TipoDisjuntor     = {read=PM_GetTipoDisjuntor};
      __property AnsiString   TipoGIS           = {read=PM_GetTipoGIS,           write=PM_SetTipoGIS};
      __property bool         TipoReligadora    = {read=PM_GetTipoReligadora};
      __property bool         TipoSeccionadora  = {read=PM_GetTipoSeccionadora};
      __property bool         ProtecaoAtiva     = {read=PM_GetProtecaoAtiva,     write=PM_SetProtecaoAtiva};
      __property bool         Telecomandada     = {read=PM_GetTelecomandada,     write=PM_SetTelecomandada};
      __property int          TecnoComunicacao  = {read=PM_GetTecnoComunicacao,  write=PM_SetTecnoComunicacao};
      __property double       CapDisruptiva_amp = {read=PM_GetCapDisruptiva_amp, write=PM_SetCapDisruptiva_amp};

   public:
                       __fastcall  VTChave(void) {};
      virtual          __fastcall ~VTChave(void) {};
      virtual VTChave* __fastcall  Clone(void)=0;
      virtual void     __fastcall  Abre(void)=0;
      virtual void     __fastcall  Fecha(void)=0;
      virtual void     __fastcall  MudaEstado(void)=0;

   protected: //métodos acessados via property
      virtual bool         __fastcall PM_GetAberta(void)=0;
      virtual bool         __fastcall PM_GetBloqueada(void)=0;
      virtual double       __fastcall PM_GetCapDisruptiva_amp(void)=0;
      virtual AnsiString   __fastcall PM_GetEloFusivel(void) = 0;
      virtual unsigned     __fastcall PM_GetEstado(void)=0;
      virtual unsigned     __fastcall PM_GetEstadoNormal(void)=0;
      virtual bool         __fastcall PM_GetFechada(void)=0;
      virtual double       __fastcall PM_GetInom_amp(void)=0;
      virtual bool         __fastcall PM_GetProtecaoAtiva(void)=0;
      virtual int          __fastcall PM_GetTecnoComunicacao(void)=0;
      virtual bool         __fastcall PM_GetTelecomandada(void)=0;
      virtual bool         __fastcall PM_GetTipoBaseFusivel(void)=0;
      virtual VTTipoChave* __fastcall PM_GetTipoChave(void)=0;
      virtual bool         __fastcall PM_GetTipoDisjuntor(void)=0;
      virtual AnsiString   __fastcall PM_GetTipoGIS(void)=0;
      virtual bool         __fastcall PM_GetTipoReligadora(void)=0;
      virtual bool         __fastcall PM_GetTipoSeccionadora(void)=0;
      virtual void         __fastcall PM_SetBloqueada(bool bloqueada)=0;
      virtual void         __fastcall PM_SetCapDisruptiva_amp(double i_amp)=0;
      virtual void         __fastcall PM_SetEloFusivel(AnsiString elo) = 0;
      virtual void         __fastcall PM_SetEstado(unsigned estado)=0;
      virtual void         __fastcall PM_SetEstadoNormal(unsigned estado)=0;
      virtual void         __fastcall PM_SetInom_amp(double i_amp)=0;
      virtual void         __fastcall PM_SetProtecaoAtiva(bool protecao_ativa)=0;
      virtual void         __fastcall PM_SetTecnoComunicacao(int tecno_comunicacao)=0;
      virtual void         __fastcall PM_SetTelecomandada(bool telecomandada)=0;
      virtual void         __fastcall PM_SetTipoChave(VTTipoChave *tipochave)=0;
      virtual void         __fastcall PM_SetTipoGIS(AnsiString tipo_gis)=0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTChave
//---------------------------------------------------------------------------
VTChave* __fastcall NewObjChave(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

