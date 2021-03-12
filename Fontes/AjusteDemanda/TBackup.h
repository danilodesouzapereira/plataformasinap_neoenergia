//---------------------------------------------------------------------------
#ifndef TBackupH
#define TBackupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include "VTBackup.h"
//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTPrimario;

//---------------------------------------------------------------------------
class TPricarga : public TObject
   {
   public:
           __fastcall  TPricarga(VTPrimario *primario);
           __fastcall ~TPricarga(void);

   public:  //dados locais
      VTPrimario *primario;
	  TList      *lisCARGA;
	  bool       cargas_originais;
   };

//---------------------------------------------------------------------------
class TBackup : public VTBackup
   {
   public:
           __fastcall  TBackup(VTApl *apl);
			  __fastcall ~TBackup(void);
      void __fastcall  Clear();
      bool __fastcall  DuplicaCargas(TList *lisCARGA);
		bool __fastcall  RestauraCargaMedicao(TList *lisMED_ERRO);
		bool __fastcall  RestauraCargaTodosPrimarios(void);
      bool __fastcall  RestauraCargaUmPrimario(VTPrimario *primario);
      bool __fastcall  RestauraCurvaCargaAtual(TList *lisCARGA);
      bool __fastcall  SalvaCargaUmPrimario(VTPrimario *rede);

   private: //métodos
	  TPricarga* __fastcall ExistePricarga(VTPrimario *primario);
	  void       __fastcall LimpaTList2(TList *lisCARGA);
      bool       __fastcall RestauraCarga(TList *lisCARGA);

   private: //objetos externos
      VTApl     *apl;

	private: //dados locais
      TList *lisPRI_CAR;
	};

//---------------------------------------------------------------------------
#endif
//eof

