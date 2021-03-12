//---------------------------------------------------------------------------
#ifndef TSuporteH
#define TSuporteH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTSuporte.h"

//---------------------------------------------------------------------------
class TSuporte : public VTSuporte
   {
   protected: //dados acessados via property
      struct   {
               int    tiposuporte;  //ver enum
               float  vmin;         //tens�o m�nima em que � utilizado
               float  vmax;         //tens�o m�xima rede  em que � utilizado
               } PD;

   public:
                    __fastcall TSuporte(void);
                    __fastcall ~TSuporte(void);
      VTSuporte*    __fastcall Clone(void);
		void          __fastcall CopiaAtributosDe(VTEqpto &ref);
		VTPtoFixacao* __fastcall ExistePtoFixacao(AnsiString codigo);
		VTPtoFixacao* __fastcall ExistePtoFixacao(int id);
		VTPtoFixacao* __fastcall ExistePtoFixacaoIndex(int index);
		bool          __fastcall InserePtoFixacao(VTPtoFixacao *pto_fixacao);
		TList*        __fastcall LisPtoFixacao(void);
		int           __fastcall Tipo(void);
      AnsiString    __fastcall TipoAsString(void);
      AnsiString    __fastcall TipoDeSuporteAsString(void);

   protected: //m�todos das classe base redefinidos
      void      __fastcall PM_SetCodigo(AnsiString codigo);

   protected: //m�todos acessados via property
      int      __fastcall PM_GetTipoDeSuporte(void);
      double   __fastcall PM_GetVmax_KV(void);
      double   __fastcall PM_GetVmin_KV(void);
      void     __fastcall PM_SetTipoDeSuporte(int tipo);
      void     __fastcall PM_SetVmax_KV(double vmax);
      void     __fastcall PM_SetVmin_KV(double vmin);

   private:  //m�todos
      //int __fastcall OrdenaPtoFixacao(void *pf1, void *pf2);

   private:  //dados locais
      TList *lisPTO;
   };

//---------------------------------------------------------------------------
#endif
//eof


 