//---------------------------------------------------------------------------
#ifndef TValMalhasH
#define TValMalhasH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTValMalhas.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTChave;
class VTLigacao;
class VTMalha;
class VTMalhas;
class VTPrimario;
class VTLog;

//---------------------------------------------------------------------------
class TValMalhas : public VTValMalhas
   {
   public:
			   __fastcall  TValMalhas(VTApl *apl);
			   __fastcall ~TValMalhas(void);
	  bool     __fastcall  Executa(void);


   private: //métodos
	   bool       __fastcall  AbreChaveExt(VTMalha *malha);
	   bool       __fastcall  AbreChaveInt(VTMalha *malha, bool so_chave);
	   bool       __fastcall  DeslocaLigacao(VTBarra *barra, VTLigacao *ligacao, bool redes_diferentes);
	   bool       __fastcall  DeslocaLigacao(VTMalha *malha, bool redes_diferentes);
	   bool       __fastcall  ExecutaMalhaExterna(void);
	   bool       __fastcall  ExecutaMalhaInterna(void);
       VTChave*   __fastcall  ExisteChaveParalela(TList *lisCHV, VTChave *chave1);
	   void       __fastcall  InsereLog(VTLigacao *ligacao);
       void       __fastcall  ReiniciaLog(void);
	   bool       __fastcall  SoChaves(VTMalha *malha);
	   bool       __fastcall  SoTrafos(VTMalha *malha);
       void       __fastcall  ValidaChavesParalelo(void);

   private: //objetos externos
      VTApl   *apl;
      VTLog   *plog;

   private: //dados locais
	  TList  *lisEQP;
	  VTMalhas *malhas;
   };

#endif
//-----------------------------------------------------------------------------
// eof
