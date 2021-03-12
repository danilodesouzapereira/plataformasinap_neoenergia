//---------------------------------------------------------------------------
#ifndef TSubestacaoH
#define TSubestacaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSubestacao.h"


//---------------------------------------------------------------------------
class VTLigacao;

//---------------------------------------------------------------------------
class TSubestacao : public VTSubestacao
   {
   private: //dados acessados via property
      struct   {
               VTRede *rede;
					} PD;

   private: //objetos externos
		VTApl *apl;

   private: //dados locais
      TList  *lisEQP;
      TList  *lisPRIMARIO;
      TList  *lisRETICULADO;

   public:  //métodos
                  __fastcall  TSubestacao(VTApl *apl_owner);
                  __fastcall ~TSubestacao(void);
      VTPrimario* __fastcall  ExistePrimario(VTRede *rede);
      void        __fastcall  InserePrimario(VTPrimario *primario);
      void        __fastcall  InsereReticulado(VTReticulado *reticulado);
      void        __fastcall  LisEqptoSubPriSec(TList *lisEXT, int tipo_eqpto, bool barra_isolada=true);
      TList*      __fastcall  LisPrimario(void);
		int         __fastcall  LisPrimario(VTTrafo *trafo, TList *lisEXT);
		int         __fastcall  LisPrimario(VTTrafo3E *trafo3E, TList *lisEXT);
		void        __fastcall  LisRede(TList *lisEXT, int segmento);
      TList*      __fastcall  LisReticulado(void);
      VTPatamar*  __fastcall  PatamarDemandaMaxima(complex<double>&s_mva);
      VTPatamar*  __fastcall  PatamarDemandaMinima(complex<double>&s_mva);

	protected:  //métodos acessados via property
		VTRede* __fastcall PM_GetRede(void);
		double  __fastcall PM_GetSfirme_mva(void);
		double  __fastcall PM_GetSnom_mva(void);
		void    __fastcall PM_SetRede(VTRede *rede);

	private:  //métodos
		int __fastcall LisPrimarioSupridoPor(VTLigacao *lig_ref, TList *lisEXT);
	};

//---------------------------------------------------------------------------
#endif
//eof
