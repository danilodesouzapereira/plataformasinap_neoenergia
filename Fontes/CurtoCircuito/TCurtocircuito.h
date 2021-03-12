//---------------------------------------------------------------------------
#ifndef TCurtocircuitoH
#define TCurtocircuitoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCurtocircuito.h"

//---------------------------------------------------------------------------
class VTNet;

//---------------------------------------------------------------------------
class TCurtocircuito : public VTCurtocircuito
   {
   public:
           __fastcall  TCurtocircuito(VTApl *apl_owner);
           __fastcall ~TCurtocircuito(void);
      bool __fastcall  Executa(VTRedes *redes,
                               STR_ZDef &Zdef,
                               bool    rede_alterada,
                               VTBarra *barra,
                               int     ind_pat,
                               bool    pre_falta);
      bool __fastcall  Executa(VTRedes  *redes,
                               STR_ZDef &Zdef,
                               bool     rede_alterada,
                               VTTrecho *trecho,
                               double   dist_bar1_km,
                               int      ind_pat,
                               bool     pre_falta);
      bool __fastcall  ExecutaCurto3FMax(VTNet *net,
                                         int    *id_barra_sinap,
                                         double *idef_3F_ka);
      bool __fastcall  ExecutaCurtoSimultaneo(VTRedes *redes,
                                              STR_ZDef &Zdef,
                                              bool     rede_alterada,
                                              TList    *lisDEF,
                                              int      ind_pat,
											  bool     pre_falta);

   protected:  //métodos acessados via property
      VTApl*    __fastcall PM_GetApl(void);
      VTRedeCC* __fastcall PM_GetRedeCC(void);

   private: //métodos
      bool __fastcall ExecutaCalculoCS(int      ind_pat,
                                       bool     pre_falta,
                                       VTNet    *net,
									   VTRedeCC *redeCC);//,
									   //VTTrecho *trecho=NULL,
									   //VTTrecho *trecho_fic1=NULL,
									   //VTTrecho *trecho_fic2=NULL);
      bool __fastcall ExecutaCalculoMA(int      ind_pat,
                                       bool     pre_falta,
                                       VTNet    *net,
									   VTRedeCC *redeCC);//,
									   //VTTrecho *trecho=NULL,
									   //VTTrecho *trecho_fic1=NULL,
									   //VTTrecho *trecho_fic2=NULL);
      bool __fastcall ExecutaCalculoSimultaneo(int      ind_pat,
                                               bool     pre_falta,
                                               VTNet    *net,
                                               VTRedeCC *redeCC,
											   TList    *lisDEF);
	  void __fastcall EliminaRelatorios(void);

   private: //objetos externos
      VTApl   *apl;
      VTRedes *redes;

   private: //dados locais
      int barras_isoladas;
   };

#endif
//---------------------------------------------------------------------------
//eof

